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

#ifndef FILETABLEDATA_H_
#define FILETABLEDATA_H_

#include <string>
#include <algorithm>
#include <deque>
#include <map>
#include "IPv4Address.h"

class FileTableData {
public:
    FileTableData(){
        fileName ="";
        blocks =0;
    }
    FileTableData(std::string _fileName, int fileSize){
        fileName = _fileName;
        blocks = fileSize;
        remainBlocks = fileSize;
        requeries = 5;
        queryTime = -1;
        queryStop = -1;
        queryStart =-1;
        transferStart = -1;
        transferStop = -1;
        transferTime = -1;
        transferComplete = false;
        blocksReceived = 0;
        totalHops = 0;
        masterQuery = false;
        systemPacketsStart=0;
        systemPacketsStop=0;
        averageBlockTime = 0;

        for( int i(0); i < fileSize; i++){
            blockStatus.insert(std::pair<int, bool>(i, false));
            blockStart.insert(std::pair<int,double>(i,0));
            blockStop.insert(std::pair<int,double>(i,0));
        }
        blockCounter = 0;
    }
    virtual ~FileTableData(){
    }
    IPvXAddress getSource(){
        IPvXAddress source = fileList.front();
        return source;

        }
    void removeSource(){
        //printSources();
        if(fileList.size()>0)
            std::cout << "Removing : " << fileList.front().str() <<std::endl;
            fileList.pop_front();
    }

    void removeSource(IPvXAddress source){
        if(hasSource(source)){
            for (std::deque<IPvXAddress>::iterator it = fileList.begin(); it != fileList.end(); /*NOTE: no incrementation of the iterator here*/) {
              if (*it==source){
                it = fileList.erase(it); // erase returns the next iterator
              }
              else
                ++it; // otherwise increment it by yourself
            }

        }

    }

    void printSources(){
        //std::cout << "count of sources: " << fileList.size() << std::endl;
        for (std::deque<IPvXAddress>::iterator it = fileList.begin(); it!=fileList.end(); ++it){
            std::cout << it->str() << std::endl;
        }
    }

    void addSource(IPvXAddress source){
        if(!hasSource(source)){
           if(source.str().compare("<unspec>") == 0){
                std::cout << "BAD source!!!!!!" << std::endl;
           }else{
            fileList.push_back(source);
           }
        }
    }

    bool hasSource(IPvXAddress source){
        for (std::deque<IPvXAddress>::iterator it = fileList.begin(); it!=fileList.end(); ++it){
            if(*it==source)
                return true;
        }
        return false;
    }

    bool hasSource(){
        if(fileList.size() > 0){
            IPvXAddress source =fileList.front();
           // std::cout <<  "current source: " << source.str() <<std::endl;
           // printSources();
           return true;
        }
        return false;
    }

    int getNextBlock(){
        int tempCounter = blockCounter;
        for(int i(blockCounter); i < blocks; ){
           // std::cout << "tempC:" << tempCounter << "  status of [" << i << "]" << blockStatus[i] << std::endl;
            if(blockStatus[i]){
                i++;
               if(i == blocks){
                   i = 0;
               }
               if(i==(tempCounter))
                return -1;
            }else{
                if(i==blocks-1){
                blockCounter=0;
                }else{
                    blockCounter=i+1;
                }
                return i;
            }
        }
        return -2;
        }
    void setBlockComplete(int block){
        blockStatus[block] = true;
    }

    bool getBlockStatus(int block){
        return blockStatus[block];
    }

    int getBlocks() const {
        return blocks;
    }

    void setBlocks(int blocks) {
        this->blocks = blocks;
    }

    int getRemainBlocks() const {
        return remainBlocks;
    }

    void setRemainBlocks(int remainBlocks) {
        this->remainBlocks = remainBlocks;
    }

    const std::string& getFileName() const {
        return fileName;
    }

    void setFileName(const std::string& fileName) {
        this->fileName = fileName;
    }

    double getQueryStart() const {
        return queryStart;
    }

