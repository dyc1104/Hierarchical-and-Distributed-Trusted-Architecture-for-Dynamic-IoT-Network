/*
 * device.cc
 *
 *  Created on: Nov 13, 2025
 *      Author: root
 */
#include <omnetpp.h>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <fstream>
#include "Report.h"
#include "message_m.h"
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>

using namespace omnetpp;

class device : public cSimpleModule{
private:
    EVP_PKEY *evpPrivateKey = nullptr;
    BIGNUM *exponent = nullptr;
    int device_id;
    int clusterSize;
    int attackLeft = 0;

    // ===== Device JOIN/LEAVE 统计 =====
    simtime_t joinRequestTime = SIMTIME_ZERO;   // 本device最近一次JoinReq发出时刻
    simtime_t leaveRequestTime = SIMTIME_ZERO;  // 本device最近一次LeaveReq发出时刻

    static std::vector<simtime_t> globalDeviceJoinTimes;
    static std::vector<simtime_t> globalDeviceRejoinTimes;
    static std::vector<simtime_t> globalDeviceLeaveTimes;


    int cpu = 1;
    int memory = 1;
    int IO = 1;
    cMessage *verifyTimer = nullptr;

    bool joined = false;
    int assignedPeerId = -1;

    bool attack = false;

    cMessage *joinTimer = nullptr;

    simtime_t joinT0 = SIMTIME_ZERO;

    cMessage *exitTimer = nullptr;
    bool isRejoinAttempt = false;

    cMessage *attackTimer = nullptr;

    enum SelfKind {
        EVT_JOIN = 1,
        EVT_VERIFY = 2,
        EVT_EXIT = 3,
        EVT_ATTACK = 4,
    };
public:
    RSA *publicKey;

protected:
    virtual void initialize() override{
        generateKeyPair();

        device_id = par("device_id").intValue();
        clusterSize = par("clusterSize").intValue();

        if(device_id == 4)
        {
            attack = true;
        }

        joinTimer = new cMessage("JoinTimer", EVT_JOIN);
        scheduleAt(SimTime(10, SIMTIME_S), joinTimer);   // 0s 发 JoinReq
        // ini 开关：enableVerify / verifyTime
        bool enableVerify = par("enableVerify").boolValue();
        int verifyTimeSec = par("verifyTime").intValue();

        if (enableVerify) {
            verifyTimer = new cMessage("VerifyTimer", EVT_VERIFY);

            simtime_t t1 = SimTime(verifyTimeSec, SIMTIME_S);
            scheduleAt(t1, verifyTimer);   // 例如 10s
            EV_INFO << "Device " << device_id << " scheduled verify at " << t1 << "\n";
        }

        bool enableExit = par("enableExit").boolValue();
        int exitTimeSec = par("exitTime").intValue();

        if (enableExit) {
            exitTimer = new cMessage("ExitTimer", EVT_EXIT);

            simtime_t t2 = SimTime(exitTimeSec, SIMTIME_S);
            scheduleAt(t2, exitTimer);
            EV_INFO << "Device " << device_id << " scheduled exit at " << t2 << "\n";
        }

        int attackTimeSec = 60;

        if(attack){
            attackLeft = 500;
            attackTimer = new cMessage("AttackTimer",EVT_ATTACK);

            simtime_t t3 = SimTime(attackTimeSec,SIMTIME_S);
            scheduleAt(t3,attackTimer);
            EV_INFO << "Device " << device_id << " scheduled attack at " << t3 << "\n";
        }
    }

    inline simtime_t netDelay() {
        return uniform(0.01, 0.012); // 10–12ms
    }

