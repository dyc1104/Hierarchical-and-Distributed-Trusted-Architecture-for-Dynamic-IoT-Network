/*
 * center.cc
 *
 *  Created on: Nov 28, 2025
 *      Author: helen
 */
#include <numeric>
#include <omnetpp.h>
#include <ctime>
#include <chrono>
#include <string>
#include <vector>
#include <atomic>
#include <queue>
#include <sstream>
#include <stdexcept> // 用于抛出异常
#include <fstream>  // 用于将结果写入文件（可选）
#include <algorithm> // 用于计算平均延迟
#include <iomanip>
#include <Report.h>
#include "report_m.h"
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>

using namespace omnetpp;

class VerifyEvent : public cMessage {
public:
    ReportMessage reportMsg; // 存储报告副本（直接用 ReportMessage 的拷贝构造）
    simtime_t startTime; // RSA 实测耗时

    VerifyEvent(const char* name, const ReportMessage& msg, simtime_t st) : cMessage(name), reportMsg(msg), startTime(st) {}
};

struct ReportVerification {
    Report report;                  // 第一个元素：Report类对象
    bool isSignatureValid;             // 第二个元素：报告来源验证结果
    bool isHashValid;          // 第三个元素：报告完整性验证结果
    bool isTimestampValid;          // 第四个元素：报告时间戳验证结果
};

class Center: public cSimpleModule {
private:
    static simtime_t globalFirstStartTime;   // 全局首份报告开始验证时间
    static simtime_t globalLastFinishTime;   // 全局末份报告完成验证时间
    static int globalTotalReports;           // 全局总验证报告数
    static std::vector<simtime_t> globalDelays; // 全局单报告延迟列表
    int maxParallel = 1; //最大并行验证数
    int busy = 0; //当前正在验证的数量
    std::queue<ReportMessage*> waitQ; //等待队列
public:
    std::vector<ReportVerification> verificationResults;
    virtual void initialize() override {
            //globalFirstStartTime = SIMTIME_ZERO;
            //globalLastFinishTime = SIMTIME_ZERO;
            //globalTotalReports = 0;
            //globalDelays.clear();
        verificationResults.clear();

    }

    virtual void handleMessage(cMessage *msg) override {
            if (msg->isSelfMessage()) {
                VerifyEvent* doneEvt = check_and_cast<VerifyEvent*>(msg);

                // 用 doneEvt->startTime 当作真正开始时间
                processReceivedReport(doneEvt->reportMsg, doneEvt->startTime);

                // 一个验证完成，释放并行槽
                busy--;

                // 如果队列里还有等待的，就启动下一条
                if (!waitQ.empty()) {
                    ReportMessage *next = waitQ.front();
                    waitQ.pop();
                    startVerify(next);
                }

                delete doneEvt;
            } else {
                ReportMessage *reportMsg = dynamic_cast<ReportMessage *>(msg);
                EV << "Received report from " << reportMsg->getSenderID() << ": " << reportMsg->getContent() << endl;
                if (reportMsg) {
                    // 拷贝一份放到自己的控制里（因为原 msg 会被 delete）
                    ReportMessage *copy = new ReportMessage(*reportMsg);

                    if (busy < maxParallel) {
                        // 还有空闲并行槽：立刻开始验证
                        busy++;
                        startVerify(copy);
                    } else {
                        // 没有空闲槽：排队
                        waitQ.push(copy);
                    }
                }
                delete reportMsg;   // 原始到达消息删掉
            }
        }

    void startVerify(ReportMessage *rm)
    {
        // 1) 测量/指定这条报告的验证耗时
        simtime_t delay = measureRsaRuntime(*rm);

        // 2) 记录开始时间（重要！）
        simtime_t st = simTime();

        // 3) 创建“验证完成事件”
        VerifyEvent *doneEvt = new VerifyEvent("verifyDone", *rm, st);

        // 5) 在 st + delay 时刻触发完成
        scheduleAt(st + delay, doneEvt);

        // rm 是队列/到达时 new 出来的指针，这里用完删掉
        delete rm;
    }

    simtime_t measureRsaRuntime(const ReportMessage& reportMsg) {
        // 取出 RSA 验证需要的字段
        std::string senderPub = reportMsg.getSenderpublickey();
        std::string sig       = reportMsg.getSignature();
        std::string hashvalue = reportMsg.getHashValue();

        std::vector<unsigned char> binData = hexToBin(sig);

        // 用 chrono 计时
        auto start = std::chrono::high_resolution_clock::now();

        // 实际跑一次 RSA 验证（这次只是为了测时间）
        bool ok = verifySignature(hashvalue, binData, senderPub);
        (void)ok; // 如果你不想用这个结果就忽略掉

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;  // 单位：秒

        double seconds = diff.count();

        // 你可以选择加一个缩放因子，比如仿真中只取真实时间的 1/10：
        // seconds *= 0.1;

        return simtime_t(seconds);  // 转成 simtime_t
    }

