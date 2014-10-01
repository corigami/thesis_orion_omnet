//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2004,2011 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//


#ifndef __INET_ORIONAPP_H
#define __INET_ORIONAPP_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "INETDefs.h"
#include "ApplicationBase.h"
#include "UDPSocket.h"
#include "FileTableData.h"
#include "OrionPackets_m.h"
#include "UDPPacket.h"

/**
 * UDP application. See NED for more info.
 */
class INET_API OrionApp : public ApplicationBase
{
  protected:
    enum SelfMsgKinds { START = 1, SEND, STOP};

    UDPSocket socket;
    bool debugEnabled;
    bool socketOpen;
    int localPort, destPort;
    int numberNodes;
    int fileNum;
    int myId;
    IPvXAddress myAddr ;

    unsigned int querySeqNum;

    std::vector<int> outputInterfaceMulticastBroadcast;
    std::vector<IPvXAddress> destAddresses;
    std::string nodeID;
    simtime_t startTime;
    simtime_t stopTime;
    simtime_t endBuffer;
    simtime_t avgQueryTime;
    simtime_t fileQueryRate;
    simtime_t fileGenRate;

    cMessage *selfMsg;
    cMessage *fileGenMsg;
    cMessage *fileRequestMsg;
    std::vector<std::string> fileList;  //!< files system abstraction (just a list of file names)
    std::map<unsigned int, IPvXAddress> queryList;
    std::map<std::string, FileTableData> queryResults;
    // statistics
    int numSent;
    int numReceived;
    int fileSize;
    bool master;

    static simsignal_t sentPkSignal;
    static simsignal_t rcvdPkSignal;


    // chooses random destination address
    virtual IPvXAddress chooseDestAddr();


    virtual void sendPacket();
    virtual void processPacket(cPacket *msg);
    virtual void setSocketOptions();


    //-----------Orion Operations-----------------

    //- send functions
    void sendQuery(std::string fileToRequest, unsigned int seq, IPvXAddress src);
    void sendResponse(OrionPacket *oPacket);
    void sendRequest(std::string fileToRequest, unsigned int block, IPvXAddress dest);
    void sendRequestAck(std::string fileToRequest, unsigned int block, IPvXAddress dest);
    void sendReply(std::string fileToRequest, unsigned int block, IPvXAddress dest);


    bool sendBroadcast(const IPvXAddress &dest, cPacket *pkt);

    //receive functions
    void handleQuery(OrionQueryPacket *qPacket);
    void handleResponse(OrionResponsePacket *rPacket);
    void handleRequest(OrionResponsePacket *reqPacket);
    void handleRequestAck(OrionResponsePacket *rackPacket);
    void handleReply(OrionResponsePacket *repPacket);


    //utility functions
    void generateFile();
    void queryFile();
    std::string selectFile();
    bool hasFile(std::string reqFile);
    void deleteFileTable();



    /*
     *
     */
    void debug(std::string msg);
  public:
    OrionApp();
    ~OrionApp();

  protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void handleMessageWhenUp(cMessage *msg);
    virtual void finish();

    virtual void processStart();
    virtual void processSend();
    virtual void processStop();

    virtual bool handleNodeStart(IDoneCallback *doneCallback);
    virtual bool handleNodeShutdown(IDoneCallback *doneCallback);
    virtual void handleNodeCrash();
};

#endif

