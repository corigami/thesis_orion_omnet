//
// Generated file, do not edit! Created by opp_msgc 4.5 from applications/orionapp/OrionPackets.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "OrionPackets_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("OrionPacketType");
    if (!e) enums.getInstance()->add(e = new cEnum("OrionPacketType"));
    e->insert(QUERY, "QUERY");
    e->insert(RESPONSE, "RESPONSE");
    e->insert(DATA_REQUEST, "DATA_REQUEST");
    e->insert(DATA_REPLY, "DATA_REPLY");
    e->insert(DATA_REQUEST_ACK, "DATA_REQUEST_ACK");
);

Register_Class(OrionPacket);

OrionPacket::OrionPacket(const char *name, int kind) : ::cPacket(name,kind)
{
    this->packetType_var = 0;
    this->SEQ_var = 0;
}

OrionPacket::OrionPacket(const OrionPacket& other) : ::cPacket(other)
{
    copy(other);
}

OrionPacket::~OrionPacket()
{
}

OrionPacket& OrionPacket::operator=(const OrionPacket& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void OrionPacket::copy(const OrionPacket& other)
{
    this->packetType_var = other.packetType_var;
    this->SRC_var = other.SRC_var;
    this->LastNode_var = other.LastNode_var;
    this->SEQ_var = other.SEQ_var;
}

void OrionPacket::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
    doPacking(b,this->SRC_var);
    doPacking(b,this->LastNode_var);
    doPacking(b,this->SEQ_var);
}

void OrionPacket::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->SRC_var);
    doUnpacking(b,this->LastNode_var);
    doUnpacking(b,this->SEQ_var);
}

unsigned int OrionPacket::getPacketType() const
{
    return packetType_var;
}

void OrionPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

IPvXAddress& OrionPacket::getSRC()
{
    return SRC_var;
}

void OrionPacket::setSRC(const IPvXAddress& SRC)
{
    this->SRC_var = SRC;
}

IPvXAddress& OrionPacket::getLastNode()
{
    return LastNode_var;
}

void OrionPacket::setLastNode(const IPvXAddress& LastNode)
{
    this->LastNode_var = LastNode;
}

unsigned int OrionPacket::getSEQ() const
{
    return SEQ_var;
}

void OrionPacket::setSEQ(unsigned int SEQ)
{
    this->SEQ_var = SEQ;
}

class OrionPacketDescriptor : public cClassDescriptor
{
  public:
    OrionPacketDescriptor();
    virtual ~OrionPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(OrionPacketDescriptor);

OrionPacketDescriptor::OrionPacketDescriptor() : cClassDescriptor("OrionPacket", "cPacket")
{
}

OrionPacketDescriptor::~OrionPacketDescriptor()
{
}

bool OrionPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OrionPacket *>(obj)!=NULL;
}

const char *OrionPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OrionPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int OrionPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *OrionPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetType",
        "SRC",
        "LastNode",
        "SEQ",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int OrionPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='S' && strcmp(fieldName, "SRC")==0) return base+1;
    if (fieldName[0]=='L' && strcmp(fieldName, "LastNode")==0) return base+2;
    if (fieldName[0]=='S' && strcmp(fieldName, "SEQ")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OrionPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "IPvXAddress",
        "IPvXAddress",
        "unsigned int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *OrionPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int OrionPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OrionPacket *pp = (OrionPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OrionPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OrionPacket *pp = (OrionPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPacketType());
        case 1: {std::stringstream out; out << pp->getSRC(); return out.str();}
        case 2: {std::stringstream out; out << pp->getLastNode(); return out.str();}
        case 3: return ulong2string(pp->getSEQ());
        default: return "";
    }
}

bool OrionPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OrionPacket *pp = (OrionPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2ulong(value)); return true;
        case 3: pp->setSEQ(string2ulong(value)); return true;
        default: return false;
    }
}

const char *OrionPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1: return opp_typename(typeid(IPvXAddress));
        case 2: return opp_typename(typeid(IPvXAddress));
        default: return NULL;
    };
}

