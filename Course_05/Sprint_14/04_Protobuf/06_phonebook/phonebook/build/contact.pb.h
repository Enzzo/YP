// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: contact.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_contact_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_contact_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_contact_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_contact_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_contact_2eproto;
namespace phone_book_serialize {
class Contact;
struct ContactDefaultTypeInternal;
extern ContactDefaultTypeInternal _Contact_default_instance_;
class ContactList;
struct ContactListDefaultTypeInternal;
extern ContactListDefaultTypeInternal _ContactList_default_instance_;
class Date;
struct DateDefaultTypeInternal;
extern DateDefaultTypeInternal _Date_default_instance_;
}  // namespace phone_book_serialize
PROTOBUF_NAMESPACE_OPEN
template<> ::phone_book_serialize::Contact* Arena::CreateMaybeMessage<::phone_book_serialize::Contact>(Arena*);
template<> ::phone_book_serialize::ContactList* Arena::CreateMaybeMessage<::phone_book_serialize::ContactList>(Arena*);
template<> ::phone_book_serialize::Date* Arena::CreateMaybeMessage<::phone_book_serialize::Date>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace phone_book_serialize {

// ===================================================================

class Date final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:phone_book_serialize.Date) */ {
 public:
  inline Date() : Date(nullptr) {}
  ~Date() override;
  explicit constexpr Date(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Date(const Date& from);
  Date(Date&& from) noexcept
    : Date() {
    *this = ::std::move(from);
  }

  inline Date& operator=(const Date& from) {
    CopyFrom(from);
    return *this;
  }
  inline Date& operator=(Date&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Date& default_instance() {
    return *internal_default_instance();
  }
  static inline const Date* internal_default_instance() {
    return reinterpret_cast<const Date*>(
               &_Date_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Date& a, Date& b) {
    a.Swap(&b);
  }
  inline void Swap(Date* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Date* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Date* New() const final {
    return new Date();
  }

  Date* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Date>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Date& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const Date& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Date* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "phone_book_serialize.Date";
  }
  protected:
  explicit Date(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kYearFieldNumber = 1,
    kMonthFieldNumber = 2,
    kDayFieldNumber = 3,
  };
  // int32 year = 1;
  void clear_year();
  ::PROTOBUF_NAMESPACE_ID::int32 year() const;
  void set_year(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_year() const;
  void _internal_set_year(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 month = 2;
  void clear_month();
  ::PROTOBUF_NAMESPACE_ID::int32 month() const;
  void set_month(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_month() const;
  void _internal_set_month(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 day = 3;
  void clear_day();
  ::PROTOBUF_NAMESPACE_ID::int32 day() const;
  void set_day(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_day() const;
  void _internal_set_day(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:phone_book_serialize.Date)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::int32 year_;
  ::PROTOBUF_NAMESPACE_ID::int32 month_;
  ::PROTOBUF_NAMESPACE_ID::int32 day_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_contact_2eproto;
};
// -------------------------------------------------------------------

class Contact final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:phone_book_serialize.Contact) */ {
 public:
  inline Contact() : Contact(nullptr) {}
  ~Contact() override;
  explicit constexpr Contact(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Contact(const Contact& from);
  Contact(Contact&& from) noexcept
    : Contact() {
    *this = ::std::move(from);
  }

  inline Contact& operator=(const Contact& from) {
    CopyFrom(from);
    return *this;
  }
  inline Contact& operator=(Contact&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Contact& default_instance() {
    return *internal_default_instance();
  }
  static inline const Contact* internal_default_instance() {
    return reinterpret_cast<const Contact*>(
               &_Contact_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(Contact& a, Contact& b) {
    a.Swap(&b);
  }
  inline void Swap(Contact* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Contact* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Contact* New() const final {
    return new Contact();
  }

  Contact* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Contact>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Contact& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const Contact& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Contact* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "phone_book_serialize.Contact";
  }
  protected:
  explicit Contact(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kPhoneNumberFieldNumber = 3,
    kNameFieldNumber = 1,
    kBirthdayFieldNumber = 2,
  };
  // repeated string phone_number = 3;
  int phone_number_size() const;
  private:
  int _internal_phone_number_size() const;
  public:
  void clear_phone_number();
  const std::string& phone_number(int index) const;
  std::string* mutable_phone_number(int index);
  void set_phone_number(int index, const std::string& value);
  void set_phone_number(int index, std::string&& value);
  void set_phone_number(int index, const char* value);
  void set_phone_number(int index, const char* value, size_t size);
  std::string* add_phone_number();
  void add_phone_number(const std::string& value);
  void add_phone_number(std::string&& value);
  void add_phone_number(const char* value);
  void add_phone_number(const char* value, size_t size);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>& phone_number() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>* mutable_phone_number();
  private:
  const std::string& _internal_phone_number(int index) const;
  std::string* _internal_add_phone_number();
  public:

  // string name = 1;
  void clear_name();
  const std::string& name() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_name(ArgT0&& arg0, ArgT... args);
  std::string* mutable_name();
  PROTOBUF_MUST_USE_RESULT std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // .phone_book_serialize.Date birthday = 2;
  bool has_birthday() const;
  private:
  bool _internal_has_birthday() const;
  public:
  void clear_birthday();
  const ::phone_book_serialize::Date& birthday() const;
  PROTOBUF_MUST_USE_RESULT ::phone_book_serialize::Date* release_birthday();
  ::phone_book_serialize::Date* mutable_birthday();
  void set_allocated_birthday(::phone_book_serialize::Date* birthday);
  private:
  const ::phone_book_serialize::Date& _internal_birthday() const;
  ::phone_book_serialize::Date* _internal_mutable_birthday();
  public:
  void unsafe_arena_set_allocated_birthday(
      ::phone_book_serialize::Date* birthday);
  ::phone_book_serialize::Date* unsafe_arena_release_birthday();

  // @@protoc_insertion_point(class_scope:phone_book_serialize.Contact)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string> phone_number_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::phone_book_serialize::Date* birthday_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_contact_2eproto;
};
// -------------------------------------------------------------------

class ContactList final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:phone_book_serialize.ContactList) */ {
 public:
  inline ContactList() : ContactList(nullptr) {}
  ~ContactList() override;
  explicit constexpr ContactList(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ContactList(const ContactList& from);
  ContactList(ContactList&& from) noexcept
    : ContactList() {
    *this = ::std::move(from);
  }

  inline ContactList& operator=(const ContactList& from) {
    CopyFrom(from);
    return *this;
  }
  inline ContactList& operator=(ContactList&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const ContactList& default_instance() {
    return *internal_default_instance();
  }
  static inline const ContactList* internal_default_instance() {
    return reinterpret_cast<const ContactList*>(
               &_ContactList_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(ContactList& a, ContactList& b) {
    a.Swap(&b);
  }
  inline void Swap(ContactList* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ContactList* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ContactList* New() const final {
    return new ContactList();
  }

  ContactList* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ContactList>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ContactList& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const ContactList& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ContactList* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "phone_book_serialize.ContactList";
  }
  protected:
  explicit ContactList(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kContactFieldNumber = 1,
  };
  // repeated .phone_book_serialize.Contact contact = 1;
  int contact_size() const;
  private:
  int _internal_contact_size() const;
  public:
  void clear_contact();
  ::phone_book_serialize::Contact* mutable_contact(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::phone_book_serialize::Contact >*
      mutable_contact();
  private:
  const ::phone_book_serialize::Contact& _internal_contact(int index) const;
  ::phone_book_serialize::Contact* _internal_add_contact();
  public:
  const ::phone_book_serialize::Contact& contact(int index) const;
  ::phone_book_serialize::Contact* add_contact();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::phone_book_serialize::Contact >&
      contact() const;

  // @@protoc_insertion_point(class_scope:phone_book_serialize.ContactList)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::phone_book_serialize::Contact > contact_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_contact_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Date

// int32 year = 1;
inline void Date::clear_year() {
  year_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Date::_internal_year() const {
  return year_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Date::year() const {
  // @@protoc_insertion_point(field_get:phone_book_serialize.Date.year)
  return _internal_year();
}
inline void Date::_internal_set_year(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  year_ = value;
}
inline void Date::set_year(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_year(value);
  // @@protoc_insertion_point(field_set:phone_book_serialize.Date.year)
}

// int32 month = 2;
inline void Date::clear_month() {
  month_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Date::_internal_month() const {
  return month_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Date::month() const {
  // @@protoc_insertion_point(field_get:phone_book_serialize.Date.month)
  return _internal_month();
}
inline void Date::_internal_set_month(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  month_ = value;
}
inline void Date::set_month(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_month(value);
  // @@protoc_insertion_point(field_set:phone_book_serialize.Date.month)
}

// int32 day = 3;
inline void Date::clear_day() {
  day_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Date::_internal_day() const {
  return day_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Date::day() const {
  // @@protoc_insertion_point(field_get:phone_book_serialize.Date.day)
  return _internal_day();
}
inline void Date::_internal_set_day(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  day_ = value;
}
inline void Date::set_day(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_day(value);
  // @@protoc_insertion_point(field_set:phone_book_serialize.Date.day)
}

// -------------------------------------------------------------------

// Contact

// string name = 1;
inline void Contact::clear_name() {
  name_.ClearToEmpty();
}
inline const std::string& Contact::name() const {
  // @@protoc_insertion_point(field_get:phone_book_serialize.Contact.name)
  return _internal_name();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Contact::set_name(ArgT0&& arg0, ArgT... args) {
 
 name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:phone_book_serialize.Contact.name)
}
inline std::string* Contact::mutable_name() {
  std::string* _s = _internal_mutable_name();
  // @@protoc_insertion_point(field_mutable:phone_book_serialize.Contact.name)
  return _s;
}
inline const std::string& Contact::_internal_name() const {
  return name_.Get();
}
inline void Contact::_internal_set_name(const std::string& value) {
  
  name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* Contact::_internal_mutable_name() {
  
  return name_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* Contact::release_name() {
  // @@protoc_insertion_point(field_release:phone_book_serialize.Contact.name)
  return name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void Contact::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:phone_book_serialize.Contact.name)
}

// .phone_book_serialize.Date birthday = 2;
inline bool Contact::_internal_has_birthday() const {
  return this != internal_default_instance() && birthday_ != nullptr;
}
inline bool Contact::has_birthday() const {
  return _internal_has_birthday();
}
inline void Contact::clear_birthday() {
  if (GetArenaForAllocation() == nullptr && birthday_ != nullptr) {
    delete birthday_;
  }
  birthday_ = nullptr;
}
inline const ::phone_book_serialize::Date& Contact::_internal_birthday() const {
  const ::phone_book_serialize::Date* p = birthday_;
  return p != nullptr ? *p : reinterpret_cast<const ::phone_book_serialize::Date&>(
      ::phone_book_serialize::_Date_default_instance_);
}
inline const ::phone_book_serialize::Date& Contact::birthday() const {
  // @@protoc_insertion_point(field_get:phone_book_serialize.Contact.birthday)
  return _internal_birthday();
}
inline void Contact::unsafe_arena_set_allocated_birthday(
    ::phone_book_serialize::Date* birthday) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(birthday_);
  }
  birthday_ = birthday;
  if (birthday) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:phone_book_serialize.Contact.birthday)
}
inline ::phone_book_serialize::Date* Contact::release_birthday() {
  
  ::phone_book_serialize::Date* temp = birthday_;
  birthday_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::phone_book_serialize::Date* Contact::unsafe_arena_release_birthday() {
  // @@protoc_insertion_point(field_release:phone_book_serialize.Contact.birthday)
  
  ::phone_book_serialize::Date* temp = birthday_;
  birthday_ = nullptr;
  return temp;
}
inline ::phone_book_serialize::Date* Contact::_internal_mutable_birthday() {
  
  if (birthday_ == nullptr) {
    auto* p = CreateMaybeMessage<::phone_book_serialize::Date>(GetArenaForAllocation());
    birthday_ = p;
  }
  return birthday_;
}
inline ::phone_book_serialize::Date* Contact::mutable_birthday() {
  ::phone_book_serialize::Date* _msg = _internal_mutable_birthday();
  // @@protoc_insertion_point(field_mutable:phone_book_serialize.Contact.birthday)
  return _msg;
}
inline void Contact::set_allocated_birthday(::phone_book_serialize::Date* birthday) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete birthday_;
  }
  if (birthday) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper<::phone_book_serialize::Date>::GetOwningArena(birthday);
    if (message_arena != submessage_arena) {
      birthday = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, birthday, submessage_arena);
    }
    
  } else {
    
  }
  birthday_ = birthday;
  // @@protoc_insertion_point(field_set_allocated:phone_book_serialize.Contact.birthday)
}

// repeated string phone_number = 3;
inline int Contact::_internal_phone_number_size() const {
  return phone_number_.size();
}
inline int Contact::phone_number_size() const {
  return _internal_phone_number_size();
}
inline void Contact::clear_phone_number() {
  phone_number_.Clear();
}
inline std::string* Contact::add_phone_number() {
  std::string* _s = _internal_add_phone_number();
  // @@protoc_insertion_point(field_add_mutable:phone_book_serialize.Contact.phone_number)
  return _s;
}
inline const std::string& Contact::_internal_phone_number(int index) const {
  return phone_number_.Get(index);
}
inline const std::string& Contact::phone_number(int index) const {
  // @@protoc_insertion_point(field_get:phone_book_serialize.Contact.phone_number)
  return _internal_phone_number(index);
}
inline std::string* Contact::mutable_phone_number(int index) {
  // @@protoc_insertion_point(field_mutable:phone_book_serialize.Contact.phone_number)
  return phone_number_.Mutable(index);
}
inline void Contact::set_phone_number(int index, const std::string& value) {
  phone_number_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set:phone_book_serialize.Contact.phone_number)
}
inline void Contact::set_phone_number(int index, std::string&& value) {
  phone_number_.Mutable(index)->assign(std::move(value));
  // @@protoc_insertion_point(field_set:phone_book_serialize.Contact.phone_number)
}
inline void Contact::set_phone_number(int index, const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  phone_number_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:phone_book_serialize.Contact.phone_number)
}
inline void Contact::set_phone_number(int index, const char* value, size_t size) {
  phone_number_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:phone_book_serialize.Contact.phone_number)
}
inline std::string* Contact::_internal_add_phone_number() {
  return phone_number_.Add();
}
inline void Contact::add_phone_number(const std::string& value) {
  phone_number_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:phone_book_serialize.Contact.phone_number)
}
inline void Contact::add_phone_number(std::string&& value) {
  phone_number_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:phone_book_serialize.Contact.phone_number)
}
inline void Contact::add_phone_number(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  phone_number_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:phone_book_serialize.Contact.phone_number)
}
inline void Contact::add_phone_number(const char* value, size_t size) {
  phone_number_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:phone_book_serialize.Contact.phone_number)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>&
Contact::phone_number() const {
  // @@protoc_insertion_point(field_list:phone_book_serialize.Contact.phone_number)
  return phone_number_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>*
Contact::mutable_phone_number() {
  // @@protoc_insertion_point(field_mutable_list:phone_book_serialize.Contact.phone_number)
  return &phone_number_;
}

// -------------------------------------------------------------------

// ContactList

// repeated .phone_book_serialize.Contact contact = 1;
inline int ContactList::_internal_contact_size() const {
  return contact_.size();
}
inline int ContactList::contact_size() const {
  return _internal_contact_size();
}
inline void ContactList::clear_contact() {
  contact_.Clear();
}
inline ::phone_book_serialize::Contact* ContactList::mutable_contact(int index) {
  // @@protoc_insertion_point(field_mutable:phone_book_serialize.ContactList.contact)
  return contact_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::phone_book_serialize::Contact >*
ContactList::mutable_contact() {
  // @@protoc_insertion_point(field_mutable_list:phone_book_serialize.ContactList.contact)
  return &contact_;
}
inline const ::phone_book_serialize::Contact& ContactList::_internal_contact(int index) const {
  return contact_.Get(index);
}
inline const ::phone_book_serialize::Contact& ContactList::contact(int index) const {
  // @@protoc_insertion_point(field_get:phone_book_serialize.ContactList.contact)
  return _internal_contact(index);
}
inline ::phone_book_serialize::Contact* ContactList::_internal_add_contact() {
  return contact_.Add();
}
inline ::phone_book_serialize::Contact* ContactList::add_contact() {
  ::phone_book_serialize::Contact* _add = _internal_add_contact();
  // @@protoc_insertion_point(field_add:phone_book_serialize.ContactList.contact)
  return _add;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::phone_book_serialize::Contact >&
ContactList::contact() const {
  // @@protoc_insertion_point(field_list:phone_book_serialize.ContactList.contact)
  return contact_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace phone_book_serialize

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_contact_2eproto