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


#include <fstream>
#include "OrionApp.h"
#include "FileTableData.h"
#include "InterfaceTableAccess.h"
#include "IPvXAddressResolver.h"
#include "IPv4ControlInfo.h"
#include "NodeOperations.h"
#include "UDPControlInfo.h"
#include "UDPControlInfo_m.h"


Define_Module(OrionApp);

//simsignal_t OrionApp::sentPkSignal = registerSignal("sentPk");
//simsignal_t OrionApp::rcvdPkSignal = registerSignal("rcvdPk");

OrionApp::OrionApp()
{
    selfMsg = NULL;
    fileGenMsg = NULL;
    fileRequestMsg = NULL;
    churnTimerMsg = NULL;
}

OrionApp::~OrionApp()
{
    if(printContainers)
        printContainerContents();
    //   delete &queryList;
    cancelAndDelete(selfMsg);
    cancelAndDelete(fileGenMsg);
    cancelAndDelete(fileRequestMsg);
    cancelAndDelete(churnTimerMsg);

    clearTimersAndLists();

}

void OrionApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == 0)
    {
        numSent = 0;
        numReceived = 0;
        xferFails = 0;
        xferReqs = 0;
        xferCompletes= 0;

        fileNum = 1;
        querySeqNum = 0;
        replicateSeqNum=0;
        reqSeqNum=0;

        //load parameters from .ned file
        localPort = par("localPort");
        master = par("master");
        destPort = par("destPort");
        retries = par("retries");
        fileQueryRate = par("fileQueryRate").doubleValue();
        fileGenRate = par("fileGenRate");
        debugEnabled[0] = par("enableDebugLevel0");
        debugEnabled[1] = par("enableDebugLevel1");
        debugEnabled[2] = par("enableDebugLevel2");
        packetTrace = par("packetTrace");
        printContainers = par("printContainers");
        startTime = par("startTime").doubleValue();
        stopTime = par("stopTime").doubleValue();
        fileSize = par("fileSize");
        numberNodes = par("numHosts");
        replicateRate = par("replicationRate");
        mobilityRate = par("speed").doubleValue();

        if(replicateRate == 0){
            repCount = 0;
        }else{
            repCount =(int)(numberNodes * replicateRate / 100);
        }
        retryDelay = par("retryDelay").doubleValue();
        churnDuration = par("churnDuration").doubleValue();
        churnRate= par("churnRate").doubleValue();
        active = true;


        if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
            error("Invalid startTime/stopTime parameters");

        //self Msg
        selfMsg = new cMessage("sendTimer");
        fileGenMsg = new cMessage("fileGenTimer");
        fileRequestMsg = new cMessage("fileRequestTimer");
        churnTimerMsg = new ChurnMsg("churnTimerMsg");

        myNameStr = this->getOwner()->getFullName();

        //register signals
        //    tranCompSignal = registerSignal("transComplete");
        //    queryCompSignal = registerSignal("queryComplete");

    }
}

void OrionApp::finish()
{
    if(master){
        printResults();
        //    recordScalar("transfers Complete", xferCompletes);
        //    recordScalar("transfers Requested", xferReqs);
        //    recordScalar("transfers failed", xferReqs - xferCompletes);
    }else{
        printFileTable();
    }

    // recordScalar("packets sent", numSent);
    // recordScalar("packets received", numReceived);
    ApplicationBase::finish();
}

