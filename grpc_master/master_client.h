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

#include "master.grpc.pb.h"

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using master::Master;
using master::UserNameRequest;
using master::AddressReply;
using master::Empty;

class master_client {
public:
    master_client(std::shared_ptr<Channel> channel)
    : stub_(Master::NewStub(channel)) {}

    // return true if success
    bool GetUserAddr(const string & username, string & addr) {
        // data that are sending to the master
        UserNameRequest request;
        request.set_username(username);

        // container for the reply message
        AddressReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->GetUserAddr(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            addr = reply.addr();
            return true;
        } else {
            std::cerr << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return false;
        }
    }

    // return true if success
    bool CreateUser(const string & username) {
        // data that are sending to the master
        UserNameRequest request;
        request.set_username(username);

        // container for the reply message
        Empty reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->CreateUser(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return true;
        } else {
            std::cerr << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return false;
        }
    }



private:
    std::unique_ptr<Master::Stub> stub_;
};


#endif //TINYCLOUD_MASTER_CLIENT_H
