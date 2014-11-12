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
unsigned int OrionApp::sentOPackets;
unsigned int OrionApp::recOPackets;


OrionApp::OrionApp() {
    selfMsg = NULL;
    fileGenMsg = NULL;
    fileRequestMsg = NULL;
    churnTimerMsg = NULL;
}

OrionApp::~OrionApp() {
    if (printContainers)
        printContainerContents();
    //   delete &queryList;
    cancelAndDelete(selfMsg);
    cancelAndDelete(fileGenMsg);
    cancelAndDelete(fileRequestMsg);
    cancelAndDelete(churnTimerMsg);

    clearTimersAndLists();

}

/*
 * Function: initialize()
 * -Param: void
 * -Returns: void
 * -Description: used in place of a constructor to ensure node is set up in proper
 *  order. Initialize has 4 states available to use.
 */
void OrionApp::initialize(int stage) {
    ApplicationBase::initialize(stage);

    if (stage == 0) {
        if (master) {
            sentOPackets = 0;
            recOPackets = 0;
        }
        //various counters and other settings not specified in .ned file
        xferFails = 0;
        xferReqs = 0;
        xferCompletes = 0;
        fileNum = 1;
        querySeqNum = 0;
        replicateSeqNum = 0;
        reqSeqNum = 0;
        myNameStr = this->getOwner()->getFullName();
        queryResults.clear();
        myQueryResults.clear();

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
        printBroadcast = par("printBroadcast");
        startTime = par("startTime").doubleValue();
        stopTime = par("stopTime").doubleValue();
        fileSize = par("fileSize");
        numberNodes = par("numHosts");
        replicateRate = par("replicationRate");
        mobilityRate = par("speed").doubleValue();
        retryDelay = par("retryDelay").doubleValue();
        churnDuration = par("churnDuration").doubleValue();
        churnRate = par("churnRate").doubleValue();
        active = true;

        //avoid divide by zero error...
        if (replicateRate == 0) {
            repCount = 0;
        } else {
            repCount = (int) (numberNodes * replicateRate / 100);
        }

        if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
            error("Invalid startTime/stopTime parameters");

        //initialize self timers
        selfMsg = new cMessage("sendTimer");
        fileGenMsg = new cMessage("fileGenTimer");
        fileRequestMsg = new cMessage("fileRequestTimer");
        churnTimerMsg = new ChurnMsg("churnTimerMsg");

        // register
        //    tranCompSignal = registerSignal("transComplete");
        //    queryCompSignal = registerSignal("queryComplete");

    }
}
/*
 * Function: finish()
 * -Param: void
 * -Returns: void
 * -Description: Called when simulation ends.  It will still be called if there is
 * 	problem with the model, but not a runtime error.
 */
void OrionApp::finish() {
    if (master) {
        printResults(); // prints information collected in queryResults.

        //Uncomment if using OMNet statistics
        //    recordScalar("transfers Complete", xferCompletes);
        //    recordScalar("transfers Requested", xferReqs);
        //    recordScalar("transfers failed", xferReqs - xferCompletes);
    } else {

        //printFileTable();
    }

    // recordScalar("packets sent", numSent);
    // recordScalar("packets received", numReceived);
    ApplicationBase::finish();
}

/*
 * Function: setSocketOptions()
 * -Param: void
 * -Returns: void
 * -Description: Establishes socket parameters to be used by application.
 * 	Copied from Basic UDP application
 */
void OrionApp::setSocketOptions() {
    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
        socket.setTimeToLive(1);

    int typeOfService = par("typeOfService");
    if (typeOfService != -1)
        socket.setTypeOfService(typeOfService);

    const char *multicastInterface = par("multicastInterface");
    if (multicastInterface[0]) {
        IInterfaceTable *ift = InterfaceTableAccess().get(this);
        InterfaceEntry *ie = ift->getInterfaceByName(multicastInterface);
        if (!ie)
            throw cRuntimeError(
                    "Wrong multicastInterface setting: no interface named \"%s\"",
                    multicastInterface);
        socket.setMulticastOutputInterface(ie->getInterfaceId());
    }

    bool receiveBroadcast = par("receiveBroadcast");
    if (receiveBroadcast)
        socket.setBroadcast(true);

    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");
    if (joinLocalMulticastGroups)
        socket.joinLocalMulticastGroups();

}

/*
 * Function: sendBroadcast()
 * -Param:    *pkt:
 *             &dest:
 * -Returns: bool
 * -Description: Allows broadcasting of packets instead of unicast.  Implemented from
 * 	code posted on Google Groups
 */
bool OrionApp::sendBroadcast(const IPvXAddress &dest, OrionPacket *pkt) {
    debug("sendBroadcast", 0);
    if (printBroadcast) {
        printPacketSend(pkt);
    }
    if (!outputInterfaceMulticastBroadcast.empty()
            && (dest.isMulticast()
                    || (!dest.isIPv6()
                            && dest.get4() == IPv4Address::ALLONES_ADDRESS))) {

        for (unsigned int i = 0; i < outputInterfaceMulticastBroadcast.size();
                i++) {
            UDPSocket::SendOptions options;
            options.outInterfaceId = outputInterfaceMulticastBroadcast[i];

            if (outputInterfaceMulticastBroadcast.size() - i > 1) {
                //emit(sentPkSignal, pkt);
                socket.sendTo(pkt->dup(), dest, destPort, &options);
            } else {
                //emit(sentPkSignal, pkt);
                socket.sendTo(pkt, dest, destPort, &options);
            }
            sentOPackets++;
        }

        return true;
    }
    return false;
}

/*
 * Function: processStart()
 * -Param:   void
 * -Returns: void
 * -Description: Starts node operation in the simulation.  Called by selfTimer event in
 * 	initialize().  Sets up simulation gates, and port bindings. Originally taken from
 * 	Basic UDP App, but tailored to support OrionApp
 */