    virtual void handleMessage(cMessage *msg) override {
        if (msg->isSelfMessage() && msg->getKind() == EVT_JOIN) {
            // 生成“加入时要提交的一份报告”（最小：只发 1 份；你也可发 clusterSize 份再打包）
            std::string senderID = "device_" + std::to_string(device_id) + "_0";
            std::string reportContent = "This is a trusted report of " + senderID;

            Report newreport(senderID, reportContent);

            std::vector<unsigned char> signature = signdata(newreport.getHash());
            newreport.signature = signature;
            newreport.publickey = extractPublicKeyPEM();

            // 组装 ReportMessage（嵌入 JoinReq.rm）
            ReportMessage rm;
            rm.setSenderID(newreport.senderID.c_str());
            rm.setContent(newreport.content.c_str());
            rm.setHashValue(newreport.hashValue.c_str());
            rm.setTimestamp(newreport.timestamp);
            rm.setSenderpublickey(newreport.publickey.c_str());
            rm.setSignature(binToHex(newreport.signature).c_str());

            isRejoinAttempt = false;
            auto *req = new JoinReq("JoinReq");
            req->setDeviceId(device_id);
            req->setRm(rm);
            joinRequestTime = simTime();
            req->setSendTime(joinRequestTime);
            req->setCpu(cpu);
            req->setMemory(memory);
            req->setIO(IO);

            cModule *mgr = getParentModule()->getSubmodule("topoMgr");
            if (!mgr) {
                EV_ERROR << "Device: topoMgr not found\n";
                delete req;
                delete msg;
                return;
            }

            SendOptions opt;
            opt.propagationDelay(netDelay());
            EV_INFO << "Device[" << device_id << "]: send JoinReq at " << simTime() << "\n";
            sendDirect(req, opt, mgr->gate("join"));

            delete msg;
            joinTimer = nullptr;
            return;
        }

        if (auto *ack = dynamic_cast<JoinAck*>(msg)) {
            if (ack->getDeviceId() == device_id) {
                joined = ack->getSuccess();
                assignedPeerId = ack->getPeerId();

                if (joined) {
                    simtime_t dt = simTime() - joinRequestTime;

                    if (isRejoinAttempt) {
                        globalDeviceRejoinTimes.push_back(dt);
                        EV_WARN << "Device[" << device_id << "] REJOIN time=" << dt << "\n";
                    } else {
                        globalDeviceJoinTimes.push_back(dt);
                        EV_INFO << "Device[" << device_id << "] JOIN time=" << dt << "\n";
                    }
                }

                isRejoinAttempt = false;

                EV_INFO << "Device[" << device_id << "] got JoinAck success=" << joined
                        << " at " << ack->getTime() << "\n";
            }
            delete ack;
            return;
        }

        if (auto *rj = dynamic_cast<ReJoin*>(msg)) {
            int did = rj->getDeviceId();
            if (did != device_id) {  // 保险：只处理发给自己的
                delete rj;
                return;
            }

            EV_WARN << "Device[" << device_id << "] got ReJoin at " << simTime()
                    << ", will re-send JoinReq\n";

            // 重新构造 JoinReq（复用你原来 EVT_JOIN 的构造逻辑）
            std::string senderID = "device_" + std::to_string(device_id) + "_0";
            std::string reportContent = "This is a trusted report of " + senderID;

            Report newreport(senderID, reportContent);


            std::vector<unsigned char> signature = signdata(newreport.getHash());
            newreport.signature = signature;
            newreport.publickey = extractPublicKeyPEM();

            ReportMessage rm;
            rm.setSenderID(newreport.senderID.c_str());
            rm.setContent(newreport.content.c_str());
            rm.setHashValue(newreport.hashValue.c_str());
            rm.setTimestamp(newreport.timestamp);
            rm.setSenderpublickey(newreport.publickey.c_str());
            rm.setSignature(binToHex(newreport.signature).c_str());

            auto *req = new JoinReq("JoinReq");
            req->setDeviceId(device_id);
            req->setRm(rm);
            isRejoinAttempt = true;
            joinRequestTime = simTime();
            req->setSendTime(joinRequestTime);
            req->setCpu(cpu);
            req->setMemory(memory);
            req->setIO(IO);

            cModule *mgr = getParentModule()->getSubmodule("topoMgr");
            if (!mgr) {
                EV_ERROR << "Device: topoMgr not found\n";
                delete req;
                delete rj;
                return;
            }

            SendOptions opt;
            opt.propagationDelay(netDelay());
            sendDirect(req, opt, mgr->gate("join"));

            delete rj;
            return;
        }


        // 只在 10s（或 ini 指定时间）发送验证报告
        if (msg->isSelfMessage() && msg->getKind() == EVT_VERIFY) {
            if (!joined) { delete msg; verifyTimer=nullptr; return; }
            for (int x = 0; x < clusterSize; ++x) {
                std::string senderID = "device_" + std::to_string(device_id) + "_" + std::to_string(x);
                std::string reportContent = "This is a trusted report of " + senderID;

                Report newreport(senderID, reportContent);
                EV << "Generated report sender=" << senderID
                   << " hash=" << newreport.getHash() << endl;

                std::vector<unsigned char> signature = signdata(newreport.getHash());
                newreport.signature = signature;

                std::string publickey = extractPublicKeyPEM();
                newreport.publickey = publickey;

                sendReport(newreport);
            }

            delete msg;
            verifyTimer = nullptr;
            return;
        }

        if (msg->isSelfMessage() && msg->getKind() == EVT_EXIT) {
            if (!joined) { delete msg; exitTimer=nullptr; return; }

            leaveRequestTime = simTime();

            // 组装退出请求
            for (int x = 0; x < clusterSize; ++x){
                std::string senderID = "device_" + std::to_string(device_id) + "_"+std::to_string(x);

                auto *lreq = new LeaveReq("LeaveReq");
                lreq->setSenderID(senderID.c_str());
                lreq->setDeviceId(device_id);
                lreq->setSendTime(leaveRequestTime);

                EV_INFO << "Device[" << device_id << "] send LeaveReq via out[0] at "
                        << simTime() << "\n";

                cModule *peerMod = getParentModule()->getSubmodule("peer", assignedPeerId);
                SendOptions opt;
                opt.propagationDelay(netDelay());

                sendDirect(lreq, opt, peerMod->gate("rein"));
            }

            delete msg;
            exitTimer = nullptr;
            return;
        }

        if (msg->isSelfMessage() && msg->getKind() == EVT_ATTACK) {

            if (attackLeft <= 0) {
                delete msg;
                attackTimer = nullptr;
                return;
            }

            cModule *mgr = getParentModule()->getSubmodule("topoMgr");
            if (!mgr) {
                EV_ERROR << "Device: topoMgr not found\n";
                delete msg;
                attackTimer = nullptr;
                return;
            }

            generateKeyPair();

            std::string senderID = "device_9_0";
            std::string reportContent = "This is a trusted report of " + senderID;

            Report newreport(senderID, reportContent);
            newreport.signature = signdata(newreport.getHash());
            newreport.publickey = extractPublicKeyPEM();

            ReportMessage rm;
            rm.setSenderID(newreport.senderID.c_str());
            rm.setContent(newreport.content.c_str());
            rm.setHashValue(newreport.hashValue.c_str());
            rm.setTimestamp(newreport.timestamp);
            rm.setSenderpublickey(newreport.publickey.c_str());
            rm.setSignature(binToHex(newreport.signature).c_str());

            auto *req = new JoinReq(("JoinReq_attack_" + std::to_string(attackLeft)).c_str());
            req->setDeviceId(device_id);
            req->setRm(rm);
            req->setSendTime(simTime());
            req->setCpu(cpu);
            req->setMemory(memory);
            req->setIO(IO);

            SendOptions opt;
            opt.propagationDelay(netDelay());
            sendDirect(req, opt, mgr->gate("join"));

            attackLeft--;

            // 下一次攻击包（例如每 1ms 发一次，或者 uniform(0, 1ms)）
            scheduleAt(simTime() + SimTime(20, SIMTIME_MS), msg);
            return;
        }



        if (auto *lack = dynamic_cast<LeaveAck*>(msg)) {
            if (lack->getDeviceId() == device_id) {
                if (leaveRequestTime != SIMTIME_ZERO) {
                    simtime_t dt = lack->getAckTime() - leaveRequestTime; // req -> ack
                    globalDeviceLeaveTimes.push_back(dt);
                    EV_WARN << "Device[" << device_id << "] LEAVE time(req->ack)=" << dt << "\n";
                    leaveRequestTime = SIMTIME_ZERO; // ✅ 只统计一次
                }

                EV_INFO << "Device[" << device_id << "] got LeaveAck at "
                        << lack->getAckTime() << " sender=" << lack->getSenderID() << "\n";
                joined = false;
            }
            delete lack;
            return;
        }

        // 其他外来消息（你当前没处理逻辑，就丢弃）
        delete msg;
    }

