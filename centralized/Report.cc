/*
 * Report.cc
 *
 *  Created on: Nov 28, 2025
 *      Author: helen
 */
#include <omnetpp.h>
#include <string.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "Report.h"
#include "report_m.h"

using namespace omnetpp;
Report::Report(const std::string& sender, const std::string& reportContent) : senderID(sender), content(reportContent), timestamp(time(nullptr)) {
    calculateHash();
}

void Report::calculateHash(){
    std::string data =senderID + content + std::to_string(timestamp);
    std::size_t stringHash = std::hash<std::string>{}(data);
    hashValue = std::to_string(stringHash);
    //EV << hashValue << endl;
}

std::string Report::getHash() const{
    EV << hashValue << endl;
    return hashValue;
}

std::string Report::getSenderID() const{
    return senderID;
}

std::string Report::getContent() const{
    return content;
}

long Report::getTimestamp() const{
    return timestamp;
}

std::string Report::getSignature() const{
    std::stringstream ss;
        // 设置输出为十六进制格式，并设置填充字符为 '0'
    ss << std::hex << std::setfill('0');

        // 遍历 vector<unsigned char> 中的每个字节
    for (unsigned char byte : signature) {
        // 将每个字节以两位十六进制数的形式写入 stringstream
        ss << std::setw(2) << static_cast<int>(byte);
    }
    // 返回构建好的十六进制字符串
    return ss.str();
}
//Report toReports() const {
        //return Report(senderID, reportContent);
    //}

std::string Report::toString() const {
        return "hash: " + hashValue + ", Sender: " + senderID + ", Content: " + content + ", Timestamp: " + std::to_string(timestamp);
    }
//Report fromReportMessage(const omnetpp::ReportMessage& reportMsg) const{
    //return Reports(
        //reportMsg.senderID(),    // 使用 protobuf 生成的访问器
        //reportMsg.content(),
        //reportMsg.hashValue(),
        //reportMsg.timestamp()
    //);
//}



