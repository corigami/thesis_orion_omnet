//
// Generated file, do not edit! Created by nedtool 4.6 from OrionPackets.msg.
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
    e->insert(DATA_ERROR, "DATA_ERROR");
    e->insert(REP_REQUEST, "REP_REQUEST");
    e->insert(REP_CONFIRM, "REP_CONFIRM");
    e->insert(REP_CONFIRM_ACK, "REP_CONFIRM_ACK");
);

Register_Class(OrionPacket);

OrionPacket::OrionPacket(const char *name, int kind) : ::cPacket(name,kind)
{
    this->packetType_var = 0;
    this->sourceId_var = 0;
    this->LastNodeId_var = 0;
    this->SEQ_var = 0;
    this->filename_var = 0;
    this->hopCount_var = 0;
    this->bid_var = 0;
    this->start_var = 0;
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
    this->DST_var = other.DST_var;
    this->SRC_var = other.SRC_var;
    this->sourceId_var = other.sourceId_var;
    this->LastNode_var = other.LastNode_var;
    this->LastNodeId_var = other.LastNodeId_var;
    this->origin_var = other.origin_var;
    this->SEQ_var = other.SEQ_var;
    this->filename_var = other.filename_var;
    this->hopCount_var = other.hopCount_var;
    this->bid_var = other.bid_var;
    this->start_var = other.start_var;
}

void OrionPacket::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
    doPacking(b,this->DST_var);
    doPacking(b,this->SRC_var);
    doPacking(b,this->sourceId_var);
    doPacking(b,this->LastNode_var);
    doPacking(b,this->LastNodeId_var);
    doPacking(b,this->origin_var);
    doPacking(b,this->SEQ_var);
    doPacking(b,this->filename_var);
    doPacking(b,this->hopCount_var);
    doPacking(b,this->bid_var);
    doPacking(b,this->start_var);
}

void OrionPacket::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->DST_var);
    doUnpacking(b,this->SRC_var);
    doUnpacking(b,this->sourceId_var);
    doUnpacking(b,this->LastNode_var);
    doUnpacking(b,this->LastNodeId_var);
    doUnpacking(b,this->origin_var);
    doUnpacking(b,this->SEQ_var);
    doUnpacking(b,this->filename_var);
    doUnpacking(b,this->hopCount_var);
    doUnpacking(b,this->bid_var);
    doUnpacking(b,this->start_var);
}

unsigned int OrionPacket::getPacketType() const
{
    return packetType_var;
}

void OrionPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

IPvXAddress& OrionPacket::getDST()
{
    return DST_var;
}

void OrionPacket::setDST(const IPvXAddress& DST)
{
    this->DST_var = DST;
}

IPvXAddress& OrionPacket::getSRC()
{
    return SRC_var;
}

void OrionPacket::setSRC(const IPvXAddress& SRC)
{
    this->SRC_var = SRC;
}

const char * OrionPacket::getSourceId() const
{
    return sourceId_var.c_str();
}

void OrionPacket::setSourceId(const char * sourceId)
{
    this->sourceId_var = sourceId;
}

IPvXAddress& OrionPacket::getLastNode()
{
    return LastNode_var;
}

void OrionPacket::setLastNode(const IPvXAddress& LastNode)
{
    this->LastNode_var = LastNode;
}

const char * OrionPacket::getLastNodeId() const
{
    return LastNodeId_var.c_str();
}

void OrionPacket::setLastNodeId(const char * LastNodeId)
{
    this->LastNodeId_var = LastNodeId;
}

IPvXAddress& OrionPacket::getOrigin()
{
    return origin_var;
}

void OrionPacket::setOrigin(const IPvXAddress& origin)
{
    this->origin_var = origin;
}

unsigned int OrionPacket::getSEQ() const
{
    return SEQ_var;
}

void OrionPacket::setSEQ(unsigned int SEQ)
{
    this->SEQ_var = SEQ;
}

const char * OrionPacket::getFilename() const
{
    return filename_var.c_str();
}

void OrionPacket::setFilename(const char * filename)
{
    this->filename_var = filename;
}

unsigned int OrionPacket::getHopCount() const
{
    return hopCount_var;
}

void OrionPacket::setHopCount(unsigned int hopCount)
{
    this->hopCount_var = hopCount;
}

const char * OrionPacket::getBid() const
{
    return bid_var.c_str();
}

void OrionPacket::setBid(const char * bid)
{
    this->bid_var = bid;
}

simtime_t OrionPacket::getStart() const
{
    return start_var;
}

void OrionPacket::setStart(simtime_t start)
{
    this->start_var = start;
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
    return basedesc ? 12+basedesc->getFieldCount(object) : 12;
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
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<12) ? fieldTypeFlags[field] : 0;
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
        "DST",
        "SRC",
        "sourceId",
        "LastNode",
        "LastNodeId",
        "origin",
        "SEQ",
        "filename",
        "hopCount",
        "bid",
        "start",
    };
    return (field>=0 && field<12) ? fieldNames[field] : NULL;
}

int OrionPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='D' && strcmp(fieldName, "DST")==0) return base+1;
    if (fieldName[0]=='S' && strcmp(fieldName, "SRC")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceId")==0) return base+3;
    if (fieldName[0]=='L' && strcmp(fieldName, "LastNode")==0) return base+4;
    if (fieldName[0]=='L' && strcmp(fieldName, "LastNodeId")==0) return base+5;
    if (fieldName[0]=='o' && strcmp(fieldName, "origin")==0) return base+6;
    if (fieldName[0]=='S' && strcmp(fieldName, "SEQ")==0) return base+7;
    if (fieldName[0]=='f' && strcmp(fieldName, "filename")==0) return base+8;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+9;
    if (fieldName[0]=='b' && strcmp(fieldName, "bid")==0) return base+10;
    if (fieldName[0]=='s' && strcmp(fieldName, "start")==0) return base+11;
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
        "string",
        "IPvXAddress",
        "string",
        "IPvXAddress",
        "unsigned int",
        "string",
        "unsigned int",
        "string",
        "simtime_t",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : NULL;
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
        case 1: {std::stringstream out; out << pp->getDST(); return out.str();}
        case 2: {std::stringstream out; out << pp->getSRC(); return out.str();}
        case 3: return oppstring2string(pp->getSourceId());
        case 4: {std::stringstream out; out << pp->getLastNode(); return out.str();}
        case 5: return oppstring2string(pp->getLastNodeId());
        case 6: {std::stringstream out; out << pp->getOrigin(); return out.str();}
        case 7: return ulong2string(pp->getSEQ());
        case 8: return oppstring2string(pp->getFilename());
        case 9: return ulong2string(pp->getHopCount());
        case 10: return oppstring2string(pp->getBid());
        case 11: return double2string(pp->getStart());
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
        case 3: pp->setSourceId((value)); return true;
        case 5: pp->setLastNodeId((value)); return true;
        case 7: pp->setSEQ(string2ulong(value)); return true;
        case 8: pp->setFilename((value)); return true;
        case 9: pp->setHopCount(string2ulong(value)); return true;
        case 10: pp->setBid((value)); return true;
        case 11: pp->setStart(string2double(value)); return true;
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
        case 4: return opp_typename(typeid(IPvXAddress));
        case 6: return opp_typename(typeid(IPvXAddress));
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
        case 1: return (void *)(&pp->getDST()); break;
        case 2: return (void *)(&pp->getSRC()); break;
        case 4: return (void *)(&pp->getLastNode()); break;
        case 6: return (void *)(&pp->getOrigin()); break;
        default: return NULL;
    }
}

Register_Class(OrionQueryPacket);

OrionQueryPacket::OrionQueryPacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = QUERY;
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
}

void OrionQueryPacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
}

void OrionQueryPacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
}

unsigned int OrionQueryPacket::getPacketType() const
{
    return packetType_var;
}

void OrionQueryPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
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
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
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
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
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
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int OrionQueryPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
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
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
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
}

void OrionResponsePacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
}

void OrionResponsePacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
}

unsigned int OrionResponsePacket::getPacketType() const
{
    return packetType_var;
}

void OrionResponsePacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
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
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
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
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
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
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int OrionResponsePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
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
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
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

Register_Class(OrionErrorPacket);

OrionErrorPacket::OrionErrorPacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = DATA_ERROR;
    this->requestId_var = 0;
}

OrionErrorPacket::OrionErrorPacket(const OrionErrorPacket& other) : ::OrionPacket(other)
{
    copy(other);
}

OrionErrorPacket::~OrionErrorPacket()
{
}

OrionErrorPacket& OrionErrorPacket::operator=(const OrionErrorPacket& other)
{
    if (this==&other) return *this;
    ::OrionPacket::operator=(other);
    copy(other);
    return *this;
}

void OrionErrorPacket::copy(const OrionErrorPacket& other)
{
    this->packetType_var = other.packetType_var;
    this->requestId_var = other.requestId_var;
}

void OrionErrorPacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
    doPacking(b,this->requestId_var);
}

void OrionErrorPacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->requestId_var);
}

unsigned int OrionErrorPacket::getPacketType() const
{
    return packetType_var;
}

void OrionErrorPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

const char * OrionErrorPacket::getRequestId() const
{
    return requestId_var.c_str();
}

void OrionErrorPacket::setRequestId(const char * requestId)
{
    this->requestId_var = requestId;
}

class OrionErrorPacketDescriptor : public cClassDescriptor
{
  public:
    OrionErrorPacketDescriptor();
    virtual ~OrionErrorPacketDescriptor();

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

Register_ClassDescriptor(OrionErrorPacketDescriptor);

OrionErrorPacketDescriptor::OrionErrorPacketDescriptor() : cClassDescriptor("OrionErrorPacket", "OrionPacket")
{
}

OrionErrorPacketDescriptor::~OrionErrorPacketDescriptor()
{
}

bool OrionErrorPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OrionErrorPacket *>(obj)!=NULL;
}

const char *OrionErrorPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OrionErrorPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int OrionErrorPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *OrionErrorPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetType",
        "requestId",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int OrionErrorPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "requestId")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OrionErrorPacketDescriptor::getFieldTypeString(void *object, int field) const
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

