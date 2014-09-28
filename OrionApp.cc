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

#include "InterfaceTableAccess.h"
#include "IPvXAddressResolver.h"
#include "IPv4ControlInfo.h"
#include "NodeOperations.h"
#include "UDPControlInfo.h"
#include "UDPControlInfo_m.h"


Define_Module(OrionApp);

simsignal_t OrionApp::sentPkSignal = registerSignal("sentPk");
simsignal_t OrionApp::rcvdPkSignal = registerSignal("rcvdPk");

OrionApp::OrionApp()
{
    selfMsg = NULL;
    fileGenMsg = NULL;
    fileRequestMsg = NULL;
    fileNum = 0;
    querySeqNum = 0;
}

OrionApp::~OrionApp()
{
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
        WATCH(numSent);
        WATCH(numReceived);

        localPort = par("localPort");

        master = par("master");
        destPort = par("destPort");
        fileQueryRate = par("fileQueryRate").doubleValue();
        fileGenRate = par("fileGenRate");
        numberNodes = par("numberNodes");
        debugEnabled = par("enableDebug");


        startTime = par("startTime").doubleValue();
        stopTime = par("stopTime").doubleValue();


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
    sprintf(msgName, "UDPBasicAppData-%d", numSent);
    cPacket *payload = new cPacket(msgName);
    payload->setByteLength(par("messageLength").longValue());
    IPvXAddress destAddr = chooseDestAddr();
   // IPvXAddress destAddr = "host[15]";
    emit(sentPkSignal, payload);
    socket.sendTo(payload, destAddr, destPort);
    numSent++;
}

void OrionApp::processStart()
{
    socket.setOutputGate(gate("udpOut"));
    socket.bind(localPort);
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
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
    }


    //orion processes to start
    if(master){ // schedules first file request from system
        simtime_t d = simTime() + par("fileQueryRate").doubleValue() + 1;
        if (stopTime < SIMTIME_ZERO || d < stopTime){
            debug("In processstart()");
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
            debug("In processstart()");
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
    IPvXAddress myAddr = IPvXAddressResolver().resolve(this->getParentModule()->getFullPath().c_str());
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
   //     scheduleAt(d, selfMsg);
    }
    else
    {
        selfMsg->setKind(STOP);
        scheduleAt(stopTime, selfMsg);
    }
}

void OrionApp::processStop()
{
    socket.close();
}

void OrionApp::handleMessageWhenUp(cMessage *msg)
{
 if (msg->isSelfMessage())
    {
    if(msg==fileGenMsg)  {
        debug("handling fileGen");
        generateFile();

    }else if(msg == fileRequestMsg){
        debug("handling requestFile");
        requestFile();
    }else if(msg == selfMsg)
        ASSERT(msg == selfMsg);
        switch (selfMsg->getKind()) {
            case START: processStart(); break;
            case SEND:
              //  processSend();
                break; debug("Sent Query");
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
                   if(std::count(queryList.begin(),queryList.end(),oPacket->getSEQ())==0){
                    std::string debugMsg("Handling Query: ");
                    debugMsg.append(check_and_cast<OrionQueryPacket *>(oPacket)->getFilename());
                    debugMsg.append(" - ");
                    std::ostringstream convertSeqNum;
                    convertSeqNum <<  check_and_cast<OrionQueryPacket *>(oPacket)->getSEQ();
                    debugMsg.append(convertSeqNum.str());
                    debug(debugMsg);
                    sendQuery(check_and_cast<OrionQueryPacket *>(oPacket)->getFilename(), oPacket->getSEQ());
             //       handleRREQ(check_and_cast<AODVRREQ *>(ctrlPacket), sourceAddr, arrivalPacketTTL);
                    }
                    break;

                case RESPONSE:
              //      handleRREP(check_and_cast<AODVRREP *>(ctrlPacket), sourceAddr);
                    break;

                case DATA_REQUEST:
               //     handleRERR(check_and_cast<AODVRERR *>(ctrlPacket), sourceAddr);
                    break;

                case DATA_REPLY:
                //    handleRREPACK(check_and_cast<AODVRREPACK *>(ctrlPacket), sourceAddr);
                    break;

                case DATA_REQUEST_ACK:
               //     handleRREPACK(check_and_cast<AODVRREPACK *>(ctrlPacket), sourceAddr);
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

//handle application level packes
void OrionApp::processPacket(cPacket *pk)
{
    debug("here");
    emit(rcvdPkSignal, pk);

    std::cout<< "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
    numReceived++;
}

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
    std::cout << this->getOwner()->getFullName() << " GENERATED FILE: " << fileName << std::endl;
    simtime_t d = simTime() + par("fileGenRate").doubleValue();
    if (stopTime < SIMTIME_ZERO || d < stopTime)
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
void OrionApp::requestFile(){
    std::string fileToRequest(selectFile());
    if(debugEnabled){
    std::ostringstream debugMessage;
        debugMessage << "Requesting file: " << fileToRequest << std::endl;
    debug(debugMessage.str());
    }
    debug("Initiating Query");

    sendQuery(fileToRequest, querySeqNum);
    querySeqNum++;
    simtime_t d = simTime() + fileQueryRate;
    if (stopTime < SIMTIME_ZERO || d < stopTime)
    {
        scheduleAt(d, fileRequestMsg);
//        fileRequestMsg->setKind(SCHFILEREQUEST);
//        scheduleAt(d, fileRequestMsg);
    }
    else
    {
        fileRequestMsg->setKind(STOP);
        scheduleAt(stopTime, fileRequestMsg);
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
void OrionApp::sendQuery(std::string _fileToRequest, unsigned int seq){
    char msgName[32];
    sprintf(msgName, "testAppData-%d", numSent);


    //  cPacket *testPacket = new cPacket(msgName);
 //  OrionPacket *testPacket = new OrionPacket(msgName);
     OrionQueryPacket *testPacket = new OrionQueryPacket("Orion-Query");

   //test of setting packet type...
     testPacket->setPacketType(QUERY);
     testPacket->setSEQ(seq);
     queryList.push_back (seq);
     testPacket->setFilename(msgName);
     testPacket->setByteLength(par("messageLength").longValue());
  //  IPvXAddress destAddr = chooseDestAddr();
 //   IPvXAddress destAddr;
//    dest.set("255.255.255.255");
 //  IPvXAddress destAddr = "255.255.255.255";
    emit(sentPkSignal, testPacket);

    //disabled to test broadcast
   // socket.sendTo(testPacket, destAddr, destPort);

    //test of sending broadcast;
    IPvXAddress destAddr(IPv4Address::ALLONES_ADDRESS);
    sendBroadcast(destAddr, testPacket);
    numSent++;
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
    int randomNode = 2+ intrand(numberNodes);
    int randomFile = (int) (fileGenRate.dbl() + simTime().dbl()/fileGenRate.dbl());
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
    std::string output(this->getOwner()->getFullName());
    output.append(" --> ");
    output.append(msg);
    std::cout << output << std::endl;
    }
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




//void
//OrionServer::DeleteFileTable(void){
//
//     Ptr<UniformRandomVariable> var = CreateObject<UniformRandomVariable> ();
//     uint32_t ranVal = var->GetValue(uint32_t(0),uint32_t(100));
//     NS_LOG_INFO(Simulator::GetContext() << " " << ranVal);
//     if(ranVal <= m_churnPercent){
//         fileList.clear();
//     NS_LOG_INFO("Cleared File Table" << "Node " << Simulator::GetContext());
//       }
//}

