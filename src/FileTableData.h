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
#include <deque>
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
    }
    virtual ~FileTableData(){

    }
    IPvXAddress getSource(){
        IPvXAddress source =fileList.front();
        return source;

        }
    void removeSource(){
        fileList.pop_front();
    }


    void addSource(IPvXAddress source){
        if(!hasSource(source))
        fileList.push_back(source);
    }

    bool hasSource(IPvXAddress source){
        for (std::deque<IPvXAddress>::iterator it = fileList.begin(); it!=fileList.end(); ++it){
            if(*it==source)
                return true;

        }
        return false ;
    }

    bool hasSource(){
        if(fileList.size() != 0)
           return true;
        return false;
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
    bool transferComplete;


};

#endif /* FILETABLEDATA_H_ */