/*
 * Report.h
 *
 *  Created on: Nov 13, 2025
 *      Author: root
 */

#ifndef REPORT_H_
#define REPORT_H_

#include <string>
#include <cstdint>
#include <functional>
#include "message_m.h"

enum class NodeState : uint8_t {
    NEW = 0,        // 初始/未加入
    JOINED = 1,     // 已加入（设备层/控制层之一）
    LEFT = 2,       // 已退出
    EXPIRED = 3     // 已过期（TTL/心跳超时）
};

class Report{
public:
    std::string hashValue;
    std::string senderID;
    std::string content;
    long timestamp;
    std::vector<unsigned char> signature;
    std::string publickey;
    NodeState state = NodeState::NEW;
public:
    Report() {};
    Report(const std::string& sender, const std::string& reportContent);
    void calculateHash();
    std::string getHash() const;
    std::string getSenderID() const;
    std::string getContent() const;
    long getTimestamp() const;
    std::string getSignature() const;
    Report toReports() const;
    std::string toString() const;
    //Report fromReportMessage(const omnetpp::ReportMessage& reportMsg)const;
};



#endif /* REPORT_H_ */