const char *OrionErrorPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int OrionErrorPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OrionErrorPacket *pp = (OrionErrorPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OrionErrorPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OrionErrorPacket *pp = (OrionErrorPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPacketType());
        case 1: return oppstring2string(pp->getRequestId());
        default: return "";
    }
}

bool OrionErrorPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OrionErrorPacket *pp = (OrionErrorPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2ulong(value)); return true;
        case 1: pp->setRequestId((value)); return true;
        default: return false;
    }
}

const char *OrionErrorPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *OrionErrorPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OrionErrorPacket *pp = (OrionErrorPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(OrionDataReqPacket);

OrionDataReqPacket::OrionDataReqPacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = DATA_REQUEST;
    this->retries_var = 0;
    this->block_var = 0;
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
    this->packetType_var = other.packetType_var;
    this->retries_var = other.retries_var;
    this->block_var = other.block_var;
}

void OrionDataReqPacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
    doPacking(b,this->retries_var);
    doPacking(b,this->block_var);
}

void OrionDataReqPacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->retries_var);
    doUnpacking(b,this->block_var);
}

unsigned int OrionDataReqPacket::getPacketType() const
{
    return packetType_var;
}

void OrionDataReqPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

unsigned int OrionDataReqPacket::getRetries() const
{
    return retries_var;
}

void OrionDataReqPacket::setRetries(unsigned int retries)
{
    this->retries_var = retries;
}

unsigned int OrionDataReqPacket::getBlock() const
{
    return block_var;
}

void OrionDataReqPacket::setBlock(unsigned int block)
{
    this->block_var = block;
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
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
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
        "packetType",
        "retries",
        "block",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int OrionDataReqPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "retries")==0) return base+1;
    if (fieldName[0]=='b' && strcmp(fieldName, "block")==0) return base+2;
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
        "unsigned int",
        "unsigned int",
        "unsigned int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
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
        case 0: return ulong2string(pp->getPacketType());
        case 1: return ulong2string(pp->getRetries());
        case 2: return ulong2string(pp->getBlock());
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
        case 0: pp->setPacketType(string2ulong(value)); return true;
        case 1: pp->setRetries(string2ulong(value)); return true;
        case 2: pp->setBlock(string2ulong(value)); return true;
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
        default: return NULL;
    }
}

Register_Class(OrionDataAckPacket);

OrionDataAckPacket::OrionDataAckPacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = DATA_REQUEST_ACK;
    this->retries_var = 0;
    this->block_var = 0;
}

OrionDataAckPacket::OrionDataAckPacket(const OrionDataAckPacket& other) : ::OrionPacket(other)
{
    copy(other);
}

OrionDataAckPacket::~OrionDataAckPacket()
{
}

OrionDataAckPacket& OrionDataAckPacket::operator=(const OrionDataAckPacket& other)
{
    if (this==&other) return *this;
    ::OrionPacket::operator=(other);
    copy(other);
    return *this;
}

void OrionDataAckPacket::copy(const OrionDataAckPacket& other)
{
    this->packetType_var = other.packetType_var;
    this->retries_var = other.retries_var;
    this->block_var = other.block_var;
}

void OrionDataAckPacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
    doPacking(b,this->retries_var);
    doPacking(b,this->block_var);
}

void OrionDataAckPacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->retries_var);
    doUnpacking(b,this->block_var);
}

unsigned int OrionDataAckPacket::getPacketType() const
{
    return packetType_var;
}

void OrionDataAckPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

unsigned int OrionDataAckPacket::getRetries() const
{
    return retries_var;
}

void OrionDataAckPacket::setRetries(unsigned int retries)
{
    this->retries_var = retries;
}

unsigned int OrionDataAckPacket::getBlock() const
{
    return block_var;
}

void OrionDataAckPacket::setBlock(unsigned int block)
{
    this->block_var = block;
}

class OrionDataAckPacketDescriptor : public cClassDescriptor
{
  public:
    OrionDataAckPacketDescriptor();
    virtual ~OrionDataAckPacketDescriptor();

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

Register_ClassDescriptor(OrionDataAckPacketDescriptor);

OrionDataAckPacketDescriptor::OrionDataAckPacketDescriptor() : cClassDescriptor("OrionDataAckPacket", "OrionPacket")
{
}

OrionDataAckPacketDescriptor::~OrionDataAckPacketDescriptor()
{
}

bool OrionDataAckPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OrionDataAckPacket *>(obj)!=NULL;
}

const char *OrionDataAckPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OrionDataAckPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int OrionDataAckPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *OrionDataAckPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetType",
        "retries",
        "block",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int OrionDataAckPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "retries")==0) return base+1;
    if (fieldName[0]=='b' && strcmp(fieldName, "block")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OrionDataAckPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "unsigned int",
        "unsigned int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *OrionDataAckPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int OrionDataAckPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataAckPacket *pp = (OrionDataAckPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OrionDataAckPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataAckPacket *pp = (OrionDataAckPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPacketType());
        case 1: return ulong2string(pp->getRetries());
        case 2: return ulong2string(pp->getBlock());
        default: return "";
    }
}

