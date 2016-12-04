// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: backend.proto

#ifndef PROTOBUF_backend_2eproto__INCLUDED
#define PROTOBUF_backend_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/map.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace backend {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_backend_2eproto();
void protobuf_AssignDesc_backend_2eproto();
void protobuf_ShutdownFile_backend_2eproto();

class GetFileListReply;
class GetFileListRequest;

// ===================================================================

class GetFileListRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:backend.GetFileListRequest) */ {
 public:
  GetFileListRequest();
  virtual ~GetFileListRequest();

  GetFileListRequest(const GetFileListRequest& from);

  inline GetFileListRequest& operator=(const GetFileListRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const GetFileListRequest& default_instance();

  void Swap(GetFileListRequest* other);

  // implements Message ----------------------------------------------

  inline GetFileListRequest* New() const { return New(NULL); }

  GetFileListRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GetFileListRequest& from);
  void MergeFrom(const GetFileListRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(GetFileListRequest* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string foldername = 1;
  void clear_foldername();
  static const int kFoldernameFieldNumber = 1;
  const ::std::string& foldername() const;
  void set_foldername(const ::std::string& value);
  void set_foldername(const char* value);
  void set_foldername(const char* value, size_t size);
  ::std::string* mutable_foldername();
  ::std::string* release_foldername();
  void set_allocated_foldername(::std::string* foldername);

  // @@protoc_insertion_point(class_scope:backend.GetFileListRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr foldername_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_backend_2eproto();
  friend void protobuf_AssignDesc_backend_2eproto();
  friend void protobuf_ShutdownFile_backend_2eproto();

  void InitAsDefaultInstance();
  static GetFileListRequest* default_instance_;
};
// -------------------------------------------------------------------

class GetFileListReply : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:backend.GetFileListReply) */ {
 public:
  GetFileListReply();
  virtual ~GetFileListReply();

  GetFileListReply(const GetFileListReply& from);

  inline GetFileListReply& operator=(const GetFileListReply& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const GetFileListReply& default_instance();

  void Swap(GetFileListReply* other);

  // implements Message ----------------------------------------------

  inline GetFileListReply* New() const { return New(NULL); }

  GetFileListReply* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GetFileListReply& from);
  void MergeFrom(const GetFileListReply& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(GetFileListReply* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------


  // accessors -------------------------------------------------------

  // map<string, string> filelist = 1;
  int filelist_size() const;
  void clear_filelist();
  static const int kFilelistFieldNumber = 1;
  const ::google::protobuf::Map< ::std::string, ::std::string >&
      filelist() const;
  ::google::protobuf::Map< ::std::string, ::std::string >*
      mutable_filelist();

  // @@protoc_insertion_point(class_scope:backend.GetFileListReply)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  typedef ::google::protobuf::internal::MapEntryLite<
      ::std::string, ::std::string,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      0 >
      GetFileListReply_FilelistEntry;
  ::google::protobuf::internal::MapField<
      ::std::string, ::std::string,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      0 > filelist_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_backend_2eproto();
  friend void protobuf_AssignDesc_backend_2eproto();
  friend void protobuf_ShutdownFile_backend_2eproto();

  void InitAsDefaultInstance();
  static GetFileListReply* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// GetFileListRequest

// optional string foldername = 1;
inline void GetFileListRequest::clear_foldername() {
  foldername_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& GetFileListRequest::foldername() const {
  // @@protoc_insertion_point(field_get:backend.GetFileListRequest.foldername)
  return foldername_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void GetFileListRequest::set_foldername(const ::std::string& value) {
  
  foldername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:backend.GetFileListRequest.foldername)
}
inline void GetFileListRequest::set_foldername(const char* value) {
  
  foldername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:backend.GetFileListRequest.foldername)
}
inline void GetFileListRequest::set_foldername(const char* value, size_t size) {
  
  foldername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:backend.GetFileListRequest.foldername)
}
inline ::std::string* GetFileListRequest::mutable_foldername() {
  
  // @@protoc_insertion_point(field_mutable:backend.GetFileListRequest.foldername)
  return foldername_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* GetFileListRequest::release_foldername() {
  // @@protoc_insertion_point(field_release:backend.GetFileListRequest.foldername)
  
  return foldername_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void GetFileListRequest::set_allocated_foldername(::std::string* foldername) {
  if (foldername != NULL) {
    
  } else {
    
  }
  foldername_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), foldername);
  // @@protoc_insertion_point(field_set_allocated:backend.GetFileListRequest.foldername)
}

// -------------------------------------------------------------------

// GetFileListReply

// map<string, string> filelist = 1;
inline int GetFileListReply::filelist_size() const {
  return filelist_.size();
}
inline void GetFileListReply::clear_filelist() {
  filelist_.Clear();
}
inline const ::google::protobuf::Map< ::std::string, ::std::string >&
GetFileListReply::filelist() const {
  // @@protoc_insertion_point(field_map:backend.GetFileListReply.filelist)
  return filelist_.GetMap();
}
inline ::google::protobuf::Map< ::std::string, ::std::string >*
GetFileListReply::mutable_filelist() {
  // @@protoc_insertion_point(field_mutable_map:backend.GetFileListReply.filelist)
  return filelist_.MutableMap();
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace backend

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_backend_2eproto__INCLUDED
