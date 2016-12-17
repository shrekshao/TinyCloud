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
        int success = master_service.create_user(request->username());
        if (success >= 0) {
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
        // this garbage collector will run every 100s
        cout << "Node Failure Scanning..." <<endl;
        this_thread::sleep_for (chrono::seconds(1));
        res = master_service.failure_checking();
    }
    pthread_exit(NULL);
}

// running the background thread
void RunFailureChecking() {
    int rc = 0;
    pthread_t failureCheckingThread;
    pthread_create(&failureCheckingThread, NULL, &failureCheckingHelper, NULL);
    pthread_join(failureCheckingThread, NULL);
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
    RunFailureChecking();
    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

// main function to run
int main(int argc, char** argv) {
    RunServer();

    return 0;
}