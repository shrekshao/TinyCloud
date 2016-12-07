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
  "/backend.Storage/GetFileList",
  "/backend.Storage/InsertFileList",
  "/backend.Storage/DeleteFileList",
};

std::unique_ptr< Storage::Stub> Storage::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< Storage::Stub> stub(new Storage::Stub(channel));
  return stub;
}

Storage::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_GetFileList_(Storage_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_InsertFileList_(Storage_method_names[1], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DeleteFileList_(Storage_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Storage::Stub::GetFileList(::grpc::ClientContext* context, const ::backend::FileListRequest& request, ::backend::FileListReply* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_GetFileList_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::FileListReply>* Storage::Stub::AsyncGetFileListRaw(::grpc::ClientContext* context, const ::backend::FileListRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::FileListReply>(channel_.get(), cq, rpcmethod_GetFileList_, context, request);
}

::grpc::Status Storage::Stub::InsertFileList(::grpc::ClientContext* context, const ::backend::FileListRequest& request, ::backend::FileListReply* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_InsertFileList_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::FileListReply>* Storage::Stub::AsyncInsertFileListRaw(::grpc::ClientContext* context, const ::backend::FileListRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::FileListReply>(channel_.get(), cq, rpcmethod_InsertFileList_, context, request);
}

::grpc::Status Storage::Stub::DeleteFileList(::grpc::ClientContext* context, const ::backend::FileListRequest& request, ::backend::FileListReply* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_DeleteFileList_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::backend::FileListReply>* Storage::Stub::AsyncDeleteFileListRaw(::grpc::ClientContext* context, const ::backend::FileListRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::backend::FileListReply>(channel_.get(), cq, rpcmethod_DeleteFileList_, context, request);
}

Storage::Service::Service() {
  (void)Storage_method_names;
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::FileListRequest, ::backend::FileListReply>(
          std::mem_fn(&Storage::Service::GetFileList), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::FileListRequest, ::backend::FileListReply>(
          std::mem_fn(&Storage::Service::InsertFileList), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Storage_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Storage::Service, ::backend::FileListRequest, ::backend::FileListReply>(
          std::mem_fn(&Storage::Service::DeleteFileList), this)));
}

Storage::Service::~Service() {
}

::grpc::Status Storage::Service::GetFileList(::grpc::ServerContext* context, const ::backend::FileListRequest* request, ::backend::FileListReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::InsertFileList(::grpc::ServerContext* context, const ::backend::FileListRequest* request, ::backend::FileListReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Storage::Service::DeleteFileList(::grpc::ServerContext* context, const ::backend::FileListRequest* request, ::backend::FileListReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace backend

