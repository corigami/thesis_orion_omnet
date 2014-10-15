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
    replicateSeqNum=0;
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
        xferFails = 0;
        xferCompletes= 0;

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
        debugEnabled[0] = par("enableDebugLevel0");
        debugEnabled[1] = par("enableDebugLevel1");
        startTime = par("startTime").doubleValue();
        stopTime = par("stopTime").doubleValue();
        fileSize = par("fileSize");
        numberNodes = par("numHosts");
        repCount = par("replicationCount");
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
    for (std::map<std::string, OrionDataReqPacket*>::iterator it = pendingPackets.begin(); it != pendingPackets.end(); it++) {
        cancelAndDelete(it->second);
    }
    pendingPackets.clear();

    for (std::map<std::string, WaitForReq*>::iterator it = pendingTimeouts.begin(); it != pendingTimeouts.end(); it++) {
        cancelAndDelete (it->second);
    }
    pendingTimeouts.clear();

    for (std::map<std::string, ReqBlockTimer*>::iterator it = blockTimers.begin(); it != blockTimers.end(); it++) {
        cancelAndDelete (it->second);
    }
    blockTimers.clear();


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

//void OrionApp::sendPacket()
//{
//    char msgName[32];
//    sprintf(msgName, "Packet-%d", numSent);
//    cPacket *payload = new cPacket(msgName);
//    payload->setByteLength(par("messageLength").longValue());
//    IPvXAddress destAddr = chooseDestAddr();
//    // IPvXAddress destAddr = "host[15]";
//    emit(sentPkSignal, payload);
//    socket.sendTo(payload, destAddr, destPort);
//    numSent++;
//}


/*
 * Function: sendBroadcast()
 * Description: prints out string messages to the console
 *  - Param:    *pkt:
 *              &dest:
 * Returns: void
 */
bool OrionApp::sendBroadcast(const IPvXAddress &dest, cPacket *pkt)
{
    debug("sendBroadcast", 0);
    if (!outputInterfaceMulticastBroadcast.empty() && (dest.isMulticast() || (!dest.isIPv6() && dest.get4() == IPv4Address::ALLONES_ADDRESS)))
    {

        for (unsigned int i = 0; i < outputInterfaceMulticastBroadcast.size(); i++)
        {
            UDPSocket::SendOptions options;
            options.outInterfaceId = outputInterfaceMulticastBroadcast[i];

            if (outputInterfaceMulticastBroadcast.size() - i > 1){
                emit(sentPkSignal, pkt);
                socket.sendTo(pkt->dup(), dest, destPort, &options);

            }
            else{
                emit(sentPkSignal, pkt);
                socket.sendTo(pkt, dest, destPort, &options);

            }
            numSent++;
        }
        return true;
    }
    return false;
}

void OrionApp::processStart()
{
    debug("processStart", 0);
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
            scheduleAt(d + uniform(0,10)/1000, fileGenMsg);
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
    debug("processSend", 0);
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
    debug("processStop", 0);
    if(socketOpen){
        socket.close();
        socketOpen = false;
    }



}

