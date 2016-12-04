#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <map>

#include <grpc++/grpc++.h>

#include "grpc_backend/backend.grpc.pb.h"

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using backend::GetFileListRequest;
using backend::GetFileListReply;
using backend::Storage;

class FileSystemClient {
 public:
  FileSystemClient(std::shared_ptr<Channel> channel)
      : stub_(Storage::NewStub(channel)) {}

  // return if rpc succeed
  bool GetFileList(const std::string& foldername
        , std::map<std::string, std::string> & fileList) {
    // Data we are sending to the server.
    GetFileListRequest request;
    request.set_foldername(foldername);

    // Container for the data we expect from the server.
    GetFileListReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->GetFileList(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      // return reply.message();

      // std::map<std::string, std::string> fileList(reply.filelist().begin(),
      //                               reply.filelist().end());

      fileList = std::map<std::string, std::string>(reply.filelist().begin(), reply.filelist().end());
      return true;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return false;
    }
  }

 private:
  std::unique_ptr<Storage::Stub> stub_;
};