    void generateKeyPair() {
        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        if (!ctx) {
            EV_ERROR << "Failed to create EVP_PKEY context!\n";
            ERR_print_errors_fp(stderr);
            return;
        }
        // 初始化密钥生成参数（RSA 2048 位，公钥指数 65537）
        if (EVP_PKEY_keygen_init(ctx) <= 0 ||
            EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) {
            EV_ERROR << "Failed to initialize RSA key generation parameters!\n";
            ERR_print_errors_fp(stderr);
            EVP_PKEY_CTX_free(ctx);
            return;
         }
         // 生成 RSA 密钥对
         if (EVP_PKEY_keygen(ctx, &evpPrivateKey) <= 0) {
             EV_ERROR << "RSA key generation failed!\n";
             ERR_print_errors_fp(stderr);
             EVP_PKEY_CTX_free(ctx);
             return;
          }
          EVP_PKEY_CTX_free(ctx);
          EV << "RSA key pair generated successfully. Key size: " << EVP_PKEY_size(evpPrivateKey) << " bytes\n";
              //finish();
    }

    virtual void finish() override {
        // 释放 timer（如果还没触发）
        cancelAndDelete(joinTimer);
        joinTimer = nullptr;
        cancelAndDelete(verifyTimer);
        verifyTimer = nullptr;

        // 释放 OpenSSL 资源（你原来的 finish() 不是 override，这里修正成 OMNeT++ 会调用的 finish）
        if (evpPrivateKey) EVP_PKEY_free(evpPrivateKey);
        evpPrivateKey = nullptr;

        EVP_cleanup();
        ERR_free_strings();

        if (getIndex() != getParentModule()->getSubmoduleVectorSize("dev") - 1)
            return;

        std::ofstream out("device_join_leave_time.txt", std::ios::app);
        if (!out.is_open()) return;

        if (!globalDeviceJoinTimes.empty()) {
            simtime_t sum = SIMTIME_ZERO;
            for (auto &t : globalDeviceJoinTimes) sum += t;
            simtime_t avg = sum / globalDeviceJoinTimes.size();

            out << "DEV_JOIN"
                << " count=" << globalDeviceJoinTimes.size()
                << " avg=" << avg << "\n";
        }

        if (!globalDeviceRejoinTimes.empty()) {
            simtime_t sum = SIMTIME_ZERO;
            for (auto &t : globalDeviceRejoinTimes) sum += t;
            simtime_t avg = sum / globalDeviceRejoinTimes.size();

            out << "DEV_REJOIN"
                << " count=" << globalDeviceRejoinTimes.size()
                << " avg=" << avg << "\n";
        }

        if (!globalDeviceLeaveTimes.empty()) {
            simtime_t sum = SIMTIME_ZERO;
            for (auto &t : globalDeviceLeaveTimes) sum += t;
            simtime_t avg = sum / globalDeviceLeaveTimes.size();

            out << "DEV_LEAVE"
                << " count=" << globalDeviceLeaveTimes.size()
                << " avg=" << avg << "\n";
        }

        out.close();
    }