void OrionApp::handleMessageWhenUp(cMessage *msg)
{
    debug("handleMessageWhenUp", 0);
    if (msg->isSelfMessage())
    {
        if(msg==fileGenMsg)  {
            //   debug("handling fileGen");
            generateFile();
        }else if(msg == fileRequestMsg){
            //    debug("handling requestFile");
            queryFile();
        }else if(dynamic_cast<ReqBlockTimer *>(msg)){
            std::string file = dynamic_cast<ReqBlockTimer *>(msg)->getFilename();
            sendRequest(file);
        }else if(dynamic_cast<WaitForReq *>(msg)){
            std::string file = dynamic_cast<WaitForReq *>(msg)->getBid();
      //      std::cout << file << " - "<< pendingTimeouts.count(file) << std::endl;
            if(pendingPackets.count(file)>0){
                resendRequest(pendingPackets[file]);
            }

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
        case DATA_ERR:
            handleRequestError(dynamic_cast<OrionErrorPacket *>(oPacket));
            break;
        case REP_REQUEST:
            handleReplicateReq(dynamic_cast<ReplicatePacket *>(oPacket));
            break;
        case REP_CONFIRM:
            handleReplicateConfirm(dynamic_cast<ReplicateConfirmPacket *>(oPacket));
            break;
        case REP_CONFIRM_ACK:
            handleReplicateConfirmAck(dynamic_cast<ReplicateConfirmAckPacket *>(oPacket));
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
    debug("processPacket", 0);
    emit(rcvdPkSignal, pk);
    //  std::cout<< "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
    numReceived++;
}


//----------------------node operations------------------------------------

bool OrionApp::handleNodeStart(IDoneCallback *doneCallback)
{
    debug("handleNodeStart", 0);
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
    debug("handleNodeShutdown", 0);
    if (selfMsg)
        cancelEvent(selfMsg);
    //TODO if(socket.isOpened()) socket.close();
    fileList.clear();
    return true;
}


void OrionApp::handleNodeCrash()
{
    debug("handleNodeCrash", 0);
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
    debug("hasFile", 0);
    //   std::string debugMsg("Searching for file: ");
    //  debugMsg.append(reqFile);
    //   debug(debugMsg);
    if(fileList.count(reqFile)>0)
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
    debug("generateFile", 0);
    std::string tempTime;
    std::ostringstream convertFileNum;
    convertFileNum << fileNum++;
    tempTime = convertFileNum.str();
    std::string fileName(this->getOwner()->getFullName());
    fileName.append ("-");
    fileName.append (tempTime);

    fileList.insert(std::pair<std::string, int>(fileName, repCount));

    std::ostringstream requestID;
    requestID << fileName << "-" << myAddr.str();

    replicateList.insert(std::pair<std::string, unsigned int >(requestID.str(), replicateSeqNum));
    sendReplicateReq(fileName, replicateSeqNum, myAddr);
    replicateSeqNum++;

    double ran = uniform(0,1)/5;
  //  std::cout << ran << std::endl;
    simtime_t d = simTime() + par("fileGenRate").doubleValue() +ran;
    if (stopTime < SIMTIME_ZERO || d < (stopTime) )
    {
        scheduleAt(d, fileGenMsg);
    }
    else
    {
  //      selfMsg->setKind(STOP);
   //     scheduleAt(stopTime, selfMsg);
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
    debug("queryFile", 0);
    std::string fileToRequest(selectFile());
    if(debugEnabled){
        std::ostringstream debugMessage;
        debugMessage << "Requesting file: " << fileToRequest << std::endl;
        debug(debugMessage.str());
    }
    debug("Initiating Query");
    std::ostringstream queryID;
    queryID << myAddr.str() << "-" << querySeqNum;

    //store query lookup based on node and seq number
    std::pair<std::string, IPvXAddress> tempPair(queryID.str(),myAddr);
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
  //      selfMsg->setKind(STOP);
   //     scheduleAt(stopTime, selfMsg);
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
    debug("sendQuery", 0);
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

    //test of sending broadcast;
    IPvXAddress destAddr(IPv4Address::ALLONES_ADDRESS);
    sendBroadcast(destAddr, testPacket);
}


//Handles query packet.  Since only the master node is sending query requests, we don't have to worry about tracking
//source nodes, just the sequence number.  If the node has already seen the sequence number, it drops the packet.
//If this node hasn't seen this query, and it has the file, it replies with a query response, otherwise
//broadcasts the request to it's neighbors.
//
void OrionApp::handleQuery(OrionQueryPacket *qPacket){
    debug("handleQuery", 0);
    //check queryList to see if we've seen this request yet

    std::ostringstream queryID;
    queryID << qPacket->getSRC().str() << "-" << qPacket->getSEQ();


    if(queryList.count(queryID.str())==0){
        //create new query list entry and add i

        IPvXAddress tempAddress = IPvXAddress(qPacket->getLastNode());
        std::pair<std::string, IPvXAddress> tempPair(queryID.str(),tempAddress);
        //   std::ostringstream convert;
        //   convert << "Adding Seq: " << qPacket->getSEQ() << "  -  Node: " << qPacket->getLastNode().str();
        //    debug(convert.str());

        queryList.insert(tempPair);
        //if we have the file, reply to the source with a RESPONSE PACKET
        if(hasFile(qPacket->getFilename())){
//debug("I have the file");
            sendResponse(qPacket);
        }else{

            //forward the request on...
            sendQuery(qPacket->getFilename(), qPacket->getSEQ(), qPacket->getSRC());
        }

    }
    else{
        //   delete qPacket;
        //do something to drop packet
    }

}


void OrionApp::sendResponse(OrionPacket *oPacket){
    debug("sendResponse", 0);
    //create Response packet
    OrionResponsePacket *rPacket = new OrionResponsePacket("RESPONSE");
    rPacket->setFilename(dynamic_cast<OrionQueryPacket *>(oPacket)->getFilename());
    rPacket->setSRC(oPacket->getSRC());
    rPacket->setHopcount(0);
    rPacket->setLastNode(myAddr);
    rPacket->setSEQ(oPacket->getSEQ());

    std::ostringstream queryID;
    queryID << oPacket->getSRC().str() << "-" << oPacket->getSEQ();

    IPvXAddress dest = queryList[queryID.str()];
    std::string output("Sending Response to -> ");
    output.append(dest.str());
//debug(output);
    //send packet
    emit(sentPkSignal, rPacket);
    socket.sendTo(rPacket, dest, destPort);
}

//check to see if I'm the final destination, if so, initiate file request phase.
//if not, update hopcount and forward request on....
void OrionApp::handleResponse(OrionResponsePacket *rPacket){
    debug("handleResponse", 0);
    std::string responseString("handleResponse: ");
    std::string tempFile(rPacket->getFilename());

    if(myAddr == rPacket->getSRC()){
// debug("Home");
        // haven't replied to the source yet, so add this to our file list

        if(queryResults[tempFile].getQueryStop()<0){
            queryResults[tempFile].setQueryStop(simTime().dbl());
            retryDelay = queryResults[tempFile].getQueryTime()+.1;
            //  debug("Adding response to file table");
            // std::cout << "             Query Time:  " << queryResults[tempFile].getQueryTime() << std::endl;
            queryResults[tempFile].addSource(rPacket->getLastNode());
            //since we are the source node, we can start the transfer process...
            requestList.insert(std::pair<std::string, IPvXAddress>(tempFile, myAddr));

            //start transfer timer
            queryResults[tempFile].setTransferStart(simTime().dbl());
            sendRequest(tempFile);

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

            std::ostringstream queryID;
            queryID << rPacket->getSRC().str() << "-" << rPacket->getSEQ();

            IPvXAddress dest = queryList[queryID.str()];
            rPacket->setLastNode(myAddr);
            rPacket->setHopcount(rPacket->getHopcount()+1);
            std::string output("Sending Response to -> ");
            output.append(dest.str());
//debug(output);
            socket.sendTo(rPacket->dup(), dest, destPort);
        }else{

            //we've replied to this file already, so we don't need to send any other updates
            //but we'll still update our list for local repair
            queryResults[tempFile].addSource(rPacket->getLastNode());
            debug("updating queryResults with new src");
        }
    }

}

void OrionApp::sendRequest(std::string fileToRequest){
    debug("sendRequest", 0);

    if(blockTimers.count(fileToRequest) != 0){
        //delete our queued packets and cancel our timeout event
        cancelAndDelete(blockTimers[fileToRequest]);
        //delete table entries
        blockTimers.erase(fileToRequest);
    }

    int block(queryResults[fileToRequest].getNextBlock());
    if(block >= 0){


        char fileName[64];
        strncpy(fileName, fileToRequest.c_str(), sizeof(fileName));
        fileName[sizeof(fileName)-1]=0;

        std::string id(fileName);
        id.append("-b");
        std::ostringstream blockString;
        blockString << block << "-s" << reqSeqNum;
        id.append(blockString.str());

        char ID[64];
        strncpy(ID, id.c_str(), sizeof(ID));
        ID[sizeof(ID)-1]=0;

        if(queryResults[fileToRequest].hasSource()){
            //         debug("Here - 3");
            OrionDataReqPacket *reqPacket = new OrionDataReqPacket("DATA_REQUEST");
            //test of setting packet type...
            reqPacket->setPacketType(DATA_REQUEST);
            reqPacket->setDST(queryResults[fileToRequest].getSource());
            reqPacket->setSEQ(reqSeqNum++);
            reqPacket->setSRC (myAddr);
            reqPacket->setLastNode(myAddr);
            reqPacket->setFilename(fileName);  //const char instead of std::string
            reqPacket->setRetries(retries);
            reqPacket->setBid(ID);
            reqPacket->setBlock(block);
            reqPacket->setByteLength(par("messageLength").longValue());

            //create  request timeoutEvent
            WaitForReq *reqTimeout =  new WaitForReq();
            reqTimeout->setFilename(fileName);
            reqTimeout->setBid(ID);
            reqTimeout->setBlock(block);

            //create block request loop timeoutEvent
            ReqBlockTimer *blockTimer =  new ReqBlockTimer();
            blockTimer->setFilename(fileName);

            //store eventTimer and packet for later lookup
            std::pair<std::string, OrionDataReqPacket*> tempPacketPair(id, reqPacket);
            std::pair<std::string, WaitForReq*> tempPacketPair2(id, reqTimeout);
            std::pair<std::string, ReqBlockTimer*> blockTimerPair(fileToRequest, blockTimer);

            //store timers and packet for later lookup
            pendingPackets.insert(tempPacketPair);
            pendingTimeouts.insert(tempPacketPair2);
            blockTimers.insert(blockTimerPair);

            emit(sentPkSignal, reqPacket);
            socket.sendTo(reqPacket->dup(), reqPacket->getDST(), destPort);
            numSent++;

            //schedule timeout event;
            scheduleAt(simTime()+retryDelay, reqTimeout);
            scheduleAt(simTime()+queryResults[fileName].getQueryTime()+.02, blockTimer);

        }else{
            debug("Error....out of sources");
            xferFails++;
        }
    }else{
//debug("Requesting blocks, but all have been received");
    }
}


void OrionApp::handleRequest(OrionDataReqPacket *reqPacket){
    debug("handleRequest", 0);
    sendRequestAck(reqPacket);

    if(hasFile(reqPacket->getFilename())){
// debug("I have the file, need to send messages with payload back");
        OrionDataRepPacket *payload = new OrionDataRepPacket("DATA_REPLY");
        std::ostringstream output;

        payload->setDST(reqPacket->getLastNode());
        payload->setSEQ(reqPacket->getSEQ());
        IPvXAddress dest = reqPacket->getLastNode();
        payload->setSRC (reqPacket->getSRC());
        payload->setBlock(reqPacket->getBlock());
        payload->setLastNode(myAddr);
        payload->setFilename(reqPacket->getFilename());
        payload->setByteLength(1048);
        payload->setBid(reqPacket->getBid());
        payload->setNumCopiesRemaining(fileList[reqPacket->getFilename()]);
        output << "Sending blocks to :  " << dest.str();
//debug(output.str());
        socket.sendTo(payload, dest, destPort);

    }else{//forward on
        if(requestList.count(reqPacket->getBid())==0){
            std::ostringstream temp;
            temp << "Adding: "  << reqPacket->getLastNode().str() << " as source for " << reqPacket->getBid();
            debug(temp.str());
            requestList.insert(std::pair<std::string, IPvXAddress>(reqPacket->getBid(), reqPacket->getLastNode()));

            std::string temp1 =reqPacket->getBid();
            std::string temp2 = "host[9]-7-b6-s2343";
            if(temp1.compare(temp2) ==true){
                debug("***************added to req list******************************************");
            }
            //create timeoutEvent
        }else{
      //      debug("Updating source");
      //      requestList[reqPacket->getBid()] = reqPacket->getLastNode();
        }
        WaitForReq *reqTimeout =  new WaitForReq();
        reqTimeout->setFilename(reqPacket->getFilename());
        reqTimeout->setBid(reqPacket->getBid());

        //store eventTimer and packet for later lookup
        std::pair<std::string, OrionDataReqPacket*> tempPacketPair(reqPacket->getBid(), reqPacket->dup());
        std::pair<std::string, WaitForReq*> tempPacketPair2(reqPacket->getBid(), reqTimeout);

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
    debug("handleRequestAck", 0);

    //delete our queued packets and cancel our timeout event
    delete pendingPackets[ackPacket->getBid()];
    cancelAndDelete(pendingTimeouts[ackPacket->getBid()]);

    //delete table entries
    pendingPackets.erase(ackPacket->getBid());
    pendingTimeouts.erase(ackPacket->getBid());
    //std::cout << "Size of pendingPackets = " << pendingPackets.size() << std::endl;
    // std::cout << "Size of pendingTimeouts = " << pendingTimeouts.size() << std::endl;
}

void OrionApp::handleRequestError(OrionErrorPacket *errPacket){
    debug("Got the error memo");
    queryResults[errPacket->getFilename()].removeSource();
}


void OrionApp::handleReply(OrionDataRepPacket *repPacket){
    debug("handleReply", 0);

    //If we are the source Node
    if(myAddr == repPacket->getSRC()){
       // debug("got a block packet");
        if(!queryResults[repPacket->getFilename()].getBlockStatus(repPacket->getBlock())){
            queryResults[repPacket->getFilename()].setBlockComplete(repPacket->getBlock());
            int blocks = queryResults[repPacket->getFilename()].getRemainBlocks();
            queryResults[repPacket->getFilename()].setRemainBlocks(--blocks);
            if(queryResults[repPacket->getFilename()].getRemainBlocks()==0){
                queryResults[repPacket->getFilename()].setTransferStop(simTime().dbl());
                queryResults[repPacket->getFilename()].setTransferComplete(true);

                fileList.insert(std::pair<std::string, int>(repPacket->getFilename(), 0));

                std::ostringstream output;
                output << "Xfer Comp - " <<  repPacket->getFilename() << " From: " << repPacket->getSRC();
                output << "  file count: " << fileList.size();
debug(output.str());

//debug("*********************Winning!*****************************");
                xferCompletes++;
            }
            if(!master){

                if(repPacket->getNumCopiesRemaining() > 0){
                ReplicatePacket *replicatePacket = new ReplicatePacket("REP_REQUEST");


                std::ostringstream requestID;
                requestID << repPacket->getFilename() << "-" << repPacket->getSRC().str();

                //test of setting packet type...
                replicatePacket->setPacketType(REP_REQUEST);
                replicatePacket->setSEQ(replicateList[requestID.str()]);
                replicatePacket->setSRC (repPacket->getSRC());
                replicatePacket->setLastNode(myAddr);
                replicatePacket->setFilename(repPacket->getFilename() );
                replicatePacket->setByteLength(par("messageLength").longValue());

                //Broadcast to my neighbors
                IPvXAddress destAddr(IPv4Address::ALLONES_ADDRESS);
                sendBroadcast(destAddr, replicatePacket);
                }

            }

        }else{
            debug("Already seen this block, discard");
        }

        //If we are an intermediate node
    }else{
        std::ostringstream temp;
        temp <<" Id for packet: " << repPacket->getBid() ;
        debug(temp.str());
        IPvXAddress dest = requestList[repPacket->getBid()];
        repPacket->setLastNode(myAddr);
        std::string output("Sending Reply to -> ");
        output.append(dest.str());
//  debug(output);
        requestList.erase(repPacket->getBid());
        socket.sendTo(repPacket->dup(), dest, destPort);
    }
}

void OrionApp::resendRequest(OrionDataReqPacket* reqPacket){
    debug("resendRequest", 0);
//  debug("Here 0");
    if(reqPacket->getRetries() >0){
//debug("Here 1");
        //duplicate message and save original.
        emit(sentPkSignal, reqPacket);
        reqPacket->setRetries(reqPacket->getRetries()-1);
        if(pendingTimeouts.count(reqPacket->getBid())> 0){
            if(!pendingTimeouts[reqPacket->getBid()]->isScheduled()){
                scheduleAt(simTime()+retryDelay, pendingTimeouts[reqPacket->getBid()]);
                socket.sendTo(reqPacket->dup(), reqPacket->getDST(), destPort);
                numSent++;
            }

        }
    }else{
debug("Here 2");
        queryResults[reqPacket->getFilename()].removeSource();
debug("Here 2b");
        //if we have another source for the file, perform local correction

        if(queryResults[reqPacket->getFilename()].hasSource()){
debug("Here 3");
            queryResults[reqPacket->getFilename()].printSources();
debug("Here 3a");
            std::cout << "new source = " << queryResults[reqPacket->getFilename()].getSource().str()<<
                    " for file: " << reqPacket->getFilename() << std::endl;
debug("Here 3b");
            reqPacket->setRetries(retries);
            reqPacket->setDST(queryResults[reqPacket->getFilename()].getSource());

            //duplicate message and save original.
            socket.sendTo(reqPacket->dup(), reqPacket->getDST(), destPort);
            numSent++;
            //schedule timout event;
debug("Here 3c");
            //resechedule timeoutEvent
            scheduleAt(simTime()+retryDelay,pendingTimeouts[reqPacket->getBid()]);

        }else{
debug("Here 4");
            pendingPackets.erase(reqPacket->getBid());
            pendingTimeouts.erase(reqPacket->getBid());
            //send error back...

            OrionErrorPacket *errPacket = new OrionErrorPacket("DATA_ERR");
            errPacket->setFilename(reqPacket->getFilename());
            errPacket->setSRC(reqPacket->getSRC());
            errPacket->setLastNode(myAddr);
            errPacket->setSEQ(reqPacket->getSEQ());

            std::ostringstream queryID;
            queryID << reqPacket->getSRC().str() << "-" << reqPacket->getSEQ();
            debug(queryID.str());
            debug(reqPacket->getBid());

            std::ostringstream info;
            info << "reqPacketSRC: " << reqPacket->getSRC().str() << std::endl;
            info << "reqPacketLastNode(): " << reqPacket->getLastNode().str() << std::endl;
            info << "reqPacketSEQ: " << reqPacket->getSEQ() << std::endl;
            info << "Last node: " << reqPacket->getLastNode().str() << std::endl;
            info << "Count of requestList " <<requestList.count(reqPacket->getBid()) << std::endl;
            debug(info.str());

            IPvXAddress dest = reqPacket->getLastNode();
          //  IPvXAddress dest = requestList[queryID.str()];
            std::string output("Sending Response to -> ");
            output.append(dest.str());
        debug(output);
            //send packet
            emit(sentPkSignal, errPacket);
            socket.sendTo(errPacket, dest, destPort);
        }

    }
}



void OrionApp::sendRequestAck(OrionDataReqPacket *reqPacket)
{
    debug("sendRequestAck", 0);
    OrionDataAckPacket *reqAck = new OrionDataAckPacket("DATA_REQUEST_ACK");
    reqAck->setFilename(reqPacket->getFilename());
    reqAck->setDST(reqPacket->getDST());
    reqAck->setSEQ(reqPacket->getSEQ());
    reqAck->setSRC (reqPacket->getSRC());
    reqAck->setLastNode(myAddr);
    reqAck->setRetries(reqPacket->getRetries());
    reqAck->setBid(reqPacket->getBid());
    reqAck->setBlock(reqPacket->getBlock());
    reqAck->setByteLength(par("messageLength").longValue());


    emit(sentPkSignal, reqAck);
    socket.sendTo(reqAck, reqPacket->getLastNode(), destPort);
}


void OrionApp::sendReplicateReq(std::string fileToRep, unsigned int seq, IPvXAddress src) {
    debug("sendReplicateReq", 0);
    //convert to char array to keep packets happy
    char fileName[64];
    strncpy(fileName, fileToRep.c_str(), sizeof(fileName));
    fileName[sizeof(fileName)-1]=0;

    ReplicatePacket *replicatePacket = new ReplicatePacket("REP_REQUEST");

    //test of setting packet type...
    replicatePacket->setPacketType(REP_REQUEST);
    replicatePacket->setSEQ(seq);
    replicatePacket->setSRC (src);
    replicatePacket->setLastNode(myAddr);
    replicatePacket->setFilename(fileName);
    replicatePacket->setByteLength(par("messageLength").longValue());

    //Broadcast to my neighbors
    IPvXAddress destAddr(IPv4Address::ALLONES_ADDRESS);
    sendBroadcast(destAddr, replicatePacket);
}

void OrionApp::handleReplicateReq(ReplicatePacket* replicate) {
    debug("handleReplicateReq", 0);

    std::ostringstream requestID;
    requestID << replicate->getFilename() << "-" << replicate->getSRC();

    //check replicateList to see if we've seen this request yet
      if(replicateList.count(requestID.str())==0){
          replicateList.insert(std::pair<std::string, unsigned int>(requestID.str(),replicate->getSEQ()));

          ReplicateConfirmPacket *confirmPacket = new ReplicateConfirmPacket("REP_CONFIRM");
          confirmPacket->setPacketType(REP_CONFIRM);
          confirmPacket->setSEQ(replicate->getSEQ());
          confirmPacket->setSRC (myAddr);
          confirmPacket->setLastNode(myAddr);
          confirmPacket->setFilename(replicate->getFilename());
          confirmPacket->setByteLength(par("messageLength").longValue());

          emit(sentPkSignal, confirmPacket);
          socket.sendTo(confirmPacket, replicate->getSRC(), destPort);
          numSent++;
          }
}

void OrionApp::handleReplicateConfirm(ReplicateConfirmPacket* replicateRes) {
    int remain = fileList[replicateRes->getFilename()];
    if(  remain > 0){
        fileList[replicateRes->getFilename()] = remain-1;

    ReplicateConfirmAckPacket *confirmPacketACK = new ReplicateConfirmAckPacket("REP_CONFIRM_ACK");
    confirmPacketACK->setPacketType(REP_CONFIRM_ACK);
    confirmPacketACK->setSEQ(replicateRes->getSEQ());
    confirmPacketACK->setSRC (myAddr);
    confirmPacketACK->setNumCopiesRemaining(remain);
    confirmPacketACK->setLastNode(myAddr);
    confirmPacketACK->setFilename(replicateRes->getFilename());
    confirmPacketACK->setByteLength(par("messageLength").longValue());

    emit(sentPkSignal, confirmPacketACK);
    socket.sendTo(confirmPacketACK, replicateRes->getSRC(), destPort);
    numSent++;
    }
}

void OrionApp::handleReplicateConfirmAck(ReplicateConfirmAckPacket* replicateAck) {
    debug("handleReplicateAck", 0);
    if(replicateAck->getNumCopiesRemaining() > 0){

        std::ostringstream output;
        output << "Requesting File: " << replicateAck->getFilename() << " from: " << replicateAck->getSRC().str();
        output << " Remaining copies: " << replicateAck->getNumCopiesRemaining();
//debug(output.str());
        //source needs us to create a copy.

        //convert to char array to keep packets happy
        char fileName[64];
        strncpy(fileName, replicateAck->getFilename(), sizeof(fileName));
        fileName[sizeof(fileName)-1]=0;

        std::ostringstream queryID;
        queryID << myAddr.str() << "-" << querySeqNum;

        //store query lookup based on node and seq number
        std::pair<std::string, IPvXAddress> tempPair(queryID.str(),myAddr);

        queryList.insert(tempPair);

        //create new entry and add it to our table
        FileTableData entry(fileName, fileSize);
        entry.setQueryStart(simTime().dbl());
        queryResults.insert(std::pair<std::string, FileTableData>(fileName, entry));

        OrionQueryPacket *queryPacket = new OrionQueryPacket("QUERY");

        //test of setting packet type...
        queryPacket->setPacketType(QUERY);
        queryPacket->setSEQ(querySeqNum);
        querySeqNum++;
        queryPacket->setSRC (myAddr);
        queryPacket->setLastNode(myAddr);
        queryPacket->setFilename(fileName);
        queryPacket->setByteLength(par("messageLength").longValue());
        emit(sentPkSignal, queryPacket);

        socket.sendTo(queryPacket, replicateAck->getSRC(), destPort);
        numSent++;



//        ReplicatePacket *replicatePacket = new ReplicatePacket("REP_REQUEST");
//
//        //test of setting packet type...
//        replicatePacket->setPacketType(REP_REQUEST);
//        replicatePacket->setSEQ(replicateAck->getSEQ());
//        replicatePacket->setSRC (replicateAck->getSRC());
//        replicatePacket->setLastNode(myAddr);
//        replicatePacket->setFilename(fileName);
//        replicatePacket->setByteLength(par("messageLength").longValue());
//
//        //Broadcast to my neighbors
//        IPvXAddress destAddr(IPv4Address::ALLONES_ADDRESS);
//        sendBroadcast(destAddr, replicatePacket);



    }else{
        //do nothing, source doesn't need us to copy
    }
}



/*
 * Function: selectFile()
 * Description: generates file name string to be requested.
 *  - Param: _fileToRequest: string value of requested file name
 * Returns: string value of file name
 */
std::string OrionApp::selectFile(){
    debug("selectFile", 0);
    std::ostringstream convert;
    std::ostringstream convert2;
    std::string fileToRequest("host[");
    int randomNode = 1 + intrand(numberNodes-1);
    int randomFile = (int) (simTime().dbl()/fileGenRate.dbl());
    convert << randomNode << "]-" << randomFile;
    fileToRequest.append(convert.str());
    return fileToRequest;
}

/*
 * Function: debug()
 * Description: prints out string messages to the console
 *  - Param: msg: string value of msg to output
 * Returns: void
 */
void OrionApp::debug(std::string msg, int level){
    if(debugEnabled[0] && level == 0){
        std::string output(simTime().str());
        output.append(" - ");
        output.append(this->getOwner()->getFullName());
        output.append(": ");
        output.append(msg);
        std::cout << output << std::endl;
    }
    if(debugEnabled[1] && level == 1){
        std::string output(simTime().str());
        output.append(" - ");
        output.append(this->getOwner()->getFullName());
        output.append(": ");
        output.append(msg);
        std::cout << output << std::endl;
    }

}