void OrionApp::setSocketOptions()
{
    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
        socket.setTimeToLive(1);

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


/*
 * Function: sendBroadcast()
 * Description: prints out string messages to the console
 *  - Param:    *pkt:
 *              &dest:
 * Returns: void
 */
bool OrionApp::sendBroadcast(const IPvXAddress &dest, OrionPacket *pkt)
{
    debug("sendBroadcast", 0);
    printPacketSend(pkt);
    if (!outputInterfaceMulticastBroadcast.empty() && (dest.isMulticast() || (!dest.isIPv6() && dest.get4() == IPv4Address::ALLONES_ADDRESS)))
    {

        for (unsigned int i = 0; i < outputInterfaceMulticastBroadcast.size(); i++)
        {
            UDPSocket::SendOptions options;
            options.outInterfaceId = outputInterfaceMulticastBroadcast[i];

            if (outputInterfaceMulticastBroadcast.size() - i > 1){

                //  debug("here",3);
                //emit(sentPkSignal, pkt);
                socket.sendTo(pkt->dup(), dest, destPort, &options);
            }
            else{
                //debug("here2",3);
                //emit(sentPkSignal, pkt);
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
        // processSend();
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

        churnNode();

    }
    else{

        simtime_t d = simTime() + par("fileGenRate").doubleValue();
        if (stopTime < SIMTIME_ZERO || d < stopTime){
            scheduleAt(d + uniform(0,10)/100, fileGenMsg);
        }
        else
        {
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
    }

    //-------------------Configuration for enabling broadcasts--------------------------
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

    //----------------------end broadcast options---------------------------------------------


}

void OrionApp::processSend()
{

    debug("processSend", 3);
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
            generateFile();
        }else if(msg == fileRequestMsg){
            queryFile();
        }else if(msg == churnTimerMsg){
            if(master){
                churnNode();
            }else{
                active = true;
            }
            cancelAndDelete(msg);
        }
        else if(dynamic_cast<ReqBlockTimer *>(msg)){
            std::string file = dynamic_cast<ReqBlockTimer *>(msg)->getFilename();
            sendRequest(file);
            cancelAndDelete(msg);
        }else if(dynamic_cast<WaitForReq *>(msg)){
            //deletion of WaitForReq is handled elsewhere
            std::string file = dynamic_cast<WaitForReq *>(msg)->getBid();
            if(pendingPackets.count(file)>0){
                resendRequest(dynamic_cast<OrionDataReqPacket*>(pendingPackets[file]));
            }
        }else if(dynamic_cast<QueryMsg *>(msg)){
            //deletion of QueryMsg is handled elsewhere
            std::string file = dynamic_cast<QueryMsg *>(msg)->getFileName();
            int seq = dynamic_cast<QueryMsg *>(msg)->getSeq();
            std::string sourceId = dynamic_cast<QueryMsg *>(msg)->getSourceId();
            IPvXAddress src = dynamic_cast<QueryMsg *>(msg)->getSRC();
            debug("Query Timed out...starting new one....",3);
            sendQuery(file,seq, src, sourceId);
        }else if(dynamic_cast<DelayMsg *>(msg)){
            std::string bid = dynamic_cast<DelayMsg *>(msg)->getBid();
            if(pendingPackets.count(bid)>0 && active){
                if(dynamic_cast<DelayMsg *>(msg)->getBroadcast()){
                    //broadcast query to all neighbor nodes
                    std::ostringstream output;
                    IPvXAddress destAddr(IPv4Address::ALLONES_ADDRESS);
                    sendBroadcast(destAddr,pendingPackets[bid]->dup() );

                }else{
                    //we are sending a regular packet
                    printPacketSend(pendingPackets[bid]);
                    socket.sendTo(pendingPackets[bid]->dup(), pendingPackets[bid]->getDST(), destPort);
                    numSent++;
                }
            }
            //cleanup our packets
            if(dynamic_cast<DelayMsg *>(msg)->getDeleteMe()){
                cancelAndDelete(pendingPackets[bid]);
                pendingPackets.erase(bid);
            }
            cancelAndDelete(msg);
        }else if(msg == selfMsg)
            ASSERT(msg == selfMsg);
        switch (selfMsg->getKind()) {
        case START: processStart(); break;
        case SEND:
            break;
        case STOP:  processStop(); break;
        default: throw cRuntimeError("Invalid kind %d in self message", (int)selfMsg->getKind());
        }
    }else if (msg->getKind() == UDP_I_DATA)
    {
        if(dynamic_cast<ChurnMsg *>(msg)){
            handleChurnMsg(dynamic_cast<ChurnMsg *>(msg));
            cancelAndDelete(msg);
        }else{
            //handle different type of packets if node is active

            if(active){
                OrionPacket *oPacket = dynamic_cast<OrionPacket *>(PK(msg));
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
            }
            cancelAndDelete(msg);
            // processPacket(PK(msg));
        }

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
    //emit(rcvdPkSignal, pk);
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

void OrionApp::printFileTable(){
    std::ofstream outputFile;
    std::string file;
    std::ostringstream fileStream;
    file = "fileTable.csv";
    bool check = false;
    int i(0);
    std::ostringstream fileStreamNew;

    do{
        outputFile.open(file.c_str(), std::ios::out | std::ios::app);
        if(outputFile.is_open()){
            for (std::map<std::string, std::pair<IPvXAddress, int> >::iterator it = fileList.begin(); it != fileList.end(); it++) {
                std::ostringstream output;
                output << repCount << ",";
                output << myNameStr << ",";
                output <<it->first << ",";
                output << it->second.second << ",";
                output << std::endl;
                outputFile << output.str();
            }
            outputFile.close();
            check = true;
        }else{
            fileStreamNew.clear();
            fileStreamNew << "fileTable" << i++ << ".csv";
            file = fileStreamNew.str();
        }

    }while(!check);



}

void OrionApp::printContainerContents(){

    std::ostringstream output;
    output << "----Packets and event timers-----" << std::endl;
    output << "size of pending Packets = " << pendingPackets.size() << std::endl;
    output << "size of pendingQueries = " << pendingQueries.size() << std::endl;
    output << "size of pendingTimeouts = " << pendingTimeouts.size() << std::endl;

    output << "---------other lists--------------" << std::endl;
    output << "size of queryList = " << queryList.size() <<std::endl;
    output << "size of replicateList = " << replicateList.size() <<std::endl;
    output << "size of requestList = " << requestList.size() <<std::endl;
    output << "size of queryResults = " << queryResults.size() <<std::endl;
    output << "size of fileList = " << fileList.size() <<std::endl;
    debug(output.str(),3);

}

void OrionApp::printResults() {
    std::ofstream outputFile;
    std::string file;
    std::ostringstream fileStream;
    //fileStream <<"n" << numberNodes << "-m" <<  mobilityRate << "-r" << replicateRate << "-c" <<
    //        churnRate << ".csv";
    //file = fileStream.str();
    file= "data.csv";
    bool check = false;
    int i(0);
    std::ostringstream fileStreamNew;
    do{
        outputFile.open(file.c_str(), std::ios::out | std::ios::app);
        if(outputFile.is_open()){
            for (std::map<std::string, FileTableData>::iterator it = queryResults.begin(); it != queryResults.end(); it++) {
                if(it->second.isMasterQuery()){
                    std::ostringstream output;
                    output << it->second.getQueryStart() << ",";
                    output << numberNodes << ",";
                    output << mobilityRate << ",";
                    output << replicateRate << "," ;
                    output << churnRate << "," ;
                    output << it->first << ",";
                    output <<it->second.getQueryTime() << ",";
                    output << it->second.getTransferTime() << ",";
                    output << it->second.getRequeries() << ",";
                    double avgHops(0);
                    if(it->second.getBlocksRecieved()> 0){
                        avgHops = (it->second.getTotalHops() / it->second.getBlocksRecieved());
                    }
                    output << avgHops;
                    output << std::endl;
                    outputFile << output.str();
                }
            }
            outputFile.close();
            check = true;
        }else{
            fileStreamNew.clear();
            fileStreamNew << "data" << i++ << ".csv";
            file = fileStreamNew.str();
        }

    }while(!check);

    //   std::ostringstream header;
    //   outputFile.open(file.c_str(), std::ios::out | std::ios::app);
    //   if(outputFile.tellp()==0){
    //   header << "Nodes" << ",";
    //   header << "mobilityRate" << ",";
    //   header << "replicateRate" << "," ;
    //   header << "churnRate" << "," ;
    //   header << "Query Time" << ",";
    //   header << "Transfer Time" << ",";
    //   header << "Requeries" << ",";
    //   header << std::endl;
    //   outputFile << header.str();
    //   }
    //   outputFile.close();

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


void OrionApp::churnNode() {
    debug("churnNode", 0);
    int offCount;
    bool randCheck=false;
    offCount = (int)(numberNodes*churnRate/100);

    std::list<int> usedNodes;
    for(int i(0); i < offCount; i++){
        int randomNode;
        do{
            randomNode = 1 + intrand(numberNodes-1);
            //check for already used nodes for this round
            std::list<int>::iterator findIter = std::find(usedNodes.begin(), usedNodes.end(), randomNode);
            if(findIter != usedNodes.end() ){
                randCheck =true;
            }else{
                randCheck = false;
            }
        }while(randCheck);
        usedNodes.push_back(randomNode);
        std::ostringstream path;
        path << "OrionNetwork.host[" <<randomNode << "].udpApp[0]";
        cModule *targetModule = getModuleByPath(path.str().c_str());
        ChurnMsg *churnMsg = new ChurnMsg("ChurnMsg");
        churnMsg->setCommand("turnOff");
        sendDirect(churnMsg, targetModule, "command$i");

    }

    simtime_t d = simTime() + churnDuration;
    if (stopTime < SIMTIME_ZERO || d < (stopTime) )
    {
        scheduleAt(d, churnTimerMsg);
    }
}

void OrionApp::handleChurnMsg(ChurnMsg* churnMsg) {
    if(master){
        std::string command = churnMsg->getCommand();
        if(command.compare("exclude") == 0){
            tabooList.push_back(churnMsg->getFileName());
        }
    }else{
        debug("turning off",2);
        active = false;
        clearTimersAndLists();
        simtime_t d = simTime() + churnDuration -.01;
        if (stopTime < SIMTIME_ZERO || d < (stopTime) ){
            scheduleAt(d, churnTimerMsg);
        }
    }

}

void OrionApp::clearTimersAndLists() {
    debug("clearTimersAndLists",0);
    for (std::map<std::string, OrionPacket*>::iterator it = pendingPackets.begin(); it != pendingPackets.end(); it++) {
        cancelAndDelete(it->second);
    }
    pendingPackets.clear();

    for (std::map<std::string, WaitForReq*>::iterator it = pendingTimeouts.begin(); it != pendingTimeouts.end(); it++) {
        cancelAndDelete (it->second);
    }
    pendingTimeouts.clear();

    for (std::map<std::string, QueryMsg*>::iterator it = pendingQueries.begin(); it != pendingQueries.end(); it++) {
        cancelAndDelete (it->second);
    }
    pendingQueries.clear();

    fileList.clear();
    queryList.clear();
    replicateList.clear();
    requestList.clear();
    queryResults.clear();
    tabooList.clear();
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
    std::string fileName(myNameStr);
    fileName.append ("-");
    fileName.append (tempTime);

    if(active){
        std::pair<IPvXAddress, int> pair = std::pair<IPvXAddress, int>(myAddr,repCount);
        fileList.insert(std::pair<std::string, std::pair<IPvXAddress, int> >(fileName, pair));
        std::ostringstream requestID;
        requestID << fileName << "-" << myAddr.str();
        replicateList.insert(std::pair<std::string, unsigned int >(requestID.str(), replicateSeqNum));
        if(repCount >0)
            sendReplicateReq(fileName, replicateSeqNum, myAddr);
        replicateSeqNum++;
    }else{

        cModule *targetModule = getModuleByPath("OrionNetwork.host[0].udpApp[0]");
        ChurnMsg *churnMsg = new ChurnMsg("ChurnMsg");
        churnMsg->setCommand("exclude");
        churnMsg->setFileName(fileName.c_str());
        sendDirect(churnMsg, targetModule, "command$i");
    }

    for (std::map<std::string, std::pair<IPvXAddress,simtime_t> >::iterator it = requestList.begin(); it != requestList.end(); it++) {
        if(it->second.second < (simTime() - fileGenRate*2)){
            requestList.erase(it->first);
        }
    }

    for (std::map<std::string, std::pair<IPvXAddress,simtime_t> >::iterator it = queryList.begin(); it != queryList.end(); it++) {
        if(it->second.second < (simTime() - fileGenRate*2)){
            queryList.erase(it->first);
        }
    }

    double ran = uniform(0,1)/5;
    simtime_t d = simTime() + par("fileGenRate").doubleValue() +ran;
    if (stopTime < SIMTIME_ZERO || d < (stopTime) )
    {
        scheduleAt(d, fileGenMsg);
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
    std::ostringstream debugMessage;
    debugMessage << "Requesting file: " << fileToRequest << std::endl;
    debug(debugMessage.str());

    debug("Initiating Query");
    //std::ostringstream queryID;
    //queryID << myAddr.str() << "-" << querySeqNum;

    //store query lookup based on node and seq number
    //std::pair<std::string, IPvXAddress> tempPair(queryID.str(),myAddr);
    //queryList.insert(tempPair);

    //create new entry and add it to our table
    FileTableData entry(fileToRequest, fileSize);
    entry.setQueryStart(simTime().dbl());
    entry.setMasterQuery(true);
    queryResults.insert(std::pair<std::string, FileTableData>(fileToRequest, entry));

    xferReqs++;
    sendQuery(fileToRequest, querySeqNum, myAddr, myNameStr);
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
void OrionApp::sendQuery(std::string _fileToRequest, unsigned int seq, IPvXAddress src, std::string sourceId){
    debug("sendQuery", 0);
    //convert to char array to keep packets happy

    OrionQueryPacket *qPacket = new OrionQueryPacket("QUERY");
    std::ostringstream bid;
    bid <<sourceId << "-" << _fileToRequest << "-q" << seq;

    //test of setting packet type...
    qPacket->setPacketType(QUERY);
    qPacket->setSEQ(seq);
    qPacket->setSRC (src);
    qPacket->setLastNode(myAddr);
    qPacket->setLastNodeId(myNameStr.c_str());
    qPacket->setBid(bid.str().c_str());
    qPacket->setSourceId(sourceId.c_str());
    qPacket->setFilename(_fileToRequest.c_str());
    qPacket->setByteLength(par("messageLength").longValue());

    //store query lookup based on node and seq number
    std::pair<IPvXAddress,simtime_t> tempPair(myAddr, simTime());
    queryList.insert(std::pair<std::string,std::pair<IPvXAddress,simtime_t> > (bid.str(),tempPair ));


    //if I am originating this query, start loop for retries in the event the query doesn't complete.
    if(src == myAddr){
        if(queryResults[_fileToRequest].getRequeries() > 0){
            queryResults[_fileToRequest].setRequeries(queryResults[_fileToRequest].getRequeries()-1);
            if(pendingQueries.count(bid.str()) == 0){
                QueryMsg *queryMsg = new QueryMsg("QueryMsg");
                queryMsg->setFileName(_fileToRequest.c_str());
                queryMsg->setSeq(seq);
                queryMsg->setSRC (myAddr);
                queryMsg->setSourceId(sourceId.c_str());
                pendingQueries.insert(std::pair<std::string, QueryMsg*>(bid.str(), queryMsg));
            }
            if (stopTime < SIMTIME_ZERO || simTime()+3 < (stopTime) ) //
            {
                cancelEvent(pendingQueries[bid.str()]);
                scheduleAt(simTime()+3, pendingQueries[bid.str()]);

            }
        }else{
            //we are out of retries...give up
            cancelAndDelete(pendingQueries[bid.str()]);
            pendingQueries.erase(bid.str());

        }

    }

    pendingPackets.insert(std::pair<std::string,OrionQueryPacket* >(qPacket->getBid(),qPacket));

    //create  request timeoutEvent
    DelayMsg *delayMsg =  new DelayMsg();
    delayMsg->setBid(qPacket->getBid());
    delayMsg->setBroadcast(true);
    delayMsg->setDeleteMe(true);
    delaySend(delayMsg);

}


//Handles query packet.  Since only the master node is sending query requests, we don't have to worry about tracking
//source nodes, just the sequence number.  If the node has already seen the sequence number, it drops the packet.
//If this node hasn't seen this query, and it has the file, it replies with a query response, otherwise
//broadcasts the request to it's neighbors.
//
void OrionApp::handleQuery(OrionQueryPacket *qPacket){
    debug("handleQuery", 0);


    //check queryList to see if we've seen this request yet
    if(queryList.count(qPacket->getBid())==0){

        if(myNameStr.compare("host[5]")==0 && printContainers){
            printContainerContents();
        }
        //create new query list entry and add i

       // printPacketRec(qPacket);

        IPvXAddress tempAddress = IPvXAddress(qPacket->getLastNode());
        std::pair<IPvXAddress, simtime_t> innerPair(tempAddress, simTime());
        std::pair<std::string,std::pair<IPvXAddress, simtime_t> >tempPair(qPacket->getBid(),innerPair);
        queryList.insert(tempPair);

        //if we have the file, reply to the source with a RESPONSE PACKET
        if(hasFile(qPacket->getFilename())){
            sendResponse(qPacket);
        }else if(queryResults.count(qPacket->getFilename()) >0){
            if(queryResults[qPacket->getFilename()].hasSource()){
                sendResponse(qPacket);
            }
        }
        else{

            //forward the request on...
            sendQuery(qPacket->getFilename(), qPacket->getSEQ(), qPacket->getSRC(), qPacket->getSourceId());
        }

    }else{

    }
}

//I have the file, sending response back to source of query
void OrionApp::sendResponse(OrionPacket *oPacket){
    debug("sendResponse", 0);
    //create Response packet
    OrionResponsePacket *rPacket = new OrionResponsePacket("RESPONSE");
    rPacket->setFilename(dynamic_cast<OrionQueryPacket *>(oPacket)->getFilename());
    rPacket->setSRC(oPacket->getSRC());
    rPacket->setSourceId(oPacket->getSourceId());

    std::pair<IPvXAddress,simtime_t> pair(queryList[oPacket->getBid()]);

    rPacket->setDST(pair.first);
    rPacket->setBid(oPacket->getBid());
    rPacket->setHopCount(0);
    rPacket->setLastNode(myAddr);
    rPacket->setLastNodeId(myNameStr.c_str());
    rPacket->setSEQ(oPacket->getSEQ());
    rPacket->setStart(simTime());

    //emit(sentPkSignal, rPacket);
    printPacketSend(rPacket);
    socket.sendTo(rPacket, rPacket->getDST(), destPort);
}



//check to see if I'm the final destination, if so, initiate file request phase.
//if not, update hopcount and forward request on....
void OrionApp::handleResponse(OrionResponsePacket *rPacket){
    debug("handleResponse", 0);

    printPacketRec(rPacket);
    std::string tempFile(rPacket->getFilename());

    //we are the source of the inital query, so start the process of requesting the file
    if(myAddr == rPacket->getSRC()){

        if(queryResults[tempFile].getQueryStop()<0){
            queryResults[tempFile].setQueryStop(simTime().dbl()); //update stop time of query
            //we can delete this from our pending queries since we got a response back
            cancelAndDelete(pendingQueries[rPacket->getBid()]);
            pendingQueries.erase(rPacket->getBid());

            retryDelay = queryResults[tempFile].getQueryTime()+.02; //set retry delay to time it took to execute query
            //emit(queryCompSignal, queryResults[tempFile].getQueryTime());
            queryResults[tempFile].addSource(rPacket->getLastNode());
            //since we are the source node, we can start the transfer process...
            std::pair<IPvXAddress,simtime_t> tempPair(myAddr, simTime());
            requestList.insert(std::pair<std::string,std::pair<IPvXAddress,simtime_t> >(tempFile, tempPair));

            //start transfer timer
            queryResults[tempFile].setTransferStart(simTime().dbl());

            sendRequest(tempFile);

        }else{
            //we already have an entry for this, so we just need to add additional sources
            if(!queryResults[tempFile].hasSource(rPacket->getLastNode()))
                queryResults[tempFile].addSource(rPacket->getLastNode());
        }


    }else{

        // haven't replied to the source yet, so add this to our file list and forward it on
        if(queryResults.count(tempFile)==0){

            FileTableData entry(tempFile, fileSize);
            entry.addSource(rPacket->getLastNode());
            queryResults.insert(std::pair<std::string, FileTableData>(tempFile, entry));

            std::ostringstream queryID;
            queryID << rPacket->getBid();

            std::pair<IPvXAddress,simtime_t> pair(queryList[queryID.str()]);
            rPacket->setDST(pair.first);
            rPacket->setLastNode(myAddr);
            rPacket->setLastNodeId(myNameStr.c_str());
            rPacket->setHopCount(rPacket->getHopCount()+1);
            printPacketSend(rPacket);
            socket.sendTo(rPacket->dup(), rPacket->getDST(), destPort);
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

    int block(queryResults[fileToRequest].getNextBlock());
    if(block >= 0){

        std::string id(fileToRequest);
        id.append("-b");
        std::ostringstream blockString;
        if(reqSeqNum>10000){
            reqSeqNum = 0;
        }
        blockString << block << "-s" << reqSeqNum;
        id.append(blockString.str());
        //debug(id,3);
        if(queryResults[fileToRequest].hasSource()){
            //         debug("Here - 3");
            OrionDataReqPacket *reqPacket = new OrionDataReqPacket("DATA_REQUEST");
            //test of setting packet type...
            reqPacket->setPacketType(DATA_REQUEST);
            reqPacket->setDST(queryResults[fileToRequest].getSource());
            reqPacket->setSEQ(reqSeqNum++);
            reqPacket->setSRC (myAddr);
            reqPacket->setLastNode(myAddr);
            reqPacket->setLastNodeId(myNameStr.c_str());
            reqPacket->setFilename(fileToRequest.c_str());  //const char instead of std::string
            reqPacket->setRetries(retries);
            reqPacket->setBid(id.c_str());
            reqPacket->setBlock(block);
            reqPacket->setByteLength(par("messageLength").longValue());

            //emit(sentPkSignal, reqPacket);
            printPacketSend(reqPacket);
            socket.sendTo(reqPacket, reqPacket->getDST(), destPort);
            numSent++;

            //schedule timeout event;
            //create block request loop timeoutEvent
            ReqBlockTimer *blockTimer =  new ReqBlockTimer();
            blockTimer->setFilename(fileToRequest.c_str());

            //  scheduleAt(simTime()+retryDelay, reqTimeout);
            scheduleAt(simTime()+retryDelay, blockTimer);

        }else{
            //  debug("Error....out of sources");
            int retries = queryResults[fileToRequest].getRequeries();
            if(retries>0){
                queryResults[fileToRequest].setRequeries(retries-1);
                sendQuery(fileToRequest, querySeqNum, myAddr,myNameStr);
                querySeqNum++;

            }else{
                debug("Transfer failed");
                xferFails++;
            }
        }
    }else{
        //debug("Requesting blocks, but all have been received");
    }
}


void OrionApp::handleRequest(OrionDataReqPacket *reqPacket){
    debug("handleRequest", 0);
    sendRequestAck(reqPacket);
    if(requestList.count(reqPacket->getBid())==0){
        std::pair<IPvXAddress,simtime_t> tempPair(reqPacket->getLastNode(), simTime());
        requestList.insert(std::pair<std::string, std::pair<IPvXAddress,simtime_t> >( reqPacket->getBid(), tempPair));
        if(hasFile(reqPacket->getFilename())){
            // debug("I have the file, need to send messages with payload back");
            OrionDataRepPacket *payload = new OrionDataRepPacket("DATA_REPLY");

            payload->setDST(reqPacket->getLastNode());
            payload->setSEQ(reqPacket->getSEQ());
            payload->setSRC (reqPacket->getSRC());
            payload->setBlock(reqPacket->getBlock());
            payload->setLastNode(myAddr);
            payload->setLastNodeId(myNameStr.c_str());
            payload->setFilename(reqPacket->getFilename());
            payload->setByteLength(1024);
            std::string newId;
            newId = reqPacket->getBid();
            newId.append("reply");
            //payload->setBid(newId.c_str());
            payload->setBid(reqPacket->getBid());
            payload->setNumCopiesRemaining(fileList[reqPacket->getFilename()].second);
            payload->setHopCount(0);
            payload->setStart(simTime());
            IPvXAddress origin = IPvXAddress(fileList[reqPacket->getFilename()].first);
            payload->setOrigin(origin);

            pendingPackets.insert(std::pair<std::string, OrionDataRepPacket* >(payload->getBid(),payload));

            //create  request timeoutEvent
            DelayMsg *delayMsg =  new DelayMsg();
            delayMsg->setBid(payload->getBid());
            delayMsg->setBroadcast(false);
            delayMsg->setDeleteMe(true);
            delaySend(delayMsg);

            //printPacketSend(payload);
           // socket.sendTo(payload, dest, destPort);

        }else{//forward on

            if(queryResults[reqPacket->getFilename()].hasSource()){

                WaitForReq *reqTimeout =  new WaitForReq();
                reqTimeout->setFilename(reqPacket->getFilename());
                reqTimeout->setBid(reqPacket->getBid());
                std::pair<std::string, WaitForReq*> tempPacketPair2(reqPacket->getBid(), reqTimeout);
                  scheduleAt(simTime()+retryDelay, reqTimeout);

                //store eventTimer and packet for later lookup
                pendingTimeouts.insert(tempPacketPair2);

                reqPacket->setDST(queryResults[reqPacket->getFilename()].getSource());

                reqPacket->setLastNode(myAddr);
                reqPacket->setLastNodeId(myNameStr.c_str());
                std::ostringstream output;

                // std::pair<std::string, OrionDataReqPacket*> tempPacketPair(reqPacket->getBid(), reqPacket->dup());
                // pendingPackets.insert(tempPacketPair)
                pendingPackets.insert(std::pair<std::string, OrionDataReqPacket* >(reqPacket->getBid(),reqPacket->dup()));

                //create  request timeoutEvent
                DelayMsg *delayMsg =  new DelayMsg();
                delayMsg->setBid(reqPacket->getBid());
                delayMsg->setBroadcast(false);
                delayMsg->setDeleteMe(false);
                delaySend(delayMsg);



              //  printPacketSend(reqPacket);
              //  socket.sendTo(reqPacket->dup(), dest, destPort);
                //schedule timeout event;

            }else{//we don't have this in our table...
                OrionErrorPacket *errPacket = new OrionErrorPacket("DATA_ERR");
                errPacket->setFilename(reqPacket->getFilename());
                errPacket->setSRC(reqPacket->getSRC());
                errPacket->setLastNode(myAddr);
                errPacket->setSEQ(reqPacket->getSEQ());

                std::ostringstream ID;
                ID << reqPacket->getSRC().str() << "-e" << reqPacket->getSEQ();
                errPacket->setBid(ID.str().c_str());

                errPacket->setDST(reqPacket->getLastNode());

                //send packet
                //emit(sentPkSignal, errPacket);

                pendingPackets.insert(std::pair<std::string, OrionErrorPacket* >(errPacket->getBid(),errPacket));

                //create  request timeoutEvent
                DelayMsg *delayMsg =  new DelayMsg();
                delayMsg->setBid(reqPacket->getBid());
                delayMsg->setBroadcast(true);
                delayMsg->setDeleteMe(true);
                delaySend(delayMsg);


               // printPacketSend(errPacket);
               // socket.sendTo(errPacket, dest, destPort);
            }
        }
    }

}

void OrionApp::handleRequestAck(OrionDataAckPacket *ackPacket){
    debug("handleRequestAck", 0);
    printPacketRec(ackPacket);
    //delete our queued packets and cancel our timeout event
    std::string bid = ackPacket->getBid();
    cancelAndDelete(pendingTimeouts[bid]);
    cancelAndDelete(pendingPackets[bid]);

    pendingTimeouts.erase(bid);
    pendingPackets.erase(bid);
}

void OrionApp::handleRequestError(OrionErrorPacket *errPacket){
    debug("Got the error memo");
    queryResults[errPacket->getFilename()].removeSource(errPacket->getSRC());
}


void OrionApp::handleReply(OrionDataRepPacket *repPacket){
    debug("handleReply", 0);
    repPacket->setHopCount(repPacket->getHopCount()+1);
    //If we are the source Node
    if(myAddr == repPacket->getSRC()){
        retryDelay = simTime()-repPacket->getStart();
        if(!queryResults[repPacket->getFilename()].getBlockStatus(repPacket->getBlock())){
            queryResults[repPacket->getFilename()].setBlockComplete(repPacket->getBlock());
            if(master){
                queryResults[repPacket->getFilename()].addHops(repPacket->getHopCount());
                queryResults[repPacket->getFilename()].incBlocksRecieved();
            }
            int blocks = queryResults[repPacket->getFilename()].getRemainBlocks();
            queryResults[repPacket->getFilename()].setRemainBlocks(blocks-1);
            if(queryResults[repPacket->getFilename()].getRemainBlocks()==0){
                queryResults[repPacket->getFilename()].setTransferStop(simTime().dbl());
                queryResults[repPacket->getFilename()].setTransferComplete(true);
                std::string file = repPacket->getFilename();
                std::pair<IPvXAddress, int> pair = std::pair<IPvXAddress, int>(repPacket->getOrigin(), -1);
                fileList.insert(std::pair<std::string, std::pair<IPvXAddress, int> >(file, pair));
                if(master){
                    std::ostringstream output;
                    output << "Xfer Comp - " <<  repPacket->getFilename() << " From: " << repPacket->getSRC() << std::endl;
                    output << " Time to Complete: " << queryResults[repPacket->getFilename()].getTransferTime();
                    //debug(output.str(),3);

                    debug("*********************Winning!*****************************");
                    xferCompletes++;
                    //emit(tranCompSignal,queryResults[repPacket->getFilename()].getTransferTime() );
                }

                if(!master){
                    std::ostringstream output;
                    output << "Copied file: " << repPacket->getFilename();
                    output << " Copies remaining: " << repPacket->getNumCopiesRemaining();
                    debug(output.str(), 1);
                    if(repPacket->getNumCopiesRemaining() > 0){
                        std::ostringstream requestID;
                        requestID << repPacket->getFilename() << "-" << repPacket->getSRC().str();
                        sendReplicateReq(repPacket->getFilename(),replicateList[requestID.str()],fileList[repPacket->getFilename()].first);
                    }

                }
            }

        }else{
            debug("Already seen this block, discard");
        }

        //If we are an intermediate node
    }else{
        std::ostringstream temp;
        temp <<" Id for packet: " << repPacket->getBid() ;
        std::pair<IPvXAddress,simtime_t> pair(requestList[repPacket->getBid()]);
        repPacket->setDST(pair.first);
        repPacket->setLastNode(myAddr);
        repPacket->setLastNodeId(myNameStr.c_str());


        printPacketSend(repPacket);
        socket.sendTo(repPacket->dup(), repPacket->getDST(), destPort);
        numSent++;
        requestList.erase(repPacket->getBid());
        delete pendingPackets[repPacket->getBid()];
        pendingPackets.erase(repPacket->getBid());

    }
}

void OrionApp::resendRequest(OrionDataReqPacket* reqPacket){
    debug("resendRequest", 0);
//    if(reqPacket->getRetries() >0){
//        //duplicate message and save original.
//        //emit(sentPkSignal, reqPacket);
//        reqPacket->setRetries(reqPacket->getRetries()-1);
//        queryResults[reqPacket->getFilename()].setRequeries(reqPacket->getRetries());
//
//        if(pendingTimeouts.count(reqPacket->getBid())> 0){
//            if(!pendingTimeouts[reqPacket->getBid()]->isScheduled()){
//                scheduleAt(simTime()+retryDelay, pendingTimeouts[reqPacket->getBid()]);
//         //       printPacketSend(reqPacket);
//          //      socket.sendTo(reqPacket->dup(), reqPacket->getDST(), destPort);
//                numSent++;
//            }
//
//        }
//    }else{

        queryResults[reqPacket->getFilename()].removeSource();
        //if we have another source for the file, perform local correction

        if(queryResults[reqPacket->getFilename()].hasSource()){
         //   queryResults[reqPacket->getFilename()].setRequeries(retries);
          //  reqPacket->setRetries(retries);
            reqPacket->setDST(queryResults[reqPacket->getFilename()].getSource());

            //duplicate message and save original.
            printPacketSend(reqPacket);
            socket.sendTo(reqPacket->dup(), reqPacket->getDST(), destPort);
            numSent++;
            //schedule timout event;
            scheduleAt(simTime()+retryDelay,pendingTimeouts[reqPacket->getBid()]);

        }else{
            IPvXAddress dest = reqPacket->getLastNode();

            //send error back...
            if(dest != myAddr){
                OrionErrorPacket *errPacket = new OrionErrorPacket("DATA_ERR");
                errPacket->setFilename(reqPacket->getFilename());
                errPacket->setSRC(reqPacket->getSRC());
                errPacket->setLastNode(myAddr);
                errPacket->setLastNodeId(myNameStr.c_str());
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
                // info << "Count of requestList " <<requestList.count(reqPacket->getBid()) << std::endl;
                debug(info.str());
                std::string output("Sending Response to -> ");
                output.append(dest.str());
                debug(output);

                //emit(sentPkSignal, errPacket);
                printPacketSend(errPacket);
                socket.sendTo(errPacket, dest, destPort);
            }
            std::string bid = reqPacket->getBid();

            delete pendingPackets[bid];
            pendingPackets.erase(bid);
            cancelAndDelete(pendingTimeouts[bid]);
            pendingTimeouts.erase(bid);
        }

 //   }
}



void OrionApp::sendRequestAck(OrionDataReqPacket *reqPacket)
{
    debug("sendRequestAck", 0);
    OrionDataAckPacket *reqAck = new OrionDataAckPacket("DATA_REQUEST_ACK");
    reqAck->setFilename(reqPacket->getFilename());
    IPvXAddress dest = reqPacket->getLastNode();
    reqAck->setDST(reqPacket->getLastNode());
    reqAck->setSEQ(reqPacket->getSEQ());
    reqAck->setSRC (reqPacket->getSRC());
    reqAck->setLastNode(myAddr);
    reqPacket->setLastNodeId(myNameStr.c_str());
    reqAck->setRetries(reqPacket->getRetries());
    reqAck->setBid(reqPacket->getBid());
    reqAck->setBlock(reqPacket->getBlock());
    reqAck->setByteLength(par("messageLength").longValue());

    debug(dest.str());
    //emit(sentPkSignal, reqAck);
    printPacketSend(reqAck);
    socket.sendTo(reqAck, dest, destPort);
}


void OrionApp::sendReplicateReq(std::string fileToRep, unsigned int seq, IPvXAddress _origin) {
    debug("sendReplicateReq", 0);

    ReplicatePacket *replicatePacket = new ReplicatePacket("REP_REQUEST");
    std::ostringstream requestID;
    requestID << fileToRep << "-r" << seq;

    //test of setting packet type...
    replicatePacket->setPacketType(REP_REQUEST);
    replicatePacket->setSEQ(seq);
    replicatePacket->setSRC (myAddr);
    IPvXAddress origin = IPvXAddress(_origin);
    replicatePacket->setOrigin(origin);
    replicatePacket->setBid(requestID.str().c_str() );

    if(debugEnabled[2]){
        std::ostringstream output;
        output << "   Origin = " << replicatePacket->getOrigin().str();
        debug(output.str(),2);
    }

    replicatePacket->setLastNode(myAddr);
    replicatePacket->setLastNodeId(myNameStr.c_str());
    replicatePacket->setFilename(fileToRep.c_str());
    replicatePacket->setByteLength(par("messageLength").longValue());
    replicatePacket->setSourceId(myNameStr.c_str());

    //Broadcast to my neighbors
    IPvXAddress destAddr(IPv4Address::ALLONES_ADDRESS);
    sendBroadcast(destAddr, replicatePacket);
}

void OrionApp::handleReplicateReq(ReplicatePacket* replicate) {
    debug("handleReplicateReq", 0);

    if(!master){


        //check replicateList to see if we've seen this request yet
        if(replicateList.count(replicate->getBid())==0 && myAddr != replicate->getSRC() && myAddr != replicate->getOrigin()){
            printPacketRec(replicate);

            replicateList.insert(std::pair<std::string, unsigned int>(replicate->getBid(),replicate->getSEQ()));
            ReplicateConfirmPacket *confirmPacket = new ReplicateConfirmPacket("REP_CONFIRM");
            confirmPacket->setPacketType(REP_CONFIRM);
            IPvXAddress origin = IPvXAddress(replicate->getOrigin());
            confirmPacket->setOrigin(origin);
            confirmPacket->setDST(origin);
            confirmPacket->setSEQ(replicate->getSEQ());
            confirmPacket->setSRC (myAddr);
            confirmPacket->setSourceId(myNameStr.c_str());
            confirmPacket->setLastNode(myAddr);
            confirmPacket->setLastNodeId(myNameStr.c_str());
            confirmPacket->setFilename(replicate->getFilename());
            confirmPacket->setByteLength(par("messageLength").longValue());

            //create  request timeoutEvent
            DelayMsg *delayMsg =  new DelayMsg();
            delayMsg->setBid(replicate->getBid());
            delayMsg->setBroadcast(false);

            //store eventTimer and packet for later lookup
            std::pair<std::string, ReplicateConfirmPacket*> tempPacketPair(replicate->getBid(), confirmPacket);
            pendingPackets.insert(tempPacketPair);
            delaySend(delayMsg);

        }
    }
}

void OrionApp::handleReplicateConfirm(ReplicateConfirmPacket* replicateRes) {

    printPacketRec(replicateRes);
    int remain = fileList[replicateRes->getFilename()].second;

    //if we have achieved the desired amount of replication, send back a confirmation
    if(  remain > 0){
        std::pair<IPvXAddress, int>pair = std::pair<IPvXAddress, int>(fileList[replicateRes->getFilename()].first,remain-1 );
        fileList[replicateRes->getFilename()] = pair;
        ReplicateConfirmAckPacket *confirmPacketACK = new ReplicateConfirmAckPacket("REP_CONFIRM_ACK");
        confirmPacketACK->setPacketType(REP_CONFIRM_ACK);
        confirmPacketACK->setSEQ(replicateRes->getSEQ());
        confirmPacketACK->setDST(replicateRes->getSRC());
        confirmPacketACK->setSRC (myAddr);
        confirmPacketACK->setSourceId(myNameStr.c_str());
        confirmPacketACK->setNumCopiesRemaining(remain);
        confirmPacketACK->setLastNode(myAddr);
        confirmPacketACK->setLastNodeId(myNameStr.c_str());
        confirmPacketACK->setFilename(replicateRes->getFilename());
        confirmPacketACK->setByteLength(par("messageLength").longValue());

        //emit(sentPkSignal, confirmPacketACK);
        printPacketSend(confirmPacketACK);
        socket.sendTo(confirmPacketACK, replicateRes->getSRC(), destPort);
        numSent++;
    }
}

void OrionApp::handleReplicateConfirmAck(ReplicateConfirmAckPacket* replicateAck) {
    debug("handleReplicateAck", 0);
    printPacketRec(replicateAck);
    if(replicateAck->getNumCopiesRemaining() > 0){

        //source needs us to create a copy.

        //  std::ostringstream queryID;
        // queryID << myAddr.str() << "-" << querySeqNum;


        //create new entry and add it to our table
        FileTableData entry(replicateAck->getFilename(), fileSize);
        entry.setQueryStart(simTime().dbl());
        queryResults.insert(std::pair<std::string, FileTableData>(replicateAck->getFilename(), entry));
        sendQuery(replicateAck->getFilename(), querySeqNum, myAddr, myNameStr);
        querySeqNum++;

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
    bool tabooCheck = false;
    std::string fileToRequest;
    do{
        std::ostringstream convert;
        std::ostringstream convert2;
        fileToRequest = "host[";
        int randomNode = 1 + intrand(numberNodes-1);
        int maxNumb = (int) (simTime().dbl()/fileGenRate.dbl());
        int randomFile = 1 + intrand(maxNumb-1);
        convert << randomNode << "]-" << randomFile;
        fileToRequest.append(convert.str());
        //check for already used nodes for this round
        std::list<std::string>::iterator findIter = std::find(tabooList.begin(), tabooList.end(), fileToRequest);
        if(findIter != tabooList.end() ){
            tabooCheck =true;
        }else{
            tabooCheck = false;
        }
    }while(tabooCheck);
    tabooList.push_back(fileToRequest);
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
        output.append(myNameStr);
        output.append(": ");
        output.append(msg);
        std::cout << output << std::endl;
    }
    if(debugEnabled[1] && level == 1){
        std::string output(simTime().str());
        output.append(" - ");
        output.append(myNameStr);
        output.append(": ");
        output.append(msg);
        std::cout << output << std::endl;
    }
    if(debugEnabled[2] && level == 2){
        std::string output(simTime().str());
        output.append(" - ");
        output.append(myNameStr);
        output.append(": ");
        output.append(msg);
        std::cout << output << std::endl;
    }
    if(level == 3){
        std::string output(simTime().str());
        output.append(" - ");
        output.append(myAddr.str());
        output.append(" - ");
        output.append(myNameStr);
        output.append(": ");
        output.append(msg);
        std::cout << output << std::endl;
    }

}
void OrionApp::delaySend(DelayMsg *delayMsg){

    double ran = uniform(0,1)/100000;
    scheduleAt(simTime()+ran, delayMsg);

}

void  OrionApp::printPacketRec(OrionPacket *oPacket){
    if(packetTrace){
        std::ostringstream output;
        output << "                 Received " << oPacket->getName() << " from " << oPacket->getLastNodeId();
        output << " - Source: " << oPacket->getSourceId();
        debug(output.str(), 3);
    }
}
void  OrionApp::printPacketSend(OrionPacket *oPacket){
    if(packetTrace){
        std::ostringstream output;
        output << "Sending " << oPacket->getName() << " to " << oPacket->getDST().str() <<" -ID: " << oPacket->getBid();
        debug(output.str(),3);
    }

}
