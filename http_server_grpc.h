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

using backend::FileListRequest;
using backend::FileListReply;
using backend::Empty;
using backend::FileChunk;
using backend::Storage;
using backend::FileInfo;

class FileSystemClient {
 public:
  FileSystemClient(std::shared_ptr<Channel> channel)
      : stub_(Storage::NewStub(channel)) {}

  // return if rpc succeed
  bool GetFileList(const std::string& foldername
        , std::map<std::string, FileInfo> & fileList) {
    // Data we are sending to the server.
    FileListRequest request;
    request.set_foldername(foldername);

    // Container for the data we expect from the server.
    FileListReply reply;

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

      fileList = std::map<std::string, FileInfo>(reply.filelist().begin(), reply.filelist().end());
      return true;
    } else {
      std::cerr << status.error_code() << ": " << status.error_message()
                << std::endl;
      return false;
    }
  }

  void InsertFolder(const std::string & username, const std::string & folder, bool isFile)
  {
    ClientContext context;

    FileListRequest request;
    request.set_foldername(username + folder);
    // request.set_is_file(isFile);

    Status status = stub_->InsertFileList(&context, request, NULL);

    if (status.ok()) {
      cerr << "insert folder ok\n";
      // return true;
    } else {
      cerr << "Err insert folder failed!\n";
      // return false;
    }
  }

  void UploadFile()
  {
    // TODO
  }

 private:
  std::unique_ptr<Storage::Stub> stub_;
};