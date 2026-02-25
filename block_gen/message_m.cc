//
// Generated file, do not edit! Created by opp_msgtool 6.2 from message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "message_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace omnetpp {

Register_Class(ReportMessage)

ReportMessage::ReportMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

ReportMessage::ReportMessage(const ReportMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

ReportMessage::~ReportMessage()
{
}

ReportMessage& ReportMessage::operator=(const ReportMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ReportMessage::copy(const ReportMessage& other)
{
    this->SenderID = other.SenderID;
    this->Content = other.Content;
    this->HashValue = other.HashValue;
    this->Timestamp = other.Timestamp;
    this->Signature = other.Signature;
    this->Senderpublickey = other.Senderpublickey;
}

void ReportMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->SenderID);
    doParsimPacking(b,this->Content);
    doParsimPacking(b,this->HashValue);
    doParsimPacking(b,this->Timestamp);
    doParsimPacking(b,this->Signature);
    doParsimPacking(b,this->Senderpublickey);
}

void ReportMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->SenderID);
    doParsimUnpacking(b,this->Content);
    doParsimUnpacking(b,this->HashValue);
    doParsimUnpacking(b,this->Timestamp);
    doParsimUnpacking(b,this->Signature);
    doParsimUnpacking(b,this->Senderpublickey);
}

const char * ReportMessage::getSenderID() const
{
    return this->SenderID.c_str();
}

void ReportMessage::setSenderID(const char * SenderID)
{
    this->SenderID = SenderID;
}

const char * ReportMessage::getContent() const
{
    return this->Content.c_str();
}

void ReportMessage::setContent(const char * Content)
{
    this->Content = Content;
}

const char * ReportMessage::getHashValue() const
{
    return this->HashValue.c_str();
}

void ReportMessage::setHashValue(const char * HashValue)
{
    this->HashValue = HashValue;
}

long ReportMessage::getTimestamp() const
{
    return this->Timestamp;
}

void ReportMessage::setTimestamp(long Timestamp)
{
    this->Timestamp = Timestamp;
}

const char * ReportMessage::getSignature() const
{
    return this->Signature.c_str();
}

void ReportMessage::setSignature(const char * Signature)
{
    this->Signature = Signature;
}

const char * ReportMessage::getSenderpublickey() const
{
    return this->Senderpublickey.c_str();
}

void ReportMessage::setSenderpublickey(const char * Senderpublickey)
{
    this->Senderpublickey = Senderpublickey;
}

class ReportMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_SenderID,
        FIELD_Content,
        FIELD_HashValue,
        FIELD_Timestamp,
        FIELD_Signature,
        FIELD_Senderpublickey,
    };
  public:
    ReportMessageDescriptor();
    virtual ~ReportMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ReportMessageDescriptor)

ReportMessageDescriptor::ReportMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::ReportMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

ReportMessageDescriptor::~ReportMessageDescriptor()
{
    delete[] propertyNames;
}

bool ReportMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ReportMessage *>(obj)!=nullptr;
}

const char **ReportMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ReportMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ReportMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 6+base->getFieldCount() : 6;
}

unsigned int ReportMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_SenderID
        FD_ISEDITABLE,    // FIELD_Content
        FD_ISEDITABLE,    // FIELD_HashValue
        FD_ISEDITABLE,    // FIELD_Timestamp
        FD_ISEDITABLE,    // FIELD_Signature
        FD_ISEDITABLE,    // FIELD_Senderpublickey
    };
    return (field >= 0 && field < 6) ? fieldTypeFlags[field] : 0;
}

const char *ReportMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "SenderID",
        "Content",
        "HashValue",
        "Timestamp",
        "Signature",
        "Senderpublickey",
    };
    return (field >= 0 && field < 6) ? fieldNames[field] : nullptr;
}

int ReportMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "SenderID") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "Content") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "HashValue") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "Timestamp") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "Signature") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "Senderpublickey") == 0) return baseIndex + 5;
    return base ? base->findField(fieldName) : -1;
}

const char *ReportMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_SenderID
        "string",    // FIELD_Content
        "string",    // FIELD_HashValue
        "long",    // FIELD_Timestamp
        "string",    // FIELD_Signature
        "string",    // FIELD_Senderpublickey
    };
    return (field >= 0 && field < 6) ? fieldTypeStrings[field] : nullptr;
}

const char **ReportMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ReportMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ReportMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ReportMessage *pp = omnetpp::fromAnyPtr<ReportMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ReportMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ReportMessage *pp = omnetpp::fromAnyPtr<ReportMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ReportMessage'", field);
    }
}

const char *ReportMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ReportMessage *pp = omnetpp::fromAnyPtr<ReportMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ReportMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ReportMessage *pp = omnetpp::fromAnyPtr<ReportMessage>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: return oppstring2string(pp->getSenderID());
        case FIELD_Content: return oppstring2string(pp->getContent());
        case FIELD_HashValue: return oppstring2string(pp->getHashValue());
        case FIELD_Timestamp: return long2string(pp->getTimestamp());
        case FIELD_Signature: return oppstring2string(pp->getSignature());
        case FIELD_Senderpublickey: return oppstring2string(pp->getSenderpublickey());
        default: return "";
    }
}

void ReportMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ReportMessage *pp = omnetpp::fromAnyPtr<ReportMessage>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: pp->setSenderID((value)); break;
        case FIELD_Content: pp->setContent((value)); break;
        case FIELD_HashValue: pp->setHashValue((value)); break;
        case FIELD_Timestamp: pp->setTimestamp(string2long(value)); break;
        case FIELD_Signature: pp->setSignature((value)); break;
        case FIELD_Senderpublickey: pp->setSenderpublickey((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ReportMessage'", field);
    }
}

omnetpp::cValue ReportMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ReportMessage *pp = omnetpp::fromAnyPtr<ReportMessage>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: return pp->getSenderID();
        case FIELD_Content: return pp->getContent();
        case FIELD_HashValue: return pp->getHashValue();
        case FIELD_Timestamp: return (omnetpp::intval_t)(pp->getTimestamp());
        case FIELD_Signature: return pp->getSignature();
        case FIELD_Senderpublickey: return pp->getSenderpublickey();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ReportMessage' as cValue -- field index out of range?", field);
    }
}

void ReportMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ReportMessage *pp = omnetpp::fromAnyPtr<ReportMessage>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: pp->setSenderID(value.stringValue()); break;
        case FIELD_Content: pp->setContent(value.stringValue()); break;
        case FIELD_HashValue: pp->setHashValue(value.stringValue()); break;
        case FIELD_Timestamp: pp->setTimestamp(omnetpp::checked_int_cast<long>(value.intValue())); break;
        case FIELD_Signature: pp->setSignature(value.stringValue()); break;
        case FIELD_Senderpublickey: pp->setSenderpublickey(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ReportMessage'", field);
    }
}

const char *ReportMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ReportMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ReportMessage *pp = omnetpp::fromAnyPtr<ReportMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ReportMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ReportMessage *pp = omnetpp::fromAnyPtr<ReportMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ReportMessage'", field);
    }
}

Register_Class(JoinReq)

JoinReq::JoinReq(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
    take(&this->rm);
}

JoinReq::JoinReq(const JoinReq& other) : ::omnetpp::cMessage(other)
{
    take(&this->rm);
    copy(other);
}

JoinReq::~JoinReq()
{
    drop(&this->rm);
}

JoinReq& JoinReq::operator=(const JoinReq& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void JoinReq::copy(const JoinReq& other)
{
    this->deviceId = other.deviceId;
    this->rm = other.rm;
    this->rm.setName(other.rm.getName());
    this->cpu = other.cpu;
    this->memory = other.memory;
    this->IO = other.IO;
    this->sendTime = other.sendTime;
}

void JoinReq::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->deviceId);
    doParsimPacking(b,this->rm);
    doParsimPacking(b,this->cpu);
    doParsimPacking(b,this->memory);
    doParsimPacking(b,this->IO);
    doParsimPacking(b,this->sendTime);
}

void JoinReq::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->deviceId);
    doParsimUnpacking(b,this->rm);
    doParsimUnpacking(b,this->cpu);
    doParsimUnpacking(b,this->memory);
    doParsimUnpacking(b,this->IO);
    doParsimUnpacking(b,this->sendTime);
}

int JoinReq::getDeviceId() const
{
    return this->deviceId;
}

void JoinReq::setDeviceId(int deviceId)
{
    this->deviceId = deviceId;
}

const ReportMessage& JoinReq::getRm() const
{
    return this->rm;
}

void JoinReq::setRm(const ReportMessage& rm)
{
    this->rm = rm;
}

int JoinReq::getCpu() const
{
    return this->cpu;
}

void JoinReq::setCpu(int cpu)
{
    this->cpu = cpu;
}

int JoinReq::getMemory() const
{
    return this->memory;
}

void JoinReq::setMemory(int memory)
{
    this->memory = memory;
}

int JoinReq::getIO() const
{
    return this->IO;
}

void JoinReq::setIO(int IO)
{
    this->IO = IO;
}

simtime_t JoinReq::getSendTime() const
{
    return this->sendTime;
}

void JoinReq::setSendTime(simtime_t sendTime)
{
    this->sendTime = sendTime;
}

class JoinReqDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_deviceId,
        FIELD_rm,
        FIELD_cpu,
        FIELD_memory,
        FIELD_IO,
        FIELD_sendTime,
    };
  public:
    JoinReqDescriptor();
    virtual ~JoinReqDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(JoinReqDescriptor)

JoinReqDescriptor::JoinReqDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::JoinReq)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

JoinReqDescriptor::~JoinReqDescriptor()
{
    delete[] propertyNames;
}

bool JoinReqDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<JoinReq *>(obj)!=nullptr;
}

const char **JoinReqDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *JoinReqDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int JoinReqDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 6+base->getFieldCount() : 6;
}

unsigned int JoinReqDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_deviceId
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,    // FIELD_rm
        FD_ISEDITABLE,    // FIELD_cpu
        FD_ISEDITABLE,    // FIELD_memory
        FD_ISEDITABLE,    // FIELD_IO
        FD_ISEDITABLE,    // FIELD_sendTime
    };
    return (field >= 0 && field < 6) ? fieldTypeFlags[field] : 0;
}

const char *JoinReqDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "deviceId",
        "rm",
        "cpu",
        "memory",
        "IO",
        "sendTime",
    };
    return (field >= 0 && field < 6) ? fieldNames[field] : nullptr;
}

int JoinReqDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "deviceId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "rm") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "cpu") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "memory") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "IO") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 5;
    return base ? base->findField(fieldName) : -1;
}

const char *JoinReqDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_deviceId
        "omnetpp::ReportMessage",    // FIELD_rm
        "int",    // FIELD_cpu
        "int",    // FIELD_memory
        "int",    // FIELD_IO
        "omnetpp::simtime_t",    // FIELD_sendTime
    };
    return (field >= 0 && field < 6) ? fieldTypeStrings[field] : nullptr;
}

const char **JoinReqDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *JoinReqDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int JoinReqDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    JoinReq *pp = omnetpp::fromAnyPtr<JoinReq>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void JoinReqDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinReq *pp = omnetpp::fromAnyPtr<JoinReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'JoinReq'", field);
    }
}

const char *JoinReqDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    JoinReq *pp = omnetpp::fromAnyPtr<JoinReq>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string JoinReqDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    JoinReq *pp = omnetpp::fromAnyPtr<JoinReq>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: return long2string(pp->getDeviceId());
        case FIELD_rm: return pp->getRm().str();
        case FIELD_cpu: return long2string(pp->getCpu());
        case FIELD_memory: return long2string(pp->getMemory());
        case FIELD_IO: return long2string(pp->getIO());
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        default: return "";
    }
}

void JoinReqDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinReq *pp = omnetpp::fromAnyPtr<JoinReq>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: pp->setDeviceId(string2long(value)); break;
        case FIELD_cpu: pp->setCpu(string2long(value)); break;
        case FIELD_memory: pp->setMemory(string2long(value)); break;
        case FIELD_IO: pp->setIO(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'JoinReq'", field);
    }
}

omnetpp::cValue JoinReqDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    JoinReq *pp = omnetpp::fromAnyPtr<JoinReq>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: return pp->getDeviceId();
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        case FIELD_cpu: return pp->getCpu();
        case FIELD_memory: return pp->getMemory();
        case FIELD_IO: return pp->getIO();
        case FIELD_sendTime: return pp->getSendTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'JoinReq' as cValue -- field index out of range?", field);
    }
}

void JoinReqDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinReq *pp = omnetpp::fromAnyPtr<JoinReq>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: pp->setDeviceId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_cpu: pp->setCpu(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_memory: pp->setMemory(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_IO: pp->setIO(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'JoinReq'", field);
    }
}

const char *JoinReqDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_rm: return omnetpp::opp_typename(typeid(ReportMessage));
        default: return nullptr;
    };
}