void *OrionPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OrionPacket *pp = (OrionPacket *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getSRC()); break;
        case 2: return (void *)(&pp->getLastNode()); break;
        default: return NULL;
    }
}

Register_Class(OrionQueryPacket);

OrionQueryPacket::OrionQueryPacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = QUERY;
    this->filename_var = 0;
}

OrionQueryPacket::OrionQueryPacket(const OrionQueryPacket& other) : ::OrionPacket(other)
{
    copy(other);
}

OrionQueryPacket::~OrionQueryPacket()
{
}

OrionQueryPacket& OrionQueryPacket::operator=(const OrionQueryPacket& other)
{
    if (this==&other) return *this;
    ::OrionPacket::operator=(other);
    copy(other);
    return *this;
}

void OrionQueryPacket::copy(const OrionQueryPacket& other)
{
    this->packetType_var = other.packetType_var;
    this->filename_var = other.filename_var;
}

void OrionQueryPacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
    doPacking(b,this->filename_var);
}

void OrionQueryPacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->filename_var);
}

unsigned int OrionQueryPacket::getPacketType() const
{
    return packetType_var;
}

void OrionQueryPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

const char * OrionQueryPacket::getFilename() const
{
    return filename_var.c_str();
}

void OrionQueryPacket::setFilename(const char * filename)
{
    this->filename_var = filename;
}

class OrionQueryPacketDescriptor : public cClassDescriptor
{
  public:
    OrionQueryPacketDescriptor();
    virtual ~OrionQueryPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(OrionQueryPacketDescriptor);

OrionQueryPacketDescriptor::OrionQueryPacketDescriptor() : cClassDescriptor("OrionQueryPacket", "OrionPacket")
{
}

OrionQueryPacketDescriptor::~OrionQueryPacketDescriptor()
{
}

bool OrionQueryPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OrionQueryPacket *>(obj)!=NULL;
}

const char *OrionQueryPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OrionQueryPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int OrionQueryPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *OrionQueryPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetType",
        "filename",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int OrionQueryPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "filename")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OrionQueryPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "string",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *OrionQueryPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int OrionQueryPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OrionQueryPacket *pp = (OrionQueryPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OrionQueryPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OrionQueryPacket *pp = (OrionQueryPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPacketType());
        case 1: return oppstring2string(pp->getFilename());
        default: return "";
    }
}

bool OrionQueryPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OrionQueryPacket *pp = (OrionQueryPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2ulong(value)); return true;
        case 1: pp->setFilename((value)); return true;
        default: return false;
    }
}

const char *OrionQueryPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *OrionQueryPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OrionQueryPacket *pp = (OrionQueryPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(OrionResponsePacket);

OrionResponsePacket::OrionResponsePacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = RESPONSE;
    this->filename_var = 0;
    this->hopcount_var = 0;
}

OrionResponsePacket::OrionResponsePacket(const OrionResponsePacket& other) : ::OrionPacket(other)
{
    copy(other);
}

OrionResponsePacket::~OrionResponsePacket()
{
}

OrionResponsePacket& OrionResponsePacket::operator=(const OrionResponsePacket& other)
{
    if (this==&other) return *this;
    ::OrionPacket::operator=(other);
    copy(other);
    return *this;
}

void OrionResponsePacket::copy(const OrionResponsePacket& other)
{
    this->packetType_var = other.packetType_var;
    this->filename_var = other.filename_var;
    this->hopcount_var = other.hopcount_var;
}

void OrionResponsePacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
    doPacking(b,this->filename_var);
    doPacking(b,this->hopcount_var);
}

void OrionResponsePacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->filename_var);
    doUnpacking(b,this->hopcount_var);
}

unsigned int OrionResponsePacket::getPacketType() const
{
    return packetType_var;
}

void OrionResponsePacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

const char * OrionResponsePacket::getFilename() const
{
    return filename_var.c_str();
}

void OrionResponsePacket::setFilename(const char * filename)
{
    this->filename_var = filename;
}

unsigned int OrionResponsePacket::getHopcount() const
{
    return hopcount_var;
}