    std::vector<unsigned char> signdata(const std::string& hashValue){
        if (!evpPrivateKey || hashValue.empty()) {
                EV_ERROR << "SignData failed: invalid private key or empty hash value\n";
                return {};
            }
        // 检查 EVP_PKEY 是否为 RSA 类型
        if (EVP_PKEY_id(evpPrivateKey) != EVP_PKEY_RSA) {
            EV_ERROR << "SignData failed: private key is not an RSA key\n";
            return {};
        }
        RSA* rsaPrivateKey = EVP_PKEY_get1_RSA(evpPrivateKey);
           if (!rsaPrivateKey) {
               EV_ERROR << "SignData failed: could not extract RSA key from EVP_PKEY\n";
               ERR_print_errors_fp(stderr);
               return {};
           }
         unsigned int sigLen = RSA_size(rsaPrivateKey);
             std::vector<unsigned char> signature(sigLen);
             if (RSA_sign(NID_sha256,
                             (unsigned char*)hashValue.c_str(),
                             hashValue.length(),
                             signature.data(),
                             &sigLen,
                             rsaPrivateKey) != 1) {
                    EV_ERROR << "SignData failed: RSA_sign failed\n";
                    ERR_print_errors_fp(stderr);
                    RSA_free(rsaPrivateKey); // 释放 RSA 结构体
                    return {};
                }
         // 释放从 EVP_PKEY 中提取的 RSA 结构体
         RSA_free(rsaPrivateKey);
         // 调整签名向量的大小以匹配实际生成的签名长度
         signature.resize(sigLen);
         EV_INFO << "SignData: Successfully signed the hash value. Signature size: " << signature.size() << " bytes.\n";
         EV_INFO << "Generated Signature (hex): ";
         for (unsigned char c : signature) {
             EV_INFO << std::hex << std::setw(2) << std::setfill('0') << (int)c;
         }
         EV_INFO << std::dec << endl;
         return signature;
    }