omnetpp::any_ptr JoinReqDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    JoinReq *pp = omnetpp::fromAnyPtr<JoinReq>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void JoinReqDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinReq *pp = omnetpp::fromAnyPtr<JoinReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'JoinReq'", field);
    }
}

Register_Class(JoinVerifyReq)

JoinVerifyReq::JoinVerifyReq(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
    take(&this->rm);
}

JoinVerifyReq::JoinVerifyReq(const JoinVerifyReq& other) : ::omnetpp::cMessage(other)
{
    take(&this->rm);
    copy(other);
}

JoinVerifyReq::~JoinVerifyReq()
{
    drop(&this->rm);
}

JoinVerifyReq& JoinVerifyReq::operator=(const JoinVerifyReq& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void JoinVerifyReq::copy(const JoinVerifyReq& other)
{
    this->deviceId = other.deviceId;
    this->assignedPeerId = other.assignedPeerId;
    this->rm = other.rm;
    this->rm.setName(other.rm.getName());
    this->sendTime = other.sendTime;
}

void JoinVerifyReq::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->deviceId);
    doParsimPacking(b,this->assignedPeerId);
    doParsimPacking(b,this->rm);
    doParsimPacking(b,this->sendTime);
}

void JoinVerifyReq::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->deviceId);
    doParsimUnpacking(b,this->assignedPeerId);
    doParsimUnpacking(b,this->rm);
    doParsimUnpacking(b,this->sendTime);
}

int JoinVerifyReq::getDeviceId() const
{
    return this->deviceId;
}

void JoinVerifyReq::setDeviceId(int deviceId)
{
    this->deviceId = deviceId;
}

int JoinVerifyReq::getAssignedPeerId() const
{
    return this->assignedPeerId;
}

void JoinVerifyReq::setAssignedPeerId(int assignedPeerId)
{
    this->assignedPeerId = assignedPeerId;
}

const ReportMessage& JoinVerifyReq::getRm() const
{
    return this->rm;
}

void JoinVerifyReq::setRm(const ReportMessage& rm)
{
    this->rm = rm;
}

simtime_t JoinVerifyReq::getSendTime() const
{
    return this->sendTime;
}

void JoinVerifyReq::setSendTime(simtime_t sendTime)
{
    this->sendTime = sendTime;
}

class JoinVerifyReqDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_deviceId,
        FIELD_assignedPeerId,
        FIELD_rm,
        FIELD_sendTime,
    };
  public:
    JoinVerifyReqDescriptor();
    virtual ~JoinVerifyReqDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(JoinVerifyReqDescriptor)

JoinVerifyReqDescriptor::JoinVerifyReqDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::JoinVerifyReq)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

JoinVerifyReqDescriptor::~JoinVerifyReqDescriptor()
{
    delete[] propertyNames;
}

bool JoinVerifyReqDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<JoinVerifyReq *>(obj)!=nullptr;
}

const char **JoinVerifyReqDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *JoinVerifyReqDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int JoinVerifyReqDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int JoinVerifyReqDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_deviceId
        FD_ISEDITABLE,    // FIELD_assignedPeerId
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,    // FIELD_rm
        FD_ISEDITABLE,    // FIELD_sendTime
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *JoinVerifyReqDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "deviceId",
        "assignedPeerId",
        "rm",
        "sendTime",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int JoinVerifyReqDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "deviceId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "assignedPeerId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "rm") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *JoinVerifyReqDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_deviceId
        "int",    // FIELD_assignedPeerId
        "omnetpp::ReportMessage",    // FIELD_rm
        "omnetpp::simtime_t",    // FIELD_sendTime
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **JoinVerifyReqDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *JoinVerifyReqDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int JoinVerifyReqDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    JoinVerifyReq *pp = omnetpp::fromAnyPtr<JoinVerifyReq>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void JoinVerifyReqDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinVerifyReq *pp = omnetpp::fromAnyPtr<JoinVerifyReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'JoinVerifyReq'", field);
    }
}

const char *JoinVerifyReqDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    JoinVerifyReq *pp = omnetpp::fromAnyPtr<JoinVerifyReq>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string JoinVerifyReqDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    JoinVerifyReq *pp = omnetpp::fromAnyPtr<JoinVerifyReq>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: return long2string(pp->getDeviceId());
        case FIELD_assignedPeerId: return long2string(pp->getAssignedPeerId());
        case FIELD_rm: return pp->getRm().str();
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        default: return "";
    }
}

void JoinVerifyReqDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinVerifyReq *pp = omnetpp::fromAnyPtr<JoinVerifyReq>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: pp->setDeviceId(string2long(value)); break;
        case FIELD_assignedPeerId: pp->setAssignedPeerId(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'JoinVerifyReq'", field);
    }
}

omnetpp::cValue JoinVerifyReqDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    JoinVerifyReq *pp = omnetpp::fromAnyPtr<JoinVerifyReq>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: return pp->getDeviceId();
        case FIELD_assignedPeerId: return pp->getAssignedPeerId();
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        case FIELD_sendTime: return pp->getSendTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'JoinVerifyReq' as cValue -- field index out of range?", field);
    }
}

void JoinVerifyReqDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinVerifyReq *pp = omnetpp::fromAnyPtr<JoinVerifyReq>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: pp->setDeviceId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_assignedPeerId: pp->setAssignedPeerId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'JoinVerifyReq'", field);
    }
}

const char *JoinVerifyReqDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_rm: return omnetpp::opp_typename(typeid(ReportMessage));
        default: return nullptr;
    };
}

omnetpp::any_ptr JoinVerifyReqDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    JoinVerifyReq *pp = omnetpp::fromAnyPtr<JoinVerifyReq>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void JoinVerifyReqDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinVerifyReq *pp = omnetpp::fromAnyPtr<JoinVerifyReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'JoinVerifyReq'", field);
    }
}

Register_Class(JoinAck)

JoinAck::JoinAck(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

JoinAck::JoinAck(const JoinAck& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

JoinAck::~JoinAck()
{
}

JoinAck& JoinAck::operator=(const JoinAck& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void JoinAck::copy(const JoinAck& other)
{
    this->deviceId = other.deviceId;
    this->peerId = other.peerId;
    this->success = other.success;
    this->time = other.time;
}

void JoinAck::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->deviceId);
    doParsimPacking(b,this->peerId);
    doParsimPacking(b,this->success);
    doParsimPacking(b,this->time);
}

void JoinAck::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->deviceId);
    doParsimUnpacking(b,this->peerId);
    doParsimUnpacking(b,this->success);
    doParsimUnpacking(b,this->time);
}

int JoinAck::getDeviceId() const
{
    return this->deviceId;
}

void JoinAck::setDeviceId(int deviceId)
{
    this->deviceId = deviceId;
}

int JoinAck::getPeerId() const
{
    return this->peerId;
}

void JoinAck::setPeerId(int peerId)
{
    this->peerId = peerId;
}

bool JoinAck::getSuccess() const
{
    return this->success;
}

void JoinAck::setSuccess(bool success)
{
    this->success = success;
}

simtime_t JoinAck::getTime() const
{
    return this->time;
}

void JoinAck::setTime(simtime_t time)
{
    this->time = time;
}

class JoinAckDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_deviceId,
        FIELD_peerId,
        FIELD_success,
        FIELD_time,
    };
  public:
    JoinAckDescriptor();
    virtual ~JoinAckDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(JoinAckDescriptor)

JoinAckDescriptor::JoinAckDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::JoinAck)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

JoinAckDescriptor::~JoinAckDescriptor()
{
    delete[] propertyNames;
}

bool JoinAckDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<JoinAck *>(obj)!=nullptr;
}

const char **JoinAckDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *JoinAckDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int JoinAckDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int JoinAckDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_deviceId
        FD_ISEDITABLE,    // FIELD_peerId
        FD_ISEDITABLE,    // FIELD_success
        FD_ISEDITABLE,    // FIELD_time
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *JoinAckDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "deviceId",
        "peerId",
        "success",
        "time",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int JoinAckDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "deviceId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "peerId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "success") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "time") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *JoinAckDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_deviceId
        "int",    // FIELD_peerId
        "bool",    // FIELD_success
        "omnetpp::simtime_t",    // FIELD_time
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **JoinAckDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *JoinAckDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int JoinAckDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    JoinAck *pp = omnetpp::fromAnyPtr<JoinAck>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void JoinAckDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinAck *pp = omnetpp::fromAnyPtr<JoinAck>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'JoinAck'", field);
    }
}

const char *JoinAckDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    JoinAck *pp = omnetpp::fromAnyPtr<JoinAck>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string JoinAckDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    JoinAck *pp = omnetpp::fromAnyPtr<JoinAck>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: return long2string(pp->getDeviceId());
        case FIELD_peerId: return long2string(pp->getPeerId());
        case FIELD_success: return bool2string(pp->getSuccess());
        case FIELD_time: return simtime2string(pp->getTime());
        default: return "";
    }
}

void JoinAckDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinAck *pp = omnetpp::fromAnyPtr<JoinAck>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: pp->setDeviceId(string2long(value)); break;
        case FIELD_peerId: pp->setPeerId(string2long(value)); break;
        case FIELD_success: pp->setSuccess(string2bool(value)); break;
        case FIELD_time: pp->setTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'JoinAck'", field);
    }
}

omnetpp::cValue JoinAckDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    JoinAck *pp = omnetpp::fromAnyPtr<JoinAck>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: return pp->getDeviceId();
        case FIELD_peerId: return pp->getPeerId();
        case FIELD_success: return pp->getSuccess();
        case FIELD_time: return pp->getTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'JoinAck' as cValue -- field index out of range?", field);
    }
}

void JoinAckDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinAck *pp = omnetpp::fromAnyPtr<JoinAck>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: pp->setDeviceId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_peerId: pp->setPeerId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_success: pp->setSuccess(value.boolValue()); break;
        case FIELD_time: pp->setTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'JoinAck'", field);
    }
}

const char *JoinAckDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr JoinAckDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    JoinAck *pp = omnetpp::fromAnyPtr<JoinAck>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void JoinAckDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    JoinAck *pp = omnetpp::fromAnyPtr<JoinAck>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'JoinAck'", field);
    }
}

Register_Class(PeerJoinReq)

PeerJoinReq::PeerJoinReq(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
    take(&this->rm);
}

PeerJoinReq::PeerJoinReq(const PeerJoinReq& other) : ::omnetpp::cMessage(other)
{
    take(&this->rm);
    copy(other);
}

PeerJoinReq::~PeerJoinReq()
{
    drop(&this->rm);
}

PeerJoinReq& PeerJoinReq::operator=(const PeerJoinReq& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void PeerJoinReq::copy(const PeerJoinReq& other)
{
    this->peerId = other.peerId;
    this->rm = other.rm;
    this->rm.setName(other.rm.getName());
    this->cpu = other.cpu;
    this->memory = other.memory;
    this->IO = other.IO;
    this->sendTime = other.sendTime;
}

void PeerJoinReq::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->peerId);
    doParsimPacking(b,this->rm);
    doParsimPacking(b,this->cpu);
    doParsimPacking(b,this->memory);
    doParsimPacking(b,this->IO);
    doParsimPacking(b,this->sendTime);
}

void PeerJoinReq::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->peerId);
    doParsimUnpacking(b,this->rm);
    doParsimUnpacking(b,this->cpu);
    doParsimUnpacking(b,this->memory);
    doParsimUnpacking(b,this->IO);
    doParsimUnpacking(b,this->sendTime);
}

int PeerJoinReq::getPeerId() const
{
    return this->peerId;
}

void PeerJoinReq::setPeerId(int peerId)
{
    this->peerId = peerId;
}

const ReportMessage& PeerJoinReq::getRm() const
{
    return this->rm;
}

void PeerJoinReq::setRm(const ReportMessage& rm)
{
    this->rm = rm;
}

int PeerJoinReq::getCpu() const
{
    return this->cpu;
}

void PeerJoinReq::setCpu(int cpu)
{
    this->cpu = cpu;
}

int PeerJoinReq::getMemory() const
{
    return this->memory;
}

void PeerJoinReq::setMemory(int memory)
{
    this->memory = memory;
}

int PeerJoinReq::getIO() const
{
    return this->IO;
}

void PeerJoinReq::setIO(int IO)
{
    this->IO = IO;
}

simtime_t PeerJoinReq::getSendTime() const
{
    return this->sendTime;
}

void PeerJoinReq::setSendTime(simtime_t sendTime)
{
    this->sendTime = sendTime;
}

class PeerJoinReqDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_peerId,
        FIELD_rm,
        FIELD_cpu,
        FIELD_memory,
        FIELD_IO,
        FIELD_sendTime,
    };
  public:
    PeerJoinReqDescriptor();
    virtual ~PeerJoinReqDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(PeerJoinReqDescriptor)

