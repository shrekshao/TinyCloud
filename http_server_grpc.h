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
using backend::FileChunkRequest;
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

  void InsertFolder(const std::string & folder)
  {
    ClientContext context;

    FileListRequest request;

    // cerr << "folder full path: " << username << folder << endl;

    request.set_foldername(folder);
    // request.set_is_file(isFile);

    backend::Empty reply;
    Status status = stub_->InsertFileList(&context, request, &reply);

    if (status.ok()) {
      cerr << "rpc: insert folder ok\n";
      // return true;
    } else {
      cerr << "rpc: Err insert folder failed!\n";
      // return false;
    }
  }

  void UploadFile(const string & username, const string & url, const string & data, const string &extname)
  {
    // url without username

    ClientContext context;

    FileChunk chunk;
    backend::Empty response;

    chunk.set_username(username);

    // get rid of /username

    cerr << "rpc received url: " << url << endl;

    chunk.set_filename(url);
    chunk.set_filetype(extname);


    chunk.set_length(data.size());
    chunk.set_data(data);

    chunk.set_orig_length(data.size());
    chunk.set_orig_data(data);

    Status status = stub_->PutFile(&context, chunk, &response);
    if (status.ok()) {
      cerr << "rpc: upload file ok\n";
      // return true;
    } else {
      cerr << "rpc: Err upload file failed!\n";
      // return false;
    }
  }

  void DeleteFile(const string & username, const string & url)
  {
    ClientContext context;

    FileChunkRequest request;
    backend::Empty response;

    request.set_username(username);
    request.set_filename(url);

    Status status = stub_->DeleteFile(&context, request, &response);
    if (status.ok()) {
      cerr << "rpc: delete file ok\n";
      // return true;
    } else {
      cerr << "rpc: delete file failed!\n";
      // return false;
    }
  }

  bool GetFile(const string & username, const string & url, string & data, string & extname)
  {
    ClientContext context;

    FileChunkRequest request;
    FileChunk response;

    request.set_username(username);
    request.set_filename(url);

    Status status = stub_->GetFile(&context, request, &response);
    if (status.ok()) {
      cerr << "rpc: get file ok\n";

      data = response.data();
      extname = response.filetype();

      return true;
    } else {
      cerr << "rpc: get file failed!\n";
      return false;
    }
  }

 private:
  std::unique_ptr<Storage::Stub> stub_;
};