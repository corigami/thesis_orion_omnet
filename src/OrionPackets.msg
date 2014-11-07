//
// Copyright (C) 2014 OpenSim Ltd.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

cplusplus {{
#include "IPvXAddress.h"
#include "simtime_t.h"
}}

class noncobject IPvXAddress;

enum OrionPacketType
{
    QUERY = 1;
    RESPONSE = 2;
    DATA_REQUEST = 3;
    DATA_REPLY = 4;
    DATA_REQUEST_ACK = 5;
    DATA_ERR = 6;
    REP_REQUEST = 7;
    REP_CONFIRM = 8;
    REP_CONFIRM_ACK = 9;
};

//
// Base packet for Orion Control Packets
//
packet OrionPacket extends cPacket
{
    unsigned int packetType;
    IPvXAddress DST;
    IPvXAddress SRC;
    string sourceId;
    IPvXAddress LastNode;
    string LastNodeId;
    IPvXAddress origin;
    unsigned int SEQ;
    string filename;
    unsigned int hopCount;
    string bid;
    simtime_t start;

}

//
// Represents an Orion Route Request
//
class OrionQueryPacket extends OrionPacket
{
    unsigned int packetType = QUERY;
   

 }
 
 class OrionResponsePacket extends OrionPacket
{
    unsigned int packetType = RESPONSE;

 }
 
 class OrionErrorPacket extends OrionPacket
{
    unsigned int packetType = DATA_ERR;
    string requestId;
 }
 

//
// Represents an Orion Route Reply
//
class OrionDataReqPacket extends OrionPacket
{
    unsigned int packetType = DATA_REQUEST;
    unsigned int retries;
   	unsigned int block;
   	
}

class OrionDataAckPacket extends OrionPacket
{
    unsigned int packetType = DATA_REQUEST_ACK;
    unsigned int retries;
   	unsigned int block;
}

//
// Represents an Orion Route Error
//
class OrionDataRepPacket extends OrionPacket
{
   
    unsigned int packetType = DATA_REPLY;
    unsigned int block;
    int numCopiesRemaining;
    
    
}

 class ReplicatePacket extends OrionPacket
{
    unsigned int packetType = REP_REQUEST;
 }
 
  class ReplicateConfirmPacket extends OrionPacket
{
    unsigned int packetType = REP_CONFIRM;
 }
 
  class ReplicateConfirmAckPacket extends OrionPacket
{
    unsigned int packetType = REP_CONFIRM_ACK;
    unsigned int numCopiesRemaining;
 }

//
// Represents a timer for a Route Reply packet
//
message WaitForReq extends cMessage
{
    string filename;
    string bid;
    unsigned int block;
}

message DelayMsg extends cMessage
{
    string bid;
    bool broadcast;
    bool deleteMe;

}

message ReqBlockTimer extends cMessage
{
    string filename;
}

message ChurnMsg extends cMessage
{
    string command;
    string fileName;
}


message QueryMsg extends cMessage
{
    string fileName;
    int seq;
    IPvXAddress SRC;
    string sourceId;

}