PeerJoinReqDescriptor::PeerJoinReqDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::PeerJoinReq)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

PeerJoinReqDescriptor::~PeerJoinReqDescriptor()
{
    delete[] propertyNames;
}

bool PeerJoinReqDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PeerJoinReq *>(obj)!=nullptr;
}

const char **PeerJoinReqDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *PeerJoinReqDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int PeerJoinReqDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 6+base->getFieldCount() : 6;
}

unsigned int PeerJoinReqDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_peerId
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,    // FIELD_rm
        FD_ISEDITABLE,    // FIELD_cpu
        FD_ISEDITABLE,    // FIELD_memory
        FD_ISEDITABLE,    // FIELD_IO
        FD_ISEDITABLE,    // FIELD_sendTime
    };
    return (field >= 0 && field < 6) ? fieldTypeFlags[field] : 0;
}

const char *PeerJoinReqDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "peerId",
        "rm",
        "cpu",
        "memory",
        "IO",
        "sendTime",
    };
    return (field >= 0 && field < 6) ? fieldNames[field] : nullptr;
}

int PeerJoinReqDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "peerId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "rm") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "cpu") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "memory") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "IO") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 5;
    return base ? base->findField(fieldName) : -1;
}

const char *PeerJoinReqDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_peerId
        "omnetpp::ReportMessage",    // FIELD_rm
        "int",    // FIELD_cpu
        "int",    // FIELD_memory
        "int",    // FIELD_IO
        "omnetpp::simtime_t",    // FIELD_sendTime
    };
    return (field >= 0 && field < 6) ? fieldTypeStrings[field] : nullptr;
}

const char **PeerJoinReqDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PeerJoinReqDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PeerJoinReqDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    PeerJoinReq *pp = omnetpp::fromAnyPtr<PeerJoinReq>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void PeerJoinReqDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinReq *pp = omnetpp::fromAnyPtr<PeerJoinReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'PeerJoinReq'", field);
    }
}

const char *PeerJoinReqDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    PeerJoinReq *pp = omnetpp::fromAnyPtr<PeerJoinReq>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PeerJoinReqDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    PeerJoinReq *pp = omnetpp::fromAnyPtr<PeerJoinReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: return long2string(pp->getPeerId());
        case FIELD_rm: return pp->getRm().str();
        case FIELD_cpu: return long2string(pp->getCpu());
        case FIELD_memory: return long2string(pp->getMemory());
        case FIELD_IO: return long2string(pp->getIO());
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        default: return "";
    }
}

void PeerJoinReqDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinReq *pp = omnetpp::fromAnyPtr<PeerJoinReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: pp->setPeerId(string2long(value)); break;
        case FIELD_cpu: pp->setCpu(string2long(value)); break;
        case FIELD_memory: pp->setMemory(string2long(value)); break;
        case FIELD_IO: pp->setIO(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerJoinReq'", field);
    }
}

omnetpp::cValue PeerJoinReqDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    PeerJoinReq *pp = omnetpp::fromAnyPtr<PeerJoinReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: return pp->getPeerId();
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        case FIELD_cpu: return pp->getCpu();
        case FIELD_memory: return pp->getMemory();
        case FIELD_IO: return pp->getIO();
        case FIELD_sendTime: return pp->getSendTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'PeerJoinReq' as cValue -- field index out of range?", field);
    }
}

void PeerJoinReqDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinReq *pp = omnetpp::fromAnyPtr<PeerJoinReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: pp->setPeerId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_cpu: pp->setCpu(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_memory: pp->setMemory(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_IO: pp->setIO(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerJoinReq'", field);
    }
}

const char *PeerJoinReqDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_rm: return omnetpp::opp_typename(typeid(ReportMessage));
        default: return nullptr;
    };
}

omnetpp::any_ptr PeerJoinReqDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    PeerJoinReq *pp = omnetpp::fromAnyPtr<PeerJoinReq>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void PeerJoinReqDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinReq *pp = omnetpp::fromAnyPtr<PeerJoinReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerJoinReq'", field);
    }
}

Register_Class(PeerJoinVerifyReq)

PeerJoinVerifyReq::PeerJoinVerifyReq(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
    take(&this->rm);
}

PeerJoinVerifyReq::PeerJoinVerifyReq(const PeerJoinVerifyReq& other) : ::omnetpp::cMessage(other)
{
    take(&this->rm);
    copy(other);
}

PeerJoinVerifyReq::~PeerJoinVerifyReq()
{
    drop(&this->rm);
}

PeerJoinVerifyReq& PeerJoinVerifyReq::operator=(const PeerJoinVerifyReq& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void PeerJoinVerifyReq::copy(const PeerJoinVerifyReq& other)
{
    this->peerId = other.peerId;
    this->rm = other.rm;
    this->rm.setName(other.rm.getName());
    this->sendTime = other.sendTime;
}

void PeerJoinVerifyReq::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->peerId);
    doParsimPacking(b,this->rm);
    doParsimPacking(b,this->sendTime);
}

void PeerJoinVerifyReq::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->peerId);
    doParsimUnpacking(b,this->rm);
    doParsimUnpacking(b,this->sendTime);
}

int PeerJoinVerifyReq::getPeerId() const
{
    return this->peerId;
}

void PeerJoinVerifyReq::setPeerId(int peerId)
{
    this->peerId = peerId;
}

const ReportMessage& PeerJoinVerifyReq::getRm() const
{
    return this->rm;
}

void PeerJoinVerifyReq::setRm(const ReportMessage& rm)
{
    this->rm = rm;
}

simtime_t PeerJoinVerifyReq::getSendTime() const
{
    return this->sendTime;
}

void PeerJoinVerifyReq::setSendTime(simtime_t sendTime)
{
    this->sendTime = sendTime;
}

class PeerJoinVerifyReqDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_peerId,
        FIELD_rm,
        FIELD_sendTime,
    };
  public:
    PeerJoinVerifyReqDescriptor();
    virtual ~PeerJoinVerifyReqDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(PeerJoinVerifyReqDescriptor)

PeerJoinVerifyReqDescriptor::PeerJoinVerifyReqDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::PeerJoinVerifyReq)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

PeerJoinVerifyReqDescriptor::~PeerJoinVerifyReqDescriptor()
{
    delete[] propertyNames;
}

bool PeerJoinVerifyReqDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PeerJoinVerifyReq *>(obj)!=nullptr;
}

const char **PeerJoinVerifyReqDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *PeerJoinVerifyReqDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int PeerJoinVerifyReqDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int PeerJoinVerifyReqDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_peerId
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,    // FIELD_rm
        FD_ISEDITABLE,    // FIELD_sendTime
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *PeerJoinVerifyReqDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "peerId",
        "rm",
        "sendTime",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int PeerJoinVerifyReqDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "peerId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "rm") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *PeerJoinVerifyReqDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_peerId
        "omnetpp::ReportMessage",    // FIELD_rm
        "omnetpp::simtime_t",    // FIELD_sendTime
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **PeerJoinVerifyReqDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PeerJoinVerifyReqDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PeerJoinVerifyReqDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    PeerJoinVerifyReq *pp = omnetpp::fromAnyPtr<PeerJoinVerifyReq>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void PeerJoinVerifyReqDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinVerifyReq *pp = omnetpp::fromAnyPtr<PeerJoinVerifyReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'PeerJoinVerifyReq'", field);
    }
}

const char *PeerJoinVerifyReqDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    PeerJoinVerifyReq *pp = omnetpp::fromAnyPtr<PeerJoinVerifyReq>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PeerJoinVerifyReqDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    PeerJoinVerifyReq *pp = omnetpp::fromAnyPtr<PeerJoinVerifyReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: return long2string(pp->getPeerId());
        case FIELD_rm: return pp->getRm().str();
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        default: return "";
    }
}

void PeerJoinVerifyReqDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinVerifyReq *pp = omnetpp::fromAnyPtr<PeerJoinVerifyReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: pp->setPeerId(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerJoinVerifyReq'", field);
    }
}

omnetpp::cValue PeerJoinVerifyReqDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    PeerJoinVerifyReq *pp = omnetpp::fromAnyPtr<PeerJoinVerifyReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: return pp->getPeerId();
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        case FIELD_sendTime: return pp->getSendTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'PeerJoinVerifyReq' as cValue -- field index out of range?", field);
    }
}

void PeerJoinVerifyReqDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinVerifyReq *pp = omnetpp::fromAnyPtr<PeerJoinVerifyReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: pp->setPeerId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerJoinVerifyReq'", field);
    }
}

const char *PeerJoinVerifyReqDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_rm: return omnetpp::opp_typename(typeid(ReportMessage));
        default: return nullptr;
    };
}

omnetpp::any_ptr PeerJoinVerifyReqDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    PeerJoinVerifyReq *pp = omnetpp::fromAnyPtr<PeerJoinVerifyReq>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void PeerJoinVerifyReqDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinVerifyReq *pp = omnetpp::fromAnyPtr<PeerJoinVerifyReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerJoinVerifyReq'", field);
    }
}

Register_Class(PeerJoinAck)

PeerJoinAck::PeerJoinAck(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

PeerJoinAck::PeerJoinAck(const PeerJoinAck& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

PeerJoinAck::~PeerJoinAck()
{
}

PeerJoinAck& PeerJoinAck::operator=(const PeerJoinAck& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void PeerJoinAck::copy(const PeerJoinAck& other)
{
    this->peerId = other.peerId;
    this->success = other.success;
    this->time = other.time;
}

void PeerJoinAck::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->peerId);
    doParsimPacking(b,this->success);
    doParsimPacking(b,this->time);
}

void PeerJoinAck::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->peerId);
    doParsimUnpacking(b,this->success);
    doParsimUnpacking(b,this->time);
}

int PeerJoinAck::getPeerId() const
{
    return this->peerId;
}

void PeerJoinAck::setPeerId(int peerId)
{
    this->peerId = peerId;
}

bool PeerJoinAck::getSuccess() const
{
    return this->success;
}

void PeerJoinAck::setSuccess(bool success)
{
    this->success = success;
}

simtime_t PeerJoinAck::getTime() const
{
    return this->time;
}

void PeerJoinAck::setTime(simtime_t time)
{
    this->time = time;
}

class PeerJoinAckDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_peerId,
        FIELD_success,
        FIELD_time,
    };
  public:
    PeerJoinAckDescriptor();
    virtual ~PeerJoinAckDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(PeerJoinAckDescriptor)

PeerJoinAckDescriptor::PeerJoinAckDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::PeerJoinAck)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

PeerJoinAckDescriptor::~PeerJoinAckDescriptor()
{
    delete[] propertyNames;
}

bool PeerJoinAckDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PeerJoinAck *>(obj)!=nullptr;
}

const char **PeerJoinAckDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *PeerJoinAckDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int PeerJoinAckDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int PeerJoinAckDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_peerId
        FD_ISEDITABLE,    // FIELD_success
        FD_ISEDITABLE,    // FIELD_time
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *PeerJoinAckDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "peerId",
        "success",
        "time",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int PeerJoinAckDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "peerId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "success") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "time") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *PeerJoinAckDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_peerId
        "bool",    // FIELD_success
        "omnetpp::simtime_t",    // FIELD_time
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **PeerJoinAckDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PeerJoinAckDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PeerJoinAckDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    PeerJoinAck *pp = omnetpp::fromAnyPtr<PeerJoinAck>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void PeerJoinAckDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinAck *pp = omnetpp::fromAnyPtr<PeerJoinAck>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'PeerJoinAck'", field);
    }
}

const char *PeerJoinAckDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    PeerJoinAck *pp = omnetpp::fromAnyPtr<PeerJoinAck>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PeerJoinAckDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    PeerJoinAck *pp = omnetpp::fromAnyPtr<PeerJoinAck>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: return long2string(pp->getPeerId());
        case FIELD_success: return bool2string(pp->getSuccess());
        case FIELD_time: return simtime2string(pp->getTime());
        default: return "";
    }
}

void PeerJoinAckDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinAck *pp = omnetpp::fromAnyPtr<PeerJoinAck>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: pp->setPeerId(string2long(value)); break;
        case FIELD_success: pp->setSuccess(string2bool(value)); break;
        case FIELD_time: pp->setTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerJoinAck'", field);
    }
}

omnetpp::cValue PeerJoinAckDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    PeerJoinAck *pp = omnetpp::fromAnyPtr<PeerJoinAck>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: return pp->getPeerId();
        case FIELD_success: return pp->getSuccess();
        case FIELD_time: return pp->getTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'PeerJoinAck' as cValue -- field index out of range?", field);
    }
}

void PeerJoinAckDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinAck *pp = omnetpp::fromAnyPtr<PeerJoinAck>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: pp->setPeerId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_success: pp->setSuccess(value.boolValue()); break;
        case FIELD_time: pp->setTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerJoinAck'", field);
    }
}

