/*
 * Report.h
 *
 *  Created on: Nov 28, 2025
 *      Author: helen
 */

#ifndef REPORT_H_
#define REPORT_H_

#include <string>
#include <functional>
#include "report_m.h"

class Report{
public:
    std::string hashValue;
    std::string senderID;
    std::string content;
    long timestamp;
    std::vector<unsigned char> signature;
    std::string publickey;
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