void OrionResponsePacket::setHopcount(unsigned int hopcount)
{
    this->hopcount_var = hopcount;
}

class OrionResponsePacketDescriptor : public cClassDescriptor
{
  public:
    OrionResponsePacketDescriptor();
    virtual ~OrionResponsePacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(OrionResponsePacketDescriptor);

OrionResponsePacketDescriptor::OrionResponsePacketDescriptor() : cClassDescriptor("OrionResponsePacket", "OrionPacket")
{
}

OrionResponsePacketDescriptor::~OrionResponsePacketDescriptor()
{
}

bool OrionResponsePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OrionResponsePacket *>(obj)!=NULL;
}

const char *OrionResponsePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OrionResponsePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int OrionResponsePacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *OrionResponsePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetType",
        "filename",
        "hopcount",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int OrionResponsePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "filename")==0) return base+1;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopcount")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OrionResponsePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "string",
        "unsigned int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *OrionResponsePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int OrionResponsePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OrionResponsePacket *pp = (OrionResponsePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OrionResponsePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OrionResponsePacket *pp = (OrionResponsePacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPacketType());
        case 1: return oppstring2string(pp->getFilename());
        case 2: return ulong2string(pp->getHopcount());
        default: return "";
    }
}

bool OrionResponsePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OrionResponsePacket *pp = (OrionResponsePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2ulong(value)); return true;
        case 1: pp->setFilename((value)); return true;
        case 2: pp->setHopcount(string2ulong(value)); return true;
        default: return false;
    }
}

const char *OrionResponsePacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *OrionResponsePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OrionResponsePacket *pp = (OrionResponsePacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(OrionDataReqPacket);

OrionDataReqPacket::OrionDataReqPacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = DATA_REQUEST;
    this->filename_var = 0;
    this->blockNum_var = 0;
}

OrionDataReqPacket::OrionDataReqPacket(const OrionDataReqPacket& other) : ::OrionPacket(other)
{
    copy(other);
}

OrionDataReqPacket::~OrionDataReqPacket()
{
}

OrionDataReqPacket& OrionDataReqPacket::operator=(const OrionDataReqPacket& other)
{
    if (this==&other) return *this;
    ::OrionPacket::operator=(other);
    copy(other);
    return *this;
}

void OrionDataReqPacket::copy(const OrionDataReqPacket& other)
{
    this->DST_var = other.DST_var;
    this->packetType_var = other.packetType_var;
    this->filename_var = other.filename_var;
    this->blockNum_var = other.blockNum_var;
}

void OrionDataReqPacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->DST_var);
    doPacking(b,this->packetType_var);
    doPacking(b,this->filename_var);
    doPacking(b,this->blockNum_var);
}

void OrionDataReqPacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->DST_var);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->filename_var);
    doUnpacking(b,this->blockNum_var);
}

IPvXAddress& OrionDataReqPacket::getDST()
{
    return DST_var;
}

void OrionDataReqPacket::setDST(const IPvXAddress& DST)
{
    this->DST_var = DST;
}

unsigned int OrionDataReqPacket::getPacketType() const
{
    return packetType_var;
}

void OrionDataReqPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

const char * OrionDataReqPacket::getFilename() const
{
    return filename_var.c_str();
}

void OrionDataReqPacket::setFilename(const char * filename)
{
    this->filename_var = filename;
}

unsigned int OrionDataReqPacket::getBlockNum() const
{
    return blockNum_var;
}

void OrionDataReqPacket::setBlockNum(unsigned int blockNum)
{
    this->blockNum_var = blockNum;
}

class OrionDataReqPacketDescriptor : public cClassDescriptor
{
  public:
    OrionDataReqPacketDescriptor();
    virtual ~OrionDataReqPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(OrionDataReqPacketDescriptor);

OrionDataReqPacketDescriptor::OrionDataReqPacketDescriptor() : cClassDescriptor("OrionDataReqPacket", "OrionPacket")
{
}

OrionDataReqPacketDescriptor::~OrionDataReqPacketDescriptor()
{
}

bool OrionDataReqPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OrionDataReqPacket *>(obj)!=NULL;
}