const char *PeerJoinAckDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr PeerJoinAckDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    PeerJoinAck *pp = omnetpp::fromAnyPtr<PeerJoinAck>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void PeerJoinAckDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerJoinAck *pp = omnetpp::fromAnyPtr<PeerJoinAck>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerJoinAck'", field);
    }
}

Register_Class(SyncLedgerReq)

SyncLedgerReq::SyncLedgerReq(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

SyncLedgerReq::SyncLedgerReq(const SyncLedgerReq& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

SyncLedgerReq::~SyncLedgerReq()
{
}

SyncLedgerReq& SyncLedgerReq::operator=(const SyncLedgerReq& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void SyncLedgerReq::copy(const SyncLedgerReq& other)
{
    this->peerId = other.peerId;
    this->fromHeight = other.fromHeight;
    this->sendTime = other.sendTime;
}

void SyncLedgerReq::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->peerId);
    doParsimPacking(b,this->fromHeight);
    doParsimPacking(b,this->sendTime);
}

void SyncLedgerReq::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->peerId);
    doParsimUnpacking(b,this->fromHeight);
    doParsimUnpacking(b,this->sendTime);
}

int SyncLedgerReq::getPeerId() const
{
    return this->peerId;
}

void SyncLedgerReq::setPeerId(int peerId)
{
    this->peerId = peerId;
}

int SyncLedgerReq::getFromHeight() const
{
    return this->fromHeight;
}

void SyncLedgerReq::setFromHeight(int fromHeight)
{
    this->fromHeight = fromHeight;
}

simtime_t SyncLedgerReq::getSendTime() const
{
    return this->sendTime;
}

void SyncLedgerReq::setSendTime(simtime_t sendTime)
{
    this->sendTime = sendTime;
}

class SyncLedgerReqDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_peerId,
        FIELD_fromHeight,
        FIELD_sendTime,
    };
  public:
    SyncLedgerReqDescriptor();
    virtual ~SyncLedgerReqDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(SyncLedgerReqDescriptor)

SyncLedgerReqDescriptor::SyncLedgerReqDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::SyncLedgerReq)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

SyncLedgerReqDescriptor::~SyncLedgerReqDescriptor()
{
    delete[] propertyNames;
}

bool SyncLedgerReqDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SyncLedgerReq *>(obj)!=nullptr;
}

const char **SyncLedgerReqDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *SyncLedgerReqDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int SyncLedgerReqDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int SyncLedgerReqDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_peerId
        FD_ISEDITABLE,    // FIELD_fromHeight
        FD_ISEDITABLE,    // FIELD_sendTime
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *SyncLedgerReqDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "peerId",
        "fromHeight",
        "sendTime",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int SyncLedgerReqDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "peerId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "fromHeight") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *SyncLedgerReqDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_peerId
        "int",    // FIELD_fromHeight
        "omnetpp::simtime_t",    // FIELD_sendTime
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **SyncLedgerReqDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SyncLedgerReqDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SyncLedgerReqDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    SyncLedgerReq *pp = omnetpp::fromAnyPtr<SyncLedgerReq>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void SyncLedgerReqDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    SyncLedgerReq *pp = omnetpp::fromAnyPtr<SyncLedgerReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'SyncLedgerReq'", field);
    }
}

const char *SyncLedgerReqDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    SyncLedgerReq *pp = omnetpp::fromAnyPtr<SyncLedgerReq>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SyncLedgerReqDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    SyncLedgerReq *pp = omnetpp::fromAnyPtr<SyncLedgerReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: return long2string(pp->getPeerId());
        case FIELD_fromHeight: return long2string(pp->getFromHeight());
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        default: return "";
    }
}

void SyncLedgerReqDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    SyncLedgerReq *pp = omnetpp::fromAnyPtr<SyncLedgerReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: pp->setPeerId(string2long(value)); break;
        case FIELD_fromHeight: pp->setFromHeight(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SyncLedgerReq'", field);
    }
}

omnetpp::cValue SyncLedgerReqDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    SyncLedgerReq *pp = omnetpp::fromAnyPtr<SyncLedgerReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: return pp->getPeerId();
        case FIELD_fromHeight: return pp->getFromHeight();
        case FIELD_sendTime: return pp->getSendTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'SyncLedgerReq' as cValue -- field index out of range?", field);
    }
}

void SyncLedgerReqDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    SyncLedgerReq *pp = omnetpp::fromAnyPtr<SyncLedgerReq>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: pp->setPeerId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_fromHeight: pp->setFromHeight(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SyncLedgerReq'", field);
    }
}

const char *SyncLedgerReqDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr SyncLedgerReqDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    SyncLedgerReq *pp = omnetpp::fromAnyPtr<SyncLedgerReq>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void SyncLedgerReqDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    SyncLedgerReq *pp = omnetpp::fromAnyPtr<SyncLedgerReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SyncLedgerReq'", field);
    }
}

Register_Class(ReportEntry)

ReportEntry::ReportEntry(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
    take(&this->rm);
}

ReportEntry::ReportEntry(const ReportEntry& other) : ::omnetpp::cMessage(other)
{
    take(&this->rm);
    copy(other);
}

ReportEntry::~ReportEntry()
{
    drop(&this->rm);
}

ReportEntry& ReportEntry::operator=(const ReportEntry& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ReportEntry::copy(const ReportEntry& other)
{
    this->rm = other.rm;
    this->rm.setName(other.rm.getName());
    this->state = other.state;
}

void ReportEntry::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->rm);
    doParsimPacking(b,this->state);
}

void ReportEntry::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->rm);
    doParsimUnpacking(b,this->state);
}

const ReportMessage& ReportEntry::getRm() const
{
    return this->rm;
}

void ReportEntry::setRm(const ReportMessage& rm)
{
    this->rm = rm;
}

int ReportEntry::getState() const
{
    return this->state;
}

void ReportEntry::setState(int state)
{
    this->state = state;
}

class ReportEntryDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_rm,
        FIELD_state,
    };
  public:
    ReportEntryDescriptor();
    virtual ~ReportEntryDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ReportEntryDescriptor)

ReportEntryDescriptor::ReportEntryDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::ReportEntry)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

ReportEntryDescriptor::~ReportEntryDescriptor()
{
    delete[] propertyNames;
}

bool ReportEntryDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ReportEntry *>(obj)!=nullptr;
}

const char **ReportEntryDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ReportEntryDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ReportEntryDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int ReportEntryDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,    // FIELD_rm
        FD_ISEDITABLE,    // FIELD_state
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *ReportEntryDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "rm",
        "state",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int ReportEntryDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "rm") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "state") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *ReportEntryDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "omnetpp::ReportMessage",    // FIELD_rm
        "int",    // FIELD_state
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **ReportEntryDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ReportEntryDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ReportEntryDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ReportEntry *pp = omnetpp::fromAnyPtr<ReportEntry>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ReportEntryDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ReportEntry *pp = omnetpp::fromAnyPtr<ReportEntry>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ReportEntry'", field);
    }
}

const char *ReportEntryDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ReportEntry *pp = omnetpp::fromAnyPtr<ReportEntry>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ReportEntryDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ReportEntry *pp = omnetpp::fromAnyPtr<ReportEntry>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return pp->getRm().str();
        case FIELD_state: return long2string(pp->getState());
        default: return "";
    }
}

void ReportEntryDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ReportEntry *pp = omnetpp::fromAnyPtr<ReportEntry>(object); (void)pp;
    switch (field) {
        case FIELD_state: pp->setState(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ReportEntry'", field);
    }
}

omnetpp::cValue ReportEntryDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ReportEntry *pp = omnetpp::fromAnyPtr<ReportEntry>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        case FIELD_state: return pp->getState();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ReportEntry' as cValue -- field index out of range?", field);
    }
}

void ReportEntryDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ReportEntry *pp = omnetpp::fromAnyPtr<ReportEntry>(object); (void)pp;
    switch (field) {
        case FIELD_state: pp->setState(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ReportEntry'", field);
    }
}

const char *ReportEntryDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_rm: return omnetpp::opp_typename(typeid(ReportMessage));
        default: return nullptr;
    };
}

omnetpp::any_ptr ReportEntryDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ReportEntry *pp = omnetpp::fromAnyPtr<ReportEntry>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ReportEntryDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ReportEntry *pp = omnetpp::fromAnyPtr<ReportEntry>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ReportEntry'", field);
    }
}

Register_Class(SyncBlockMsg)

SyncBlockMsg::SyncBlockMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

SyncBlockMsg::SyncBlockMsg(const SyncBlockMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

SyncBlockMsg::~SyncBlockMsg()
{
    for (size_t i = 0; i < entries_arraysize; i++)
        drop(&this->entries[i]);
    delete [] this->entries;
}

SyncBlockMsg& SyncBlockMsg::operator=(const SyncBlockMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void SyncBlockMsg::copy(const SyncBlockMsg& other)
{
    this->peerId = other.peerId;
    this->blockNum = other.blockNum;
    this->previousHash = other.previousHash;
    this->blockHash = other.blockHash;
    this->timestamp = other.timestamp;
    this->lastHeight = other.lastHeight;
    for (size_t i = 0; i < entries_arraysize; i++)
        drop(&this->entries[i]);
    delete [] this->entries;
    this->entries = (other.entries_arraysize==0) ? nullptr : new ReportEntry[other.entries_arraysize];
    entries_arraysize = other.entries_arraysize;
    for (size_t i = 0; i < entries_arraysize; i++) {
        this->entries[i] = other.entries[i];
        this->entries[i].setName(other.entries[i].getName());
        take(&this->entries[i]);
    }
}

void SyncBlockMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->peerId);
    doParsimPacking(b,this->blockNum);
    doParsimPacking(b,this->previousHash);
    doParsimPacking(b,this->blockHash);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->lastHeight);
    b->pack(entries_arraysize);
    doParsimArrayPacking(b,this->entries,entries_arraysize);
}

void SyncBlockMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->peerId);
    doParsimUnpacking(b,this->blockNum);
    doParsimUnpacking(b,this->previousHash);
    doParsimUnpacking(b,this->blockHash);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->lastHeight);
    delete [] this->entries;
    b->unpack(entries_arraysize);
    if (entries_arraysize == 0) {
        this->entries = nullptr;
    } else {
        this->entries = new ReportEntry[entries_arraysize];
        doParsimArrayUnpacking(b,this->entries,entries_arraysize);
    }
}

int SyncBlockMsg::getPeerId() const
{
    return this->peerId;
}

void SyncBlockMsg::setPeerId(int peerId)
{
    this->peerId = peerId;
}

int SyncBlockMsg::getBlockNum() const
{
    return this->blockNum;
}

void SyncBlockMsg::setBlockNum(int blockNum)
{
    this->blockNum = blockNum;
}

const char * SyncBlockMsg::getPreviousHash() const
{
    return this->previousHash.c_str();
}

void SyncBlockMsg::setPreviousHash(const char * previousHash)
{
    this->previousHash = previousHash;
}

const char * SyncBlockMsg::getBlockHash() const
{
    return this->blockHash.c_str();
}

void SyncBlockMsg::setBlockHash(const char * blockHash)
{
    this->blockHash = blockHash;
}

long SyncBlockMsg::getTimestamp() const
{
    return this->timestamp;
}

void SyncBlockMsg::setTimestamp(long timestamp)
{
    this->timestamp = timestamp;
}

int SyncBlockMsg::getLastHeight() const
{
    return this->lastHeight;
}

void SyncBlockMsg::setLastHeight(int lastHeight)
{
    this->lastHeight = lastHeight;
}

size_t SyncBlockMsg::getEntriesArraySize() const
{
    return entries_arraysize;
}

const ReportEntry& SyncBlockMsg::getEntries(size_t k) const
{
    if (k >= entries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)entries_arraysize, (unsigned long)k);
    return this->entries[k];
}

void SyncBlockMsg::setEntriesArraySize(size_t newSize)
{
    ReportEntry *entries2 = (newSize==0) ? nullptr : new ReportEntry[newSize];
    size_t minSize = entries_arraysize < newSize ? entries_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        entries2[i] = this->entries[i];
    for (size_t i = 0; i < entries_arraysize; i++)
        drop(&this->entries[i]);
    delete [] this->entries;
    this->entries = entries2;
    entries_arraysize = newSize;
    for (size_t i = 0; i < entries_arraysize; i++)
        take(&this->entries[i]);
}

void SyncBlockMsg::setEntries(size_t k, const ReportEntry& entries)
{
    if (k >= entries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)entries_arraysize, (unsigned long)k);
    this->entries[k] = entries;
}

