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

class FileInfo;
class FileListReply;
class FileListRequest;

// ===================================================================

class FileListRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:backend.FileListRequest) */ {
 public:
  FileListRequest();
  virtual ~FileListRequest();

  FileListRequest(const FileListRequest& from);

  inline FileListRequest& operator=(const FileListRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const FileListRequest& default_instance();

  void Swap(FileListRequest* other);

  // implements Message ----------------------------------------------

  inline FileListRequest* New() const { return New(NULL); }

  FileListRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FileListRequest& from);
  void MergeFrom(const FileListRequest& from);
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
  void InternalSwap(FileListRequest* other);
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

  // optional bool is_file = 2;
  void clear_is_file();
  static const int kIsFileFieldNumber = 2;
  bool is_file() const;
  void set_is_file(bool value);

  // @@protoc_insertion_point(class_scope:backend.FileListRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr foldername_;
  bool is_file_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_backend_2eproto();
  friend void protobuf_AssignDesc_backend_2eproto();
  friend void protobuf_ShutdownFile_backend_2eproto();

  void InitAsDefaultInstance();
  static FileListRequest* default_instance_;
};
// -------------------------------------------------------------------

class FileListReply : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:backend.FileListReply) */ {
 public:
  FileListReply();
  virtual ~FileListReply();

  FileListReply(const FileListReply& from);

  inline FileListReply& operator=(const FileListReply& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const FileListReply& default_instance();

  void Swap(FileListReply* other);

  // implements Message ----------------------------------------------

  inline FileListReply* New() const { return New(NULL); }

  FileListReply* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FileListReply& from);
  void MergeFrom(const FileListReply& from);
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
  void InternalSwap(FileListReply* other);
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

  // map<string, .backend.FileInfo> filelist = 1;
  int filelist_size() const;
  void clear_filelist();
  static const int kFilelistFieldNumber = 1;
  const ::google::protobuf::Map< ::std::string, ::backend::FileInfo >&
      filelist() const;
  ::google::protobuf::Map< ::std::string, ::backend::FileInfo >*
      mutable_filelist();

  // @@protoc_insertion_point(class_scope:backend.FileListReply)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  typedef ::google::protobuf::internal::MapEntryLite<
      ::std::string, ::backend::FileInfo,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_MESSAGE,
      0 >
      FileListReply_FilelistEntry;
  ::google::protobuf::internal::MapField<
      ::std::string, ::backend::FileInfo,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_MESSAGE,
      0 > filelist_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_backend_2eproto();
  friend void protobuf_AssignDesc_backend_2eproto();
  friend void protobuf_ShutdownFile_backend_2eproto();

  void InitAsDefaultInstance();
  static FileListReply* default_instance_;
};
// -------------------------------------------------------------------

class FileInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:backend.FileInfo) */ {
 public:
  FileInfo();
  virtual ~FileInfo();

  FileInfo(const FileInfo& from);

  inline FileInfo& operator=(const FileInfo& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const FileInfo& default_instance();

  void Swap(FileInfo* other);

  // implements Message ----------------------------------------------

  inline FileInfo* New() const { return New(NULL); }

  FileInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FileInfo& from);
  void MergeFrom(const FileInfo& from);
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
  void InternalSwap(FileInfo* other);
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

