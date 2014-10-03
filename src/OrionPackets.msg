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
};

//
// Base packet for Orion Control Packets
//
packet OrionPacket extends cPacket
{
    unsigned int packetType;
    IPvXAddress SRC;
    IPvXAddress LastNode;
    unsigned int SEQ;
}

//
// Represents an Orion Route Request
//
class OrionQueryPacket extends OrionPacket
{
    unsigned int packetType = QUERY;
    string filename;
 }
 
 class OrionResponsePacket extends OrionPacket
{
    unsigned int packetType = RESPONSE;
    string filename;
    unsigned int hopcount;
 }

//
// Represents an Orion Route Reply
//
class OrionDataReqPacket extends OrionPacket
{
   // IPvXAddress DST;
    unsigned int packetType = DATA_REQUEST;
    string filename;
   // unsigned int blockNum;
    

}

//
// Represents an Orion Route Error
//
class OrionDataRepPacket extends OrionPacket
{
    IPvXAddress DST;
    unsigned int packetType = DATA_REPLY;
    string filename;
    unsigned int blockNum;
}


class OrionDataReqPacketAck extends OrionPacket
{
unsigned int packetType = DATA_REQUEST_ACK;
}