void SyncBlockMsg::insertEntries(size_t k, const ReportEntry& entries)
{
    if (k > entries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)entries_arraysize, (unsigned long)k);
    size_t newSize = entries_arraysize + 1;
    ReportEntry *entries2 = new ReportEntry[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        entries2[i] = this->entries[i];
    entries2[k] = entries;
    for (i = k + 1; i < newSize; i++)
        entries2[i] = this->entries[i-1];
    for (size_t i = 0; i < entries_arraysize; i++)
        drop(&this->entries[i]);
    delete [] this->entries;
    this->entries = entries2;
    entries_arraysize = newSize;
    for (size_t i = 0; i < entries_arraysize; i++)
        take(&this->entries[i]);
}

void SyncBlockMsg::appendEntries(const ReportEntry& entries)
{
    insertEntries(entries_arraysize, entries);
}

void SyncBlockMsg::eraseEntries(size_t k)
{
    if (k >= entries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)entries_arraysize, (unsigned long)k);
    size_t newSize = entries_arraysize - 1;
    ReportEntry *entries2 = (newSize == 0) ? nullptr : new ReportEntry[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        entries2[i] = this->entries[i];
    for (i = k; i < newSize; i++)
        entries2[i] = this->entries[i+1];
    for (size_t i = 0; i < entries_arraysize; i++)
        drop(&this->entries[i]);
    delete [] this->entries;
    this->entries = entries2;
    entries_arraysize = newSize;
    for (size_t i = 0; i < entries_arraysize; i++)
        take(&this->entries[i]);
}

class SyncBlockMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_peerId,
        FIELD_blockNum,
        FIELD_previousHash,
        FIELD_blockHash,
        FIELD_timestamp,
        FIELD_lastHeight,
        FIELD_entries,
    };
  public:
    SyncBlockMsgDescriptor();
    virtual ~SyncBlockMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(SyncBlockMsgDescriptor)

SyncBlockMsgDescriptor::SyncBlockMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::SyncBlockMsg)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

SyncBlockMsgDescriptor::~SyncBlockMsgDescriptor()
{
    delete[] propertyNames;
}

bool SyncBlockMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SyncBlockMsg *>(obj)!=nullptr;
}

const char **SyncBlockMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *SyncBlockMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int SyncBlockMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 7+base->getFieldCount() : 7;
}

unsigned int SyncBlockMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_peerId
        FD_ISEDITABLE,    // FIELD_blockNum
        FD_ISEDITABLE,    // FIELD_previousHash
        FD_ISEDITABLE,    // FIELD_blockHash
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_lastHeight
        FD_ISARRAY | FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT | FD_ISRESIZABLE,    // FIELD_entries
    };
    return (field >= 0 && field < 7) ? fieldTypeFlags[field] : 0;
}

const char *SyncBlockMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "peerId",
        "blockNum",
        "previousHash",
        "blockHash",
        "timestamp",
        "lastHeight",
        "entries",
    };
    return (field >= 0 && field < 7) ? fieldNames[field] : nullptr;
}

int SyncBlockMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "peerId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "blockNum") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "previousHash") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "blockHash") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "lastHeight") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "entries") == 0) return baseIndex + 6;
    return base ? base->findField(fieldName) : -1;
}

const char *SyncBlockMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_peerId
        "int",    // FIELD_blockNum
        "string",    // FIELD_previousHash
        "string",    // FIELD_blockHash
        "long",    // FIELD_timestamp
        "int",    // FIELD_lastHeight
        "omnetpp::ReportEntry",    // FIELD_entries
    };
    return (field >= 0 && field < 7) ? fieldTypeStrings[field] : nullptr;
}

const char **SyncBlockMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SyncBlockMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SyncBlockMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    SyncBlockMsg *pp = omnetpp::fromAnyPtr<SyncBlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_entries: return pp->getEntriesArraySize();
        default: return 0;
    }
}

void SyncBlockMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    SyncBlockMsg *pp = omnetpp::fromAnyPtr<SyncBlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_entries: pp->setEntriesArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'SyncBlockMsg'", field);
    }
}

const char *SyncBlockMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    SyncBlockMsg *pp = omnetpp::fromAnyPtr<SyncBlockMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SyncBlockMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    SyncBlockMsg *pp = omnetpp::fromAnyPtr<SyncBlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: return long2string(pp->getPeerId());
        case FIELD_blockNum: return long2string(pp->getBlockNum());
        case FIELD_previousHash: return oppstring2string(pp->getPreviousHash());
        case FIELD_blockHash: return oppstring2string(pp->getBlockHash());
        case FIELD_timestamp: return long2string(pp->getTimestamp());
        case FIELD_lastHeight: return long2string(pp->getLastHeight());
        case FIELD_entries: return pp->getEntries(i).str();
        default: return "";
    }
}

void SyncBlockMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    SyncBlockMsg *pp = omnetpp::fromAnyPtr<SyncBlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: pp->setPeerId(string2long(value)); break;
        case FIELD_blockNum: pp->setBlockNum(string2long(value)); break;
        case FIELD_previousHash: pp->setPreviousHash((value)); break;
        case FIELD_blockHash: pp->setBlockHash((value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2long(value)); break;
        case FIELD_lastHeight: pp->setLastHeight(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SyncBlockMsg'", field);
    }
}

omnetpp::cValue SyncBlockMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    SyncBlockMsg *pp = omnetpp::fromAnyPtr<SyncBlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: return pp->getPeerId();
        case FIELD_blockNum: return pp->getBlockNum();
        case FIELD_previousHash: return pp->getPreviousHash();
        case FIELD_blockHash: return pp->getBlockHash();
        case FIELD_timestamp: return (omnetpp::intval_t)(pp->getTimestamp());
        case FIELD_lastHeight: return pp->getLastHeight();
        case FIELD_entries: return omnetpp::toAnyPtr(&pp->getEntries(i)); break;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'SyncBlockMsg' as cValue -- field index out of range?", field);
    }
}

void SyncBlockMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    SyncBlockMsg *pp = omnetpp::fromAnyPtr<SyncBlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_peerId: pp->setPeerId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_blockNum: pp->setBlockNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_previousHash: pp->setPreviousHash(value.stringValue()); break;
        case FIELD_blockHash: pp->setBlockHash(value.stringValue()); break;
        case FIELD_timestamp: pp->setTimestamp(omnetpp::checked_int_cast<long>(value.intValue())); break;
        case FIELD_lastHeight: pp->setLastHeight(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SyncBlockMsg'", field);
    }
}

const char *SyncBlockMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_entries: return omnetpp::opp_typename(typeid(ReportEntry));
        default: return nullptr;
    };
}

omnetpp::any_ptr SyncBlockMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    SyncBlockMsg *pp = omnetpp::fromAnyPtr<SyncBlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_entries: return omnetpp::toAnyPtr(&pp->getEntries(i)); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void SyncBlockMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    SyncBlockMsg *pp = omnetpp::fromAnyPtr<SyncBlockMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SyncBlockMsg'", field);
    }
}

Register_Class(LeaveReq)

LeaveReq::LeaveReq(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

LeaveReq::LeaveReq(const LeaveReq& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

LeaveReq::~LeaveReq()
{
}

LeaveReq& LeaveReq::operator=(const LeaveReq& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void LeaveReq::copy(const LeaveReq& other)
{
    this->SenderID = other.SenderID;
    this->deviceId = other.deviceId;
    this->sendTime = other.sendTime;
}

void LeaveReq::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->SenderID);
    doParsimPacking(b,this->deviceId);
    doParsimPacking(b,this->sendTime);
}

void LeaveReq::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->SenderID);
    doParsimUnpacking(b,this->deviceId);
    doParsimUnpacking(b,this->sendTime);
}

const char * LeaveReq::getSenderID() const
{
    return this->SenderID.c_str();
}

void LeaveReq::setSenderID(const char * SenderID)
{
    this->SenderID = SenderID;
}

int LeaveReq::getDeviceId() const
{
    return this->deviceId;
}

void LeaveReq::setDeviceId(int deviceId)
{
    this->deviceId = deviceId;
}

simtime_t LeaveReq::getSendTime() const
{
    return this->sendTime;
}

void LeaveReq::setSendTime(simtime_t sendTime)
{
    this->sendTime = sendTime;
}

class LeaveReqDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_SenderID,
        FIELD_deviceId,
        FIELD_sendTime,
    };
  public:
    LeaveReqDescriptor();
    virtual ~LeaveReqDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(LeaveReqDescriptor)

LeaveReqDescriptor::LeaveReqDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::LeaveReq)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

LeaveReqDescriptor::~LeaveReqDescriptor()
{
    delete[] propertyNames;
}

bool LeaveReqDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LeaveReq *>(obj)!=nullptr;
}

const char **LeaveReqDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *LeaveReqDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int LeaveReqDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int LeaveReqDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_SenderID
        FD_ISEDITABLE,    // FIELD_deviceId
        FD_ISEDITABLE,    // FIELD_sendTime
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *LeaveReqDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "SenderID",
        "deviceId",
        "sendTime",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int LeaveReqDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "SenderID") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "deviceId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *LeaveReqDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_SenderID
        "int",    // FIELD_deviceId
        "omnetpp::simtime_t",    // FIELD_sendTime
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **LeaveReqDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *LeaveReqDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int LeaveReqDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    LeaveReq *pp = omnetpp::fromAnyPtr<LeaveReq>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void LeaveReqDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    LeaveReq *pp = omnetpp::fromAnyPtr<LeaveReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LeaveReq'", field);
    }
}

const char *LeaveReqDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    LeaveReq *pp = omnetpp::fromAnyPtr<LeaveReq>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LeaveReqDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    LeaveReq *pp = omnetpp::fromAnyPtr<LeaveReq>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: return oppstring2string(pp->getSenderID());
        case FIELD_deviceId: return long2string(pp->getDeviceId());
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        default: return "";
    }
}

void LeaveReqDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LeaveReq *pp = omnetpp::fromAnyPtr<LeaveReq>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: pp->setSenderID((value)); break;
        case FIELD_deviceId: pp->setDeviceId(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LeaveReq'", field);
    }
}

omnetpp::cValue LeaveReqDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    LeaveReq *pp = omnetpp::fromAnyPtr<LeaveReq>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: return pp->getSenderID();
        case FIELD_deviceId: return pp->getDeviceId();
        case FIELD_sendTime: return pp->getSendTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'LeaveReq' as cValue -- field index out of range?", field);
    }
}

void LeaveReqDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LeaveReq *pp = omnetpp::fromAnyPtr<LeaveReq>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: pp->setSenderID(value.stringValue()); break;
        case FIELD_deviceId: pp->setDeviceId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LeaveReq'", field);
    }
}

const char *LeaveReqDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr LeaveReqDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    LeaveReq *pp = omnetpp::fromAnyPtr<LeaveReq>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void LeaveReqDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    LeaveReq *pp = omnetpp::fromAnyPtr<LeaveReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LeaveReq'", field);
    }
}

Register_Class(LeaveAck)

LeaveAck::LeaveAck(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

LeaveAck::LeaveAck(const LeaveAck& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

LeaveAck::~LeaveAck()
{
}

LeaveAck& LeaveAck::operator=(const LeaveAck& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void LeaveAck::copy(const LeaveAck& other)
{
    this->SenderID = other.SenderID;
    this->deviceId = other.deviceId;
    this->ackTime = other.ackTime;
}

void LeaveAck::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->SenderID);
    doParsimPacking(b,this->deviceId);
    doParsimPacking(b,this->ackTime);
}

void LeaveAck::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->SenderID);
    doParsimUnpacking(b,this->deviceId);
    doParsimUnpacking(b,this->ackTime);
}

const char * LeaveAck::getSenderID() const
{
    return this->SenderID.c_str();
}

void LeaveAck::setSenderID(const char * SenderID)
{
    this->SenderID = SenderID;
}

int LeaveAck::getDeviceId() const
{
    return this->deviceId;
}

void LeaveAck::setDeviceId(int deviceId)
{
    this->deviceId = deviceId;
}

simtime_t LeaveAck::getAckTime() const
{
    return this->ackTime;
}

void LeaveAck::setAckTime(simtime_t ackTime)
{
    this->ackTime = ackTime;
}

class LeaveAckDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_SenderID,
        FIELD_deviceId,
        FIELD_ackTime,
    };
  public:
    LeaveAckDescriptor();
    virtual ~LeaveAckDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(LeaveAckDescriptor)

LeaveAckDescriptor::LeaveAckDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::LeaveAck)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

LeaveAckDescriptor::~LeaveAckDescriptor()
{
    delete[] propertyNames;
}

bool LeaveAckDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LeaveAck *>(obj)!=nullptr;
}

const char **LeaveAckDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *LeaveAckDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int LeaveAckDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int LeaveAckDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_SenderID
        FD_ISEDITABLE,    // FIELD_deviceId
        FD_ISEDITABLE,    // FIELD_ackTime
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *LeaveAckDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "SenderID",
        "deviceId",
        "ackTime",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int LeaveAckDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "SenderID") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "deviceId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "ackTime") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *LeaveAckDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_SenderID
        "int",    // FIELD_deviceId
        "omnetpp::simtime_t",    // FIELD_ackTime
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **LeaveAckDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *LeaveAckDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int LeaveAckDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    LeaveAck *pp = omnetpp::fromAnyPtr<LeaveAck>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void LeaveAckDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    LeaveAck *pp = omnetpp::fromAnyPtr<LeaveAck>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LeaveAck'", field);
    }
}

