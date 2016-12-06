//
// Created by tianli on 12/4/16.
//

#include <iostream>
#include <memory>
#include <string>
#include <map>

#include <grpc++/grpc++.h>

#include "backend.grpc.pb.h"
#include "Indexer.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using backend::GetFileListRequest;
using backend::GetFileListReply;
using backend::Storage;

// Indexer service in-memory storage
Indexer indexer_service();

// Logic and data behind the server's behavior.
class StorageServiceImpl final : public Storage::Service {
    Status GetFileList(ServerContext* context, const GetFileListRequest* request, GetFileListReply* reply) override {
        indexer_service.display(request->foldername());
        (*reply->mutable_filelist())[request->foldername()] = "test";
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    StorageServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();

    return 0;
}
