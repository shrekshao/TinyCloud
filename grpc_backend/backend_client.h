//
// Created by Frank Wu on 18/12/2016.
//

#ifndef TINYCLOUD_BACKEND_CLIENT_H
#define TINYCLOUD_BACKEND_CLIENT_H


#include <iostream>
#include <memory>
#include <map>
#include <unistd.h>
#include <string>
#include <string.h>
#include <pthread.h>

#include <grpc++/grpc++.h>

#include "backend.grpc.pb.h"
//#include "../grpc_master/MasterNode.h"

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using backend::Storage;
using backend::MemTableInfo;


// we need to log when a function is get called

class BackendClient {
public:

    BackendClient(std::shared_ptr<Channel> channel)
            : stub_(Storage::NewStub(channel)) {}

    // getting memtable info from storage node
    bool GetMemTableInfo(int &buffer_length) {
        // data that are sending to the master
        backend::Empty request;

        // container for the reply message
        MemTableInfo reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->GetMemTableInfo(&context, request, &reply);

        if (status.ok()) {
//            res = reply;
//            res.buffer_length = reply.buffer_length();
            buffer_length = reply.buffer_length();
            return true;
        } else {
            std::cerr << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return false;
        }
    }

private:
    std::unique_ptr<Storage::Stub> stub_;
};


#endif //TINYCLOUD_BACKEND_CLIENT_H