bool OrionDataAckPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataAckPacket *pp = (OrionDataAckPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2ulong(value)); return true;
        case 1: pp->setRetries(string2ulong(value)); return true;
        case 2: pp->setBlock(string2ulong(value)); return true;
        default: return false;
    }
}

const char *OrionDataAckPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *OrionDataAckPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OrionDataAckPacket *pp = (OrionDataAckPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(OrionDataRepPacket);

OrionDataRepPacket::OrionDataRepPacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = DATA_REPLY;
    this->block_var = 0;
    this->numCopiesRemaining_var = 0;
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
    this->packetType_var = other.packetType_var;
    this->block_var = other.block_var;
    this->numCopiesRemaining_var = other.numCopiesRemaining_var;
}

void OrionDataRepPacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
    doPacking(b,this->block_var);
    doPacking(b,this->numCopiesRemaining_var);
}

void OrionDataRepPacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->block_var);
    doUnpacking(b,this->numCopiesRemaining_var);
}

unsigned int OrionDataRepPacket::getPacketType() const
{
    return packetType_var;
}

void OrionDataRepPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

unsigned int OrionDataRepPacket::getBlock() const
{
    return block_var;
}

void OrionDataRepPacket::setBlock(unsigned int block)
{
    this->block_var = block;
}

int OrionDataRepPacket::getNumCopiesRemaining() const
{
    return numCopiesRemaining_var;
}

void OrionDataRepPacket::setNumCopiesRemaining(int numCopiesRemaining)
{
    this->numCopiesRemaining_var = numCopiesRemaining;
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
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
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
        "packetType",
        "block",
        "numCopiesRemaining",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int OrionDataRepPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "block")==0) return base+1;
    if (fieldName[0]=='n' && strcmp(fieldName, "numCopiesRemaining")==0) return base+2;
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
        "unsigned int",
        "unsigned int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
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
        case 0: return ulong2string(pp->getPacketType());
        case 1: return ulong2string(pp->getBlock());
        case 2: return long2string(pp->getNumCopiesRemaining());
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
        case 0: pp->setPacketType(string2ulong(value)); return true;
        case 1: pp->setBlock(string2ulong(value)); return true;
        case 2: pp->setNumCopiesRemaining(string2long(value)); return true;
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
        default: return NULL;
    }
}

Register_Class(ReplicatePacket);

ReplicatePacket::ReplicatePacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = REP_REQUEST;
}

ReplicatePacket::ReplicatePacket(const ReplicatePacket& other) : ::OrionPacket(other)
{
    copy(other);
}

ReplicatePacket::~ReplicatePacket()
{
}

ReplicatePacket& ReplicatePacket::operator=(const ReplicatePacket& other)
{
    if (this==&other) return *this;
    ::OrionPacket::operator=(other);
    copy(other);
    return *this;
}

void ReplicatePacket::copy(const ReplicatePacket& other)
{
    this->packetType_var = other.packetType_var;
}

void ReplicatePacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
}

void ReplicatePacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
}

unsigned int ReplicatePacket::getPacketType() const
{
    return packetType_var;
}

void ReplicatePacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

class ReplicatePacketDescriptor : public cClassDescriptor
{
  public:
    ReplicatePacketDescriptor();
    virtual ~ReplicatePacketDescriptor();

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

Register_ClassDescriptor(ReplicatePacketDescriptor);

ReplicatePacketDescriptor::ReplicatePacketDescriptor() : cClassDescriptor("ReplicatePacket", "OrionPacket")
{
}

ReplicatePacketDescriptor::~ReplicatePacketDescriptor()
{
}

bool ReplicatePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ReplicatePacket *>(obj)!=NULL;
}

const char *ReplicatePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ReplicatePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int ReplicatePacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *ReplicatePacketDescriptor::getFieldName(void *object, int field) const
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

int ReplicatePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ReplicatePacketDescriptor::getFieldTypeString(void *object, int field) const
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

const char *ReplicatePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ReplicatePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ReplicatePacket *pp = (ReplicatePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ReplicatePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ReplicatePacket *pp = (ReplicatePacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPacketType());
        default: return "";
    }
}

bool ReplicatePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ReplicatePacket *pp = (ReplicatePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2ulong(value)); return true;
        default: return false;
    }
}

const char *ReplicatePacketDescriptor::getFieldStructName(void *object, int field) const
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

void *ReplicatePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ReplicatePacket *pp = (ReplicatePacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(ReplicateConfirmPacket);

ReplicateConfirmPacket::ReplicateConfirmPacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = REP_CONFIRM;
}

ReplicateConfirmPacket::ReplicateConfirmPacket(const ReplicateConfirmPacket& other) : ::OrionPacket(other)
{
    copy(other);
}

ReplicateConfirmPacket::~ReplicateConfirmPacket()
{
}

ReplicateConfirmPacket& ReplicateConfirmPacket::operator=(const ReplicateConfirmPacket& other)
{
    if (this==&other) return *this;
    ::OrionPacket::operator=(other);
    copy(other);
    return *this;
}

