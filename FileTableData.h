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
    FileTableData(std::string _fileName, IPvXAddress source, int fileSize){
        fileName = _fileName;
        fileList.push_back(source);
        blocks = fileSize;
        remainBlocks = fileSize;
    }
    virtual ~FileTableData(){

    }
    IPvXAddress getNextSource(){
        IPvXAddress source =fileList.front();
        fileList.pop_front();
        return source;

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



protected:
    std::deque<IPvXAddress> fileList;
    std::string fileName;
    int blocks;
    int remainBlocks;


};

#endif /* FILETABLEDATA_H_ */
