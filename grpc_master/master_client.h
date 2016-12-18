//
// Created by Frank Wu on 16/12/2016.
//

#ifndef TINYCLOUD_MASTER_CLIENT_H
#define TINYCLOUD_MASTER_CLIENT_H

#include <iostream>
#include <memory>
#include <map>
#include <unistd.h>
#include <string>
#include <string.h>
#include <pthread.h>

#include <grpc++/grpc++.h>

#include "../grpc_backend/backend.grpc.pb.h"
#include "../grpc_backend/backend.grpc.pb.cc"
#include "../grpc_backend/backend.pb.cc"
#include "../grpc_backend/backend.pb.h"

#include "master.grpc.pb.h"

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using backend::Storage;
using backend::MemTableInfo;

using master::Master;
using master::UserNameRequest;
using master::AddressReply;
using master::Empty;
using master::NodesStatusReply;
using master::NodesInfoReply;
using master::NodeIndexRequest;
using master::MemTableInfoReply;


// we need to log when a function is get called

class MasterClient {
public:

    MasterClient(std::shared_ptr<Channel> channel)
    : stub_(backend::Storage::NewStub(channel)) {}

//    // return true if success
//    bool GetUserAddr(const string & username, string & addr) {
//        // data that are sending to the master
//        UserNameRequest request;
//        request.set_username(username);
//
//        // container for the reply message
//        AddressReply reply;
//
//        // Context for the client. It could be used to convey extra information to
//        // the server and/or tweak certain RPC behaviors.
//        ClientContext context;
//
//        // The actual RPC.
//        Status status = stub_->GetUserAddr(&context, request, &reply);
//
//        // Act upon its status.
//        if (status.ok()) {
//            addr = reply.addr();
//            return true;
//        } else {
//            std::cerr << status.error_code() << ": " << status.error_message()
//                      << std::endl;
//            return false;
//        }
//    }
//
//    // return true if success
//    bool CreateUser(const string & username) {
//        // data that are sending to the master
//        UserNameRequest request;
//        request.set_username(username);
//
//        // container for the reply message
//        master::Empty reply;
//
//        // Context for the client. It could be used to convey extra information to
//        // the server and/or tweak certain RPC behaviors.
//        ClientContext context;
//
//        // The actual RPC.
//        Status status = stub_->CreateUser(&context, request, &reply);
//        //cout << "[MASTER] Usered Created Successfully!\n";
//        // Act upon its status.
//        if (status.ok()) {
//            return true;
//        } else {
//            std::cerr << status.error_code() << ": " << status.error_message()
//                      << std::endl;
//            return false;
//        }
//    }
//
//    // return a map of node status
//    bool GetNodesStatus(std::map<string, bool> & res) {
//        // data that are sending to the master
//        master::Empty request;
//
//        // container for the reply message
//        NodesStatusReply reply;
//
//        // Context for the client. It could be used to convey extra information to
//        // the server and/or tweak certain RPC behaviors.
//        ClientContext context;
//
//        // The actual RPC.
//        Status status = stub_->GetNodesStatus(&context, request, &reply);
//
//        if (status.ok()) {
//            res = std::map<string, bool>(reply.nodesstatus().begin(), reply.nodesstatus().end());
//            return true;
//        } else {
//            std::cerr << status.error_code() << ": " << status.error_message()
//                      << std::endl;
//            return false;
//        }
//    }
//
//    // disenable a node
//    bool DisableNode(const int index) {
//        // data that are sending to the master
//        NodeIndexRequest request;
//        request.set_index(index);
//
//        // container for the reply message
//        master::Empty reply;
//
//        // Context for the client. It could be used to convey extra information to
//        // the server and/or tweak certain RPC behaviors.
//        ClientContext context;
//
//        // The actual RPC.
//        Status status = stub_->DisableNode(&context, request, &reply);
//
//        if (status.ok()) {
//            return true;
//        } else {
//            std::cerr << status.error_code() << ": " << status.error_message()
//                      << std::endl;
//            return false;
//        }
//    }
//
//    // return a map of node info
//    bool GetNodesInfo(map<string, NodeInfo> &res) {
//        // data that are sending to the master
//        master::Empty request;
//
//        // container for the reply message
//        NodesInfoReply reply;
//
//        // Context for the client. It could be used to convey extra information to
//        // the server and/or tweak certain RPC behaviors.
//        ClientContext context;
//
//        // The actual RPC.
//        Status status = stub_->GetNodesInfo(&context, request, &reply);
//
//        if (status.ok()) {
//            res = std::map<string, NodeInfo>(reply.nodeinfo().begin(), reply.nodeinfo().end());
//            return true;
//        } else {
//            std::cerr << status.error_code() << ": " << status.error_message()
//                      << std::endl;
//            return false;
//        }
//    }

    // getting memtable info from storage node
    bool GetMemTableInfo(backend::MemTableInfo &res) {
        // data that are sending to the master
        backend::Empty request;

        // container for the reply message
        backend::MemTableInfo reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->GetMemTableInfo(&context, request, &reply);

        if (status.ok()) {
            res = reply;
            return true;
        } else {
            return false;
        }
    }

private:
    std::unique_ptr<backend::Storage::Stub> stub_;
};


#endif //TINYCLOUD_MASTER_CLIENT_H