void ReplicateConfirmPacket::copy(const ReplicateConfirmPacket& other)
{
    this->packetType_var = other.packetType_var;
}

void ReplicateConfirmPacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
}

void ReplicateConfirmPacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
}

unsigned int ReplicateConfirmPacket::getPacketType() const
{
    return packetType_var;
}

void ReplicateConfirmPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

class ReplicateConfirmPacketDescriptor : public cClassDescriptor
{
  public:
    ReplicateConfirmPacketDescriptor();
    virtual ~ReplicateConfirmPacketDescriptor();

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

Register_ClassDescriptor(ReplicateConfirmPacketDescriptor);

ReplicateConfirmPacketDescriptor::ReplicateConfirmPacketDescriptor() : cClassDescriptor("ReplicateConfirmPacket", "OrionPacket")
{
}

ReplicateConfirmPacketDescriptor::~ReplicateConfirmPacketDescriptor()
{
}

bool ReplicateConfirmPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ReplicateConfirmPacket *>(obj)!=NULL;
}

const char *ReplicateConfirmPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ReplicateConfirmPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int ReplicateConfirmPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *ReplicateConfirmPacketDescriptor::getFieldName(void *object, int field) const
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

int ReplicateConfirmPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ReplicateConfirmPacketDescriptor::getFieldTypeString(void *object, int field) const
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

const char *ReplicateConfirmPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ReplicateConfirmPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ReplicateConfirmPacket *pp = (ReplicateConfirmPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ReplicateConfirmPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ReplicateConfirmPacket *pp = (ReplicateConfirmPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPacketType());
        default: return "";
    }
}

bool ReplicateConfirmPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ReplicateConfirmPacket *pp = (ReplicateConfirmPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2ulong(value)); return true;
        default: return false;
    }
}

const char *ReplicateConfirmPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *ReplicateConfirmPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ReplicateConfirmPacket *pp = (ReplicateConfirmPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(ReplicateConfirmAckPacket);

ReplicateConfirmAckPacket::ReplicateConfirmAckPacket(const char *name) : ::OrionPacket(name)
{
    this->packetType_var = REP_CONFIRM_ACK;
    this->numCopiesRemaining_var = 0;
}

ReplicateConfirmAckPacket::ReplicateConfirmAckPacket(const ReplicateConfirmAckPacket& other) : ::OrionPacket(other)
{
    copy(other);
}

ReplicateConfirmAckPacket::~ReplicateConfirmAckPacket()
{
}

ReplicateConfirmAckPacket& ReplicateConfirmAckPacket::operator=(const ReplicateConfirmAckPacket& other)
{
    if (this==&other) return *this;
    ::OrionPacket::operator=(other);
    copy(other);
    return *this;
}

void ReplicateConfirmAckPacket::copy(const ReplicateConfirmAckPacket& other)
{
    this->packetType_var = other.packetType_var;
    this->numCopiesRemaining_var = other.numCopiesRemaining_var;
}

void ReplicateConfirmAckPacket::parsimPack(cCommBuffer *b)
{
    ::OrionPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
    doPacking(b,this->numCopiesRemaining_var);
}

void ReplicateConfirmAckPacket::parsimUnpack(cCommBuffer *b)
{
    ::OrionPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
    doUnpacking(b,this->numCopiesRemaining_var);
}

unsigned int ReplicateConfirmAckPacket::getPacketType() const
{
    return packetType_var;
}

void ReplicateConfirmAckPacket::setPacketType(unsigned int packetType)
{
    this->packetType_var = packetType;
}

unsigned int ReplicateConfirmAckPacket::getNumCopiesRemaining() const
{
    return numCopiesRemaining_var;
}

void ReplicateConfirmAckPacket::setNumCopiesRemaining(unsigned int numCopiesRemaining)
{
    this->numCopiesRemaining_var = numCopiesRemaining;
}

class ReplicateConfirmAckPacketDescriptor : public cClassDescriptor
{
  public:
    ReplicateConfirmAckPacketDescriptor();
    virtual ~ReplicateConfirmAckPacketDescriptor();

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

Register_ClassDescriptor(ReplicateConfirmAckPacketDescriptor);

ReplicateConfirmAckPacketDescriptor::ReplicateConfirmAckPacketDescriptor() : cClassDescriptor("ReplicateConfirmAckPacket", "OrionPacket")
{
}

ReplicateConfirmAckPacketDescriptor::~ReplicateConfirmAckPacketDescriptor()
{
}

bool ReplicateConfirmAckPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ReplicateConfirmAckPacket *>(obj)!=NULL;
}

const char *ReplicateConfirmAckPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ReplicateConfirmAckPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int ReplicateConfirmAckPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *ReplicateConfirmAckPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetType",
        "numCopiesRemaining",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int ReplicateConfirmAckPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "numCopiesRemaining")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ReplicateConfirmAckPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "unsigned int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *ReplicateConfirmAckPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ReplicateConfirmAckPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ReplicateConfirmAckPacket *pp = (ReplicateConfirmAckPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ReplicateConfirmAckPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ReplicateConfirmAckPacket *pp = (ReplicateConfirmAckPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPacketType());
        case 1: return ulong2string(pp->getNumCopiesRemaining());
        default: return "";
    }
}

