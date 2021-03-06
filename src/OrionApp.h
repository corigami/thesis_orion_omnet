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


#ifndef ORIONAPP_H
#define ORIONAPP_H

#include <time.h>
#include <vector>
#include <string>
#include <iostream>
#include <deque>
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
class /*INET_API */OrionApp : public ApplicationBase
{
  protected:
    enum SelfMsgKinds { START = 1, SEND, STOP};
    UDPSocket socket;
    bool debugEnabled[3];
    bool socketOpen;
    bool active;
    bool packetTrace;
    bool printBroadcast;
    bool printContainers;
    int localPort, destPort;
    int numberNodes;
    int fileNum;
    int myId;
    IPvXAddress myAddr ;
    std::string myNameStr;

    unsigned int querySeqNum;
    unsigned int reqSeqNum;
    unsigned int replicateSeqNum;
    unsigned int retries;

    std::vector<int> outputInterfaceMulticastBroadcast;
    std::vector<IPvXAddress> destAddresses;

    std::map<std::string, OrionPacket*> pendingPackets;
    std::map<std::string, QueryMsg*> pendingQueries;
    std::map<std::string, WaitForReq*> pendingTimeouts;
    std::map<std::string, ReqBlockTimer*> pendingBlockTimers;

    std::map<std::string, std::pair<IPvXAddress, int> > fileList;  //!< files system abstraction (just a list of file names)
    std::map<std::string, std::pair<IPvXAddress, simtime_t> > queryList;
    std::map<std::string, unsigned int> replicateList;
    std::map<std::string, std::pair<IPvXAddress,simtime_t> > requestList;
    std::map<std::string, FileTableData> myQueryResults;
    std::map<std::string, FileTableData> queryResults;
    std::list<std::string> tabooList;

    std::string nodeID;
    simtime_t startTime;
    simtime_t retryDelay;
    simtime_t stopTime;
    simtime_t avgQueryTime;
    simtime_t fileQueryRate;
    simtime_t fileGenRate;
    simtime_t churnDuration;
    double churnRate;
    double replicateRate;
    double mobilityRate;

    cMessage *selfMsg;
    cMessage *fileGenMsg;
    cMessage *fileRequestMsg;
    ChurnMsg *churnTimerMsg;

    // statistics
    static unsigned int sentOPackets;
    static unsigned int recOPackets;

    int xferReqs;
    int xferFails;
    int xferCompletes;
    int fileSize;
    int repCount;
    bool master;

    static simsignal_t sentPkSignal;
    static simsignal_t rcvdPkSignal;
    simsignal_t qTimeSignal;
    simsignal_t tranCompSignal;
    simsignal_t queryCompSignal;

    // chooses random destination address
    //virtual IPvXAddress chooseDestAddr();


  //  virtual void sendPacket();
    virtual void processPacket(cPacket *msg);
    virtual void setSocketOptions();


    //-----------Orion Operations-----------------

    //- send functions
    void sendQuery(std::string fileToRequest, unsigned int seq, IPvXAddress src, std::string sourceId);
    void sendResponse(OrionPacket *oPacket);
    void sendRequest(std::string fileToRequest);
    void sendError(std::string fileName, IPvXAddress dst, int seq, std::string id,bool delay);
    void sendRequestAck(OrionDataReqPacket* reqPacket);
    void resendRequest(OrionDataReqPacket* reqPacket);
    void requestNextBlock(std::string filename);
    void sendReply(std::string fileToRequest, unsigned int block, IPvXAddress dest);

    //replication functions
    void sendReplicateReq(std::string fileToRep, unsigned int seq, IPvXAddress origin);
    void sendReplicateConfirm(ReplicateConfirmPacket *replicateRes);
    void sendReplicateResponseAck(ReplicateConfirmAckPacket *replicateAck);
    void sendPacket(OrionPacket * pkt);
    bool sendBroadcast(const IPvXAddress &dest, OrionPacket *pkt);
    void delaySend(DelayMsg *message);
    //receive functions
    void handleChurnMsg(ChurnMsg *churnMsg);
    void handleQuery(OrionQueryPacket *qPacket);
    void handleResponse(OrionResponsePacket *rPacket);
    void handleRequest(OrionDataReqPacket *reqPacket);
    void handleRequestError(OrionErrorPacket *errPacket);
    void handleReqAckTimeout();
    void handleRequestAck(OrionDataAckPacket *ackPacket);
    void handleReply(OrionDataRepPacket *repPacket);
    void handleReplicateReq(ReplicatePacket *replicate);
    void handleReplicateConfirm(ReplicateConfirmPacket *replicateRes);
    void handleReplicateConfirmAck(ReplicateConfirmAckPacket *replicateAck);


    //utility functions
    void clearTimersAndLists();
    void generateFile();
    void churnNode();
    void queryFile();
    std::string selectFile();
    bool hasFile(std::string reqFile);
    void deleteFileTable();

    //print functions
    void printTransfer(std::string fileName);
    void printResults();
    void printContainerContents();
    void printFileTable();
    void printPacketRec(OrionPacket *oPacket);
    void printPacketSend(OrionPacket *oPacket);





    /*
     *
     */
    void debug(std::string msg, int level = 1);
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