const char *LeaveAckDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    LeaveAck *pp = omnetpp::fromAnyPtr<LeaveAck>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LeaveAckDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    LeaveAck *pp = omnetpp::fromAnyPtr<LeaveAck>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: return oppstring2string(pp->getSenderID());
        case FIELD_deviceId: return long2string(pp->getDeviceId());
        case FIELD_ackTime: return simtime2string(pp->getAckTime());
        default: return "";
    }
}

void LeaveAckDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LeaveAck *pp = omnetpp::fromAnyPtr<LeaveAck>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: pp->setSenderID((value)); break;
        case FIELD_deviceId: pp->setDeviceId(string2long(value)); break;
        case FIELD_ackTime: pp->setAckTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LeaveAck'", field);
    }
}

omnetpp::cValue LeaveAckDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    LeaveAck *pp = omnetpp::fromAnyPtr<LeaveAck>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: return pp->getSenderID();
        case FIELD_deviceId: return pp->getDeviceId();
        case FIELD_ackTime: return pp->getAckTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'LeaveAck' as cValue -- field index out of range?", field);
    }
}

void LeaveAckDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LeaveAck *pp = omnetpp::fromAnyPtr<LeaveAck>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: pp->setSenderID(value.stringValue()); break;
        case FIELD_deviceId: pp->setDeviceId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_ackTime: pp->setAckTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LeaveAck'", field);
    }
}

const char *LeaveAckDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr LeaveAckDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    LeaveAck *pp = omnetpp::fromAnyPtr<LeaveAck>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void LeaveAckDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    LeaveAck *pp = omnetpp::fromAnyPtr<LeaveAck>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LeaveAck'", field);
    }
}

Register_Class(PeerLeaveReq)

PeerLeaveReq::PeerLeaveReq(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

PeerLeaveReq::PeerLeaveReq(const PeerLeaveReq& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

PeerLeaveReq::~PeerLeaveReq()
{
}

PeerLeaveReq& PeerLeaveReq::operator=(const PeerLeaveReq& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void PeerLeaveReq::copy(const PeerLeaveReq& other)
{
    this->SenderID = other.SenderID;
    this->peerId = other.peerId;
    this->sendTime = other.sendTime;
}

void PeerLeaveReq::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->SenderID);
    doParsimPacking(b,this->peerId);
    doParsimPacking(b,this->sendTime);
}

void PeerLeaveReq::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->SenderID);
    doParsimUnpacking(b,this->peerId);
    doParsimUnpacking(b,this->sendTime);
}

const char * PeerLeaveReq::getSenderID() const
{
    return this->SenderID.c_str();
}

void PeerLeaveReq::setSenderID(const char * SenderID)
{
    this->SenderID = SenderID;
}

int PeerLeaveReq::getPeerId() const
{
    return this->peerId;
}

void PeerLeaveReq::setPeerId(int peerId)
{
    this->peerId = peerId;
}

simtime_t PeerLeaveReq::getSendTime() const
{
    return this->sendTime;
}

void PeerLeaveReq::setSendTime(simtime_t sendTime)
{
    this->sendTime = sendTime;
}

class PeerLeaveReqDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_SenderID,
        FIELD_peerId,
        FIELD_sendTime,
    };
  public:
    PeerLeaveReqDescriptor();
    virtual ~PeerLeaveReqDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(PeerLeaveReqDescriptor)

PeerLeaveReqDescriptor::PeerLeaveReqDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::PeerLeaveReq)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

PeerLeaveReqDescriptor::~PeerLeaveReqDescriptor()
{
    delete[] propertyNames;
}

bool PeerLeaveReqDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PeerLeaveReq *>(obj)!=nullptr;
}

const char **PeerLeaveReqDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *PeerLeaveReqDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int PeerLeaveReqDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int PeerLeaveReqDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_SenderID
        FD_ISEDITABLE,    // FIELD_peerId
        FD_ISEDITABLE,    // FIELD_sendTime
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *PeerLeaveReqDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "SenderID",
        "peerId",
        "sendTime",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int PeerLeaveReqDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "SenderID") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "peerId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *PeerLeaveReqDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_SenderID
        "int",    // FIELD_peerId
        "omnetpp::simtime_t",    // FIELD_sendTime
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **PeerLeaveReqDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PeerLeaveReqDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PeerLeaveReqDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    PeerLeaveReq *pp = omnetpp::fromAnyPtr<PeerLeaveReq>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void PeerLeaveReqDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerLeaveReq *pp = omnetpp::fromAnyPtr<PeerLeaveReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'PeerLeaveReq'", field);
    }
}

const char *PeerLeaveReqDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    PeerLeaveReq *pp = omnetpp::fromAnyPtr<PeerLeaveReq>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PeerLeaveReqDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    PeerLeaveReq *pp = omnetpp::fromAnyPtr<PeerLeaveReq>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: return oppstring2string(pp->getSenderID());
        case FIELD_peerId: return long2string(pp->getPeerId());
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        default: return "";
    }
}

void PeerLeaveReqDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerLeaveReq *pp = omnetpp::fromAnyPtr<PeerLeaveReq>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: pp->setSenderID((value)); break;
        case FIELD_peerId: pp->setPeerId(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerLeaveReq'", field);
    }
}

omnetpp::cValue PeerLeaveReqDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    PeerLeaveReq *pp = omnetpp::fromAnyPtr<PeerLeaveReq>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: return pp->getSenderID();
        case FIELD_peerId: return pp->getPeerId();
        case FIELD_sendTime: return pp->getSendTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'PeerLeaveReq' as cValue -- field index out of range?", field);
    }
}

void PeerLeaveReqDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerLeaveReq *pp = omnetpp::fromAnyPtr<PeerLeaveReq>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: pp->setSenderID(value.stringValue()); break;
        case FIELD_peerId: pp->setPeerId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerLeaveReq'", field);
    }
}

const char *PeerLeaveReqDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr PeerLeaveReqDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    PeerLeaveReq *pp = omnetpp::fromAnyPtr<PeerLeaveReq>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void PeerLeaveReqDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerLeaveReq *pp = omnetpp::fromAnyPtr<PeerLeaveReq>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerLeaveReq'", field);
    }
}

Register_Class(ReJoin)

ReJoin::ReJoin(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

ReJoin::ReJoin(const ReJoin& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

ReJoin::~ReJoin()
{
}

ReJoin& ReJoin::operator=(const ReJoin& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ReJoin::copy(const ReJoin& other)
{
    this->deviceId = other.deviceId;
    this->sendTime = other.sendTime;
}

void ReJoin::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->deviceId);
    doParsimPacking(b,this->sendTime);
}

void ReJoin::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->deviceId);
    doParsimUnpacking(b,this->sendTime);
}

int ReJoin::getDeviceId() const
{
    return this->deviceId;
}

void ReJoin::setDeviceId(int deviceId)
{
    this->deviceId = deviceId;
}

simtime_t ReJoin::getSendTime() const
{
    return this->sendTime;
}

void ReJoin::setSendTime(simtime_t sendTime)
{
    this->sendTime = sendTime;
}

class ReJoinDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_deviceId,
        FIELD_sendTime,
    };
  public:
    ReJoinDescriptor();
    virtual ~ReJoinDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ReJoinDescriptor)

ReJoinDescriptor::ReJoinDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::ReJoin)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

ReJoinDescriptor::~ReJoinDescriptor()
{
    delete[] propertyNames;
}

bool ReJoinDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ReJoin *>(obj)!=nullptr;
}

const char **ReJoinDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ReJoinDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ReJoinDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int ReJoinDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_deviceId
        FD_ISEDITABLE,    // FIELD_sendTime
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *ReJoinDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "deviceId",
        "sendTime",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int ReJoinDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "deviceId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *ReJoinDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_deviceId
        "omnetpp::simtime_t",    // FIELD_sendTime
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **ReJoinDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ReJoinDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ReJoinDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ReJoin *pp = omnetpp::fromAnyPtr<ReJoin>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ReJoinDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ReJoin *pp = omnetpp::fromAnyPtr<ReJoin>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ReJoin'", field);
    }
}

const char *ReJoinDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ReJoin *pp = omnetpp::fromAnyPtr<ReJoin>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ReJoinDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ReJoin *pp = omnetpp::fromAnyPtr<ReJoin>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: return long2string(pp->getDeviceId());
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        default: return "";
    }
}

void ReJoinDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ReJoin *pp = omnetpp::fromAnyPtr<ReJoin>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: pp->setDeviceId(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ReJoin'", field);
    }
}

omnetpp::cValue ReJoinDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ReJoin *pp = omnetpp::fromAnyPtr<ReJoin>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: return pp->getDeviceId();
        case FIELD_sendTime: return pp->getSendTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ReJoin' as cValue -- field index out of range?", field);
    }
}

void ReJoinDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ReJoin *pp = omnetpp::fromAnyPtr<ReJoin>(object); (void)pp;
    switch (field) {
        case FIELD_deviceId: pp->setDeviceId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ReJoin'", field);
    }
}

const char *ReJoinDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ReJoinDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ReJoin *pp = omnetpp::fromAnyPtr<ReJoin>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ReJoinDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ReJoin *pp = omnetpp::fromAnyPtr<ReJoin>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ReJoin'", field);
    }
}

Register_Class(PeerLeaveAck)

PeerLeaveAck::PeerLeaveAck(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

PeerLeaveAck::PeerLeaveAck(const PeerLeaveAck& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

PeerLeaveAck::~PeerLeaveAck()
{
}

PeerLeaveAck& PeerLeaveAck::operator=(const PeerLeaveAck& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void PeerLeaveAck::copy(const PeerLeaveAck& other)
{
    this->SenderID = other.SenderID;
    this->peerId = other.peerId;
    this->ackTime = other.ackTime;
}

void PeerLeaveAck::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->SenderID);
    doParsimPacking(b,this->peerId);
    doParsimPacking(b,this->ackTime);
}

void PeerLeaveAck::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->SenderID);
    doParsimUnpacking(b,this->peerId);
    doParsimUnpacking(b,this->ackTime);
}

const char * PeerLeaveAck::getSenderID() const
{
    return this->SenderID.c_str();
}

void PeerLeaveAck::setSenderID(const char * SenderID)
{
    this->SenderID = SenderID;
}

int PeerLeaveAck::getPeerId() const
{
    return this->peerId;
}

void PeerLeaveAck::setPeerId(int peerId)
{
    this->peerId = peerId;
}

simtime_t PeerLeaveAck::getAckTime() const
{
    return this->ackTime;
}

void PeerLeaveAck::setAckTime(simtime_t ackTime)
{
    this->ackTime = ackTime;
}

class PeerLeaveAckDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_SenderID,
        FIELD_peerId,
        FIELD_ackTime,
    };
  public:
    PeerLeaveAckDescriptor();
    virtual ~PeerLeaveAckDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(PeerLeaveAckDescriptor)

PeerLeaveAckDescriptor::PeerLeaveAckDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::PeerLeaveAck)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

PeerLeaveAckDescriptor::~PeerLeaveAckDescriptor()
{
    delete[] propertyNames;
}

bool PeerLeaveAckDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PeerLeaveAck *>(obj)!=nullptr;
}

const char **PeerLeaveAckDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *PeerLeaveAckDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int PeerLeaveAckDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int PeerLeaveAckDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_SenderID
        FD_ISEDITABLE,    // FIELD_peerId
        FD_ISEDITABLE,    // FIELD_ackTime
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *PeerLeaveAckDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "SenderID",
        "peerId",
        "ackTime",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int PeerLeaveAckDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "SenderID") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "peerId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "ackTime") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *PeerLeaveAckDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_SenderID
        "int",    // FIELD_peerId
        "omnetpp::simtime_t",    // FIELD_ackTime
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **PeerLeaveAckDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PeerLeaveAckDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PeerLeaveAckDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    PeerLeaveAck *pp = omnetpp::fromAnyPtr<PeerLeaveAck>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void PeerLeaveAckDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerLeaveAck *pp = omnetpp::fromAnyPtr<PeerLeaveAck>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'PeerLeaveAck'", field);
    }
}

const char *PeerLeaveAckDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    PeerLeaveAck *pp = omnetpp::fromAnyPtr<PeerLeaveAck>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PeerLeaveAckDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    PeerLeaveAck *pp = omnetpp::fromAnyPtr<PeerLeaveAck>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: return oppstring2string(pp->getSenderID());
        case FIELD_peerId: return long2string(pp->getPeerId());
        case FIELD_ackTime: return simtime2string(pp->getAckTime());
        default: return "";
    }
}

void PeerLeaveAckDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerLeaveAck *pp = omnetpp::fromAnyPtr<PeerLeaveAck>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: pp->setSenderID((value)); break;
        case FIELD_peerId: pp->setPeerId(string2long(value)); break;
        case FIELD_ackTime: pp->setAckTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerLeaveAck'", field);
    }
}

omnetpp::cValue PeerLeaveAckDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    PeerLeaveAck *pp = omnetpp::fromAnyPtr<PeerLeaveAck>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: return pp->getSenderID();
        case FIELD_peerId: return pp->getPeerId();
        case FIELD_ackTime: return pp->getAckTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'PeerLeaveAck' as cValue -- field index out of range?", field);
    }
}

void PeerLeaveAckDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerLeaveAck *pp = omnetpp::fromAnyPtr<PeerLeaveAck>(object); (void)pp;
    switch (field) {
        case FIELD_SenderID: pp->setSenderID(value.stringValue()); break;
        case FIELD_peerId: pp->setPeerId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_ackTime: pp->setAckTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerLeaveAck'", field);
    }
}

const char *PeerLeaveAckDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr PeerLeaveAckDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    PeerLeaveAck *pp = omnetpp::fromAnyPtr<PeerLeaveAck>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void PeerLeaveAckDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    PeerLeaveAck *pp = omnetpp::fromAnyPtr<PeerLeaveAck>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PeerLeaveAck'", field);
    }
}

Register_Class(VerifyMessage)

VerifyMessage::VerifyMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
    take(&this->rm);
}

VerifyMessage::VerifyMessage(const VerifyMessage& other) : ::omnetpp::cMessage(other)
{
    take(&this->rm);
    copy(other);
}

VerifyMessage::~VerifyMessage()
{
    drop(&this->rm);
}

VerifyMessage& VerifyMessage::operator=(const VerifyMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void VerifyMessage::copy(const VerifyMessage& other)
{
    this->rm = other.rm;
    this->rm.setName(other.rm.getName());
    this->state = other.state;
}

void VerifyMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->rm);
    doParsimPacking(b,this->state);
}

void VerifyMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->rm);
    doParsimUnpacking(b,this->state);
}

const ReportMessage& VerifyMessage::getRm() const
{
    return this->rm;
}

void VerifyMessage::setRm(const ReportMessage& rm)
{
    this->rm = rm;
}

int VerifyMessage::getState() const
{
    return this->state;
}

void VerifyMessage::setState(int state)
{
    this->state = state;
}

class VerifyMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_rm,
        FIELD_state,
    };
  public:
    VerifyMessageDescriptor();
    virtual ~VerifyMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(VerifyMessageDescriptor)

VerifyMessageDescriptor::VerifyMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::VerifyMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

VerifyMessageDescriptor::~VerifyMessageDescriptor()
{
    delete[] propertyNames;
}

bool VerifyMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<VerifyMessage *>(obj)!=nullptr;
}

const char **VerifyMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *VerifyMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int VerifyMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int VerifyMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,    // FIELD_rm
        FD_ISEDITABLE,    // FIELD_state
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *VerifyMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "rm",
        "state",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int VerifyMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "rm") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "state") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *VerifyMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "omnetpp::ReportMessage",    // FIELD_rm
        "int",    // FIELD_state
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **VerifyMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *VerifyMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int VerifyMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    VerifyMessage *pp = omnetpp::fromAnyPtr<VerifyMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void VerifyMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    VerifyMessage *pp = omnetpp::fromAnyPtr<VerifyMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'VerifyMessage'", field);
    }
}

const char *VerifyMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    VerifyMessage *pp = omnetpp::fromAnyPtr<VerifyMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string VerifyMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    VerifyMessage *pp = omnetpp::fromAnyPtr<VerifyMessage>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return pp->getRm().str();
        case FIELD_state: return long2string(pp->getState());
        default: return "";
    }
}

void VerifyMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    VerifyMessage *pp = omnetpp::fromAnyPtr<VerifyMessage>(object); (void)pp;
    switch (field) {
        case FIELD_state: pp->setState(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VerifyMessage'", field);
    }
}

omnetpp::cValue VerifyMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    VerifyMessage *pp = omnetpp::fromAnyPtr<VerifyMessage>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        case FIELD_state: return pp->getState();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'VerifyMessage' as cValue -- field index out of range?", field);
    }
}

void VerifyMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    VerifyMessage *pp = omnetpp::fromAnyPtr<VerifyMessage>(object); (void)pp;
    switch (field) {
        case FIELD_state: pp->setState(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VerifyMessage'", field);
    }
}

const char *VerifyMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_rm: return omnetpp::opp_typename(typeid(ReportMessage));
        default: return nullptr;
    };
}

omnetpp::any_ptr VerifyMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    VerifyMessage *pp = omnetpp::fromAnyPtr<VerifyMessage>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void VerifyMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    VerifyMessage *pp = omnetpp::fromAnyPtr<VerifyMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VerifyMessage'", field);
    }
}

Register_Class(PrePrepareMsg)

PrePrepareMsg::PrePrepareMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
    take(&this->rm);
}

PrePrepareMsg::PrePrepareMsg(const PrePrepareMsg& other) : ::omnetpp::cMessage(other)
{
    take(&this->rm);
    copy(other);
}

PrePrepareMsg::~PrePrepareMsg()
{
    drop(&this->rm);
}

PrePrepareMsg& PrePrepareMsg::operator=(const PrePrepareMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void PrePrepareMsg::copy(const PrePrepareMsg& other)
{
    this->viewNum = other.viewNum;
    this->seqNum = other.seqNum;
    this->rm = other.rm;
    this->rm.setName(other.rm.getName());
    this->state = other.state;
}

void PrePrepareMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->viewNum);
    doParsimPacking(b,this->seqNum);
    doParsimPacking(b,this->rm);
    doParsimPacking(b,this->state);
}

void PrePrepareMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->viewNum);
    doParsimUnpacking(b,this->seqNum);
    doParsimUnpacking(b,this->rm);
    doParsimUnpacking(b,this->state);
}

int PrePrepareMsg::getViewNum() const
{
    return this->viewNum;
}

void PrePrepareMsg::setViewNum(int viewNum)
{
    this->viewNum = viewNum;
}

int PrePrepareMsg::getSeqNum() const
{
    return this->seqNum;
}

void PrePrepareMsg::setSeqNum(int seqNum)
{
    this->seqNum = seqNum;
}

const ReportMessage& PrePrepareMsg::getRm() const
{
    return this->rm;
}

void PrePrepareMsg::setRm(const ReportMessage& rm)
{
    this->rm = rm;
}

int PrePrepareMsg::getState() const
{
    return this->state;
}

void PrePrepareMsg::setState(int state)
{
    this->state = state;
}

class PrePrepareMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_viewNum,
        FIELD_seqNum,
        FIELD_rm,
        FIELD_state,
    };
  public:
    PrePrepareMsgDescriptor();
    virtual ~PrePrepareMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(PrePrepareMsgDescriptor)

PrePrepareMsgDescriptor::PrePrepareMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::PrePrepareMsg)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

PrePrepareMsgDescriptor::~PrePrepareMsgDescriptor()
{
    delete[] propertyNames;
}

bool PrePrepareMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PrePrepareMsg *>(obj)!=nullptr;
}

const char **PrePrepareMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *PrePrepareMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int PrePrepareMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int PrePrepareMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_viewNum
        FD_ISEDITABLE,    // FIELD_seqNum
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,    // FIELD_rm
        FD_ISEDITABLE,    // FIELD_state
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *PrePrepareMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "viewNum",
        "seqNum",
        "rm",
        "state",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int PrePrepareMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "viewNum") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "seqNum") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "rm") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "state") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *PrePrepareMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_viewNum
        "int",    // FIELD_seqNum
        "omnetpp::ReportMessage",    // FIELD_rm
        "int",    // FIELD_state
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **PrePrepareMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PrePrepareMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PrePrepareMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    PrePrepareMsg *pp = omnetpp::fromAnyPtr<PrePrepareMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void PrePrepareMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    PrePrepareMsg *pp = omnetpp::fromAnyPtr<PrePrepareMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'PrePrepareMsg'", field);
    }
}

const char *PrePrepareMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    PrePrepareMsg *pp = omnetpp::fromAnyPtr<PrePrepareMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PrePrepareMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    PrePrepareMsg *pp = omnetpp::fromAnyPtr<PrePrepareMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: return long2string(pp->getViewNum());
        case FIELD_seqNum: return long2string(pp->getSeqNum());
        case FIELD_rm: return pp->getRm().str();
        case FIELD_state: return long2string(pp->getState());
        default: return "";
    }
}

void PrePrepareMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PrePrepareMsg *pp = omnetpp::fromAnyPtr<PrePrepareMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: pp->setViewNum(string2long(value)); break;
        case FIELD_seqNum: pp->setSeqNum(string2long(value)); break;
        case FIELD_state: pp->setState(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PrePrepareMsg'", field);
    }
}

omnetpp::cValue PrePrepareMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    PrePrepareMsg *pp = omnetpp::fromAnyPtr<PrePrepareMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: return pp->getViewNum();
        case FIELD_seqNum: return pp->getSeqNum();
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        case FIELD_state: return pp->getState();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'PrePrepareMsg' as cValue -- field index out of range?", field);
    }
}

void PrePrepareMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PrePrepareMsg *pp = omnetpp::fromAnyPtr<PrePrepareMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: pp->setViewNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_seqNum: pp->setSeqNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_state: pp->setState(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PrePrepareMsg'", field);
    }
}

const char *PrePrepareMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_rm: return omnetpp::opp_typename(typeid(ReportMessage));
        default: return nullptr;
    };
}

omnetpp::any_ptr PrePrepareMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    PrePrepareMsg *pp = omnetpp::fromAnyPtr<PrePrepareMsg>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void PrePrepareMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    PrePrepareMsg *pp = omnetpp::fromAnyPtr<PrePrepareMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PrePrepareMsg'", field);
    }
}

Register_Class(PrepareMsg)

PrepareMsg::PrepareMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
    take(&this->rm);
}

PrepareMsg::PrepareMsg(const PrepareMsg& other) : ::omnetpp::cMessage(other)
{
    take(&this->rm);
    copy(other);
}

PrepareMsg::~PrepareMsg()
{
    drop(&this->rm);
}

PrepareMsg& PrepareMsg::operator=(const PrepareMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void PrepareMsg::copy(const PrepareMsg& other)
{
    this->viewNum = other.viewNum;
    this->seqNum = other.seqNum;
    this->orderId = other.orderId;
    this->rm = other.rm;
    this->rm.setName(other.rm.getName());
    this->state = other.state;
}

void PrepareMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->viewNum);
    doParsimPacking(b,this->seqNum);
    doParsimPacking(b,this->orderId);
    doParsimPacking(b,this->rm);
    doParsimPacking(b,this->state);
}

void PrepareMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->viewNum);
    doParsimUnpacking(b,this->seqNum);
    doParsimUnpacking(b,this->orderId);
    doParsimUnpacking(b,this->rm);
    doParsimUnpacking(b,this->state);
}

int PrepareMsg::getViewNum() const
{
    return this->viewNum;
}

void PrepareMsg::setViewNum(int viewNum)
{
    this->viewNum = viewNum;
}

int PrepareMsg::getSeqNum() const
{
    return this->seqNum;
}

void PrepareMsg::setSeqNum(int seqNum)
{
    this->seqNum = seqNum;
}

int PrepareMsg::getOrderId() const
{
    return this->orderId;
}

void PrepareMsg::setOrderId(int orderId)
{
    this->orderId = orderId;
}

const ReportMessage& PrepareMsg::getRm() const
{
    return this->rm;
}

void PrepareMsg::setRm(const ReportMessage& rm)
{
    this->rm = rm;
}

int PrepareMsg::getState() const
{
    return this->state;
}

void PrepareMsg::setState(int state)
{
    this->state = state;
}

class PrepareMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_viewNum,
        FIELD_seqNum,
        FIELD_orderId,
        FIELD_rm,
        FIELD_state,
    };
  public:
    PrepareMsgDescriptor();
    virtual ~PrepareMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(PrepareMsgDescriptor)

PrepareMsgDescriptor::PrepareMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::PrepareMsg)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

PrepareMsgDescriptor::~PrepareMsgDescriptor()
{
    delete[] propertyNames;
}

bool PrepareMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PrepareMsg *>(obj)!=nullptr;
}