bool ReplicateConfirmAckPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ReplicateConfirmAckPacket *pp = (ReplicateConfirmAckPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2ulong(value)); return true;
        case 1: pp->setNumCopiesRemaining(string2ulong(value)); return true;
        default: return false;
    }
}

const char *ReplicateConfirmAckPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *ReplicateConfirmAckPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ReplicateConfirmAckPacket *pp = (ReplicateConfirmAckPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(WaitForReq);

WaitForReq::WaitForReq(const char *name, int kind) : ::cMessage(name,kind)
{
    this->filename_var = 0;
    this->bid_var = 0;
    this->block_var = 0;
}

WaitForReq::WaitForReq(const WaitForReq& other) : ::cMessage(other)
{
    copy(other);
}

WaitForReq::~WaitForReq()
{
}

WaitForReq& WaitForReq::operator=(const WaitForReq& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void WaitForReq::copy(const WaitForReq& other)
{
    this->filename_var = other.filename_var;
    this->bid_var = other.bid_var;
    this->block_var = other.block_var;
}

void WaitForReq::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->filename_var);
    doPacking(b,this->bid_var);
    doPacking(b,this->block_var);
}

void WaitForReq::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->filename_var);
    doUnpacking(b,this->bid_var);
    doUnpacking(b,this->block_var);
}

const char * WaitForReq::getFilename() const
{
    return filename_var.c_str();
}

void WaitForReq::setFilename(const char * filename)
{
    this->filename_var = filename;
}

const char * WaitForReq::getBid() const
{
    return bid_var.c_str();
}

void WaitForReq::setBid(const char * bid)
{
    this->bid_var = bid;
}

unsigned int WaitForReq::getBlock() const
{
    return block_var;
}

void WaitForReq::setBlock(unsigned int block)
{
    this->block_var = block;
}

class WaitForReqDescriptor : public cClassDescriptor
{
  public:
    WaitForReqDescriptor();
    virtual ~WaitForReqDescriptor();

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

Register_ClassDescriptor(WaitForReqDescriptor);

WaitForReqDescriptor::WaitForReqDescriptor() : cClassDescriptor("WaitForReq", "cMessage")
{
}

WaitForReqDescriptor::~WaitForReqDescriptor()
{
}

bool WaitForReqDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<WaitForReq *>(obj)!=NULL;
}

const char *WaitForReqDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int WaitForReqDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int WaitForReqDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *WaitForReqDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "filename",
        "bid",
        "block",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int WaitForReqDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "filename")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bid")==0) return base+1;
    if (fieldName[0]=='b' && strcmp(fieldName, "block")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *WaitForReqDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "unsigned int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *WaitForReqDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int WaitForReqDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    WaitForReq *pp = (WaitForReq *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string WaitForReqDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    WaitForReq *pp = (WaitForReq *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getFilename());
        case 1: return oppstring2string(pp->getBid());
        case 2: return ulong2string(pp->getBlock());
        default: return "";
    }
}

bool WaitForReqDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    WaitForReq *pp = (WaitForReq *)object; (void)pp;
    switch (field) {
        case 0: pp->setFilename((value)); return true;
        case 1: pp->setBid((value)); return true;
        case 2: pp->setBlock(string2ulong(value)); return true;
        default: return false;
    }
}

const char *WaitForReqDescriptor::getFieldStructName(void *object, int field) const
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

void *WaitForReqDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    WaitForReq *pp = (WaitForReq *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(DelayMsg);

DelayMsg::DelayMsg(const char *name, int kind) : ::cMessage(name,kind)
{
    this->bid_var = 0;
    this->broadcast_var = 0;
    this->deleteMe_var = 0;
}

DelayMsg::DelayMsg(const DelayMsg& other) : ::cMessage(other)
{
    copy(other);
}

DelayMsg::~DelayMsg()
{
}

DelayMsg& DelayMsg::operator=(const DelayMsg& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void DelayMsg::copy(const DelayMsg& other)
{
    this->bid_var = other.bid_var;
    this->broadcast_var = other.broadcast_var;
    this->deleteMe_var = other.deleteMe_var;
}

void DelayMsg::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->bid_var);
    doPacking(b,this->broadcast_var);
    doPacking(b,this->deleteMe_var);
}

void DelayMsg::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->bid_var);
    doUnpacking(b,this->broadcast_var);
    doUnpacking(b,this->deleteMe_var);
}

const char * DelayMsg::getBid() const
{
    return bid_var.c_str();
}

void DelayMsg::setBid(const char * bid)
{
    this->bid_var = bid;
}

bool DelayMsg::getBroadcast() const
{
    return broadcast_var;
}

void DelayMsg::setBroadcast(bool broadcast)
{
    this->broadcast_var = broadcast;
}

bool DelayMsg::getDeleteMe() const
{
    return deleteMe_var;
}