    std::string extractPublicKeyPEM() {
            if (!evpPrivateKey) {
                EV_ERROR << "extractPublicKeyPEM: evpPrivateKey is nullptr. Key pair not generated.\n";
                return "";
            }

            // 使用 BIO (Basic I/O) 对象在内存中操作字符串
            BIO *bio = BIO_new(BIO_s_mem());
            if (!bio) {
                EV_ERROR << "extractPublicKeyPEM: Failed to create BIO object.\n";
                ERR_print_errors_fp(stderr);
                return "";
            }

            // 将公钥从 EVP_PKEY 对象写入到 BIO 中，格式为 PEM
            if (PEM_write_bio_PUBKEY(bio, evpPrivateKey) != 1) {
                EV_ERROR << "extractPublicKeyPEM: Failed to write public key to BIO.\n";
                ERR_print_errors_fp(stderr);
                BIO_free(bio); // 释放已分配的 BIO
                return "";
            }

            // 从 BIO 中获取内存中的字符串指针和长度
            BUF_MEM *bptr;
            BIO_get_mem_ptr(bio, &bptr);

            // 从指针和长度创建一个 std::string
            std::string publicKeyPEM(bptr->data, bptr->length);

            // 释放 BIO 对象。注意，BIO_free 会自动释放其内部的 BUF_MEM 结构
            BIO_free(bio);

            EV_INFO << "Successfully extracted public key in PEM format.\n";
            // EV_INFO << "Public Key PEM:\n" << publicKeyPEM << "\n"; // 如果想在日志中查看公钥

            return publicKeyPEM;
        }

    void sendReport(Report& reportToSend) {
        ReportMessage *msg = new ReportMessage("ReportMsg");
        msg->setSenderID(reportToSend.senderID.c_str());
        msg->setContent(reportToSend.content.c_str());
        msg->setHashValue(reportToSend.hashValue.c_str());
        msg->setTimestamp(reportToSend.timestamp);
        msg->setSenderpublickey(reportToSend.publickey.c_str());
        if (!reportToSend.signature.empty()) {
           // 将 std::vector<unsigned char> 转换为十六进制 std::string
           std::string signatureStr = binToHex(reportToSend.signature);
           // 将字符串签名设置到消息中
           msg->setSignature(signatureStr.c_str());
           EV_INFO << "Signature added to message as hex string. Original size: "
                   << reportToSend.signature.size() << " bytes, String size: " << signatureStr.size() << " bytes.\n";
        }
        else {
           EV_WARN << "Report signature is empty, not adding to message.\n";
           // 可以选择设置一个空字符串或不设置
           msg->setSignature("");
        }
        EV << "Sending report from" << reportToSend.senderID << endl;
        //send(msg, "out", 0);

        cModule *peerMod = getParentModule()->getSubmodule("peer", assignedPeerId);
        SendOptions opt;
        opt.propagationDelay(netDelay());

        sendDirect(msg, opt, peerMod->gate("rein"));
    }
    std::string binToHex(const std::vector<unsigned char>& binData) {
            std::stringstream ss;
            ss << std::hex << std::setfill('0');
            for (unsigned char c : binData) {
                ss << std::setw(2) << static_cast<int>(c);
            }
            return ss.str();
        }
};
Define_Module(device);

std::vector<simtime_t> device::globalDeviceJoinTimes;
std::vector<simtime_t> device::globalDeviceLeaveTimes;
std::vector<simtime_t> device::globalDeviceRejoinTimes;
