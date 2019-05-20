// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BakedDescriptor.proto

#ifndef PROTOBUF_INCLUDED_BakedDescriptor_2eproto
#define PROTOBUF_INCLUDED_BakedDescriptor_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "Vector.pb.h"
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_BakedDescriptor_2eproto 

namespace protobuf_BakedDescriptor_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_BakedDescriptor_2eproto
namespace Persistance {
class BakedDescriptor;
class BakedDescriptorDefaultTypeInternal;
extern BakedDescriptorDefaultTypeInternal _BakedDescriptor_default_instance_;
}  // namespace Persistance
namespace google {
namespace protobuf {
template<> ::Persistance::BakedDescriptor* Arena::CreateMaybeMessage<::Persistance::BakedDescriptor>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace Persistance {

// ===================================================================

class BakedDescriptor : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Persistance.BakedDescriptor) */ {
 public:
  BakedDescriptor();
  virtual ~BakedDescriptor();

  BakedDescriptor(const BakedDescriptor& from);

  inline BakedDescriptor& operator=(const BakedDescriptor& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  BakedDescriptor(BakedDescriptor&& from) noexcept
    : BakedDescriptor() {
    *this = ::std::move(from);
  }

  inline BakedDescriptor& operator=(BakedDescriptor&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const BakedDescriptor& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const BakedDescriptor* internal_default_instance() {
    return reinterpret_cast<const BakedDescriptor*>(
               &_BakedDescriptor_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(BakedDescriptor* other);
  friend void swap(BakedDescriptor& a, BakedDescriptor& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline BakedDescriptor* New() const final {
    return CreateMaybeMessage<BakedDescriptor>(NULL);
  }

  BakedDescriptor* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<BakedDescriptor>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const BakedDescriptor& from);
  void MergeFrom(const BakedDescriptor& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(BakedDescriptor* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // bytes grid = 1;
  void clear_grid();
  static const int kGridFieldNumber = 1;
  const ::std::string& grid() const;
  void set_grid(const ::std::string& value);
  #if LANG_CXX11
  void set_grid(::std::string&& value);
  #endif
  void set_grid(const char* value);
  void set_grid(const void* value, size_t size);
  ::std::string* mutable_grid();
  ::std::string* release_grid();
  void set_allocated_grid(::std::string* grid);

  // .Persistance.Vector3 position = 2;
  bool has_position() const;
  void clear_position();
  static const int kPositionFieldNumber = 2;
  private:
  const ::Persistance::Vector3& _internal_position() const;
  public:
  const ::Persistance::Vector3& position() const;
  ::Persistance::Vector3* release_position();
  ::Persistance::Vector3* mutable_position();
  void set_allocated_position(::Persistance::Vector3* position);

  // .Persistance.Vector3 direction = 3;
  bool has_direction() const;
  void clear_direction();
  static const int kDirectionFieldNumber = 3;
  private:
  const ::Persistance::Vector3& _internal_direction() const;
  public:
  const ::Persistance::Vector3& direction() const;
  ::Persistance::Vector3* release_direction();
  ::Persistance::Vector3* mutable_direction();
  void set_allocated_direction(::Persistance::Vector3* direction);

  // float power = 4;
  void clear_power();
  static const int kPowerFieldNumber = 4;
  float power() const;
  void set_power(float value);

  // @@protoc_insertion_point(class_scope:Persistance.BakedDescriptor)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr grid_;
  ::Persistance::Vector3* position_;
  ::Persistance::Vector3* direction_;
  float power_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_BakedDescriptor_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// BakedDescriptor

// bytes grid = 1;
inline void BakedDescriptor::clear_grid() {
  grid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& BakedDescriptor::grid() const {
  // @@protoc_insertion_point(field_get:Persistance.BakedDescriptor.grid)
  return grid_.GetNoArena();
}
inline void BakedDescriptor::set_grid(const ::std::string& value) {
  
  grid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Persistance.BakedDescriptor.grid)
}
#if LANG_CXX11
inline void BakedDescriptor::set_grid(::std::string&& value) {
  
  grid_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Persistance.BakedDescriptor.grid)
}
#endif
inline void BakedDescriptor::set_grid(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  grid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Persistance.BakedDescriptor.grid)
}
inline void BakedDescriptor::set_grid(const void* value, size_t size) {
  
  grid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Persistance.BakedDescriptor.grid)
}
inline ::std::string* BakedDescriptor::mutable_grid() {
  
  // @@protoc_insertion_point(field_mutable:Persistance.BakedDescriptor.grid)
  return grid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* BakedDescriptor::release_grid() {
  // @@protoc_insertion_point(field_release:Persistance.BakedDescriptor.grid)
  
  return grid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void BakedDescriptor::set_allocated_grid(::std::string* grid) {
  if (grid != NULL) {
    
  } else {
    
  }
  grid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), grid);
  // @@protoc_insertion_point(field_set_allocated:Persistance.BakedDescriptor.grid)
}

// .Persistance.Vector3 position = 2;
inline bool BakedDescriptor::has_position() const {
  return this != internal_default_instance() && position_ != NULL;
}
inline const ::Persistance::Vector3& BakedDescriptor::_internal_position() const {
  return *position_;
}
inline const ::Persistance::Vector3& BakedDescriptor::position() const {
  const ::Persistance::Vector3* p = position_;
  // @@protoc_insertion_point(field_get:Persistance.BakedDescriptor.position)
  return p != NULL ? *p : *reinterpret_cast<const ::Persistance::Vector3*>(
      &::Persistance::_Vector3_default_instance_);
}
inline ::Persistance::Vector3* BakedDescriptor::release_position() {
  // @@protoc_insertion_point(field_release:Persistance.BakedDescriptor.position)
  
  ::Persistance::Vector3* temp = position_;
  position_ = NULL;
  return temp;
}
inline ::Persistance::Vector3* BakedDescriptor::mutable_position() {
  
  if (position_ == NULL) {
    auto* p = CreateMaybeMessage<::Persistance::Vector3>(GetArenaNoVirtual());
    position_ = p;
  }
  // @@protoc_insertion_point(field_mutable:Persistance.BakedDescriptor.position)
  return position_;
}
inline void BakedDescriptor::set_allocated_position(::Persistance::Vector3* position) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete reinterpret_cast< ::google::protobuf::MessageLite*>(position_);
  }
  if (position) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      position = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, position, submessage_arena);
    }
    
  } else {
    
  }
  position_ = position;
  // @@protoc_insertion_point(field_set_allocated:Persistance.BakedDescriptor.position)
}

// .Persistance.Vector3 direction = 3;
inline bool BakedDescriptor::has_direction() const {
  return this != internal_default_instance() && direction_ != NULL;
}
inline const ::Persistance::Vector3& BakedDescriptor::_internal_direction() const {
  return *direction_;
}
inline const ::Persistance::Vector3& BakedDescriptor::direction() const {
  const ::Persistance::Vector3* p = direction_;
  // @@protoc_insertion_point(field_get:Persistance.BakedDescriptor.direction)
  return p != NULL ? *p : *reinterpret_cast<const ::Persistance::Vector3*>(
      &::Persistance::_Vector3_default_instance_);
}
inline ::Persistance::Vector3* BakedDescriptor::release_direction() {
  // @@protoc_insertion_point(field_release:Persistance.BakedDescriptor.direction)
  
  ::Persistance::Vector3* temp = direction_;
  direction_ = NULL;
  return temp;
}
inline ::Persistance::Vector3* BakedDescriptor::mutable_direction() {
  
  if (direction_ == NULL) {
    auto* p = CreateMaybeMessage<::Persistance::Vector3>(GetArenaNoVirtual());
    direction_ = p;
  }
  // @@protoc_insertion_point(field_mutable:Persistance.BakedDescriptor.direction)
  return direction_;
}
inline void BakedDescriptor::set_allocated_direction(::Persistance::Vector3* direction) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete reinterpret_cast< ::google::protobuf::MessageLite*>(direction_);
  }
  if (direction) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      direction = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, direction, submessage_arena);
    }
    
  } else {
    
  }
  direction_ = direction;
  // @@protoc_insertion_point(field_set_allocated:Persistance.BakedDescriptor.direction)
}

// float power = 4;
inline void BakedDescriptor::clear_power() {
  power_ = 0;
}
inline float BakedDescriptor::power() const {
  // @@protoc_insertion_point(field_get:Persistance.BakedDescriptor.power)
  return power_;
}
inline void BakedDescriptor::set_power(float value) {
  
  power_ = value;
  // @@protoc_insertion_point(field_set:Persistance.BakedDescriptor.power)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Persistance

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_BakedDescriptor_2eproto