const char *OrionDataReqPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OrionDataReqPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int OrionDataReqPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *OrionDataReqPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "DST",
        "packetType",
        "filename",
        "blockNum",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int OrionDataReqPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='D' && strcmp(fieldName, "DST")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "filename")==0) return base+2;
    if (fieldName[0]=='b' && strcmp(fieldName, "blockNum")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OrionDataReqPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "IPvXAddress",
        "unsigned int",
        "string",
        "unsigned int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *OrionDataReqPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int OrionDataReqPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataReqPacket *pp = (OrionDataReqPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OrionDataReqPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataReqPacket *pp = (OrionDataReqPacket *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getDST(); return out.str();}
        case 1: return ulong2string(pp->getPacketType());
        case 2: return oppstring2string(pp->getFilename());
        case 3: return ulong2string(pp->getBlockNum());
        default: return "";
    }
}

bool OrionDataReqPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataReqPacket *pp = (OrionDataReqPacket *)object; (void)pp;
    switch (field) {
        case 1: pp->setPacketType(string2ulong(value)); return true;
        case 2: pp->setFilename((value)); return true;
        case 3: pp->setBlockNum(string2ulong(value)); return true;
        default: return false;
    }
}

const char *OrionDataReqPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(IPvXAddress));
        default: return NULL;
    };
}

void *OrionDataReqPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataReqPacket *pp = (OrionDataReqPacket *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getDST()); break;
        default: return NULL;
    }
}

Register_Class(OrionDataRepPacket);

OrionDataRepPacket::OrionDataRepPacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = DATA_REPLY;
    this->filename_var = 0;
    this->blockNum_var = 0;
}

OrionDataRepPacket::OrionDataRepPacket(const OrionDataRepPacket& other) : ::OrionPacket(other)
{
    copy(other);
}

OrionDataRepPacket::~OrionDataRepPacket()
{
}

OrionDataRepPacket& OrionDataRepPacket::operator=(const OrionDataRepPacket& other)
{
    if (this==&other) return *this;
    ::OrionPacket::operator=(other);
    copy(other);
    return *this;
}

void OrionDataRepPacket::copy(const OrionDataRepPacket& other)
{
    this->DST_var = other.DST_var;
    this->packetType_var = other.packetType_var;
    this->filename_var = other.filename_var;
    this->blockNum_var = other.blockNum_var;
}

void OrionDataRepPacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->DST_var);
    doPacking(b,this->packetType_var);
    doPacking(b,this->filename_var);
    doPacking(b,this->blockNum_var);
}

void OrionDataRepPacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->DST_var);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->filename_var);
    doUnpacking(b,this->blockNum_var);
}

IPvXAddress& OrionDataRepPacket::getDST()
{
    return DST_var;
}

void OrionDataRepPacket::setDST(const IPvXAddress& DST)
{
    this->DST_var = DST;
}

unsigned int OrionDataRepPacket::getPacketType() const
{
    return packetType_var;
}

void OrionDataRepPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

const char * OrionDataRepPacket::getFilename() const
{
    return filename_var.c_str();
}

void OrionDataRepPacket::setFilename(const char * filename)
{
    this->filename_var = filename;
}

unsigned int OrionDataRepPacket::getBlockNum() const
{
    return blockNum_var;
}

void OrionDataRepPacket::setBlockNum(unsigned int blockNum)
{
    this->blockNum_var = blockNum;
}

class OrionDataRepPacketDescriptor : public cClassDescriptor
{
  public:
    OrionDataRepPacketDescriptor();
    virtual ~OrionDataRepPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(OrionDataRepPacketDescriptor);

OrionDataRepPacketDescriptor::OrionDataRepPacketDescriptor() : cClassDescriptor("OrionDataRepPacket", "OrionPacket")
{
}

OrionDataRepPacketDescriptor::~OrionDataRepPacketDescriptor()
{
}

bool OrionDataRepPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OrionDataRepPacket *>(obj)!=NULL;
}

