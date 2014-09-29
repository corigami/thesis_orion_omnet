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
#include "IPv4Address.h"

class FileTableData {
public:
    FileTableData(std::string _fileName){
        fileName = _fileName;
        priority = 0;
    }
    virtual ~FileTableData(){

    }
    const std::string& getFileName() const
    {
        return fileName;
    }

    void setFileName(const std::string& fileName) {
        this->fileName = fileName;
    }

    void setHost(const IPv4Address& host) {
        this->host = host;
    }

    int getPriority() const {
        return priority;
    }

    void setPriority(int priority) {
        this->priority = priority;
    }

    const IPv4Address& getHost() const {
        return host;
    }

protected:
    std::string fileName;
    IPv4Address host;
    int priority;


};

#endif /* FILETABLEDATA_H_ */