    void setQueryStart(double queryStart) {
        this->queryStart = queryStart;
    }

    double getQueryStop() const {
        return queryStop;
    }

    void setQueryStop(double _queryStop) {
        this->queryStop = _queryStop;
        queryTime = queryStop-queryStart;
    }

    double getQueryTime() const {
        if(queryStop == -1)
            return queryStop;
        return queryTime;
    }

    void setQueryTime(double queryTime) {
        this->queryTime = queryTime;
    }

    bool isTransferComplete() const {
        return transferComplete;
    }

    void setTransferComplete(bool transferComplete) {
        this->transferComplete = transferComplete;
    }

    double getTransferStart()  {
        return transferStart;
    }

    void setTransferStart(double transferStart) {
        this->transferStart = transferStart;
    }

    double getTransferStop()  {
        return transferStop;
    }

    void setTransferStop(double transferStop) {
        this->transferStop = transferStop;
    }

    double getTransferTime()  {
        if(transferStop == -1)
            return transferStop;
        return (transferStop - transferStart);
    }

    void setTransferTime(double transferTime) {
        this->transferTime = transferTime;
    }

    int getSourceCount(){
        return fileList.size();
    }

    int getRequeries() {
        return requeries;
    }

    void setRequeries(unsigned int requeries) {
        this->requeries = requeries;
    }

    int getBlocksReceived() const {
        return blocksReceived;
    }

    void setBlocksRecieved(int blocksReceived) {
        this->blocksReceived = blocksReceived;
    }
    void incBlocksReceived() {
        blocksReceived++;
    }

    int getTotalHops() const {
        return totalHops;
    }

    void setTotalHops(int totalHops) {
        this->totalHops = totalHops;
    }

    void addHops(int hops){
        totalHops += hops;
    }

    const IPvXAddress& getOrigin() const {
        return origin;
    }

    void setOrigin(const IPvXAddress& origin) {
        this->origin = origin;
    }

    bool isMasterQuery() const {
        return masterQuery;
    }

    void setMasterQuery(bool masterQuery) {
        this->masterQuery = masterQuery;
    }

    double getTimeOfLastBlock() const {
        return timeOfLastBlock;
    }

    void setTimeOfLastBlock(double timeOfLastBlock) {
        this->timeOfLastBlock = timeOfLastBlock;
    }

    void setSystemPacketsStart(int systemPacketsStart) {
        this->systemPacketsStart = systemPacketsStart;
    }

    void setSystemPacketsStop(int systemPacketsStop) {
        this->systemPacketsStop = systemPacketsStop;
    }

    int getTotalPackets(){
       return( systemPacketsStop - systemPacketsStart);
    }

    double getAverageBlockTime() const {
        return averageBlockTime;
    }

    void setBlockStart(const unsigned int block, const double start) {
        blockStart[block] = start;
    }

    void setBlockStop(const unsigned int block, const double stop) {
        blockStart[block] = stop;
        totalBlockTime += (blockStop[block] - blockStart[block]);
        averageBlockTime = totalBlockTime / blocksReceived;
    }

    void setAverageBlockTime(double averageBlockTime) {
        this->averageBlockTime = averageBlockTime;
    }

protected:
    std::deque<IPvXAddress> fileList;
    IPvXAddress origin;
    std::string fileName;
    double queryStart;
    double queryStop;
    double queryTime;

    double transferStart;
    double transferStop;
    double transferTime;

    double timeOfLastBlock;

    int blocks;
    int remainBlocks;
    int blockCounter;
    unsigned int requeries;
    bool transferComplete;
    bool masterQuery;

    std::map<int, bool> blockStatus;
    std::map<unsigned int, double> blockStart;
    std::map<unsigned int, double> blockStop;
    double averageBlockTime;
    double totalBlockTime;
    int blocksReceived;
    int totalHops;

    unsigned int systemPacketsStart;
    unsigned int systemPacketsStop;



};

#endif /* FILETABLEDATA_H_ */