const char **PrepareMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *PrepareMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int PrepareMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int PrepareMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_viewNum
        FD_ISEDITABLE,    // FIELD_seqNum
        FD_ISEDITABLE,    // FIELD_orderId
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,    // FIELD_rm
        FD_ISEDITABLE,    // FIELD_state
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *PrepareMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "viewNum",
        "seqNum",
        "orderId",
        "rm",
        "state",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int PrepareMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "viewNum") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "seqNum") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "orderId") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "rm") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "state") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *PrepareMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_viewNum
        "int",    // FIELD_seqNum
        "int",    // FIELD_orderId
        "omnetpp::ReportMessage",    // FIELD_rm
        "int",    // FIELD_state
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **PrepareMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PrepareMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PrepareMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    PrepareMsg *pp = omnetpp::fromAnyPtr<PrepareMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void PrepareMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    PrepareMsg *pp = omnetpp::fromAnyPtr<PrepareMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'PrepareMsg'", field);
    }
}

const char *PrepareMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    PrepareMsg *pp = omnetpp::fromAnyPtr<PrepareMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PrepareMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    PrepareMsg *pp = omnetpp::fromAnyPtr<PrepareMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: return long2string(pp->getViewNum());
        case FIELD_seqNum: return long2string(pp->getSeqNum());
        case FIELD_orderId: return long2string(pp->getOrderId());
        case FIELD_rm: return pp->getRm().str();
        case FIELD_state: return long2string(pp->getState());
        default: return "";
    }
}

void PrepareMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PrepareMsg *pp = omnetpp::fromAnyPtr<PrepareMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: pp->setViewNum(string2long(value)); break;
        case FIELD_seqNum: pp->setSeqNum(string2long(value)); break;
        case FIELD_orderId: pp->setOrderId(string2long(value)); break;
        case FIELD_state: pp->setState(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PrepareMsg'", field);
    }
}

omnetpp::cValue PrepareMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    PrepareMsg *pp = omnetpp::fromAnyPtr<PrepareMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: return pp->getViewNum();
        case FIELD_seqNum: return pp->getSeqNum();
        case FIELD_orderId: return pp->getOrderId();
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        case FIELD_state: return pp->getState();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'PrepareMsg' as cValue -- field index out of range?", field);
    }
}

void PrepareMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PrepareMsg *pp = omnetpp::fromAnyPtr<PrepareMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: pp->setViewNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_seqNum: pp->setSeqNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_orderId: pp->setOrderId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_state: pp->setState(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PrepareMsg'", field);
    }
}

const char *PrepareMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_rm: return omnetpp::opp_typename(typeid(ReportMessage));
        default: return nullptr;
    };
}

omnetpp::any_ptr PrepareMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    PrepareMsg *pp = omnetpp::fromAnyPtr<PrepareMsg>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void PrepareMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    PrepareMsg *pp = omnetpp::fromAnyPtr<PrepareMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PrepareMsg'", field);
    }
}

Register_Class(CommitMsg)

CommitMsg::CommitMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
    take(&this->rm);
}

CommitMsg::CommitMsg(const CommitMsg& other) : ::omnetpp::cMessage(other)
{
    take(&this->rm);
    copy(other);
}

CommitMsg::~CommitMsg()
{
    drop(&this->rm);
}

CommitMsg& CommitMsg::operator=(const CommitMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void CommitMsg::copy(const CommitMsg& other)
{
    this->viewNum = other.viewNum;
    this->seqNum = other.seqNum;
    this->orderId = other.orderId;
    this->rm = other.rm;
    this->rm.setName(other.rm.getName());
    this->state = other.state;
}

void CommitMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->viewNum);
    doParsimPacking(b,this->seqNum);
    doParsimPacking(b,this->orderId);
    doParsimPacking(b,this->rm);
    doParsimPacking(b,this->state);
}

void CommitMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->viewNum);
    doParsimUnpacking(b,this->seqNum);
    doParsimUnpacking(b,this->orderId);
    doParsimUnpacking(b,this->rm);
    doParsimUnpacking(b,this->state);
}

int CommitMsg::getViewNum() const
{
    return this->viewNum;
}

void CommitMsg::setViewNum(int viewNum)
{
    this->viewNum = viewNum;
}

int CommitMsg::getSeqNum() const
{
    return this->seqNum;
}

void CommitMsg::setSeqNum(int seqNum)
{
    this->seqNum = seqNum;
}

int CommitMsg::getOrderId() const
{
    return this->orderId;
}

void CommitMsg::setOrderId(int orderId)
{
    this->orderId = orderId;
}

const ReportMessage& CommitMsg::getRm() const
{
    return this->rm;
}

void CommitMsg::setRm(const ReportMessage& rm)
{
    this->rm = rm;
}

int CommitMsg::getState() const
{
    return this->state;
}

void CommitMsg::setState(int state)
{
    this->state = state;
}

class CommitMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_viewNum,
        FIELD_seqNum,
        FIELD_orderId,
        FIELD_rm,
        FIELD_state,
    };
  public:
    CommitMsgDescriptor();
    virtual ~CommitMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(CommitMsgDescriptor)

CommitMsgDescriptor::CommitMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::CommitMsg)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

CommitMsgDescriptor::~CommitMsgDescriptor()
{
    delete[] propertyNames;
}

bool CommitMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CommitMsg *>(obj)!=nullptr;
}

const char **CommitMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *CommitMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int CommitMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int CommitMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_viewNum
        FD_ISEDITABLE,    // FIELD_seqNum
        FD_ISEDITABLE,    // FIELD_orderId
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,    // FIELD_rm
        FD_ISEDITABLE,    // FIELD_state
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *CommitMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "viewNum",
        "seqNum",
        "orderId",
        "rm",
        "state",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int CommitMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "viewNum") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "seqNum") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "orderId") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "rm") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "state") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *CommitMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_viewNum
        "int",    // FIELD_seqNum
        "int",    // FIELD_orderId
        "omnetpp::ReportMessage",    // FIELD_rm
        "int",    // FIELD_state
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **CommitMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CommitMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CommitMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    CommitMsg *pp = omnetpp::fromAnyPtr<CommitMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void CommitMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    CommitMsg *pp = omnetpp::fromAnyPtr<CommitMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'CommitMsg'", field);
    }
}

const char *CommitMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    CommitMsg *pp = omnetpp::fromAnyPtr<CommitMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CommitMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    CommitMsg *pp = omnetpp::fromAnyPtr<CommitMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: return long2string(pp->getViewNum());
        case FIELD_seqNum: return long2string(pp->getSeqNum());
        case FIELD_orderId: return long2string(pp->getOrderId());
        case FIELD_rm: return pp->getRm().str();
        case FIELD_state: return long2string(pp->getState());
        default: return "";
    }
}

void CommitMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    CommitMsg *pp = omnetpp::fromAnyPtr<CommitMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: pp->setViewNum(string2long(value)); break;
        case FIELD_seqNum: pp->setSeqNum(string2long(value)); break;
        case FIELD_orderId: pp->setOrderId(string2long(value)); break;
        case FIELD_state: pp->setState(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CommitMsg'", field);
    }
}

omnetpp::cValue CommitMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    CommitMsg *pp = omnetpp::fromAnyPtr<CommitMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: return pp->getViewNum();
        case FIELD_seqNum: return pp->getSeqNum();
        case FIELD_orderId: return pp->getOrderId();
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        case FIELD_state: return pp->getState();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'CommitMsg' as cValue -- field index out of range?", field);
    }
}

void CommitMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    CommitMsg *pp = omnetpp::fromAnyPtr<CommitMsg>(object); (void)pp;
    switch (field) {
        case FIELD_viewNum: pp->setViewNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_seqNum: pp->setSeqNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_orderId: pp->setOrderId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_state: pp->setState(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CommitMsg'", field);
    }
}

const char *CommitMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_rm: return omnetpp::opp_typename(typeid(ReportMessage));
        default: return nullptr;
    };
}

omnetpp::any_ptr CommitMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    CommitMsg *pp = omnetpp::fromAnyPtr<CommitMsg>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void CommitMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    CommitMsg *pp = omnetpp::fromAnyPtr<CommitMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CommitMsg'", field);
    }
}

Register_Class(BlockMsg)

BlockMsg::BlockMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
    take(&this->rm);
}

BlockMsg::BlockMsg(const BlockMsg& other) : ::omnetpp::cMessage(other)
{
    take(&this->rm);
    copy(other);
}

BlockMsg::~BlockMsg()
{
    drop(&this->rm);
}

BlockMsg& BlockMsg::operator=(const BlockMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void BlockMsg::copy(const BlockMsg& other)
{
    this->blockNum = other.blockNum;
    this->rm = other.rm;
    this->rm.setName(other.rm.getName());
    this->BlockTimestamp = other.BlockTimestamp;
    this->state = other.state;
}

void BlockMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->blockNum);
    doParsimPacking(b,this->rm);
    doParsimPacking(b,this->BlockTimestamp);
    doParsimPacking(b,this->state);
}

void BlockMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->blockNum);
    doParsimUnpacking(b,this->rm);
    doParsimUnpacking(b,this->BlockTimestamp);
    doParsimUnpacking(b,this->state);
}

int BlockMsg::getBlockNum() const
{
    return this->blockNum;
}

void BlockMsg::setBlockNum(int blockNum)
{
    this->blockNum = blockNum;
}

const ReportMessage& BlockMsg::getRm() const
{
    return this->rm;
}

void BlockMsg::setRm(const ReportMessage& rm)
{
    this->rm = rm;
}

simtime_t BlockMsg::getBlockTimestamp() const
{
    return this->BlockTimestamp;
}

void BlockMsg::setBlockTimestamp(simtime_t BlockTimestamp)
{
    this->BlockTimestamp = BlockTimestamp;
}

int BlockMsg::getState() const
{
    return this->state;
}

void BlockMsg::setState(int state)
{
    this->state = state;
}

class BlockMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_blockNum,
        FIELD_rm,
        FIELD_BlockTimestamp,
        FIELD_state,
    };
  public:
    BlockMsgDescriptor();
    virtual ~BlockMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(BlockMsgDescriptor)

BlockMsgDescriptor::BlockMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(omnetpp::BlockMsg)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

BlockMsgDescriptor::~BlockMsgDescriptor()
{
    delete[] propertyNames;
}

bool BlockMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BlockMsg *>(obj)!=nullptr;
}

const char **BlockMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *BlockMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int BlockMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int BlockMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_blockNum
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,    // FIELD_rm
        FD_ISEDITABLE,    // FIELD_BlockTimestamp
        FD_ISEDITABLE,    // FIELD_state
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *BlockMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "blockNum",
        "rm",
        "BlockTimestamp",
        "state",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int BlockMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "blockNum") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "rm") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "BlockTimestamp") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "state") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *BlockMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_blockNum
        "omnetpp::ReportMessage",    // FIELD_rm
        "omnetpp::simtime_t",    // FIELD_BlockTimestamp
        "int",    // FIELD_state
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **BlockMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BlockMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BlockMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    BlockMsg *pp = omnetpp::fromAnyPtr<BlockMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void BlockMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    BlockMsg *pp = omnetpp::fromAnyPtr<BlockMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'BlockMsg'", field);
    }
}

const char *BlockMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    BlockMsg *pp = omnetpp::fromAnyPtr<BlockMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BlockMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    BlockMsg *pp = omnetpp::fromAnyPtr<BlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_blockNum: return long2string(pp->getBlockNum());
        case FIELD_rm: return pp->getRm().str();
        case FIELD_BlockTimestamp: return simtime2string(pp->getBlockTimestamp());
        case FIELD_state: return long2string(pp->getState());
        default: return "";
    }
}

void BlockMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BlockMsg *pp = omnetpp::fromAnyPtr<BlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_blockNum: pp->setBlockNum(string2long(value)); break;
        case FIELD_BlockTimestamp: pp->setBlockTimestamp(string2simtime(value)); break;
        case FIELD_state: pp->setState(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BlockMsg'", field);
    }
}

omnetpp::cValue BlockMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    BlockMsg *pp = omnetpp::fromAnyPtr<BlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_blockNum: return pp->getBlockNum();
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        case FIELD_BlockTimestamp: return pp->getBlockTimestamp().dbl();
        case FIELD_state: return pp->getState();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'BlockMsg' as cValue -- field index out of range?", field);
    }
}

void BlockMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BlockMsg *pp = omnetpp::fromAnyPtr<BlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_blockNum: pp->setBlockNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_BlockTimestamp: pp->setBlockTimestamp(value.doubleValue()); break;
        case FIELD_state: pp->setState(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BlockMsg'", field);
    }
}

const char *BlockMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_rm: return omnetpp::opp_typename(typeid(ReportMessage));
        default: return nullptr;
    };
}

omnetpp::any_ptr BlockMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    BlockMsg *pp = omnetpp::fromAnyPtr<BlockMsg>(object); (void)pp;
    switch (field) {
        case FIELD_rm: return omnetpp::toAnyPtr(&pp->getRm()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void BlockMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    BlockMsg *pp = omnetpp::fromAnyPtr<BlockMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BlockMsg'", field);
    }
}

}  // namespace omnetpp

namespace omnetpp {

}  // namespace omnetpp

