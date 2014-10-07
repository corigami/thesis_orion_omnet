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


#include "OrionApp.h"
#include "FileTableData.h"
#include "InterfaceTableAccess.h"
#include "IPvXAddressResolver.h"
#include "IPv4ControlInfo.h"
#include "NodeOperations.h"
#include "UDPControlInfo.h"
#include "UDPControlInfo_m.h"


Define_Module(OrionApp);

simsignal_t OrionApp::sentPkSignal = registerSignal("sentPk");
simsignal_t OrionApp::rcvdPkSignal = registerSignal("rcvdPk");
simsignal_t OrionApp::qTimeSignal = registerSignal("qTime");

OrionApp::OrionApp()
{
    selfMsg = NULL;
    fileGenMsg = NULL;
    fileRequestMsg = NULL;
    fileNum = 1;
    querySeqNum = 0;
    endBuffer = 5;
}

OrionApp::~OrionApp()
{
 //   delete &queryList;
    cancelAndDelete(selfMsg);
    cancelAndDelete(fileGenMsg);
    cancelAndDelete(fileRequestMsg);
}

void OrionApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == 0)
    {
        numSent = 0;
        numReceived = 0;

        //variables to watch in GUI environment
        WATCH(numSent);
        WATCH(numReceived);

        //load parameters from .ned file
        localPort = par("localPort");
        master = par("master");
        destPort = par("destPort");
        retries = par("retries");
        fileQueryRate = par("fileQueryRate").doubleValue();
        fileGenRate = par("fileGenRate");
        debugEnabled = par("enableDebug");
        startTime = par("startTime").doubleValue();
        stopTime = par("stopTime").doubleValue();
        fileSize = par("fileSize");
        numberNodes = par("numHosts");
        retryDelay = par("retryDelay").doubleValue();
      //  queryList = *(new std::map<int, IPvXAddress>());

        if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
            error("Invalid startTime/stopTime parameters");

        //self Msg
        selfMsg = new cMessage("sendTimer");
        fileGenMsg = new cMessage("fileGenTimer");
        fileRequestMsg = new cMessage("fileRequestTimer");

    }
}

void OrionApp::finish()
{
    recordScalar("packets sent", numSent);
    recordScalar("packets received", numReceived);
    ApplicationBase::finish();
}

void OrionApp::setSocketOptions()
{
    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
        socket.setTimeToLive(timeToLive);

    int typeOfService = par("typeOfService");
    if (typeOfService != -1)
        socket.setTypeOfService(typeOfService);

    const char *multicastInterface = par("multicastInterface");
    if (multicastInterface[0])
    {
        IInterfaceTable *ift = InterfaceTableAccess().get(this);
        InterfaceEntry *ie = ift->getInterfaceByName(multicastInterface);
        if (!ie)
            throw cRuntimeError("Wrong multicastInterface setting: no interface named \"%s\"", multicastInterface);
        socket.setMulticastOutputInterface(ie->getInterfaceId());
    }

    bool receiveBroadcast = par("receiveBroadcast");
    if (receiveBroadcast)
        socket.setBroadcast(true);

    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");
    if (joinLocalMulticastGroups)
        socket.joinLocalMulticastGroups();
}

IPvXAddress OrionApp::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    if (destAddresses[k].isIPv6() && destAddresses[k].get6().isLinkLocal()) // KLUDGE for IPv6
    {
        const char *destAddrs = par("destAddresses");
        cStringTokenizer tokenizer(destAddrs);
        const char *token;

        for (int i = 0; i <= k; ++i)
            token = tokenizer.nextToken();
        destAddresses[k] = IPvXAddressResolver().resolve(token);
    }
    return destAddresses[k];
}

void OrionApp::sendPacket()
{
    char msgName[32];
    sprintf(msgName, "Packet-%d", numSent);
    cPacket *payload = new cPacket(msgName);
    payload->setByteLength(par("messageLength").longValue());
    IPvXAddress destAddr = chooseDestAddr();
   // IPvXAddress destAddr = "host[15]";
    emit(sentPkSignal, payload);
    socket.sendTo(payload, destAddr, destPort);
    numSent++;
}