const char *OrionDataRepPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OrionDataRepPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int OrionDataRepPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *OrionDataRepPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "DST",
        "packetType",
        "filename",
        "blockNum",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int OrionDataRepPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='D' && strcmp(fieldName, "DST")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "filename")==0) return base+2;
    if (fieldName[0]=='b' && strcmp(fieldName, "blockNum")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OrionDataRepPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "IPvXAddress",
        "unsigned int",
        "string",
        "unsigned int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *OrionDataRepPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int OrionDataRepPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataRepPacket *pp = (OrionDataRepPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OrionDataRepPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataRepPacket *pp = (OrionDataRepPacket *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getDST(); return out.str();}
        case 1: return ulong2string(pp->getPacketType());
        case 2: return oppstring2string(pp->getFilename());
        case 3: return ulong2string(pp->getBlockNum());
        default: return "";
    }
}

bool OrionDataRepPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataRepPacket *pp = (OrionDataRepPacket *)object; (void)pp;
    switch (field) {
        case 1: pp->setPacketType(string2ulong(value)); return true;
        case 2: pp->setFilename((value)); return true;
        case 3: pp->setBlockNum(string2ulong(value)); return true;
        default: return false;
    }
}

const char *OrionDataRepPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(IPvXAddress));
        default: return NULL;
    };
}

void *OrionDataRepPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataRepPacket *pp = (OrionDataRepPacket *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getDST()); break;
        default: return NULL;
    }
}

Register_Class(OrionDataReqPacketAck);

OrionDataReqPacketAck::OrionDataReqPacketAck(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = DATA_REQUEST_ACK;
}

OrionDataReqPacketAck::OrionDataReqPacketAck(const OrionDataReqPacketAck& other) : ::OrionPacket(other)
{
    copy(other);
}

OrionDataReqPacketAck::~OrionDataReqPacketAck()
{
}

OrionDataReqPacketAck& OrionDataReqPacketAck::operator=(const OrionDataReqPacketAck& other)
{
    if (this==&other) return *this;
    ::OrionPacket::operator=(other);
    copy(other);
    return *this;
}

void OrionDataReqPacketAck::copy(const OrionDataReqPacketAck& other)
{
    this->packetType_var = other.packetType_var;
}

void OrionDataReqPacketAck::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
}

void OrionDataReqPacketAck::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
}

unsigned int OrionDataReqPacketAck::getPacketType() const
{
    return packetType_var;
}

void OrionDataReqPacketAck::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

class OrionDataReqPacketAckDescriptor : public cClassDescriptor
{
  public:
    OrionDataReqPacketAckDescriptor();
    virtual ~OrionDataReqPacketAckDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(OrionDataReqPacketAckDescriptor);

OrionDataReqPacketAckDescriptor::OrionDataReqPacketAckDescriptor() : cClassDescriptor("OrionDataReqPacketAck", "OrionPacket")
{
}

OrionDataReqPacketAckDescriptor::~OrionDataReqPacketAckDescriptor()
{
}

bool OrionDataReqPacketAckDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OrionDataReqPacketAck *>(obj)!=NULL;
}

const char *OrionDataReqPacketAckDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OrionDataReqPacketAckDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int OrionDataReqPacketAckDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *OrionDataReqPacketAckDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetType",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int OrionDataReqPacketAckDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OrionDataReqPacketAckDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *OrionDataReqPacketAckDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int OrionDataReqPacketAckDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataReqPacketAck *pp = (OrionDataReqPacketAck *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OrionDataReqPacketAckDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataReqPacketAck *pp = (OrionDataReqPacketAck *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPacketType());
        default: return "";
    }
}

bool OrionDataReqPacketAckDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataReqPacketAck *pp = (OrionDataReqPacketAck *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2ulong(value)); return true;
        default: return false;
    }
}

const char *OrionDataReqPacketAckDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *OrionDataReqPacketAckDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataReqPacketAck *pp = (OrionDataReqPacketAck *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