void DelayMsg::setDeleteMe(bool deleteMe)
{
    this->deleteMe_var = deleteMe;
}

class DelayMsgDescriptor : public cClassDescriptor
{
  public:
    DelayMsgDescriptor();
    virtual ~DelayMsgDescriptor();

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

Register_ClassDescriptor(DelayMsgDescriptor);

DelayMsgDescriptor::DelayMsgDescriptor() : cClassDescriptor("DelayMsg", "cMessage")
{
}

DelayMsgDescriptor::~DelayMsgDescriptor()
{
}

bool DelayMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DelayMsg *>(obj)!=NULL;
}

const char *DelayMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DelayMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int DelayMsgDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *DelayMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "bid",
        "broadcast",
        "deleteMe",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int DelayMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bid")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "broadcast")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "deleteMe")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DelayMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "bool",
        "bool",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *DelayMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int DelayMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DelayMsg *pp = (DelayMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DelayMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DelayMsg *pp = (DelayMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getBid());
        case 1: return bool2string(pp->getBroadcast());
        case 2: return bool2string(pp->getDeleteMe());
        default: return "";
    }
}

bool DelayMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DelayMsg *pp = (DelayMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setBid((value)); return true;
        case 1: pp->setBroadcast(string2bool(value)); return true;
        case 2: pp->setDeleteMe(string2bool(value)); return true;
        default: return false;
    }
}

const char *DelayMsgDescriptor::getFieldStructName(void *object, int field) const
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

void *DelayMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DelayMsg *pp = (DelayMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(ReqBlockTimer);

ReqBlockTimer::ReqBlockTimer(const char *name, int kind) : ::cMessage(name,kind)
{
    this->filename_var = 0;
    this->bid_var = 0;
}

ReqBlockTimer::ReqBlockTimer(const ReqBlockTimer& other) : ::cMessage(other)
{
    copy(other);
}

ReqBlockTimer::~ReqBlockTimer()
{
}

ReqBlockTimer& ReqBlockTimer::operator=(const ReqBlockTimer& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ReqBlockTimer::copy(const ReqBlockTimer& other)
{
    this->filename_var = other.filename_var;
    this->bid_var = other.bid_var;
}

void ReqBlockTimer::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->filename_var);
    doPacking(b,this->bid_var);
}

void ReqBlockTimer::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->filename_var);
    doUnpacking(b,this->bid_var);
}

const char * ReqBlockTimer::getFilename() const
{
    return filename_var.c_str();
}

void ReqBlockTimer::setFilename(const char * filename)
{
    this->filename_var = filename;
}

const char * ReqBlockTimer::getBid() const
{
    return bid_var.c_str();
}

void ReqBlockTimer::setBid(const char * bid)
{
    this->bid_var = bid;
}

class ReqBlockTimerDescriptor : public cClassDescriptor
{
  public:
    ReqBlockTimerDescriptor();
    virtual ~ReqBlockTimerDescriptor();

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

Register_ClassDescriptor(ReqBlockTimerDescriptor);

ReqBlockTimerDescriptor::ReqBlockTimerDescriptor() : cClassDescriptor("ReqBlockTimer", "cMessage")
{
}

ReqBlockTimerDescriptor::~ReqBlockTimerDescriptor()
{
}

bool ReqBlockTimerDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ReqBlockTimer *>(obj)!=NULL;
}

const char *ReqBlockTimerDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ReqBlockTimerDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int ReqBlockTimerDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *ReqBlockTimerDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "filename",
        "bid",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int ReqBlockTimerDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "filename")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bid")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ReqBlockTimerDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *ReqBlockTimerDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ReqBlockTimerDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ReqBlockTimer *pp = (ReqBlockTimer *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ReqBlockTimerDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ReqBlockTimer *pp = (ReqBlockTimer *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getFilename());
        case 1: return oppstring2string(pp->getBid());
        default: return "";
    }
}

bool ReqBlockTimerDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ReqBlockTimer *pp = (ReqBlockTimer *)object; (void)pp;
    switch (field) {
        case 0: pp->setFilename((value)); return true;
        case 1: pp->setBid((value)); return true;
        default: return false;
    }
}

const char *ReqBlockTimerDescriptor::getFieldStructName(void *object, int field) const
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

void *ReqBlockTimerDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ReqBlockTimer *pp = (ReqBlockTimer *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(ChurnMsg);

ChurnMsg::ChurnMsg(const char *name, int kind) : ::cMessage(name,kind)
{
    this->command_var = 0;
    this->fileName_var = 0;
}

ChurnMsg::ChurnMsg(const ChurnMsg& other) : ::cMessage(other)
{
    copy(other);
}

ChurnMsg::~ChurnMsg()
{
}

ChurnMsg& ChurnMsg::operator=(const ChurnMsg& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ChurnMsg::copy(const ChurnMsg& other)
{
    this->command_var = other.command_var;
    this->fileName_var = other.fileName_var;
}

void ChurnMsg::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->command_var);
    doPacking(b,this->fileName_var);
}

void ChurnMsg::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->command_var);
    doUnpacking(b,this->fileName_var);
}