  // optional string name = 1;
  void clear_name();
  static const int kNameFieldNumber = 1;
  const ::std::string& name() const;
  void set_name(const ::std::string& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  ::std::string* mutable_name();
  ::std::string* release_name();
  void set_allocated_name(::std::string* name);

  // optional bool is_file = 2;
  void clear_is_file();
  static const int kIsFileFieldNumber = 2;
  bool is_file() const;
  void set_is_file(bool value);

  // @@protoc_insertion_point(class_scope:backend.FileInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr name_;
  bool is_file_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_backend_2eproto();
  friend void protobuf_AssignDesc_backend_2eproto();
  friend void protobuf_ShutdownFile_backend_2eproto();

  void InitAsDefaultInstance();
  static FileInfo* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// FileListRequest

// optional string foldername = 1;
inline void FileListRequest::clear_foldername() {
  foldername_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& FileListRequest::foldername() const {
  // @@protoc_insertion_point(field_get:backend.FileListRequest.foldername)
  return foldername_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void FileListRequest::set_foldername(const ::std::string& value) {
  
  foldername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:backend.FileListRequest.foldername)
}
inline void FileListRequest::set_foldername(const char* value) {
  
  foldername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:backend.FileListRequest.foldername)
}
inline void FileListRequest::set_foldername(const char* value, size_t size) {
  
  foldername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:backend.FileListRequest.foldername)
}
inline ::std::string* FileListRequest::mutable_foldername() {
  
  // @@protoc_insertion_point(field_mutable:backend.FileListRequest.foldername)
  return foldername_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* FileListRequest::release_foldername() {
  // @@protoc_insertion_point(field_release:backend.FileListRequest.foldername)
  
  return foldername_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void FileListRequest::set_allocated_foldername(::std::string* foldername) {
  if (foldername != NULL) {
    
  } else {
    
  }
  foldername_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), foldername);
  // @@protoc_insertion_point(field_set_allocated:backend.FileListRequest.foldername)
}

// optional bool is_file = 2;
inline void FileListRequest::clear_is_file() {
  is_file_ = false;
}
inline bool FileListRequest::is_file() const {
  // @@protoc_insertion_point(field_get:backend.FileListRequest.is_file)
  return is_file_;
}
inline void FileListRequest::set_is_file(bool value) {
  
  is_file_ = value;
  // @@protoc_insertion_point(field_set:backend.FileListRequest.is_file)
}

// -------------------------------------------------------------------

// FileListReply

// map<string, .backend.FileInfo> filelist = 1;
inline int FileListReply::filelist_size() const {
  return filelist_.size();
}
inline void FileListReply::clear_filelist() {
  filelist_.Clear();
}
inline const ::google::protobuf::Map< ::std::string, ::backend::FileInfo >&
FileListReply::filelist() const {
  // @@protoc_insertion_point(field_map:backend.FileListReply.filelist)
  return filelist_.GetMap();
}
inline ::google::protobuf::Map< ::std::string, ::backend::FileInfo >*
FileListReply::mutable_filelist() {
  // @@protoc_insertion_point(field_mutable_map:backend.FileListReply.filelist)
  return filelist_.MutableMap();
}

// -------------------------------------------------------------------

// FileInfo

// optional string name = 1;
inline void FileInfo::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& FileInfo::name() const {
  // @@protoc_insertion_point(field_get:backend.FileInfo.name)
  return name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void FileInfo::set_name(const ::std::string& value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:backend.FileInfo.name)
}
inline void FileInfo::set_name(const char* value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:backend.FileInfo.name)
}
inline void FileInfo::set_name(const char* value, size_t size) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:backend.FileInfo.name)
}
inline ::std::string* FileInfo::mutable_name() {
  
  // @@protoc_insertion_point(field_mutable:backend.FileInfo.name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* FileInfo::release_name() {
  // @@protoc_insertion_point(field_release:backend.FileInfo.name)
  
  return name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void FileInfo::set_allocated_name(::std::string* name) {
  if (name != NULL) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:backend.FileInfo.name)
}

// optional bool is_file = 2;
inline void FileInfo::clear_is_file() {
  is_file_ = false;
}
inline bool FileInfo::is_file() const {
  // @@protoc_insertion_point(field_get:backend.FileInfo.is_file)
  return is_file_;
}
inline void FileInfo::set_is_file(bool value) {
  
  is_file_ = value;
  // @@protoc_insertion_point(field_set:backend.FileInfo.is_file)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace backend

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_backend_2eproto__INCLUDED