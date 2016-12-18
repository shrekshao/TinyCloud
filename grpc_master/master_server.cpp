//
// Created by Zhengxuan Wu on 16/12/2016.
//

#include <iostream>
#include <memory>
#include <map>
#include <unistd.h>
#include <string>
#include <string.h>
#include <pthread.h>
#include <sstream>
#include <algorithm>

#include <grpc++/grpc++.h>

#include "master.grpc.pb.h"
#include "MasterNode.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using master::Master;
using master::UserNameRequest;
using master::AddressReply;
using master::Empty;
using master::NodesStatusReply;
using master::NodesInfoReply;
using master::NodeIndexRequest;
using backend::MemTableInfo;
using master::MemTableInfoReply;

// master server IP:Port
const char*  server_ip = "0.0.0.0:52013";
// master config file
string CONFIG = "master_config.txt";
// construct a master service
MasterNode master_service(CONFIG);
// error message compare
string ERR = "ERR";

class MasterServiceImpl final : public Master::Service {
    // getting the user address
    Status GetUserAddr(ServerContext* context, const UserNameRequest* request, AddressReply* reply) override {
        //cout << "[GRPC]:Receiving GET USER Call\n";
        string addr = master_service.get_user_node(request->username());
        // check if it is an error
        if(addr.substr(0, ERR.size()) == ERR) {
            return Status::CANCELLED;
        } else {
            reply->set_addr(addr);
            return Status::OK;
        }
    }
    // create a master
    Status CreateUser(ServerContext* context, const UserNameRequest* request, Empty* reply) override {
        //cout << "[GRPC]:Receiving CREATE USER Call\n";
        int success = master_service.create_user(request->username());
        if (success >= 0) {
            return Status::OK;
        } else {
            return Status::CANCELLED;
        }
    }
    // getting a map indicating the status of all the nodes
    Status GetNodesStatus(ServerContext* context, const Empty* request, NodesStatusReply* reply) override {
        //cout << "[GRPC]:Receiving GET STATUS Call\n";
        map<string, bool> res;
        int success = master_service.get_status(res);
        if (success == 1) {
            // writing to the reply map
            for (map<string, bool>::iterator it = res.begin(); it != res.end(); ++it) {
                (*reply->mutable_nodesstatus())[it->first] = it->second;
            }
            return Status::OK;
        } else {
            return Status::CANCELLED;
        }
    }
    // getting a info map indicating the raw data of all the nodes
    Status GetNodesInfo(ServerContext* context, const Empty* request, NodesInfoReply* reply) override {
        //cout << "[GRPC]:Receiving GET INFO Call\n";
        map<string, StorageNodeInfo> res;
        int success = master_service.get_info(res);
        if (success == 1) {
            // writing to the reply map
            for (map<string, StorageNodeInfo>::iterator it = res.begin(); it != res.end(); ++it) {
                // convert the list to a printable string
                stringstream ss;
                copy( it->second.user_list.begin(), it->second.user_list.end(), ostream_iterator<string>(ss, " "));
                string s = ss.str();
                s = s.substr(0, s.length()-1);  // get rid of the trailing space
                master::NodeInfo ni;
                ni.set_user_list(s);
                ni.set_user_number(it->second.user_number);
                ni.set_status(it->second.crashed);
                ni.set_buffer("[ADMIN] dummy message for testing displaying raw data");
                ni.set_buffer_length(9999);
                (*reply->mutable_nodeinfo())[it->first] = ni;
            }
            return Status::OK;
        } else {
            return Status::CANCELLED;
        }
    }
    // disenable a node
    Status DisableNode(ServerContext* context, const NodeIndexRequest* request, Empty* reply) override {
        //cout << "[GRPC]:Receiving DisanleNode Call\n";
        int node_index = (int)request->index();
        int success = master_service.disenable_node(node_index);
        if (success == 1) {
            return Status::OK;
        } else {
            return Status::CANCELLED;
        }
    }
    // disenable a node
    Status EnableNode(ServerContext* context, const NodeIndexRequest* request, Empty* reply) override {
        //cout << "[GRPC]:Receiving DisanleNode Call\n";
        int node_index = (int)request->index();
        int success = master_service.enable_node(node_index);
        if (success == 1) {
            return Status::OK;
        } else {
            return Status::CANCELLED;
        }
    }
    // send back storage meta data
    Status SendMemTableInfo(ServerContext* context, const Empty* request, MemTableInfoReply* reply) override {
        map<string, MemTableInfo> res;
        int success = master_service.send_node_date(res);
        if (success == 1) {
            // writing to the reply map
            for (map<string, MemTableInfo>::iterator it = res.begin(); it != res.end(); ++it) {
                master::MemTableInfo mi;
                mi.set_buffer_length(it->second.buffer_length());
                (*reply->mutable_nodememinfo())[it->first] = mi;
            }
            return Status::OK;
        } else {
            return Status::CANCELLED;
        }
    }

};

// function for background thread
void* failureCheckingHelper(void*) {
    int res = 1;
    while (res == 1) {
        //cout << "Node Failure Scanning..." <<endl;
        this_thread::sleep_for (chrono::seconds(1));
        int res = master_service.failure_checking();
    }
    pthread_exit(NULL);
}

void* checkPointHelper(void*) {
    int res = 1;
    while (res == 1) {
        cout << "Master Check Pointer (Mem to Disk)..." <<endl;
        this_thread::sleep_for (chrono::seconds(50));
        res = master_service.checking_to_disk();
    }
    pthread_exit(NULL);
}

void* storageNodeCheckingHelper(void*) {
    int res = 1;
    while (res == 1) {
        //cout << "Node Meta Info Retrieving..." <<endl;
        this_thread::sleep_for (chrono::seconds(1));
        res = master_service.checking_node_data();
    }
    pthread_exit(NULL);
}

void RunBackgroundThread() {
    pthread_t failureCheckingThread;
    pthread_create(&failureCheckingThread, NULL, &failureCheckingHelper, NULL);
    pthread_t checkPointThread;
    pthread_create(&checkPointThread, NULL, &checkPointHelper, NULL);
    pthread_t storageNodeCheckingThread;
    pthread_create(&storageNodeCheckingThread, NULL, &storageNodeCheckingHelper, NULL);

    pthread_join(failureCheckingThread, NULL);
    pthread_join(checkPointThread, NULL);
    pthread_join(storageNodeCheckingThread, NULL);
}

// main method to run the server
void RunServer() {
    std::string server_address(server_ip);
    MasterServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    // checking thread
    //master_service.create_user("tianli");
    RunBackgroundThread();
    //RunStorageNodeChecking();
    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

// main function to run
int main(int argc, char** argv) {
    RunServer();
    return 0;
}