const char * ChurnMsg::getCommand() const
{
    return command_var.c_str();
}

void ChurnMsg::setCommand(const char * command)
{
    this->command_var = command;
}

const char * ChurnMsg::getFileName() const
{
    return fileName_var.c_str();
}

void ChurnMsg::setFileName(const char * fileName)
{
    this->fileName_var = fileName;
}

class ChurnMsgDescriptor : public cClassDescriptor
{
  public:
    ChurnMsgDescriptor();
    virtual ~ChurnMsgDescriptor();

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

Register_ClassDescriptor(ChurnMsgDescriptor);

ChurnMsgDescriptor::ChurnMsgDescriptor() : cClassDescriptor("ChurnMsg", "cMessage")
{
}

ChurnMsgDescriptor::~ChurnMsgDescriptor()
{
}

bool ChurnMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ChurnMsg *>(obj)!=NULL;
}

const char *ChurnMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ChurnMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int ChurnMsgDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *ChurnMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "command",
        "fileName",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int ChurnMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "command")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fileName")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ChurnMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *ChurnMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ChurnMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ChurnMsg *pp = (ChurnMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ChurnMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ChurnMsg *pp = (ChurnMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getCommand());
        case 1: return oppstring2string(pp->getFileName());
        default: return "";
    }
}

bool ChurnMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ChurnMsg *pp = (ChurnMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setCommand((value)); return true;
        case 1: pp->setFileName((value)); return true;
        default: return false;
    }
}

const char *ChurnMsgDescriptor::getFieldStructName(void *object, int field) const
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

void *ChurnMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ChurnMsg *pp = (ChurnMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(QueryMsg);

QueryMsg::QueryMsg(const char *name, int kind) : ::cMessage(name,kind)
{
    this->fileName_var = 0;
    this->seq_var = 0;
    this->sourceId_var = 0;
}

QueryMsg::QueryMsg(const QueryMsg& other) : ::cMessage(other)
{
    copy(other);
}

QueryMsg::~QueryMsg()
{
}

QueryMsg& QueryMsg::operator=(const QueryMsg& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void QueryMsg::copy(const QueryMsg& other)
{
    this->fileName_var = other.fileName_var;
    this->seq_var = other.seq_var;
    this->SRC_var = other.SRC_var;
    this->sourceId_var = other.sourceId_var;
}

void QueryMsg::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->fileName_var);
    doPacking(b,this->seq_var);
    doPacking(b,this->SRC_var);
    doPacking(b,this->sourceId_var);
}

void QueryMsg::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->fileName_var);
    doUnpacking(b,this->seq_var);
    doUnpacking(b,this->SRC_var);
    doUnpacking(b,this->sourceId_var);
}

const char * QueryMsg::getFileName() const
{
    return fileName_var.c_str();
}

void QueryMsg::setFileName(const char * fileName)
{
    this->fileName_var = fileName;
}

int QueryMsg::getSeq() const
{
    return seq_var;
}

void QueryMsg::setSeq(int seq)
{
    this->seq_var = seq;
}

IPvXAddress& QueryMsg::getSRC()
{
    return SRC_var;
}

void QueryMsg::setSRC(const IPvXAddress& SRC)
{
    this->SRC_var = SRC;
}

const char * QueryMsg::getSourceId() const
{
    return sourceId_var.c_str();
}

void QueryMsg::setSourceId(const char * sourceId)
{
    this->sourceId_var = sourceId;
}

class QueryMsgDescriptor : public cClassDescriptor
{
  public:
    QueryMsgDescriptor();
    virtual ~QueryMsgDescriptor();

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

Register_ClassDescriptor(QueryMsgDescriptor);

QueryMsgDescriptor::QueryMsgDescriptor() : cClassDescriptor("QueryMsg", "cMessage")
{
}

QueryMsgDescriptor::~QueryMsgDescriptor()
{
}

bool QueryMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<QueryMsg *>(obj)!=NULL;
}

const char *QueryMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int QueryMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int QueryMsgDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *QueryMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "fileName",
        "seq",
        "SRC",
        "sourceId",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int QueryMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fileName")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "seq")==0) return base+1;
    if (fieldName[0]=='S' && strcmp(fieldName, "SRC")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceId")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *QueryMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "int",
        "IPvXAddress",
        "string",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *QueryMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int QueryMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    QueryMsg *pp = (QueryMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string QueryMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    QueryMsg *pp = (QueryMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getFileName());
        case 1: return long2string(pp->getSeq());
        case 2: {std::stringstream out; out << pp->getSRC(); return out.str();}
        case 3: return oppstring2string(pp->getSourceId());
        default: return "";
    }
}

bool QueryMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    QueryMsg *pp = (QueryMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setFileName((value)); return true;
        case 1: pp->setSeq(string2long(value)); return true;
        case 3: pp->setSourceId((value)); return true;
        default: return false;
    }
}

const char *QueryMsgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 2: return opp_typename(typeid(IPvXAddress));
        default: return NULL;
    };
}

void *QueryMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    QueryMsg *pp = (QueryMsg *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getSRC()); break;
        default: return NULL;
    }
}