void OrionApp::processStart() {
    debug("processStart", 0);
    socket.setOutputGate(gate("udpOut"));
    socket.bind(localPort);
    socketOpen = true;
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

    //starts sending packets (not part of orion but required for proper operation)
    if (!destAddresses.empty()) {
        selfMsg->setKind(SEND);
        // processSend();
    } else {
        if (stopTime >= SIMTIME_ZERO) {
            debug("ProcessStart calling stop");
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
    }

    //orion processes to start
    if (master) { // schedules first file request from system
        simtime_t d = simTime() + par("fileGenRate").doubleValue() * 3;
        if (stopTime < SIMTIME_ZERO || d < stopTime) {
            scheduleAt(d, fileRequestMsg);
        } else {
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }

        //schedule selfMsg to call churnNode().
        simtime_t e = simTime() + churnDuration;
        if (stopTime < SIMTIME_ZERO || e < (stopTime)) {
            scheduleAt(e, churnTimerMsg);
        }

    } else { //if we aren't the master node, start generating files

        simtime_t d = simTime() + par("fileGenRate").doubleValue();
        if (stopTime < SIMTIME_ZERO || d < stopTime) {
            scheduleAt(d + uniform(0, 10) / 100, fileGenMsg);
        } else {
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
    }

    //-------------------Configuration for enabling broadcasts--------------------------

    outputInterfaceMulticastBroadcast.clear();
    if (strcmp(par("outputInterfaceMulticastBroadcast").stringValue(), "")
            != 0) {
        IInterfaceTable* ift = InterfaceTableAccess().get();
        const char *ports = par("outputInterfaceMulticastBroadcast");
        cStringTokenizer tokenizer(ports);
        const char *token;
        while ((token = tokenizer.nextToken()) != NULL) {
            if (strstr(token, "ALL") != NULL) {
                for (int i = 0; i < ift->getNumInterfaces(); i++) {
                    InterfaceEntry *ie = ift->getInterface(i);
                    if (ie->isLoopback())
                        continue;
                    if (ie == NULL)
                        throw cRuntimeError(this,
                                "Invalid output interface name : %s", token);
                    outputInterfaceMulticastBroadcast.push_back(
                            ie->getInterfaceId());
                }
            } else {
                InterfaceEntry *ie = ift->getInterfaceByName(token);
                if (ie == NULL)
                    throw cRuntimeError(this,
                            "Invalid output interface name : %s", token);
                outputInterfaceMulticastBroadcast.push_back(
                        ie->getInterfaceId());
            }
        }
    }

    //----------------------end broadcast options---------------------------------------------

    //stores simulation naming parameters for easier lookup.
    myAddr = IPvXAddressResolver().resolve(
            this->getParentModule()->getFullPath().c_str());
    myId = this->getParentModule()->getId();
}

/*
 * Function: processSend()
 * -Param: void
 * -Returns: void
 * -Description: Original call to generate packets in Basic UDP App.  Still required for
 * 	operation, but doesn't generate packets.
 * 	*/
void OrionApp::processSend() {
    debug("processSend", 3);
    //  sendPacket();
    simtime_t d = simTime() + par("sendInterval").doubleValue();
    if (stopTime < SIMTIME_ZERO || d < stopTime) {
        selfMsg->setKind(SEND);
        scheduleAt(d, selfMsg);
    } else {
        debug("ProcessSend stopping");
        selfMsg->setKind(STOP);
        scheduleAt(stopTime, selfMsg);
    }
}

/*
 * Function: processStop()
 * -Param: void
 * -Returns: void
 * -Description: Called when simulation timer expires.
 *
 * Need to look into using this to complete shut off node from receiving packets.
 * 	*/
void OrionApp::processStop() {
    debug("processStop", 0);
    if (socketOpen) {
        socket.close();
        socketOpen = false;
    }
}

/*
 * Function: handleMessageWhenUp()
 * -Param: cMessage *msg
 * -Returns: void
 * -Description: Handles all messages coming into the node including self generated messages, directSend messages
 *  and Orion Packets generated by the application.  This is specified by OMNet as to how to handle messages and packets.
 *  Code is divided in to several segments, the first is for selfMessages, or those that are schedule through the simulation
 *  framework as a timer event to the same node. The second group is Orion UDP packets that traverse the simulated OSI stack
 *  and are transmitted wirelessly.  The only exception is the ChurnMsg*, which is used by the simulation to turn nodes on
 *  and off.
 * 	*/
void OrionApp::handleMessageWhenUp(cMessage *msg) {
    if (!active) {
        std::cout << "Received: " << msg->getName() << std::endl;
    }
    debug("handleMessageWhenUp", 0);
    //if the message came from ourself, handle it
    if (msg->isSelfMessage()) {
        if (msg == fileGenMsg) { //Self msg to generate file (basic node)
            generateFile();
        } else if (msg == fileRequestMsg) { //Self msg to request file (master node)
            queryFile();
        } else if (msg == churnTimerMsg) { //Self msg to select nodes to turn off (master node) and on (basic node)
            if (master) {
                churnNode();
            } else {
                socket.setOutputGate(gate("udpOut"));
                socket.bind(localPort);
                socketOpen = true;
                setSocketOptions();
                active = true;
            }

        } else if (dynamic_cast<ReqBlockTimer *>(msg)) { //Self msg to request next block for file transfer
            std::string file =
                    dynamic_cast<ReqBlockTimer *>(msg)->getFilename();
            sendRequest(file);
            cancelAndDelete(msg);
        } else if (dynamic_cast<WaitForReq *>(msg)) { //Self msg to check to see if we received request acknowledgment
            //deletion of WaitForReq is handled elsewhere
            std::string file = dynamic_cast<WaitForReq *>(msg)->getBid();
            if (pendingPackets.count(file) > 0) {
                resendRequest(
                        dynamic_cast<OrionDataReqPacket*>(pendingPackets[file]));
            }
        } else if (dynamic_cast<QueryMsg *>(msg)) { //Self msg to initiate another query (will be canceled if query completes)
            //deletion of QueryMsg is handled elsewhere
            std::string file = dynamic_cast<QueryMsg *>(msg)->getFileName();
            int seq = dynamic_cast<QueryMsg *>(msg)->getSeq();
            std::string sourceId = dynamic_cast<QueryMsg *>(msg)->getSourceId();
            IPvXAddress src = dynamic_cast<QueryMsg *>(msg)->getSRC();
            sendQuery(file, seq, src, sourceId);
        } else if (dynamic_cast<DelayMsg *>(msg)) { //Self Msg used to send msg after a random delay has been added
            std::string bid = dynamic_cast<DelayMsg *>(msg)->getBid();
            if (pendingPackets.count(bid) > 0 && active) {
                if (dynamic_cast<DelayMsg *>(msg)->getBroadcast()) { //if msg to send is a broadcast type
                    //broadcast query to all neighbor nodes
                    std::ostringstream output;
                    IPvXAddress destAddr(IPv4Address::ALLONES_ADDRESS);
                    sendBroadcast(destAddr, pendingPackets[bid]->dup());

                } else {
                    //we are sending a regular packet
                    sendPacket(pendingPackets[bid]->dup());
                }
            }
            //cleanup our packets
            if (dynamic_cast<DelayMsg *>(msg)->getDeleteMe()) {
                cancelAndDelete(pendingPackets[bid]);
                pendingPackets.erase(bid);
            }
            cancelAndDelete(msg);
        } else if (msg == selfMsg) //specific kind of msg used by UDP app, required for proper operation
            ASSERT(msg == selfMsg);
        switch (selfMsg->getKind()) {
        case START:
            processStart();
            break;
        case SEND:
            break;
        case STOP:
            processStop();
            break;
        default:
            throw cRuntimeError("Invalid kind %d in self message",
                    (int) selfMsg->getKind());
        }
    }
    //Packet did not originate from this node, so it is either an Orion Packet or a directSend msg.
    else if (msg->getKind() == UDP_I_DATA) {
        if (dynamic_cast<ChurnMsg *>(msg)) { //directSend churnMsg, used by simulation to shut off nodes
            handleChurnMsg(dynamic_cast<ChurnMsg *>(msg));
            cancelAndDelete(msg);
        } else {
            //handle different types of Orion Packets (only if node is active)

            if (active) {
                recOPackets++;
                OrionPacket *oPacket = dynamic_cast<OrionPacket *>(PK(msg));
                switch (oPacket->getPacketType()) {
                case QUERY: //implemented Orion QUERY message
                    handleQuery(dynamic_cast<OrionQueryPacket *>(oPacket));
                    break;
                case RESPONSE: //implemented Orion RESPONSE message
                    handleResponse(
                            dynamic_cast<OrionResponsePacket *>(oPacket));
                    break;
                case DATA_REQUEST:  //implemented Orion DATA_REQUEST message
                    handleRequest(dynamic_cast<OrionDataReqPacket *>(oPacket));
                    break;
                case DATA_REQUEST_ACK: //application layer acknowledgment
                    handleRequestAck(
                            dynamic_cast<OrionDataAckPacket *>(oPacket));
                    break;
                case DATA_REPLY:  //implemented Orion DATA_REPLY message
                    handleReply(dynamic_cast<OrionDataRepPacket *>(oPacket));
                    break;
                case DATA_ERROR: //implemented Orion QUERY message
                    handleRequestError(
                            dynamic_cast<OrionErrorPacket *>(oPacket));
                    break;
                case REP_REQUEST:
                    handleReplicateReq(
                            dynamic_cast<ReplicatePacket *>(oPacket));
                    break;
                case REP_CONFIRM:
                    handleReplicateConfirm(
                            dynamic_cast<ReplicateConfirmPacket *>(oPacket));
                    break;
                case REP_CONFIRM_ACK:
                    handleReplicateConfirmAck(
                            dynamic_cast<ReplicateConfirmAckPacket *>(oPacket));
                    break;
                default:
                    throw cRuntimeError(
                            "AODV Control Packet arrived with undefined packet type: %d",
                            oPacket->getPacketType());
                }
            }
            cancelAndDelete(msg);

        }

    } else if (msg->getKind() == UDP_I_ERROR) {
        EV << "Ignoring UDP error report\n";
        delete msg;
    } else {
        error("Unrecognized message (%s)%s", msg->getClassName(),
                msg->getName());
    }
}

//handle application level packets not used
void OrionApp::processPacket(cPacket *pk) {
    debug("processPacket", 0);
    //emit(rcvdPkSignal, pk);
    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
}

//----------------------node operations------------------------------------

bool OrionApp::handleNodeStart(IDoneCallback *doneCallback) {
    debug("handleNodeStart", 0);
    simtime_t start = std::max(startTime, simTime());
    if ((stopTime < SIMTIME_ZERO) || (start < stopTime)
            || (start == stopTime && startTime == stopTime)) {
        selfMsg->setKind(START);
        scheduleAt(start, selfMsg);
    }
    return true;
}

bool OrionApp::handleNodeShutdown(IDoneCallback *doneCallback) {
    debug("handleNodeShutdown", 0);
    if (selfMsg)
        cancelEvent(selfMsg);

    fileList.clear();
    return true;
}

/*
 * Function: printFileTable()
 * -Param:   void
 * -Returns: void
 * -Output: CSV file
 * -Description: Outputs contents of fileList.  Allows verification of what
 *  files are on what nodes and amount of replication.
 */
void OrionApp::printFileTable() {
    std::ofstream outputFile;
    std::string file;
    std::ostringstream fileStream;
    file = "fileTable.csv";
    bool check = false;
    int i(0);
    std::ostringstream fileStreamNew;

    do {
        outputFile.open(file.c_str(), std::ios::out | std::ios::app);
        if (outputFile.is_open()) {
            for (std::map<std::string, std::pair<IPvXAddress, int> >::iterator it =
                    fileList.begin(); it != fileList.end(); it++) {
                std::ostringstream output;
                output << repCount << ",";
                output << myNameStr << ",";
                output << it->first << ",";
                output << it->second.second << ",";
                output << std::endl;
                outputFile << output.str();
            }
            outputFile.close();
            check = true;
        } else {
            fileStreamNew.clear();
            fileStreamNew << "fileTable" << i++ << ".csv";
            file = fileStreamNew.str();
        }

    } while (!check);

}

/*
 * Function: printContainerContents()
 * -Param:   void
 * -Returns: void
 * -Description: Outputs the size of each container for verifying proper handling of packets
 *  and timer events.
 */
void OrionApp::printContainerContents() {

    std::ostringstream output;
    output << "----Packets and event timers-----" << std::endl;
    output << "size of pending Packets = " << pendingPackets.size()
            << std::endl;
    output << "size of pendingQueries = " << pendingQueries.size() << std::endl;
    output << "size of pendingTimeouts = " << pendingTimeouts.size()
            << std::endl;

    output << "---------other lists--------------" << std::endl;
    output << "size of queryList = " << queryList.size() << std::endl;
    output << "size of replicateList = " << replicateList.size() << std::endl;
    output << "size of requestList = " << requestList.size() << std::endl;
    output << "size of queryResults = " << queryResults.size() << std::endl;
    output << "size of fileList = " << fileList.size() << std::endl;
    debug(output.str(), 3);

    for (std::map<std::string, OrionPacket*>::iterator it =
            pendingPackets.begin(); it != pendingPackets.end(); it++) {
        std::cout << it->second->getName() << "   -    " << it->second->getBid()
                << std::endl;
    }

}

/*
 * Function: printTransfer()
 * -Param:   string fileName
 * -Returns: void
 * -Description: outputs the results of each file transfer attempt.  Similiar to printResults()
 *  but is called for each individual transfer.  Records each transfer in file based on simulation
 *  parameters rather than in one large file.
 */
void OrionApp::printTransfer(std::string fileName) {
    std::ofstream outputFile;
    std::string file;
    std::string base;
    std::ostringstream fileStream;
    fileStream << "data/" << "n" << numberNodes << "-m" << mobilityRate << "-r"
            << replicateRate << "-c" << churnRate;
    ;
    base = fileStream.str();
    file = base;
    file.append(".csv");

    bool check = false;
    int i(0);
    do {
        outputFile.open(file.c_str(), std::ios::out | std::ios::app);
        if (outputFile.is_open()) {
            std::ostringstream output;
            output
                    << myQueryResults[fileName].getQueryStart()
                            - (4 - myQueryResults[fileName].getRequeries()) * 3
                    << ",";
            output << numberNodes << ",";
            output << mobilityRate << ",";
            output << replicateRate << ",";
            output << churnRate << ",";
            output << fileName << ",";
            output << queryResults[fileName].getQueryTime() << ",";
            output << queryResults[fileName].getTransferTime() << ",";
            output << queryResults[fileName].getRequeries() << ",";
            double avgHops(0);
            if (queryResults[fileName].getBlocksRecieved() > 0) {
                avgHops = (queryResults[fileName].getTotalHops()
                        / queryResults[fileName].getBlocksRecieved());
            }
            output << avgHops << ",";
            output << queryResults[fileName].getTotalPackets();
            output << std::endl;

            outputFile << output.str();
            outputFile.close();
            check = true;
        } else {
            file = "";
            file.append(base);
            std::ostringstream num;
            num << ++i;
            file.append(num.str());
            file.append(".csv");

        }

    } while (!check);
}

/*
 * Function: printResults()
 * -Param:   void
 * -Returns: void
 * -Output: CSV file
 * -Description: Outputs contents of queryResults from the master node.
 */
void OrionApp::printResults() {
    std::ofstream outputFile;
    std::string file;
    std::ostringstream fileStream;
    file = "data.csv";
    bool check = false;
    int i(0);
    std::ostringstream fileStreamNew;
    do {
        outputFile.open(file.c_str(), std::ios::out | std::ios::app);
        if (outputFile.is_open()) {
            for (std::map<std::string, FileTableData>::iterator it =
                    myQueryResults.begin(); it != myQueryResults.end(); it++) {
                if (it->second.isMasterQuery()) {
                    std::ostringstream output;
                    output
                            << it->second.getQueryStart()
                                   // - (4 - it->second.getRequeries()) * 3
                            << ",";
                    output << numberNodes << ",";
                    output << mobilityRate << ",";
                    output << replicateRate << ",";
                    output << churnRate << ",";
                    output << it->first << ",";
                    output << it->second.getQueryTime() << ",";
                    output << it->second.getTransferTime() << ",";
                    output << it->second.getRequeries() << ",";
                    double avgHops(0);
                    if (it->second.getBlocksRecieved() > 0) {
                        avgHops = (it->second.getTotalHops()
                                / it->second.getBlocksRecieved());
                    }
                    output << avgHops << ",";
                    output << it->second.getTotalPackets();
                    output << std::endl;
                    outputFile << output.str();
                }
            }
            outputFile.close();
            check = true;
        } else {
            fileStreamNew.clear();
            fileStreamNew << "data" << i++ << ".csv";
            file = fileStreamNew.str();
        }

    } while (!check);

}

/*
 * Function: sendError()
 * -Param:   string filename: name of file currently being requested
 *           IPvXAddress dst: destination to send packet
 *           int seq: sequence number of reqPacket that failed
 *           string requestId: id of requestPacket that failed
 *           bool delay: flag for added delay jitter
 * -Returns: void
 * -Description: Builds errorPacket and sends it to the destination
 */
void OrionApp::sendError(std::string fileName, IPvXAddress dst, int seq,
        std::string requestId, bool delay) {

    OrionErrorPacket *errorPacket = new OrionErrorPacket("DATA_ERR");
    errorPacket->setFilename(fileName.c_str());
    errorPacket->setSRC(myAddr);
    errorPacket->setDST(dst);
    errorPacket->setLastNode(myAddr);
    errorPacket->setLastNodeId(myNameStr.c_str());
    errorPacket->setSEQ(seq);
    errorPacket->setRequestId(requestId.c_str());
    if (!delay) {
        sendPacket(errorPacket);
    } else {
        std::ostringstream ID;
        ID << myAddr.str() << "-e" << seq;
        errorPacket->setBid(ID.str().c_str());

        pendingPackets.insert(
                std::pair<std::string, OrionErrorPacket*>(errorPacket->getBid(),
                        errorPacket));

        //create  request timeoutEvent
        DelayMsg *delayMsg = new DelayMsg();
        delayMsg->setBid(errorPacket->getBid());
        delayMsg->setBroadcast(false);
        delayMsg->setDeleteMe(true);
        delaySend(delayMsg);

    }

}

void OrionApp::handleNodeCrash() {
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
bool OrionApp::hasFile(std::string reqFile) {
    debug("hasFile", 0);
    if (fileList.count(reqFile) > 0)
        return true;
    return false;

}

/*
 * Function: churnNode()
 * -Param: void
 * -Returns: void
 * -Description: selects a number of nodes based on simulation parameters and
 *  sends a direct message to that node to shut off.  Required for simulation and
 *  not part of the orion application.
 */
void OrionApp::churnNode() {
    debug("churnNode", 0);
    int offCount;
    bool randCheck = false;
    offCount = (int) (numberNodes * churnRate / 100);

    std::list<int> usedNodes;
    for (int i(0); i < offCount; i++) {
        int randomNode;
        do {
            randomNode = 1 + intrand(numberNodes - 1);
            //check for already used nodes for this round
            std::list<int>::iterator findIter = std::find(usedNodes.begin(),
                    usedNodes.end(), randomNode);
            if (findIter != usedNodes.end()) {
                randCheck = true;
            } else {
                randCheck = false;
            }
        } while (randCheck);
        usedNodes.push_back(randomNode);

        //get the module path name so we can directly to that module.
        std::ostringstream path;
        path << "OrionNetwork.host[" << randomNode << "].udpApp[0]";
        cModule *targetModule = getModuleByPath(path.str().c_str());
        ChurnMsg *churnMsg = new ChurnMsg("ChurnMsg");
        churnMsg->setCommand("turnOff");
        sendDirect(churnMsg, targetModule, "command$i");

    }

    //schedule the next churn
    simtime_t d = simTime() + churnDuration.dbl();
    if (stopTime < SIMTIME_ZERO || d < (stopTime)) {
        scheduleAt(d, churnTimerMsg);
    }
}

/*
 * Function: handleChurnMsg()
 * -Param: ChurnMsg* churnMsg: message containing required information
 * -Returns: void
 * -Description:
 *   - If we are the master node, add the fileName sent to the taboo list.
 *  This file would have been generated while the node is offline and should never be searched for
 *  as it does not exist.
 *   - If we are a regular node, this was a message from the master node to turn off.
 */
void OrionApp::handleChurnMsg(ChurnMsg* churnMsg) {
    debug("handleChurnMsg", 0);
    if (master) {
        std::string command = churnMsg->getCommand();
        if (command.compare("exclude") == 0) {
            tabooList.push_back(churnMsg->getFileName());
        }
    } else {
        debug("turning off",3);
        active = false;
        socket.close();
        clearTimersAndLists();

        //schedule ourselves to turn back on (simulation replacement)
        simtime_t d = simTime() + churnDuration - .01;
        if (stopTime < SIMTIME_ZERO || d < (stopTime)) {
            cancelEvent(churnTimerMsg);
            scheduleAt(d, churnTimerMsg);
        }
    }

}

/*
 * Function: clearTimersAndLists()
 * -Param: void
 * -Returns: void
 * -Description: Clears all lists
 */
void OrionApp::clearTimersAndLists() {

    debug("clearTimersAndLists", 0);
    //  debug("Clearing Lists",3);
    for (std::map<std::string, OrionPacket*>::iterator it =
            pendingPackets.begin(); it != pendingPackets.end(); it++) {
        if (pendingPackets.count(it->first) > 0)
            cancelAndDelete(it->second);
    }
    pendingPackets.clear();
    for (std::map<std::string, WaitForReq*>::iterator it =
            pendingTimeouts.begin(); it != pendingTimeouts.end(); it++) {
        if (pendingTimeouts.count(it->first) > 0)
            cancelAndDelete(it->second);
    }
    pendingTimeouts.clear();
    for (std::map<std::string, QueryMsg*>::iterator it = pendingQueries.begin();
            it != pendingQueries.end(); it++) {
        if (pendingQueries.count(it->first) > 0)
            cancelAndDelete(it->second);
    }
    pendingQueries.clear();
    fileList.clear();
    queryList.clear();
    replicateList.clear();
    requestList.clear();
    queryResults.clear();
    myQueryResults.clear();
    tabooList.clear();
}

/*
 * Function: generateFile()
 *  - Param: void
 *  - Returns: void
 * Description: constructs a string value to represent new files and adds it to
 *              the apps file table.
 */
void OrionApp::generateFile() {
    debug("generateFile", 0);
    std::string tempTime;
    std::ostringstream convertFileNum;
    convertFileNum << fileNum++;
    tempTime = convertFileNum.str();
    std::string fileName(myNameStr);
    fileName.append("-");
    fileName.append(tempTime);

    if (active) {
        std::pair<IPvXAddress, int> pair = std::pair<IPvXAddress, int>(myAddr,
                repCount);
        fileList.insert(
                std::pair<std::string, std::pair<IPvXAddress, int> >(fileName,
                        pair));
        std::ostringstream requestID;
        requestID << fileName << "-" << myAddr.str();
        replicateList.insert(
                std::pair<std::string, unsigned int>(requestID.str(),
                        replicateSeqNum));
        if (repCount > 0)
            sendReplicateReq(fileName, replicateSeqNum, myAddr);
        replicateSeqNum++;
    } else {

        cModule *targetModule = getModuleByPath(
                "OrionNetwork.host[0].udpApp[0]");
        ChurnMsg *churnMsg = new ChurnMsg("ChurnMsg");
        churnMsg->setCommand("exclude");
        churnMsg->setFileName(fileName.c_str());
        // debug("sending Churn to home",3);
        sendDirect(churnMsg, targetModule, "command$i");
    }

    for (std::map<std::string, std::pair<IPvXAddress, simtime_t> >::iterator it =
            requestList.begin(); it != requestList.end(); it++) {
        if (it->second.second < (simTime() - fileGenRate * 2)) {
            requestList.erase(it->first);
        }
    }

    for (std::map<std::string, std::pair<IPvXAddress, simtime_t> >::iterator it =
            queryList.begin(); it != queryList.end(); it++) {
        if (it->second.second < (simTime() - fileGenRate * 2)) {
            queryList.erase(it->first);
        }
    }

    double ran = uniform(0, 1) / 5;
    simtime_t d = simTime() + par("fileGenRate").doubleValue() + ran;
    if (stopTime < SIMTIME_ZERO || d < (stopTime)) {
        scheduleAt(d, fileGenMsg);
    }

}

/*
 * Function: requestFile()
 *  - Param: void
 *  - Returns: void
 * Description: Initiates request for file transfer.
 *              Implements Orion file transfer algorithm by performing a round robin
 *              request of each file block (each block size is less than 1 MTU - I use
 *              1024 as a convenience factor). The function loops through all the blocks
 *              are successfully transferred or function exceeds file-transfer timeout.
 */
void OrionApp::queryFile() {
    debug("queryFile", 0);

    std::string fileToRequest(selectFile());
    //create new entry and add it to our table
    FileTableData entry(fileToRequest, fileSize);
   // FileTableData entry = *new FileTableData(fileToRequest, fileSize);
    entry.setQueryStart(simTime().dbl());
    entry.setMasterQuery(true);
    entry.setSystemPacketsStart(sentOPackets);
    std::ostringstream output2;
    output2 << "Creating Entry for  " <<  fileToRequest;
    debug(output2.str(),3);

    std::pair<std::string, FileTableData> tempPair = std::pair<std::string, FileTableData>(fileToRequest, entry);
    myQueryResults.insert(tempPair);
    xferReqs++;
    sendQuery(fileToRequest, querySeqNum, myAddr, myNameStr);
    querySeqNum++;
    simtime_t d = simTime() + fileQueryRate;

    if (stopTime < SIMTIME_ZERO || d < (stopTime)) //
            {
        scheduleAt(d, fileRequestMsg);

    } else {
        debug("Req File Stopping");
    }
}

/*
 * Function: sendQuery()
 *  - Param: string fileToRequest: file to be queried
 *           uint seq: current query sequence number of node
 *           IPvXAddress src: address of node that initiated query
 *           string sourceId: simulate name of source node
 *  - Returns: void
 * Description: Builds query packet and marshells it for broadcasting.  If master
 *              also starts required timers for query bookkeeping.
 */
void OrionApp::sendQuery(std::string _fileToRequest, unsigned int seq,
        IPvXAddress src, std::string sourceId) {
    debug("sendQuery", 0);

    OrionQueryPacket *qPacket = new OrionQueryPacket("QUERY");
    std::ostringstream bid;
    bid << sourceId << "-" << _fileToRequest << "-q" << seq;

    //test of setting packet type...
    qPacket->setPacketType(QUERY);
    qPacket->setSEQ(seq);
    qPacket->setSRC(src);
    qPacket->setLastNode(myAddr);
    qPacket->setLastNodeId(myNameStr.c_str());
    qPacket->setBid(bid.str().c_str());
    qPacket->setSourceId(sourceId.c_str());
    qPacket->setFilename(_fileToRequest.c_str());
    qPacket->setByteLength(par("messageLength").longValue());

    //store query lookup based on node and seq number
    if(queryList.count(bid.str())==0){
    std::pair<IPvXAddress, simtime_t> tempPair(myAddr, simTime());
    queryList.insert(
            std::pair<std::string, std::pair<IPvXAddress, simtime_t> >(
                    bid.str(), tempPair));
    }



    //if I am originating this query, start loop for retries in the event the query doesn't complete.
    if (src == myAddr) {
        FileTableData *entry = &myQueryResults[_fileToRequest];
        if(entry->getRequeries() < 0 ||entry->getRequeries() >5){
        std::ostringstream output;
        output << "file: "<< _fileToRequest << " - sendQuery: getRequeries =  " <<  entry->getRequeries();
        debug(output.str(),3);
        }

        if (entry->getRequeries() > 0) {
            entry->setRequeries(entry->getRequeries() - 1);
           // entry->setQueryStart(simTime().dbl());
            entry->setQueryStop(-1);
            if (pendingQueries.count(bid.str()) == 0) {
                QueryMsg *queryMsg = new QueryMsg("QueryMsg");
                queryMsg->setFileName(_fileToRequest.c_str());
                queryMsg->setSeq(seq);
                queryMsg->setSRC(myAddr);
                queryMsg->setSourceId(sourceId.c_str());
                pendingQueries.insert(
                        std::pair<std::string, QueryMsg*>(bid.str(), queryMsg));
            }
            if (stopTime < SIMTIME_ZERO || simTime() + 3 < (stopTime)) //
                    {
                cancelEvent(pendingQueries[bid.str()]);
                scheduleAt(simTime() + 3, pendingQueries[bid.str()]);

            }
        } else {
            //we are out of retries...give up
            cancelAndDelete(pendingQueries[bid.str()]);
            pendingQueries.erase(bid.str());

        }

    }

    if(pendingPackets.count(bid.str())==0){
    pendingPackets.insert(
            std::pair<std::string, OrionQueryPacket*>(qPacket->getBid(),
                    qPacket));
    }

    //create  request timeoutEvent
    DelayMsg *delayMsg = new DelayMsg();
    delayMsg->setBid(qPacket->getBid());
    delayMsg->setBroadcast(true);
    delayMsg->setDeleteMe(true);
    delaySend(delayMsg);

}


/*
 * Function: handleQuery()
 *  - Param: OrionQueryPacket *qPacket: received query packet
 *  - Returns: void
 * Description: Handles query packet.
 *              If the node has already seen the packet ID (source & sequence), it drops the packet.
 *              If this node hasn't seen this query, and it has the file, it replies with a query
 *              response, otherwise broadcasts the request to it's neighbors.
 */
void OrionApp::handleQuery(OrionQueryPacket *qPacket) {
    debug("handleQuery", 0);

    //check queryList to see if we've seen this request yet
    if (queryList.count(qPacket->getBid()) == 0) {

        //add this query to our list of on-going queries.  Record the current time for bookkeeping.
        IPvXAddress tempAddress = IPvXAddress(qPacket->getLastNode());
        std::pair<IPvXAddress, simtime_t> innerPair(tempAddress, simTime());
        std::pair<std::string, std::pair<IPvXAddress, simtime_t> > tempPair(
                qPacket->getBid(), innerPair);
        queryList.insert(tempPair);

        //if we have the file, reply to the source with a RESPONSE PACKET
        if (hasFile(qPacket->getFilename())) {
            sendResponse(qPacket);
        } else if (queryResults.count(qPacket->getFilename()) > 0) {
            if (queryResults[qPacket->getFilename()].hasSource()) {
                sendResponse(qPacket);
            }
        } else {

            //forward the request on...
            sendQuery(qPacket->getFilename(), qPacket->getSEQ(),
                    qPacket->getSRC(), qPacket->getSourceId());
        }
    }
}

/*
 * Function: sendResponse()
 *  - Param: OrionPacket *oPacket: received query packet
 *  - Returns: void
 * Description: Builds a response packet and sends it back along the reverse path to the source.
 */
void OrionApp::sendResponse(OrionPacket *oPacket) {
    debug("sendResponse", 0);
    //create Response packet
    OrionResponsePacket *rPacket = new OrionResponsePacket("RESPONSE");
    rPacket->setFilename(
            dynamic_cast<OrionQueryPacket *>(oPacket)->getFilename());
    rPacket->setSRC(oPacket->getSRC());
    rPacket->setSourceId(oPacket->getSourceId());

    std::pair<IPvXAddress, simtime_t> pair(queryList[oPacket->getBid()]);
    rPacket->setDST(pair.first);
    rPacket->setBid(oPacket->getBid());
    rPacket->setHopCount(0);
    rPacket->setLastNode(myAddr);
    rPacket->setLastNodeId(myNameStr.c_str());
    rPacket->setSEQ(oPacket->getSEQ());
    rPacket->setStart(simTime());
    sendPacket(rPacket);

}

//check to see if I'm the final destination, if so, initiate file request phase.
//if not, update hopcount and forward request on....
void OrionApp::handleResponse(OrionResponsePacket *rPacket) {
    debug("handleResponse", 0);
    printPacketRec(rPacket);
    std::string tempFile(rPacket->getFilename());

    //we are the source of the inital query, so start the process of requesting the file
    if (myAddr == rPacket->getSRC()) {
        FileTableData *entry = &myQueryResults[tempFile];
        if (entry->getQueryStop() < 0) {
           entry->setQueryStop(simTime().dbl());
           //we can delete this from our pending queries since we got a response back
            cancelAndDelete(pendingQueries[rPacket->getBid()]);
            pendingQueries.erase(rPacket->getBid());
            retryDelay = entry->getQueryTime() + .02; //set retry delay to time it took to execute query
            entry->addSource(rPacket->getLastNode());
            //since we are the source node, we can start the transfer process...
            std::pair<IPvXAddress, simtime_t> tempPair(myAddr, simTime());
            requestList.insert(
                    std::pair<std::string, std::pair<IPvXAddress, simtime_t> >(
                            tempFile, tempPair));

            //start transfer timer
            entry->setTransferStart(simTime().dbl());
            entry->setTimeOfLastBlock(simTime().dbl());
            sendRequest(tempFile);
        } else {
            //we already have an entry for this, so we just need to add additional sources
            if (!entry->hasSource(rPacket->getLastNode()))
                entry->addSource(rPacket->getLastNode());
        }
    } else {

        // haven't replied to the source yet, so add this to our file list and forward it on

        if (queryResults.count(tempFile) == 0) {
            FileTableData newEntry(tempFile, fileSize);
            newEntry.addSource(rPacket->getLastNode());
            std::ostringstream output2;
            output2 << "Creating Entry for  " <<  tempFile;
            debug(output2.str(),3);
            std::pair<std::string, FileTableData>tempPair = std::pair<std::string, FileTableData>(tempFile, newEntry);
            queryResults.insert(tempPair);

            std::ostringstream queryID;
            queryID << rPacket->getBid();

            std::pair<IPvXAddress, simtime_t> pair(queryList[queryID.str()]);
            rPacket->setDST(pair.first);
            rPacket->setLastNode(myAddr);
            rPacket->setLastNodeId(myNameStr.c_str());
            rPacket->setHopCount(rPacket->getHopCount() + 1);
            // printPacketSend(rPacket);
            sendPacket(rPacket->dup());
            // socket.sendTo(rPacket->dup(), rPacket->getDST(), destPort);
        } else {
            FileTableData *entry = &queryResults[tempFile];
            //we've replied to this file already, so we don't need to send any other updates
            //but we'll still update our list for local repair
            entry->addSource(rPacket->getLastNode());
            debug("updating queryResults with new src");
        }
    }

}

void OrionApp::sendRequest(std::string fileToRequest) {
    debug("sendRequest", 0);

    FileTableData *entry = &myQueryResults[fileToRequest];

    if (entry->getTimeOfLastBlock() > simTime().dbl() - 1) {
        int block(entry->getNextBlock());
        if (block >= 0) {

            std::string id(fileToRequest);
            id.append("-b");
            std::ostringstream blockString;
            if (reqSeqNum > 10000) {
                reqSeqNum = 0;
            }
            blockString << block << "-s" << reqSeqNum;
            id.append(blockString.str());
            //debug(id,3);
            if (entry->hasSource()) {
                OrionDataReqPacket *reqPacket = new OrionDataReqPacket(
                        "DATA_REQUEST");
                //test of setting packet type...
                reqPacket->setPacketType(DATA_REQUEST);
                reqPacket->setDST(entry->getSource());
                reqPacket->setSEQ(reqSeqNum++);
                reqPacket->setSRC(myAddr);
                reqPacket->setLastNode(myAddr);
                reqPacket->setLastNodeId(myNameStr.c_str());
                reqPacket->setFilename(fileToRequest.c_str());
                reqPacket->setRetries(retries);
                reqPacket->setBid(id.c_str());
                reqPacket->setBlock(block);
                reqPacket->setByteLength(par("messageLength").longValue());
                sendPacket(reqPacket);

                //create block request loop timeoutEvent
                ReqBlockTimer *blockTimer = new ReqBlockTimer();
                blockTimer->setFilename(fileToRequest.c_str());

                //  scheduleAt(simTime()+retryDelay, reqTimeout);

                scheduleAt(simTime() + retryDelay, blockTimer);

            } else {
                //  debug("Error....out of sources");
                unsigned int requeries = entry->getRequeries();
                std::ostringstream output;
                output << "sendRequest-OOS: getRequeries =  " << requeries;
                debug(output.str(),3);
                if (requeries > 0) {
                    entry->setRequeries(requeries - 1);
                    sendQuery(fileToRequest, querySeqNum, myAddr, myNameStr);
                    querySeqNum++;

                } else {
                    debug("Transfer failed");
                    printTransfer(fileToRequest);
                    xferFails++;
                }
            }
        } else {
            //debug("Requesting blocks, but all have been received");
        }
    } else {
        //  debug("Error....took too long to get lastblock");
        unsigned int requeries = entry->getRequeries();
        std::ostringstream output;
        output << "sendRequest-Timeout: getRequeries =  " <<  requeries;
        debug(output.str(),3);
        if (requeries > 0) {
            entry->setRequeries(requeries - 1);
            sendQuery(fileToRequest, querySeqNum, myAddr, myNameStr);
            querySeqNum++;

        } else {
            debug("Transfer failed");
            xferFails++;
        }
    }
}

void OrionApp::handleRequest(OrionDataReqPacket *reqPacket) {
    debug("handleRequest", 0);

    if (requestList.count(reqPacket->getBid()) == 0) {
        printPacketRec(reqPacket);
        sendRequestAck(reqPacket);
        std::pair<IPvXAddress, simtime_t> tempPair(reqPacket->getLastNode(),
                simTime());

        if (hasFile(reqPacket->getFilename())) {
            // debug("I have the file, need to send messages with payload back");
            OrionDataRepPacket *payload = new OrionDataRepPacket("DATA_REPLY");

            payload->setDST(reqPacket->getLastNode());
            payload->setSEQ(reqPacket->getSEQ());
            payload->setSRC(reqPacket->getSRC());
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
            payload->setNumCopiesRemaining(
                    fileList[reqPacket->getFilename()].second);
            payload->setHopCount(0);
            payload->setStart(simTime());
            IPvXAddress origin = IPvXAddress(
                    fileList[reqPacket->getFilename()].first);
            payload->setOrigin(origin);

            pendingPackets.insert(
                    std::pair<std::string, OrionDataRepPacket*>(
                            payload->getBid(), payload));

            //create  request timeoutEvent
            DelayMsg *delayMsg = new DelayMsg();
            delayMsg->setBid(payload->getBid());
            delayMsg->setBroadcast(false);
            delayMsg->setDeleteMe(true);
            delaySend(delayMsg);

        } else {            //forward on

            if (queryResults[reqPacket->getFilename()].hasSource()) {
                requestList.insert(
                        std::pair<std::string, std::pair<IPvXAddress, simtime_t> >(
                                reqPacket->getBid(), tempPair));
                WaitForReq *reqTimeout = new WaitForReq();
                reqTimeout->setFilename(reqPacket->getFilename());
                reqTimeout->setBid(reqPacket->getBid());
                std::pair<std::string, WaitForReq*> tempPacketPair2(
                        reqPacket->getBid(), reqTimeout);
                scheduleAt(simTime() + .2, reqTimeout);

                //store eventTimer and packet for later lookup
                pendingTimeouts.insert(tempPacketPair2);

                reqPacket->setDST(
                        queryResults[reqPacket->getFilename()].getSource());

                reqPacket->setLastNode(myAddr);
                reqPacket->setLastNodeId(myNameStr.c_str());
                std::ostringstream output;
                pendingPackets.insert(
                        std::pair<std::string, OrionDataReqPacket*>(
                                reqPacket->getBid(), reqPacket->dup()));

                //create  request timeoutEvent
                DelayMsg *delayMsg = new DelayMsg();
                delayMsg->setBid(reqPacket->getBid());
                delayMsg->setBroadcast(false);
                delayMsg->setDeleteMe(false);
                delaySend(delayMsg);



            } else {//we don't have this in our table...

                std::ostringstream ID;
                ID << reqPacket->getSRC().str() << "-e" << reqPacket->getSEQ();
                // errorPacket->setBid(ID.str().c_str());

                sendError(reqPacket->getFilename(), reqPacket->getLastNode(),
                        reqPacket->getSEQ(), reqPacket->getBid(), true);
            }
        }
    }

}

void OrionApp::handleRequestAck(OrionDataAckPacket *ackPacket) {
    debug("handleRequestAck", 0);
    printPacketRec(ackPacket);
    //delete our queued packets and cancel our timeout event
    std::string bid = ackPacket->getBid();
    cancelAndDelete(pendingTimeouts[bid]);
    cancelAndDelete(pendingPackets[bid]);

    pendingTimeouts.erase(bid);
    pendingPackets.erase(bid);
}

void OrionApp::handleRequestError(OrionErrorPacket *errorPacket) {
    debug("handleRequestError", 0);
    queryResults[errorPacket->getFilename()].removeSource(
            errorPacket->getSRC());
    if (!queryResults[errorPacket->getFilename()].hasSource()) {

        if (requestList.count(errorPacket->getRequestId()) > 0) {

            errorPacket->setSRC(myAddr);
            errorPacket->setLastNode(myAddr);
            errorPacket->setDST(requestList[errorPacket->getRequestId()].first);

            //   printPacketSend(errorPacket);
            sendPacket(errorPacket->dup());
            //  socket.sendTo(errorPacket->dup(), errorPacket->getDST(), destPort);
            sendError(errorPacket->getFilename(),
                    requestList[errorPacket->getRequestId()].first,
                    errorPacket->getSEQ(), errorPacket->getRequestId(), false);
        }
    }
}

void OrionApp::handleReply(OrionDataRepPacket *repPacket) {
    debug("handleReply", 0);
    repPacket->setHopCount(repPacket->getHopCount() + 1);
    //If we are the source Node
    FileTableData *entry = &myQueryResults[repPacket->getFilename()];
    if (myAddr == repPacket->getSRC()) {
        retryDelay = (simTime() - repPacket->getStart()) * 2;
        entry->setTimeOfLastBlock(simTime().dbl());

        if (!entry->getBlockStatus(repPacket->getBlock())) {
            entry->setBlockComplete(repPacket->getBlock());
            entry->addHops(repPacket->getHopCount());
            entry->incBlocksRecieved();

            int blocks = entry->getRemainBlocks();
            entry->setRemainBlocks(blocks - 1);
            if (entry->getRemainBlocks() == 0) {
                entry->setTransferStop(simTime().dbl());
                entry->setTransferComplete(true);
                entry->setSystemPacketsStop(sentOPackets);
                std::string file = repPacket->getFilename();
                std::pair<IPvXAddress, int> pair = std::pair<IPvXAddress, int>(
                        repPacket->getOrigin(), -1);
                fileList.insert(
                        std::pair<std::string, std::pair<IPvXAddress, int> >(
                                file, pair));
                if (master) {
                    printTransfer(repPacket->getFilename());
                    xferCompletes++;
                }

                if (!master) {
                    std::ostringstream output;
                    output << "Copied file: " << repPacket->getFilename();
                    output << " Copies remaining: "
                            << repPacket->getNumCopiesRemaining();
                    debug(output.str(), 1);
                    if (repPacket->getNumCopiesRemaining() > 0) {
                        std::ostringstream requestID;
                        requestID << repPacket->getFilename() << "-"
                                << repPacket->getSRC().str();
                        sendReplicateReq(repPacket->getFilename(),
                                replicateList[requestID.str()],
                                fileList[repPacket->getFilename()].first);
                    }
                }

            }

        } else {
            debug("Already seen this block, discard");
        }

        //If we are an intermediate node
    } else {
        std::ostringstream temp;
        temp << " Id for packet: " << repPacket->getBid();
        std::pair<IPvXAddress, simtime_t> pair(
                requestList[repPacket->getBid()]);
        repPacket->setDST(pair.first);
        repPacket->setLastNode(myAddr);
        repPacket->setLastNodeId(myNameStr.c_str());

        // printPacketSend(repPacket);
        sendPacket(repPacket->dup());
        requestList.erase(repPacket->getBid());
        delete pendingPackets[repPacket->getBid()];
        pendingPackets.erase(repPacket->getBid());

    }
}

void OrionApp::resendRequest(OrionDataReqPacket* reqPacket) {
    debug("resendRequest", 0);

    FileTableData *entry = &queryResults[reqPacket->getFilename()];
    std::ostringstream output;
    output << "resendRequest: getRequeries =  " <<  entry->getRequeries();
    debug(output.str(),3);

    if (reqPacket->getRetries() > 0) {
        //duplicate message and save original.
        reqPacket->setRetries(reqPacket->getRetries() - 1);
        //entry->setRequeries(reqPacket->getRetries());

        if (pendingTimeouts.count(reqPacket->getBid()) > 0) {
            if (!pendingTimeouts[reqPacket->getBid()]->isScheduled()) {
                scheduleAt(simTime() + .02, pendingTimeouts[reqPacket->getBid()]);
                sendPacket(reqPacket->dup());
            }

        }
    } else {
        //debug("Didn't get ack in time...",3);
        if (entry->hasSource(reqPacket->getDST())) {
            entry->removeSource(reqPacket->getDST());
        }

        //if we have another source for the file, perform local correction
        // if (queryResults[reqPacket->getFilename()].hasSource()) {
        if (entry->hasSource()) {
            reqPacket->setDST(entry->getSource());
            //duplicate message and save original.
            sendPacket(reqPacket->dup());

            //schedule timout event;
            scheduleAt(simTime() + .02, pendingTimeouts[reqPacket->getBid()]);

        } else {
            // debug("Out of sources...sending error back",3);
            //send error back...
            if (reqPacket->getLastNode() != myAddr) {
                sendError(reqPacket->getFilename(), reqPacket->getLastNode(),
                        reqPacket->getSEQ(), reqPacket->getBid(), false);
            }
            std::string bid = reqPacket->getBid();

            delete pendingPackets[bid];
            pendingPackets.erase(bid);
            cancelAndDelete(pendingTimeouts[bid]);
            pendingTimeouts.erase(bid);
        }

    }
}

void OrionApp::sendRequestAck(OrionDataReqPacket *reqPacket) {
    debug("sendRequestAck", 0);
    OrionDataAckPacket *reqAck = new OrionDataAckPacket("DATA_REQUEST_ACK");
    reqAck->setFilename(reqPacket->getFilename());
    reqAck->setDST(reqPacket->getLastNode());
    reqAck->setSEQ(reqPacket->getSEQ());
    reqAck->setSRC(reqPacket->getSRC());
    reqAck->setLastNode(myAddr);
    reqPacket->setLastNodeId(myNameStr.c_str());
    reqAck->setRetries(reqPacket->getRetries());
    reqAck->setBid(reqPacket->getBid());
    reqAck->setBlock(reqPacket->getBlock());
    reqAck->setByteLength(par("messageLength").longValue());

    sendPacket(reqAck);
}

void OrionApp::sendReplicateReq(std::string fileToRep, unsigned int seq,
        IPvXAddress _origin) {
    debug("sendReplicateReq", 0);

    ReplicatePacket *replicatePacket = new ReplicatePacket("REP_REQUEST");
    std::ostringstream requestID;
    requestID << fileToRep << "-r" << seq;

    //test of setting packet type...
    replicatePacket->setPacketType(REP_REQUEST);
    replicatePacket->setSEQ(seq);
    replicatePacket->setSRC(myAddr);
    IPvXAddress origin = IPvXAddress(_origin);
    replicatePacket->setOrigin(origin);
    replicatePacket->setBid(requestID.str().c_str());

    if (debugEnabled[2]) {
        std::ostringstream output;
        output << "   Origin = " << replicatePacket->getOrigin().str();
        debug(output.str(), 2);
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

    if (!master  && queryResults.count(replicate->getFilename())==0) {

        //check replicateList to see if we've seen this request yet
        if (replicateList.count(replicate->getBid()) == 0
                && myAddr != replicate->getSRC()
                && myAddr != replicate->getOrigin()) {
            printPacketRec(replicate);

            replicateList.insert(
                    std::pair<std::string, unsigned int>(replicate->getBid(),
                            replicate->getSEQ()));
            ReplicateConfirmPacket *confirmPacket = new ReplicateConfirmPacket(
                    "REP_CONFIRM");
            confirmPacket->setPacketType(REP_CONFIRM);
            IPvXAddress origin = IPvXAddress(replicate->getOrigin());
            confirmPacket->setOrigin(origin);
            confirmPacket->setDST(origin);
            confirmPacket->setBid(replicate->getBid());
            confirmPacket->setSEQ(replicate->getSEQ());
            confirmPacket->setSRC(myAddr);
            confirmPacket->setSourceId(myNameStr.c_str());
            confirmPacket->setLastNode(myAddr);
            confirmPacket->setLastNodeId(myNameStr.c_str());
            confirmPacket->setFilename(replicate->getFilename());
            confirmPacket->setByteLength(par("messageLength").longValue());

            //create  request timeoutEvent
            DelayMsg *delayMsg = new DelayMsg();
            delayMsg->setBid(confirmPacket->getBid());
            delayMsg->setBroadcast(false);
            delayMsg->setDeleteMe(true);

            //store eventTimer and packet for later lookup
            std::pair<std::string, ReplicateConfirmPacket*> tempPacketPair(
                    replicate->getBid(), confirmPacket);
            pendingPackets.insert(tempPacketPair);
            delaySend(delayMsg);

        }
    }
}

void OrionApp::handleReplicateConfirm(ReplicateConfirmPacket* replicateRes) {

    printPacketRec(replicateRes);
    int remain = fileList[replicateRes->getFilename()].second;

    //if we haven't achieved the desired amount of replication, send back a confirmation
    if (remain > 0) {
//        std::pair<IPvXAddress, int> pair = std::pair<IPvXAddress, int>(
//                fileList[replicateRes->getFilename()].first, remain - 1);
        fileList[replicateRes->getFilename()].second = remain - 1;
        ReplicateConfirmAckPacket *confirmPacketACK =
                new ReplicateConfirmAckPacket("REP_CONFIRM_ACK");
        confirmPacketACK->setPacketType(REP_CONFIRM_ACK);
        confirmPacketACK->setSEQ(replicateRes->getSEQ());
        confirmPacketACK->setDST(replicateRes->getSRC());
        confirmPacketACK->setSRC(myAddr);
        confirmPacketACK->setSourceId(myNameStr.c_str());
        confirmPacketACK->setNumCopiesRemaining(remain);
        confirmPacketACK->setLastNode(myAddr);
        confirmPacketACK->setLastNodeId(myNameStr.c_str());
        confirmPacketACK->setFilename(replicateRes->getFilename());
        confirmPacketACK->setByteLength(par("messageLength").longValue());

        sendPacket(confirmPacketACK);

    }
}

void OrionApp::handleReplicateConfirmAck(
        ReplicateConfirmAckPacket* replicateAck) {
    debug("handleReplicateAck", 0);
    printPacketRec(replicateAck);
    if (replicateAck->getNumCopiesRemaining() > 0 && !master) {
        //create new entry and add it to our table
        FileTableData entry(replicateAck->getFilename(), fileSize);
        entry.setQueryStart(simTime().dbl());
        std::pair<std::string, FileTableData>tempPair = std::pair<std::string, FileTableData>(
                replicateAck->getFilename(), entry);
        myQueryResults.insert(tempPair);
        sendQuery(replicateAck->getFilename(), querySeqNum, myAddr, myNameStr);
        querySeqNum++;

    } else {
        //do nothing, source doesn't need us to copy
    }
}

/*
 * Function: selectFile()
 * Description: generates file name string to be requested.
 *  - Param: _fileToRequest: string value of requested file name
 * Returns: string value of file name
 */
std::string OrionApp::selectFile() {
    debug("selectFile", 0);
    bool tabooCheck = false;
    std::string fileToRequest;
    do {
        std::ostringstream convert;
        std::ostringstream convert2;
        fileToRequest = "host[";
        int randomNode = 1 + intrand(numberNodes - 1);
        int maxNumb = (int) (simTime().dbl() / fileGenRate.dbl());
        if (maxNumb < 2)
            maxNumb = 2;
        int randomFile = 1 + intrand(maxNumb - 1);

        convert << randomNode << "]-" << randomFile;
        fileToRequest.append(convert.str());
        //check for already used nodes for this round
        std::list<std::string>::iterator findIter = std::find(tabooList.begin(),
                tabooList.end(), fileToRequest);
        if (findIter != tabooList.end()) {
            tabooCheck = true;
        } else {
            tabooCheck = false;
        }
    } while (tabooCheck);
    tabooList.push_back(fileToRequest);
    return fileToRequest;
}

/*
 * Function: debug()
 * Description: prints out string messages to the console
 *  - Param: msg: string value of msg to output
 * Returns: void
 */
void OrionApp::debug(std::string msg, int level) {
    if (debugEnabled[0] && level == 0) {
        std::string output(simTime().str());
        output.append(" - ");
        output.append(myNameStr);
        output.append(": ");
        output.append(msg);
        std::cout << output << std::endl;
    }
    if (debugEnabled[1] && level == 1) {
        std::string output(simTime().str());
        output.append(" - ");
        output.append(myNameStr);
        output.append(": ");
        output.append(msg);
        std::cout << output << std::endl;
    }
    if (debugEnabled[2] && level == 2) {
        std::string output(simTime().str());
        output.append(" - ");
        output.append(myNameStr);
        output.append(": ");
        output.append(msg);
        std::cout << output << std::endl;
    }
    if (level == 3) {
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
void OrionApp::delaySend(DelayMsg *delayMsg) {

    double ran = uniform(0, 1) / 1000000;
    scheduleAt(simTime() + ran, delayMsg);

}

void OrionApp::sendPacket(OrionPacket * oPacket) {
    if (oPacket->getDST().str().compare("<unspec>") != 0) {
        printPacketSend(oPacket);
        //emit(sentPkSignal, oPacket);
        socket.sendTo(oPacket, oPacket->getDST(), destPort);
        sentOPackets++;
    } else {
        std::ostringstream output;
        output << " Error Sending " << oPacket->getName() << " to "
                << oPacket->getDST().str() << " -ID: " << oPacket->getBid();
        debug(output.str(), 3);
    }
}

void OrionApp::printPacketRec(OrionPacket *oPacket) {
    if (packetTrace) {
        std::ostringstream output;
        output << "                 Received " << oPacket->getName() << " from "
                << oPacket->getLastNodeId();
        output << " - Source: " << oPacket->getSourceId();
        debug(output.str(), 3);
    }
}
void OrionApp::printPacketSend(OrionPacket *oPacket) {
    if (packetTrace) {
        std::ostringstream output;
        output << "Sending " << oPacket->getName() << " to "
                << oPacket->getDST().str() << " -ID: " << oPacket->getBid();
        debug(output.str(), 3);
    }

}
