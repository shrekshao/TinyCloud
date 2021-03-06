// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: backend.proto

#include "backend.pb.h"
#include "backend.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace backend {

static const char* Storage_method_names[] = {
  "/backend.Storage/CreateUser",
  "/backend.Storage/CheckPassword",
  "/backend.Storage/GetFileList",
  "/backend.Storage/InsertFileList",
  "/backend.Storage/PutFile",
  "/backend.Storage/UpdateFile",
  "/backend.Storage/GetFile",
  "/backend.Storage/DeleteFile",
  "/backend.Storage/GetMemTableInfo",
  "/backend.Storage/GetLog",
  "/backend.Storage/GetBuffer",
};

std::unique_ptr< Storage::Stub> Storage::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< Storage::Stub> stub(new Storage::Stub(channel));
  return stub;
}

Storage::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_CreateUser_(Storage_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_CheckPassword_(Storage_method_names[1], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetFileList_(Storage_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_InsertFileList_(Storage_method_names[3], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_PutFile_(Storage_method_names[4], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_UpdateFile_(Storage_method_names[5], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetFile_(Storage_method_names[6], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DeleteFile_(Storage_method_names[7], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetMemTableInfo_(Storage_method_names[8], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetLog_(Storage_method_names[9], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetBuffer_(Storage_method_names[10], ::grpc::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Storage::Stub::CreateUser(::grpc::ClientContext* context, const ::backend::UserAccount& request, ::backend::Empty* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_CreateUser_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::Empty>* Storage::Stub::AsyncCreateUserRaw(::grpc::ClientContext* context, const ::backend::UserAccount& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::Empty>(channel_.get(), cq, rpcmethod_CreateUser_, context, request);
}

::grpc::Status Storage::Stub::CheckPassword(::grpc::ClientContext* context, const ::backend::UserAccount& request, ::backend::UserAccountReply* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_CheckPassword_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::UserAccountReply>* Storage::Stub::AsyncCheckPasswordRaw(::grpc::ClientContext* context, const ::backend::UserAccount& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::UserAccountReply>(channel_.get(), cq, rpcmethod_CheckPassword_, context, request);
}

::grpc::Status Storage::Stub::GetFileList(::grpc::ClientContext* context, const ::backend::FileListRequest& request, ::backend::FileListReply* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_GetFileList_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::FileListReply>* Storage::Stub::AsyncGetFileListRaw(::grpc::ClientContext* context, const ::backend::FileListRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::FileListReply>(channel_.get(), cq, rpcmethod_GetFileList_, context, request);
}

::grpc::Status Storage::Stub::InsertFileList(::grpc::ClientContext* context, const ::backend::FileListRequest& request, ::backend::Empty* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_InsertFileList_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::Empty>* Storage::Stub::AsyncInsertFileListRaw(::grpc::ClientContext* context, const ::backend::FileListRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::Empty>(channel_.get(), cq, rpcmethod_InsertFileList_, context, request);
}

::grpc::Status Storage::Stub::PutFile(::grpc::ClientContext* context, const ::backend::FileChunk& request, ::backend::Empty* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_PutFile_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::Empty>* Storage::Stub::AsyncPutFileRaw(::grpc::ClientContext* context, const ::backend::FileChunk& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::Empty>(channel_.get(), cq, rpcmethod_PutFile_, context, request);
}

::grpc::Status Storage::Stub::UpdateFile(::grpc::ClientContext* context, const ::backend::FileChunk& request, ::backend::Empty* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_UpdateFile_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::Empty>* Storage::Stub::AsyncUpdateFileRaw(::grpc::ClientContext* context, const ::backend::FileChunk& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::Empty>(channel_.get(), cq, rpcmethod_UpdateFile_, context, request);
}

::grpc::Status Storage::Stub::GetFile(::grpc::ClientContext* context, const ::backend::FileChunkRequest& request, ::backend::FileChunk* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_GetFile_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::FileChunk>* Storage::Stub::AsyncGetFileRaw(::grpc::ClientContext* context, const ::backend::FileChunkRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::FileChunk>(channel_.get(), cq, rpcmethod_GetFile_, context, request);
}

::grpc::Status Storage::Stub::DeleteFile(::grpc::ClientContext* context, const ::backend::FileChunkRequest& request, ::backend::Empty* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_DeleteFile_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::Empty>* Storage::Stub::AsyncDeleteFileRaw(::grpc::ClientContext* context, const ::backend::FileChunkRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::Empty>(channel_.get(), cq, rpcmethod_DeleteFile_, context, request);
}

::grpc::Status Storage::Stub::GetMemTableInfo(::grpc::ClientContext* context, const ::backend::Empty& request, ::backend::MemTableInfo* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_GetMemTableInfo_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::MemTableInfo>* Storage::Stub::AsyncGetMemTableInfoRaw(::grpc::ClientContext* context, const ::backend::Empty& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::MemTableInfo>(channel_.get(), cq, rpcmethod_GetMemTableInfo_, context, request);
}

::grpc::Status Storage::Stub::GetLog(::grpc::ClientContext* context, const ::backend::Empty& request, ::backend::Log* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_GetLog_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::Log>* Storage::Stub::AsyncGetLogRaw(::grpc::ClientContext* context, const ::backend::Empty& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::Log>(channel_.get(), cq, rpcmethod_GetLog_, context, request);
}

::grpc::Status Storage::Stub::GetBuffer(::grpc::ClientContext* context, const ::backend::Empty& request, ::backend::Buffer* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_GetBuffer_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::Buffer>* Storage::Stub::AsyncGetBufferRaw(::grpc::ClientContext* context, const ::backend::Empty& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::Buffer>(channel_.get(), cq, rpcmethod_GetBuffer_, context, request);
}

Storage::Service::Service() {
  (void)Storage_method_names;
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::UserAccount, ::backend::Empty>(
          std::mem_fn(&Storage::Service::CreateUser), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::UserAccount, ::backend::UserAccountReply>(
          std::mem_fn(&Storage::Service::CheckPassword), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::FileListRequest, ::backend::FileListReply>(
          std::mem_fn(&Storage::Service::GetFileList), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[3],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::FileListRequest, ::backend::Empty>(
          std::mem_fn(&Storage::Service::InsertFileList), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[4],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::FileChunk, ::backend::Empty>(
          std::mem_fn(&Storage::Service::PutFile), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[5],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::FileChunk, ::backend::Empty>(
          std::mem_fn(&Storage::Service::UpdateFile), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[6],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::FileChunkRequest, ::backend::FileChunk>(
          std::mem_fn(&Storage::Service::GetFile), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[7],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::FileChunkRequest, ::backend::Empty>(
          std::mem_fn(&Storage::Service::DeleteFile), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[8],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::Empty, ::backend::MemTableInfo>(
          std::mem_fn(&Storage::Service::GetMemTableInfo), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[9],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::Empty, ::backend::Log>(
          std::mem_fn(&Storage::Service::GetLog), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[10],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::Empty, ::backend::Buffer>(
          std::mem_fn(&Storage::Service::GetBuffer), this)));
}

Storage::Service::~Service() {
}

::grpc::Status Storage::Service::CreateUser(::grpc::ServerContext* context, const ::backend::UserAccount* request, ::backend::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::CheckPassword(::grpc::ServerContext* context, const ::backend::UserAccount* request, ::backend::UserAccountReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::GetFileList(::grpc::ServerContext* context, const ::backend::FileListRequest* request, ::backend::FileListReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::InsertFileList(::grpc::ServerContext* context, const ::backend::FileListRequest* request, ::backend::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::PutFile(::grpc::ServerContext* context, const ::backend::FileChunk* request, ::backend::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::UpdateFile(::grpc::ServerContext* context, const ::backend::FileChunk* request, ::backend::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::GetFile(::grpc::ServerContext* context, const ::backend::FileChunkRequest* request, ::backend::FileChunk* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::DeleteFile(::grpc::ServerContext* context, const ::backend::FileChunkRequest* request, ::backend::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::GetMemTableInfo(::grpc::ServerContext* context, const ::backend::Empty* request, ::backend::MemTableInfo* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::GetLog(::grpc::ServerContext* context, const ::backend::Empty* request, ::backend::Log* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::GetBuffer(::grpc::ServerContext* context, const ::backend::Empty* request, ::backend::Buffer* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace backend

