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
        queryTime = -1;
        queryStop = -1;
        queryStart =-1;
        transferStart = -1;
        transferStop = -1;
        transferTime = -1;
        transferComplete = false;

        for( int i(0); i < fileSize; i++){
            blockStatus.insert(std::pair<int, bool>(i, false));
        }
        blockCounter = 0;
    }
    virtual ~FileTableData(){

    }
    IPvXAddress getSource(){
        IPvXAddress source =fileList.front();
        return source;

        }
    void removeSource(){
// std::cout << "Removing Source" << std::endl;
//std::cout << "count of sources: " << fileList.size() << std::endl;
        if(fileList.size()>0)
            fileList.pop_front();
//  std::cout << "count of sources: " << fileList.size() << std::endl;
    }

    void removeSource(IPvXAddress source){
        if(hasSource(source)){
            for (std::deque<IPvXAddress>::iterator it = fileList.begin(); it != fileList.end(); /*NOTE: no incrementation of the iterator here*/) {
              if (*it==source){
                it = fileList.erase(it); // erase returns the next iterator
//std::cout << "Deleted: " << source.str() << std::endl;
              }
              else
                ++it; // otherwise increment it by yourself
            }

        }

    }

    void printSources(){
// std::cout << "count of sources: " << fileList.size() << std::endl;
        for (std::deque<IPvXAddress>::iterator it = fileList.begin(); it!=fileList.end(); ++it){
            std::cout << *it << std::endl;
        }
    }

    void addSource(IPvXAddress source){
 //       std::cout << "Adding Source" << std::endl;
        if(!hasSource(source)){
           if(source.str().compare("<unspec>") == true){
                std::cout << "BAD source!!!!!!" << std::endl;
           }else{
  //             std::cout << "count of sources: " << fileList.size() << std::endl;
            fileList.push_back(source);
   //         std::cout << "count of sources: " << fileList.size() << std::endl;
           }
        }
   //     std::cout << "adding: "<< source.str();
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
          //  std::cout <<  "current source: " << source.str() <<std::endl;
           return true;
        }
        return false;
    }

    int getNextBlock(){
        int tempCounter = blockCounter;
        for(int i(blockCounter); i < blocks; i++){
            if(blockStatus[i]){
               i++;
               if(i == blocks){
                   i = 0;
               } else if(i==(tempCounter-1))
                return -1;
            }else{
                blockCounter=i+1;
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

    double getTransferStart() const {
        return transferStart;
    }

    void setTransferStart(double transferStart) {
        this->transferStart = transferStart;
    }

    double getTransferStop() const {
        return transferStop;
    }

    void setTransferStop(double transferStop) {
        this->transferStop = transferStop;
    }

    double getTransferTime() const {
        return transferTime;
    }

    void setTransferTime(double transferTime) {
        this->transferTime = transferTime;
    }

    int getSourceCount(){
        return fileList.size();
    }


protected:
    std::deque<IPvXAddress> fileList;
    std::string fileName;
    double queryStart;
    double queryStop;
    double queryTime;

    double transferStart;
    double transferStop;
    double transferTime;

    int blocks;
    int remainBlocks;
    int blockCounter;
    bool transferComplete;

    std::map<int, bool> blockStatus;


};

#endif /* FILETABLEDATA_H_ */