    void processReceivedReport(ReportMessage& reportMsg, simtime_t reportStartTime) {
        EV << " 开始验证报告 " << reportMsg.getSenderID() << "，开始时间：" << reportStartTime << "\n";
            //EV << "Received report from " << reportMsg.getSenderID() << "\n";
            EV << "Content: " << reportMsg.getContent() << "\n";
            EV << "Hash: " << reportMsg.getHashValue() << "\n";
            EV << "Timestamp: " << reportMsg.getTimestamp() << "\n";
            EV << "signature: " << reportMsg.getSignature() << "\n";
            EV << "publickey: " << reportMsg.getSenderpublickey() << "\n";
            std::string sender_id = reportMsg.getSenderID();
            std::string sender_con = reportMsg.getContent();
            long sender_time = reportMsg.getTimestamp();
            std::string senderPub = reportMsg.getSenderpublickey();
            std::string sig= reportMsg.getSignature();
            std::string hashvalue = reportMsg.getHashValue();
            std::vector<unsigned char> binData = hexToBin(sig);
            EV << "转换后的二进制（十六进制显示）：";
            std::ostringstream oss;
            for (unsigned char c : binData) {
                oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
            }
            EV << oss.str() << "\n";

            bool isSignatureValid = verifySignature(hashvalue, binData, senderPub);
            if (isSignatureValid) {
                EV << " correct Report verification at signature check!" << "\n";
            }
            bool isHashValid = VerifyHash(sender_id, sender_con, sender_time, hashvalue);
            //if (isHashValid == hashvalue){
               // bool ishashvalid = 1;
               // EV << " correct Report verification at hashvalue check!" << "\n";
           // } else {
               //bool ishashvalid = 0;
            //}
            bool isTimestampValid = processReceivedReportTime(sender_time);
            // 记录当前报告的完成验证时间，更新统计
            simtime_t reportFinishTime = simTime();
            simtime_t reportDelay = reportFinishTime - reportStartTime;
            EV << " 完成验证报告 " << reportMsg.getSenderID() << "，完成时间：" << reportFinishTime << "，延迟：" << reportDelay << "\n";
            // 更新全局统计（线程安全，OMNeT++模块事件串行执行）
            globalTotalReports++;
            EV << "baogao_num:" << globalTotalReports <<"\n";
            globalDelays.push_back(reportDelay);

            // 更新全局首份报告开始时间（取最早的开始时间）
            if (globalFirstStartTime == SIMTIME_ZERO || reportStartTime < globalFirstStartTime) {
                globalFirstStartTime = reportStartTime;
            }

            // 更新全局末份报告完成时间（取最晚的完成时间）
            if (reportFinishTime > globalLastFinishTime) {
                globalLastFinishTime = reportFinishTime;
            }
            if(isSignatureValid && isHashValid && isTimestampValid){
                Report newReport;
                newReport.senderID = sender_id;
                newReport.content = sender_con;
                newReport.timestamp = sender_time;
                newReport.hashValue = hashvalue;
                newReport.signature = binData;
                newReport.publickey = senderPub;
                ReportVerification rv;
                rv.report = newReport;
                rv.isSignatureValid = isSignatureValid;
                rv.isHashValid = isHashValid;
                rv.isTimestampValid = isTimestampValid;
                verificationResults.push_back(rv);
            }
            //EV << isSignatureValid << isHashValid << isTimestampValid << endl;
    }
    std::vector<unsigned char> hexToBin(const std::string& hexString) {
        // 检查输入字符串长度是否为偶数，因为每两个十六进制字符代表一个字节
        if (hexString.size() % 2 != 0) {
            throw std::invalid_argument("hexToBin: Hex string has an odd length.");
        }

        std::vector<unsigned char> binData;
        binData.reserve(hexString.size() / 2); // 预分配内存，提高效率

        std::stringstream ss(hexString);
        std::string byteString;

        // 每次从字符串流中读取两个字符
        while (ss >> std::setw(2) >> byteString) {
            try {
                // 将两个字符的十六进制字符串转换为一个整数
                unsigned int byteValue = std::stoul(byteString, nullptr, 16);
                // 将整数转换为 unsigned char 并添加到二进制数据向量中
                binData.push_back(static_cast<unsigned char>(byteValue));
            } catch (const std::invalid_argument& e) {
                // 如果遇到无法转换的非法字符，抛出异常
                throw std::invalid_argument("hexToBin: Invalid hex character found in string.");
            } catch (const std::out_of_range& e) {
                // 如果值超出了 unsigned char 的范围（虽然对于两个十六进制字符来说不可能）
                throw std::out_of_range("hexToBin: Value out of range for unsigned char.");
            }
        }
        return binData;
    }
    bool verifySignature(const std::string& plainText, const std::vector<unsigned char>& signature, const std::string& publicKeyPEM) {
        BIO *bio = BIO_new_mem_buf(publicKeyPEM.c_str(), -1);
           if (!bio) {
               EV_ERROR << "BIO_new_mem_buf failed for public key.\n";
               return false;
           }

           // 使用 PEM_read_bio_RSA_PUBKEY 来直接加载 RSA 公钥
           RSA *rsaPubKey = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
           BIO_free(bio);

           if (!rsaPubKey) {
               EV_ERROR << "PEM_read_bio_RSA_PUBKEY failed. Invalid PEM format or not an RSA key.\n";
               ERR_print_errors_fp(stderr);
               return false;
           }

           // 2. 执行 RSA 验证
           // RSA_verify 会自动对 plainText 进行 SHA-256 哈希，然后与解密后的签名进行比较
           int verifyResult = RSA_verify(
               NID_sha256,                          // 哈希算法
               (const unsigned char*)plainText.c_str(), // 原始数据（待哈希）
               plainText.length(),                  // 原始数据长度
               signature.data(),                    // 签名
               signature.size(),                    // 签名长度
               rsaPubKey                            // RSA 公钥
           );

           // 3. 清理
           RSA_free(rsaPubKey);

           // 4. 检查结果
           if (verifyResult == 1) {
               EV_INFO << "RSA Signature verified successfully.\n";
               return true;
           } else if (verifyResult == 0) {
               EV_ERROR << "RSA Signature verification failed: Invalid signature or hash.\n";
               return false;
           } else { // verifyResult < 0
               EV_ERROR << "RSA Signature verification error.\n";
               ERR_print_errors_fp(stderr);
               return false;
           }
       }
    bool VerifyHash(const std::string& senderID, const std::string& content, const long& timestamp, const std::string& plaintext){
        bool ishashvalid;
        std::string data =senderID + content + std::to_string(timestamp);
        std::size_t stringHash = std::hash<std::string>{}(data);
        std::string hashvalue = std::to_string(stringHash);
        if (plaintext == hashvalue){
            ishashvalid = 1;
            EV << " correct Report verification at hashvalue check!" << "\n";
            } else {
                ishashvalid = 0;
            }
        return ishashvalid;
    }
    bool processReceivedReportTime(const long sender_time) {
        bool istimestampvalid;
        // --- 时间戳验证 ---
        // 1. 获取当前时间 (推荐使用 std::chrono 获得更高精度)
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        time_t currentTime = static_cast<time_t>(now_time_t);

        // 2. 提取报告中的时间戳
        //time_t reportTimestamp = reportMsg.getTimestamp();

        // 3. 计算时间差 (单位：秒)
        double timeDifference = difftime(currentTime, sender_time);

        // 4. 定义合理的时间窗口 (例如：报告不能早于10分钟，也不能晚于1分钟)
        const double MAX_AGE_SECONDS = 10 * 60;    // 10分钟
        const double MAX_FUTURE_SECONDS = 60;      // 1分钟

        EV_INFO << "Time difference for report: " << timeDifference << " seconds.\n";

        if (timeDifference < -MAX_FUTURE_SECONDS) {
            EV_ERROR << "Report verification failed: Timestamp is too far in the future.\n";
            istimestampvalid =0;
            //return; // 拒绝该报告
        }

        if (timeDifference > MAX_AGE_SECONDS) {
            EV_ERROR << "Report verification failed: Timestamp is too old. Possible replay attack.\n";
            istimestampvalid = 0;
            //return; // 拒绝该报告
        }
        istimestampvalid = 1;
        EV_INFO << "Timestamp verification passed.\n";
        return istimestampvalid;
    }