/*
 * Function: sendBroadcast()
 * Description: prints out string messages to the console
 *  - Param:    *pkt:
 *              &dest:
 * Returns: void
 */
bool OrionApp::sendBroadcast(const IPvXAddress &dest, cPacket *pkt)
{
    if (!outputInterfaceMulticastBroadcast.empty() && (dest.isMulticast() || (!dest.isIPv6() && dest.get4() == IPv4Address::ALLONES_ADDRESS)))
    {

        for (unsigned int i = 0; i < outputInterfaceMulticastBroadcast.size(); i++)
        {
            UDPSocket::SendOptions options;
            options.outInterfaceId = outputInterfaceMulticastBroadcast[i];
            if (outputInterfaceMulticastBroadcast.size() - i > 1)
                socket.sendTo(pkt->dup(), dest, destPort, &options);
            else
                socket.sendTo(pkt, dest, destPort, &options);

        }
        return true;
    }
    return false;
}

void OrionApp::processStart()
{
    socket.setOutputGate(gate("udpOut"));
    socket.bind(localPort);
    socketOpen=true;
    setSocketOptions();

    const char *destAddrs = par("destAddresses");
    cStringTokenizer tokenizer(destAddrs);
    const char *token;

    while ((token = tokenizer.nextToken()) != NULL) {
        IPvXAddress result;
        IPvXAddressResolver().tryResolve(token, result);
        if (result.isUnspecified())
            EV << "cannot resolve destination address: " << token << endl;
        else
            destAddresses.push_back(result);
    }

    //starts sending packets (not part of orion... )
    if (!destAddresses.empty())
    {
        selfMsg->setKind(SEND);
        processSend();
    }
    else
    {
        if (stopTime >= SIMTIME_ZERO)
        {
            debug("ProcessStart calling stop");
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
    }


    //orion processes to start
    if(master){ // schedules first file request from system
        simtime_t d = simTime() + par("fileQueryRate").doubleValue() + 1;
        if (stopTime < SIMTIME_ZERO || d < stopTime){
            scheduleAt(d, fileRequestMsg);
        }
        else
        {
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
    }
    else{

        simtime_t d = simTime() + par("fileGenRate").doubleValue();
        if (stopTime < SIMTIME_ZERO || d < stopTime){
            scheduleAt(d, fileGenMsg);
        }
        else
        {
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
    }

    //-------------------Broadcast options test--------------------------

    outputInterfaceMulticastBroadcast.clear();
    if (strcmp(par("outputInterfaceMulticastBroadcast").stringValue(),"") != 0)
    {
        IInterfaceTable* ift = InterfaceTableAccess().get();
        const char *ports = par("outputInterfaceMulticastBroadcast");
        cStringTokenizer tokenizer(ports);
        const char *token;
        while ((token = tokenizer.nextToken()) != NULL)
        {
            if (strstr(token, "ALL") != NULL)
            {
                for (int i = 0; i < ift->getNumInterfaces(); i++)
                {
                    InterfaceEntry *ie = ift->getInterface(i);
                    if (ie->isLoopback())
                        continue;
                    if (ie == NULL)
                        throw cRuntimeError(this, "Invalid output interface name : %s", token);
                    outputInterfaceMulticastBroadcast.push_back(ie->getInterfaceId());
                }
            }
            else
            {
                InterfaceEntry *ie = ift->getInterfaceByName(token);
                if (ie == NULL)
                    throw cRuntimeError(this, "Invalid output interface name : %s", token);
                outputInterfaceMulticastBroadcast.push_back(ie->getInterfaceId());
            }
        }
    }
    myAddr = IPvXAddressResolver().resolve(this->getParentModule()->getFullPath().c_str());
    myId = this->getParentModule()->getId();

    //----------------------end broadcast options test


}

void OrionApp::processSend()
{
  //  sendPacket();
    simtime_t d = simTime() + par("sendInterval").doubleValue();
    if (stopTime < SIMTIME_ZERO || d < stopTime)
    {
        selfMsg->setKind(SEND);
        scheduleAt(d, selfMsg);
    }
    else
    {
        debug("ProcessSend stopping");
        selfMsg->setKind(STOP);
        scheduleAt(stopTime, selfMsg);
    }
}

void OrionApp::processStop()
{
    if(socketOpen){
    socket.close();
    socketOpen = false;
    }
}

void OrionApp::handleMessageWhenUp(cMessage *msg)
{
 if (msg->isSelfMessage())
    {
    if(msg==fileGenMsg)  {
     //   debug("handling fileGen");
        generateFile();
    }else if(msg == fileRequestMsg){
    //    debug("handling requestFile");
        queryFile();
    }else if(dynamic_cast<ReqBlockTimer *>(msg)){
        std::string temp = dynamic_cast<WaitForReq *>(msg)->getFilename();
        requestNextBlock(temp);
    }else if(dynamic_cast<WaitForReq *>(msg)){

        debug("Timeout expired resending");
        std::string temp = dynamic_cast<WaitForReq *>(msg)->getFilename();
        resendRequest(pendingPackets[temp]);


    }else if(msg == selfMsg)
        ASSERT(msg == selfMsg);
        switch (selfMsg->getKind()) {
            case START: processStart(); break;
            case SEND:
               break;
            case STOP:  processStop(); break;
            default: throw cRuntimeError("Invalid kind %d in self message", (int)selfMsg->getKind());
        }
    }
    else if (msg->getKind() == UDP_I_DATA)
    {
        OrionPacket *oPacket = dynamic_cast<OrionPacket *>(PK(msg));
            //handle different type of packets
            switch (oPacket->getPacketType()) {
                case QUERY:
                    handleQuery(dynamic_cast<OrionQueryPacket *>(oPacket));
                    break;
                case RESPONSE:
                    handleResponse(dynamic_cast<OrionResponsePacket *>(oPacket));
                    break;
                case DATA_REQUEST:
                    handleRequest(dynamic_cast<OrionDataReqPacket *>(oPacket));
                    break;
                case DATA_REQUEST_ACK:
                    handleRequestAck(dynamic_cast<OrionDataAckPacket *>(oPacket));
                    break;
                case DATA_REPLY:
                    handleReply(dynamic_cast<OrionDataRepPacket *>(oPacket));
                    break;
                default:
                    throw cRuntimeError("AODV Control Packet arrived with undefined packet type: %d", oPacket->getPacketType());
            }
       processPacket(PK(msg));
    }
    else if (msg->getKind() == UDP_I_ERROR)
    {
        EV << "Ignoring UDP error report\n";
        delete msg;
    }
    else
    {
        error("Unrecognized message (%s)%s", msg->getClassName(), msg->getName());
    }

    if (ev.isGUI())
    {
        char buf[40];
        sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
        getDisplayString().setTagArg("t", 0, buf);
    }
}




//handle application level packets
void OrionApp::processPacket(cPacket *pk)
{
    emit(rcvdPkSignal, pk);
  //  std::cout<< "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
    numReceived++;
}


//----------------------node operations------------------------------------

bool OrionApp::handleNodeStart(IDoneCallback *doneCallback)
{
    simtime_t start = std::max(startTime, simTime());
    if ((stopTime < SIMTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime))
    {
        selfMsg->setKind(START);
        scheduleAt(start, selfMsg);
    }
    return true;
}

bool OrionApp::handleNodeShutdown(IDoneCallback *doneCallback)
{
    if (selfMsg)
        cancelEvent(selfMsg);
    //TODO if(socket.isOpened()) socket.close();
    fileList.clear();
    return true;
}

void OrionApp::handleNodeCrash()
{
    if (selfMsg)
        cancelEvent(selfMsg);
}



//----------------------------Functions added by Capt Willinger--------------------

/*
 * Function: hasFile
 * Description: checks "file table" for passed file name
 *  - Param: reqfile - string value
 * Returns: bool
 */
bool OrionApp::hasFile(std::string reqFile){
 //   std::string debugMsg("Searching for file: ");
  //  debugMsg.append(reqFile);
 //   debug(debugMsg);
  if(std::count(fileList.begin(),fileList.end(),reqFile)>0)
    return true;
  return false;

}

/*
 * Function: generateFile()
 * Description: constructs a string value to represent new files and adds it to
 *              the apps file table.
 *  - Param: void
 * Returns: void
 */
void OrionApp::generateFile(){

    std::string tempTime;
    std::ostringstream convertFileNum;
    convertFileNum << fileNum++;
    tempTime = convertFileNum.str();
    std::string fileName(this->getOwner()->getFullName());
    fileName.append ("-");
    fileName.append (tempTime);
    fileList.push_back (fileName);
    simtime_t d = simTime() + par("fileGenRate").doubleValue();
    if (stopTime < SIMTIME_ZERO || d < (stopTime) )
    {
        scheduleAt(d, fileGenMsg);
    }
    else
    {
        selfMsg->setKind(STOP);
        scheduleAt(stopTime, selfMsg);
    }

}

/*
 * Function: requestFile()
 * Description: Initiates request for file transfer.
 *              Implements Orion file transfer algorithm by performing a round robin
 *              request of each file block (each block size is less than 1 MTU - I use
 *              1024 as a convenience factor). The function loops through all the blocks
 *              are successfully transferred or function exceeds file-transfer timeout.
 *  - Param: void
 * Returns: void
 */
void OrionApp::queryFile(){
    std::string fileToRequest(selectFile());
    if(debugEnabled){
    std::ostringstream debugMessage;
        debugMessage << "Requesting file: " << fileToRequest << std::endl;
    debug(debugMessage.str());
    }
    debug("Initiating Query");
    std::pair<int, IPvXAddress> tempPair(querySeqNum,myAddr);
    queryList.insert(tempPair);

    //create new entry and add it to our table
    FileTableData entry(fileToRequest, fileSize);
    entry.setQueryStart(simTime().dbl());
    queryResults.insert(std::pair<std::string, FileTableData>(fileToRequest, entry));

    sendQuery(fileToRequest, querySeqNum, myAddr);
    querySeqNum++;
    simtime_t d = simTime() + fileQueryRate;
    if (stopTime < SIMTIME_ZERO || d < (stopTime) ) //
    {
        scheduleAt(d, fileRequestMsg);

    }
    else
    {
        debug("Req File Stopping");
          selfMsg->setKind(STOP);
           scheduleAt(stopTime, selfMsg);
    }
}

/*
 * Function: requestFile()
 * Description: Initiates request for file transfer.
 *              Implements Orion file query algorithm by performing a broadcast send of
 *              file query packet.
 *  - Param: _fileToRequest: string value of requested file name
 * Returns: void
 */
//TODO add packet Param and logic to create new packet if null
void OrionApp::sendQuery(std::string _fileToRequest, unsigned int seq, IPvXAddress src){
    //convert to char array to keep packets happy
    char fileName[64];
    strncpy(fileName, _fileToRequest.c_str(), sizeof(fileName));
    fileName[sizeof(fileName)-1]=0;

    OrionQueryPacket *testPacket = new OrionQueryPacket("QUERY");

   //test of setting packet type...
     testPacket->setPacketType(QUERY);
     testPacket->setSEQ(seq);
     testPacket->setSRC (src);
     testPacket->setLastNode(myAddr);
     testPacket->setFilename(fileName);
     testPacket->setByteLength(par("messageLength").longValue());

    emit(sentPkSignal, testPacket);

    //test of sending broadcast;
    IPvXAddress destAddr(IPv4Address::ALLONES_ADDRESS);
    sendBroadcast(destAddr, testPacket);
    numSent++;
}


//Handles query packet.  Since only the master node is sending query requests, we don't have to worry about tracking
//source nodes, just the sequence number.  If the node has already seen the sequence number, it drops the packet.
//If this node hasn't seen this query, and it has the file, it replies with a query response, otherwise
//broadcasts the request to it's neighbors.
//
void OrionApp::handleQuery(OrionQueryPacket *qPacket){
    bubble("Handling Query");
    //check queryList to see if we've seen this request yet
    if(queryList.count(qPacket->getSEQ())==0){

        //create new query list entry and add it

        IPvXAddress tempAddress = IPvXAddress(qPacket->getLastNode());
        std::pair<int, IPvXAddress> tempPair(qPacket->getSEQ(),tempAddress);
        std::ostringstream convert;
        convert << "Adding Seq: " << qPacket->getSEQ() << "  -  Node: " << qPacket->getLastNode().str();
        debug(convert.str());

        queryList.insert(tempPair);
        //if we have the file, reply to the source with a RESPONSE PACKET
         if(hasFile(qPacket->getFilename())){
         debug("I have the file");
         sendResponse(qPacket);
         }

         //forward the request on...
         sendQuery(qPacket->getFilename(), qPacket->getSEQ(), qPacket->getSRC());
     }
    else{
     //   delete qPacket;
        //do something to drop packet
    }
}


void OrionApp::sendResponse(OrionPacket *oPacket){
    bubble("Sending Response");
    //create Response packet
    OrionResponsePacket *rPacket = new OrionResponsePacket("RESPONSE");
    rPacket->setFilename(dynamic_cast<OrionQueryPacket *>(oPacket)->getFilename());
    rPacket->setSRC(oPacket->getSRC());
    rPacket->setHopcount(0);
    rPacket->setLastNode(myAddr);
    rPacket->setSEQ(oPacket->getSEQ());
    IPvXAddress dest = queryList[oPacket->getSEQ()];
    std::string output("Sending Response to -> ");
    output.append(dest.str());
    debug(output);
    //send packet
    emit(sentPkSignal, rPacket);
    socket.sendTo(rPacket, dest, destPort);
}

//check to see if I'm the final destination, if so, initiate file request phase.
//if not, update hopcount and forward request on....
void OrionApp::handleResponse(OrionResponsePacket *rPacket){

    std::string responseString("handleResponse: ");
    std::string tempFile(rPacket->getFilename());

    if(myAddr == rPacket->getSRC()){
        debug("Home");
        // haven't replied to the source yet, so add this to our file list

        if(queryResults[tempFile].getQueryStop()<0){
            queryResults[tempFile].setQueryStop(simTime().dbl());
//  debug("Adding response to file table");
// std::cout << "             Query Time:  " << queryResults[tempFile].getQueryTime() << std::endl;
        queryResults[tempFile].addSource(rPacket->getLastNode());
        //since we are the source node, we can start the transfer process...
        requestList.insert(std::pair<std::string, IPvXAddress>(tempFile, myAddr));
        sendRequest(tempFile, reqSeqNum, queryResults[tempFile].getSource());
        reqSeqNum++;
        }else{
            //we already have an entry for this, so we just need to add additional sources
        queryResults[tempFile].addSource(rPacket->getSRC());
        }


    }else{
        // haven't replied to the source yet, so add this to our file list and forward it on
        if(queryResults.count(tempFile)==0){
        FileTableData entry(tempFile, fileSize);
        entry.addSource(rPacket->getLastNode());
        queryResults.insert(std::pair<std::string, FileTableData>(tempFile, entry));
debug("Adding response to file table");
        IPvXAddress dest = queryList[rPacket->getSEQ()];
        rPacket->setLastNode(myAddr);
        rPacket->setHopcount(rPacket->getHopcount()+1);
        std::string output("Sending Response to -> ");
        output.append(dest.str());
debug(output);
        socket.sendTo(rPacket->dup(), dest, destPort);
        }else{
            //we've replied to this file already, so we don't need to send any other updates
            //but we'll still update our list for local repair
            queryResults[tempFile].addSource(rPacket->getLastNode());
            debug("updating queryResults with new src");
        }
    }

}
void OrionApp::transferFile(std::string file){

    if(queryResults[file].getBlocks() == queryResults[file].getRemainBlocks()){
        queryResults[file].setTransferStart(simTime().dbl());
    }
   if(queryResults[file].hasSource()){
       sendRequest(file,reqSeqNum,queryResults[file].getSource());
   }

}

void OrionApp::sendRequest(std::string fileToRequest, unsigned int rSeq, IPvXAddress dest){

    //convert to char array to keep packets happy
    char fileName[64];
    strncpy(fileName, fileToRequest.c_str(), sizeof(fileName));
    fileName[sizeof(fileName)-1]=0;

    OrionDataReqPacket *reqPacket = new OrionDataReqPacket("DATA_REQUEST");

   //test of setting packet type...
    reqPacket->setPacketType(DATA_REQUEST);
    reqPacket->setDST(dest);
    reqPacket->setSEQ(rSeq);
    reqPacket->setSRC (myAddr);
    reqPacket->setLastNode(myAddr);
    reqPacket->setFilename(fileName);
    reqPacket->setRetries(retries);
    reqPacket->setByteLength(par("messageLength").longValue());


    //create timeoutEvent
    WaitForReq *reqTimeout =  new WaitForReq();
    reqTimeout->setFilename(fileName);

    //create timeoutEvent
    ReqBlockTimer *blockTimer =  new ReqBlockTimer();
    blockTimer->setFilename(fileName);

    //store eventTimer and packet for later lookup
    std::pair<std::string, OrionDataReqPacket*> tempPacketPair(fileName, reqPacket);
    std::pair<std::string, WaitForReq*> tempPacketPair2(fileName, reqTimeout);
    std::pair<std::string, ReqBlockTimer*> blockTimerPair(fileName, blockTimer);

    //store timers and packet for later lookup
    pendingPackets.insert(tempPacketPair);
    pendingTimeouts.insert(tempPacketPair2);
    blockTimers.insert(blockTimerPair);

    emit(sentPkSignal, reqPacket);
    socket.sendTo(reqPacket->dup(), dest, destPort);
    numSent++;

    //schedule timeout event;
    scheduleAt(simTime()+retryDelay, reqTimeout);
    scheduleAt(simTime()+queryResults[fileName].getQueryTime(), blockTimer);


}


void OrionApp::handleRequest(OrionDataReqPacket *reqPacket){
    sendRequestAck(reqPacket);

 //send requestAckknowledgement
  if(hasFile(reqPacket->getFilename())){
      debug("I have the file, need to send messages with payload back");
      OrionDataRepPacket *payload = new OrionDataRepPacket("DATA_REPLY");
      std::cout << "SRC:  " << reqPacket->getSRC() << std::endl;
      payload->setDST(reqPacket->getLastNode());
      payload->setSEQ(reqPacket->getSEQ());
      payload->setSRC (reqPacket->getSRC());
      payload->setBlockNum(reqPacket->getBlockNum());
      payload->setLastNode(myAddr);
      payload->setFilename(reqPacket->getFilename());
      payload->setByteLength(1024);
      socket.sendTo(payload, payload->getSRC(), destPort);

  }else{//forward on

      //create timeoutEvent
      WaitForReq *reqTimeout =  new WaitForReq();
      reqTimeout->setFilename(reqPacket->getFilename());

      //store eventTimer and packet for later lookup
      std::pair<std::string, OrionDataReqPacket*> tempPacketPair(reqPacket->getFilename(), reqPacket->dup());
      std::pair<std::string, WaitForReq*> tempPacketPair2(reqPacket->getFilename(), reqTimeout);

      pendingPackets.insert(tempPacketPair);
      pendingTimeouts.insert(tempPacketPair2);

      IPvXAddress dest = queryResults[reqPacket->getFilename()].getSource();
      reqPacket->setLastNode(myAddr);
      socket.sendTo(reqPacket->dup(), dest, destPort);
      //schedule timeout event;
      scheduleAt(simTime()+retryDelay, reqTimeout);

  }

}

void OrionApp::handleRequestAck(OrionDataAckPacket *ackPacket){
debug("handling request Ack");

std::string temp = ackPacket->getFilename();

    //delete our queued packets and cancel our timeout event
    delete pendingPackets[temp];
    cancelAndDelete(pendingTimeouts[temp]);

    //delete table entries
    pendingPackets.erase(temp);
    pendingTimeouts.erase(temp);
    //std::cout << "Size of pendingPackets = " << pendingPackets.size() << std::endl;
   // std::cout << "Size of pendingTimeouts = " << pendingTimeouts.size() << std::endl;
}

void OrionApp::handleReply(OrionDataRepPacket *repPacket){
    if(myAddr == repPacket->getSRC()){
        int blocks(queryResults[repPacket->getFilename()].getRemainBlocks());
        if(blocks > 1){
            blocks--;
            queryResults[repPacket->getFilename()].setRemainBlocks(blocks);
        }
        if(blocks ==1){
            queryResults[repPacket->getFilename()].setTransferStop(simTime().dbl());
            queryResults[repPacket->getFilename()].setTransferComplete(true);
            debug("*********************Winning!*****************************");
        }
    }
}

void OrionApp::requestNextBlock(std::string file){


}



void OrionApp::resendRequest(OrionDataReqPacket* reqPacket){

    bubble("Re-sending Request");
    if(reqPacket->getRetries() >0){
    //duplicate message and save original.
    emit(sentPkSignal, reqPacket);
    reqPacket->setRetries(reqPacket->getRetries()-1);
    scheduleAt(simTime()+retryDelay,pendingTimeouts[reqPacket->getFilename()]);
    socket.sendTo(reqPacket->dup(), reqPacket->getDST(), destPort);
    numSent++;
    }else{
        queryResults[reqPacket->getFilename()].removeSource();

        //if we have another source for the file, perform local correction
        if(queryResults[reqPacket->getFilename()].hasSource()){
            reqPacket->setRetries(retries);
            reqPacket->setDST(queryResults[reqPacket->getFilename()].getSource());

            //duplicate message and save original.
            socket.sendTo(reqPacket->dup(), reqPacket->getDST(), destPort);
            numSent++;
            //schedule timout event;

            //resechedule timeoutEvent
            scheduleAt(simTime()+retryDelay,pendingTimeouts[reqPacket->getFilename()]);

        }else{
            pendingPackets.erase(reqPacket->getFilename());
            pendingTimeouts.erase(reqPacket->getFilename());
            //send error back...

        }

    }
}



void OrionApp::sendRequestAck(OrionDataReqPacket *reqPacket)
{
    debug("Sending ReqAck!");
    OrionDataAckPacket *reqAck = new OrionDataAckPacket("DATA_REQUEST_ACK");
  //  reqAck->setPacketType(DATA_REQUEST_ACK);
    reqAck->setFilename(reqPacket->getFilename());
    reqAck->setDST(reqPacket->getDST());
    reqAck->setSEQ(reqPacket->getSEQ());
    reqAck->setSRC (reqPacket->getSRC());
    reqAck->setLastNode(myAddr);
    reqAck->setRetries(reqPacket->getRetries());
    reqAck->setByteLength(par("messageLength").longValue());


    emit(sentPkSignal, reqAck);
    socket.sendTo(reqAck, reqPacket->getLastNode(), destPort);
}


/*
 * Function: selectFile()
 * Description: generates file name string to be requested.
 *  - Param: _fileToRequest: string value of requested file name
 * Returns: string value of file name
 */
std::string OrionApp::selectFile(){
    std::ostringstream convert;
    std::ostringstream convert2;
    std::string fileToRequest("host[");
    int randomNode = 1 + intrand(numberNodes-1);
    int randomFile = (int) (simTime().dbl()/fileGenRate.dbl());
    convert << randomNode;
    convert2 << randomFile;
    fileToRequest.append(convert.str());
    fileToRequest.append("]-");
    fileToRequest.append(convert2.str());
  return fileToRequest;
}

/*
 * Function: debug()
 * Description: prints out string messages to the console
 *  - Param: msg: string value of msg to output
 * Returns: void
 */
void OrionApp::debug(std::string msg){
    if(debugEnabled){
    std::string output(simTime().str());
    output.append(" - ");
    output.append(this->getOwner()->getFullName());
    output.append(msg);
    std::cout << output << std::endl;
    }
}