    void finish(){
        // 仅最后一个Peer输出统计（getSubmoduleVectorSize()获取Peer总数）
            EV << "\n========================================" << endl;
            EV << "分层区块链网络 - 验证性能统计" << endl;
            EV << "========================================" << endl;
            EV << "总验证报告数：" << globalTotalReports << endl;
            EV << "首份报告开始时间：" << globalFirstStartTime << endl;
            EV << "末份报告完成时间：" << globalLastFinishTime << endl;

            // 计算核心指标
            simtime_t totalVerifyTime = (globalFirstStartTime == SIMTIME_ZERO) ? SIMTIME_ZERO : (globalLastFinishTime - globalFirstStartTime);
            simtime_t avgDelay = (globalDelays.empty()) ? SIMTIME_ZERO : (std::accumulate(globalDelays.begin(), globalDelays.end(), SIMTIME_ZERO) / globalDelays.size());
            double throughput = (totalVerifyTime == SIMTIME_ZERO) ? 0.0 : (globalTotalReports / totalVerifyTime.dbl());

            EV << "总验证时间：" << totalVerifyTime << endl;
            EV << "平均验证延迟：" << avgDelay << endl;
            EV << "吞吐量：" << throughput << " 报告/秒" << endl;
            EV << "========================================\n" << endl;

            // 可选：将结果写入文件，方便与中心化网络对比
            std::ofstream outFile("centralized_performance.txt");
            outFile << "总验证报告数：" << globalTotalReports << endl;
            outFile << "总验证时间：" << totalVerifyTime << endl;
            outFile << "平均验证延迟：" << avgDelay << endl;
            outFile << "吞吐量：" << throughput << endl;
            outFile.close();
    }
 };
Define_Module(Center);

simtime_t Center::globalFirstStartTime = SIMTIME_ZERO;
simtime_t Center::globalLastFinishTime = SIMTIME_ZERO;
int Center::globalTotalReports = 0;
std::vector<simtime_t> Center::globalDelays;


