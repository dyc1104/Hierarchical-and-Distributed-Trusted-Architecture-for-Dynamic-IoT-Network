/*
 * peer.cc
 *
 *  Created on: Nov 13, 2025
 *      Author: root
 */
#include <numeric>
#include <omnetpp.h>
#include <ctime>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <sstream>
#include <stdexcept> // 用于抛出异常
#include <fstream>  // 用于将结果写入文件（可选）
#include <algorithm> // 用于计算平均延迟
#include <iomanip>
#include "Block.h"
#include "Ledger.h"
#include "Report.h"
#include "message_m.h"
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

class JoinVerifyEvent : public cMessage {
public:
    int deviceId;
    int assignedPeerId;
    ReportMessage reportMsg;
    simtime_t joinSendTime;

    JoinVerifyEvent(const char* name, int devId, int peerId,
                    const ReportMessage& msg, simtime_t st)
        : cMessage(name), deviceId(devId), assignedPeerId(peerId),
          reportMsg(msg), joinSendTime(st) {}
};

class PeerJoinVerifyEvent : public cMessage {
public:
    int peerId;
    ReportMessage reportMsg;
    simtime_t joinSendTime;

    PeerJoinVerifyEvent(const char* name, int pid,
                        const ReportMessage& msg, simtime_t st)
        : cMessage(name), peerId(pid), reportMsg(msg), joinSendTime(st) {}
};

class SyncApplyDoneEvent : public cMessage {
public:
    Block block;
    int lastHeight;

    SyncApplyDoneEvent(const Block& b, int lh)
        : cMessage("SyncApplyDoneEvent"), block(b), lastHeight(lh) {}
};

class Peer : public cSimpleModule {
    // 性能统计成员变量（全局共享，统计所有Peer的总性能）
private:
    EVP_PKEY *evpPrivateKey = nullptr;
    int peer_id;

    int cpu = 3;
    int memory = 3;
    int IO = 3;

    static simtime_t globalFirstStartTime;   // 全局首份报告开始验证时间
    static simtime_t globalLastFinishTime;   // 全局末份报告完成验证时间
    static int globalTotalReports;           // 全局总验证报告数
    // ===== 新增：端到端（验证开始 -> 共识结束并更新本地账本）统计 =====
    static simtime_t globalE2EFirstStartTime;
    static simtime_t globalE2ELastFinishTime;
    static std::vector<simtime_t> globalE2EDelays;
    // senderID -> verifyStartTime（用于把 BlockMsg 对应回起点）
    std::map<std::string, simtime_t> verifyStartBySender;
    static std::vector<simtime_t> globalDelays; // 全局单报告延迟列表
    int maxParallel = 1; //最大并行验证数
    int busy = 0; //当前正在验证的数量
    std::queue<ReportMessage*> waitQ; //等待队列
    // joinTxKey(senderID) -> deviceId
    std::map<std::string, int> pendingJoinDeviceBySender;
    std::map<std::string, int> pendingLeaveDeviceBySender;

    simtime_t joinRequestTime;  // 加入请求的时间
    simtime_t joinSyncFinishTime;  // 加入同步完成的时间
    simtime_t leaveRequestTime;
    static std::vector<simtime_t> globalJoinTimes;
    static std::vector<simtime_t> globalLeaveTimes;

    std::map<int, SyncBlockMsg*> pendingSyncBlocks;  // height -> msg
    bool syncApplying = false;                       // 正在 apply（防并发）

    bool joinTimingOn = false;        // 这次 join 是否正在计时
    bool leaveTimingOn = false;

    bool AttackTimingOn = false;
    cMessage *attackOnTimer = nullptr;

    std::deque<simtime_t> attackT1Queue;
    static std::vector<simtime_t> globalAttackDelays;

    Ledger ledger;
    std::vector<Report> reports;
    bool online = true;

    cMessage *joinTimer = nullptr;
    cMessage *exitTimer = nullptr;

    enum SelfKind {
        EVT_JOIN = 1,
        EVT_EXIT = 2,
        EVT_ATTACK_ON = 3,
    };

    static std::unordered_map<std::string, std::string> boundPubkeyBySender;

public:
    RSA *publicKey;

    virtual void initialize() override {
        generateKeyPair();

        Block genesisBlock = Block::createGenesisBlock();
        ledger.addBlock(genesisBlock);
        bool enableExit = false;

        int exitTimeSec = 100;
        int Myid = getIndex();

        // Peer 启动时也走共识上链
        ReportMessage rm;

        std::string sid = "peer_" + std::to_string(getIndex());   // ✅ 唯一标识
        std::string content = "Peer online (init)";
        long ts = (long)time(nullptr);  // 或者你想用别的时间源
        std::string hv = VerifyHash(sid, content, ts);

        Report newreport(sid, content);
        EV << "Generated report sender=" << sid
           << " hash=" << newreport.getHash() << endl;

        std::vector<unsigned char> signature = signdata(newreport.getHash());
        newreport.signature = signature;

        std::string publickey = extractPublicKeyPEM();
        newreport.publickey = publickey;

        rm.setSenderID(sid.c_str());
        rm.setContent(content.c_str());
        rm.setTimestamp(ts);
        rm.setHashValue(hv.c_str());
        rm.setSenderpublickey(newreport.publickey.c_str());
        if (!newreport.signature.empty()) {
           // 将 std::vector<unsigned char> 转换为十六进制 std::string
           std::string signatureStr = binToHex(newreport.signature);
           // 将字符串签名设置到消息中
           rm.setSignature(signatureStr.c_str());
           EV_INFO << "Signature added to message as hex string. Original size: "
                   << newreport.signature.size() << " bytes, String size: " << signatureStr.size() << " bytes.\n";
        }
        else {
           EV_WARN << "Report signature is empty, not adding to message.\n";
           // 可以选择设置一个空字符串或不设置
           rm.setSignature("");
        }

        auto *vm = new VerifyMessage("PeerInitVerify");
        vm->setRm(rm);
        vm->setState(1);

        EV_INFO << "Peer[" << getIndex()
                << "] init: send VerifyMessage(state=1) senderID=" << sid
                << " to mapped orderer via out[0]\n";

        send(vm, "out", 0);

        if (Myid == 3) {
            online = false;

            joinTimer = new cMessage("PeerJoinTimer", EVT_JOIN);
            scheduleAt(SimTime(5, SIMTIME_S), joinTimer);
            EV_WARN << "Peer[3] scheduled join at 5s\n";
        }


        if(Myid == 0)
        {
            enableExit = true;
            exitTimeSec = 30;
        }

        if (enableExit) {
            exitTimer = new cMessage("ExitTimer", EVT_EXIT);

            simtime_t t = SimTime(exitTimeSec, SIMTIME_S);
            scheduleAt(t, exitTimer);
            EV_INFO << "Peer " << getIndex() << " scheduled exit at " << t << "\n";
        }

        auto *atk = new cMessage("AttackTimingOn");
        atk->setKind(EVT_ATTACK_ON);
        scheduleAt(SimTime(60, SIMTIME_S), atk);
    }

    inline simtime_t netDelay() {
        return uniform(0.01, 0.012); // 10–12ms
    }

    static simtime_t mapRealToSim(double realSeconds, double scale)
    {
        double simSeconds = realSeconds * scale;
        if (simSeconds < 1e-9) simSeconds = 1e-9; // 避免 0
        return SimTime(simSeconds, SIMTIME_S);
    }

    virtual void handleMessage(cMessage *msg) override {
        if (msg->isSelfMessage()) {
            if (msg->getKind() == EVT_EXIT) {
                leaveTimingOn = true;
                leaveRequestTime = simTime();

                EV_WARN << "Peer[" << getIndex()
                        << "] LEAVE timing start at " << leaveRequestTime << "\n";


                // 1) 构造退出请求
                auto *pl = new PeerLeaveReq("PeerLeaveReq");
                std::string sid = "peer_" + std::to_string(getIndex());
                pl->setSenderID(sid.c_str());
                pl->setPeerId(getIndex());
                pl->setSendTime(simTime());

                // 2) 发给 topoMgr
                cModule *mgr = getParentModule()->getSubmodule("topoMgr");
                if (!mgr) {
                    EV_ERROR << "Peer[" << getIndex() << "]: topoMgr not found\n";
                    delete pl;
                } else {
                    SendOptions opt;
                    opt.propagationDelay(netDelay());
                    EV_INFO << "Peer[" << getIndex() << "] EXIT -> send PeerLeaveReq to topoMgr at "
                            << simTime() << "\n";
                    sendDirect(pl, opt, mgr->gate("join")); // 如果 topoMgr 没有专门 gate，可先复用 join
                    // 如果你给 topoMgr 加了专门 gate，比如 "inPeer", 那就改成 mgr->gate("inPeer")
                }

                delete msg;
                exitTimer = nullptr;
                return;
            }

            if (msg->getKind() == EVT_JOIN) {
                int Myid = getIndex();
                if (Myid != 3) { delete msg; return; }

                // ✅【加这里】开始统计：发送加入请求时刻
                joinRequestTime = simTime();
                joinTimingOn = true;
                EV_WARN << "Peer[" << Myid << "] JOIN timing start at " << joinRequestTime << "\n";

                // ===== 1) 构造 peer 的加入报告 ReportMessage rm =====
                std::string sid = "peer_" + std::to_string(Myid);
                std::string content = "Peer join request at 5s";
                long ts = (long)time(nullptr);

                // 你已有：生成报告、签名、公钥
                Report newreport(sid, content);
                std::vector<unsigned char> signature = signdata(newreport.getHash());
                newreport.signature = signature;
                newreport.publickey = extractPublicKeyPEM();

                ReportMessage rm;
                rm.setSenderID(sid.c_str());
                rm.setContent(content.c_str());
                rm.setTimestamp(ts);
                rm.setHashValue(newreport.hashValue.c_str());               // 或 VerifyHash(sid,content,ts)
                rm.setSenderpublickey(newreport.publickey.c_str());
                rm.setSignature(binToHex(newreport.signature).c_str());

                // ===== 2) 组装 PeerJoinReq =====
                auto *pj = new PeerJoinReq("PeerJoinReq");
                pj->setPeerId(Myid);
                pj->setRm(rm);
                pj->setCpu(3);
                pj->setMemory(3);
                pj->setIO(3);
                pj->setSendTime(simTime());

                // ===== 3) sendDirect 给 topoMgr =====
                cModule *mgr = getParentModule()->getSubmodule("topoMgr");
                if (!mgr) {
                    EV_ERROR << "Peer: topoMgr not found\n";
                    delete pj;
                    delete msg;
                    return;
                }



                SendOptions opt;
                opt.propagationDelay(netDelay());

                EV_WARN << "Peer[3] send PeerJoinReq -> topoMgr at " << simTime() << "\n";
                sendDirect(pj, opt, mgr->gate("join"));

                delete msg;
                joinTimer = nullptr;
                return;
            }

            // 2.1 加入验证
            if (auto *jdone = dynamic_cast<JoinVerifyEvent*>(msg)) {

                std::string sender_id  = jdone->reportMsg.getSenderID();
                std::string sender_con = jdone->reportMsg.getContent();
                long sender_time       = jdone->reportMsg.getTimestamp();
                std::string senderPub  = jdone->reportMsg.getSenderpublickey();
                std::string sig        = jdone->reportMsg.getSignature();
                std::string hashvalue  = jdone->reportMsg.getHashValue();

                bool verified = true;

                // ===== 1) 签名 + hash 验证 =====
                try {
                    std::vector<unsigned char> binData = hexToBin(sig);
                    bool sigOk = verifySignature(hashvalue, binData, senderPub);

                    std::string expectHash = VerifyHash(sender_id, sender_con, sender_time);
                    bool hashOk = (expectHash == hashvalue);

                    verified = sigOk && hashOk;
                } catch (...) {
                    verified = false;
                }

                // ===== 2) 身份绑定检查：senderID -> pubkey 必须一致 =====
                // 只有在“密码学验证通过”的情况下才考虑绑定（不然垃圾包也会污染绑定表）
                if (verified) {
                    auto it = boundPubkeyBySender.find(sender_id);
                    if (it == boundPubkeyBySender.end()) {
                        // 第一次看到这个 senderID：绑定公钥
                        boundPubkeyBySender.emplace(sender_id, senderPub);

                        EV_WARN << "Peer[" << getIndex() << "] bind senderID="
                                << sender_id << " to pubkey (first seen)\n";
                    } else {
                        // senderID 已存在，检查公钥是否一致
                        if (it->second != senderPub) {
                            EV_ERROR << "Peer[" << getIndex()
                                     << "] senderID=" << sender_id
                                     << " pubkey mismatch! possible Sybil/impersonation attack. "
                                     << "bound=" << it->second << " got=" << senderPub << "\n";

                            verified = false;  // ✅ 直接判失败
                        }
                    }
                }

                // ===== 3) 统一构造 VerifyMessage 发给 orderer =====
                auto *vm = new VerifyMessage("VerifyMessage");
                vm->setRm(jdone->reportMsg);

                int state = verified ? 1 : 2;  // 1=allow, 2=reject
                vm->setState(state);

                EV_INFO << "Peer[" << getIndex() << "] join verify done for device "
                        << jdone->deviceId
                        << " verified=" << verified
                        << " -> send VerifyMessage(state=" << state << ")\n";

                send(vm, "out", 0);

                delete jdone;
                return;
            }

            // ===== 新增：peer 加入验证完成事件（PeerJoinVerifyEvent）=====
            if (auto *pjdone = dynamic_cast<PeerJoinVerifyEvent*>(msg)) {

                // ===== 复用现有验证逻辑（同步判定正确性）=====
                std::string sender_id  = pjdone->reportMsg.getSenderID();
                std::string sender_con = pjdone->reportMsg.getContent();
                long sender_time       = pjdone->reportMsg.getTimestamp();
                std::string senderPub  = pjdone->reportMsg.getSenderpublickey();
                std::string sig        = pjdone->reportMsg.getSignature();
                std::string hashvalue  = pjdone->reportMsg.getHashValue();

                bool verified = true;
                try {
                    std::vector<unsigned char> binData = hexToBin(sig);
                    bool sigOk = verifySignature(hashvalue, binData, senderPub);

                    std::string expectHash = VerifyHash(sender_id, sender_con, sender_time);
                    bool hashOk = (expectHash == hashvalue);

                    verified = sigOk && hashOk;
                } catch (...) {
                    verified = false;
                }

                // ===== 组装 VerifyMessage（发给 orderer 上链）=====
                auto *vm = new VerifyMessage("VerifyMessage");
                vm->setRm(pjdone->reportMsg);

                // state: 1=allow(join), 2=reject
                int state = verified ? 1 : 2;
                vm->setState(state);

                EV_INFO << "Peer[" << getIndex() << "] peer-join verify done for peerId="
                        << pjdone->peerId
                        << " senderID=" << sender_id
                        << " verified=" << verified
                        << " -> send VerifyMessage(state=" << state << ") to orderer via out[0]\n";

                send(vm, "out", 0);

                delete pjdone;
                return;
            }

            if (auto *evt = dynamic_cast<SyncApplyDoneEvent*>(msg)) {
                ledger.addBlock(evt->block);

                EV_WARN << "Peer[" << getIndex() << "] synced block(applied) "
                        << evt->block.blockheight << " / " << evt->lastHeight
                        << " at " << simTime() << "\n";

                if (evt->block.blockheight == evt->lastHeight) {
                    EV_WARN << "Peer[" << getIndex() << "] full ledger sync DONE. height="
                            << evt->block.blockheight << "\n";

                    if (joinTimingOn) {
                        joinSyncFinishTime = simTime();
                        simtime_t dt = joinSyncFinishTime - joinRequestTime;
                        Peer::globalJoinTimes.push_back(dt);
                        joinTimingOn = false;

                        EV_WARN << "Peer[" << getIndex()
                                << "] JOIN total time (req->syncDone) = " << dt
                                << " (start=" << joinRequestTime
                                << ", end=" << joinSyncFinishTime << ")\n";
                    }
                }

                delete evt;

                // ✅ 释放“正在 apply”标志，然后继续推进后续缓存块
                syncApplying = false;
                tryStartNextSyncApply();
                return;
            }

            if (msg->getKind() == EVT_ATTACK_ON) {
                AttackTimingOn = true;
                EV_WARN << "Peer[" << getIndex()
                        << "] AttackTimingOn enabled at " << simTime() << "\n";
                delete msg;
                return;
            }

            // 2.2 原有报告验证完成事件：保持不变
            VerifyEvent* doneEvt = check_and_cast<VerifyEvent*>(msg);
            processReceivedReport(doneEvt->reportMsg, doneEvt->startTime);

            busy--;
            if (!waitQ.empty()) {
                ReportMessage *next = waitQ.front();
                waitQ.pop();
                startVerify(next);
            }
            delete doneEvt;
            return;
        }

        if (auto *ack = dynamic_cast<PeerJoinAck*>(msg)) {
                   bool ok = ack->getSuccess();
                   EV_WARN << "Peer[" << getIndex() << "] got PeerJoinAck success=" << ok
                           << " at " << ack->getTime() << "\n";

                   if (ok) {
                       online = true;
                       // 我本地账本当前高度
                       int curH = ledger.getLatestBlock().blockheight;

                       auto *req = new SyncLedgerReq("SyncLedgerReq");
                       req->setPeerId(getIndex());
                       req->setFromHeight(curH + 1);
                       req->setSendTime(simTime());

                       // ✅ 关键：把同步请求发给 orderer_0
                       cModule *ord0 = getParentModule()->getSubmodule("orderer_0");
                       if (!ord0) {
                           EV_ERROR << "Peer[" << getIndex() << "]: orderer_0 not found\n";
                           delete req;
                       } else {
                           SendOptions opt;
                           opt.propagationDelay(netDelay());
                           EV_INFO << "Peer[" << getIndex() << "] request ledger sync fromHeight="
                                   << (curH + 1) << " -> orderer_0\n";

                           // orderer 需要有一个 gate 来接收（建议用 in[] 或新增 sync gate）
                           sendDirect(req, opt, ord0->gate("syn"));
                       }
                   }

                   delete ack;
                   return;
               }

        if (!online) {
            delete msg;
            return;
        }

        // ===== 新增：处理 orderer 下发的区块 =====
        if (auto *blk = dynamic_cast<BlockMsg*>(msg)) {
            if(online){
            handleBlockMsg(blk);
            }
            delete blk;
            return;
        }

        // ===== 新增：处理 TopoManager 转发的加入验证请求 =====
        if (auto *jreq = dynamic_cast<JoinVerifyReq*>(msg)) {

            if (AttackTimingOn) {
                simtime_t t1 = jreq->getSendTime();
                attackT1Queue.push_back(t1);

                EV_WARN << "Peer[" << getIndex()
                        << "] ATTACK t1 queued: " << t1
                        << " queue_size=" << attackT1Queue.size() << "\n";
            }

            int deviceId = jreq->getDeviceId();
            int assignedPeerId = jreq->getAssignedPeerId();
            ReportMessage rm = jreq->getRm();
            simtime_t sendTime = jreq->getSendTime();
            pendingJoinDeviceBySender[rm.getSenderID()] = deviceId;

            // 用你已有的 RSA 测时函数模拟验证耗时
            simtime_t delay = measureRsaRuntime(rm);

            // schedule 一个 JoinVerifyEvent：delay 后回 JoinDecision
            auto *evt = new JoinVerifyEvent("joinVerifyDone", deviceId, assignedPeerId, rm, sendTime);
            scheduleAt(simTime() + delay, evt);

            EV_INFO << "Peer[" << getIndex() << "] received JoinVerifyReq for device "
                    << deviceId << ", schedule verifyDone at " << (simTime() + delay) << "\n";

            delete jreq;
            return;
        }

        // ===== 新增：处理 TopoManager 转发的 peer 加入验证请求 =====
        if (auto *pjreq = dynamic_cast<PeerJoinVerifyReq*>(msg)) {

            int peerId = pjreq->getPeerId();
            ReportMessage rm = pjreq->getRm();
            simtime_t sendTime = pjreq->getSendTime();

            // 用你已有的 RSA 测时函数模拟验证耗时
            simtime_t delay = measureRsaRuntime(rm);

            // schedule 一个 PeerJoinVerifyEvent：delay 后进入“验证完成处理”
            auto *evt = new PeerJoinVerifyEvent("peerJoinVerifyDone", peerId, rm, sendTime);
            scheduleAt(simTime() + delay, evt);

            EV_INFO << "Peer[" << getIndex() << "] received PeerJoinVerifyReq for peer "
                    << peerId << ", schedule verifyDone at " << (simTime() + delay) << "\n";

            delete pjreq;
            return;
        }


        // ===== 原有：处理 device 上送的报告 =====
        if (auto *reportMsg = dynamic_cast<ReportMessage*>(msg)) {
            EV << "Received report from " << reportMsg->getSenderID()
               << ": " << reportMsg->getContent() << endl;

            // 拷贝一份放到自己的控制里（因为原 msg 会被 delete）
            ReportMessage *copy = new ReportMessage(*reportMsg);

            if (busy < maxParallel) {
                busy++;
                startVerify(copy);
            } else {
                waitQ.push(copy);
            }

            delete reportMsg;
            return;
        }

        // ===== 新增：处理 device 发送的退出请求 =====
        if (auto *lreq = dynamic_cast<LeaveReq*>(msg)) {
            int deviceId = lreq->getDeviceId();
            std::string senderID = lreq->getSenderID();
            // ✅ 记录：等区块确认后回 LeaveAck
            pendingLeaveDeviceBySender[senderID] = deviceId;

            handleLeaveReq(lreq);
            delete lreq;
            return;
        }



        /*if (auto *sb = dynamic_cast<SyncBlockMsg*>(msg)) {

            // 1) 基本顺序检查：必须是 next height
            int expect = ledger.getLatestBlock().blockheight + 1;
            if (sb->getBlockNum() != expect) {
                EV_ERROR << "Peer[" << getIndex() << "] sync block out-of-order, got="
                         << sb->getBlockNum() << " expect=" << expect << "\n";
                delete sb;
                return;
            }

            // 2) hash 链接检查：prevHash 必须匹配本地最新块 hash
            //std::string localHash = ledger.getLatestBlock().BlockHash;
            //if (sb->getPreviousHash() != localHash) {
            //    EV_ERROR << "Peer[" << getIndex() << "] sync prevHash mismatch, got="
            //             << sb->getPreviousHash() << " local=" << localHash << "\n";
            //    delete sb;
            //    return;
            //}

            // 3) 组装 reports（从 entries[] 还原 Report）
            std::vector<Report> synced;
            int n = sb->getEntriesArraySize();
            synced.reserve(n);

            for (int i = 0; i < n; ++i) {
                const ReportEntry &e = sb->getEntries(i);
                const ReportMessage &rm = e.getRm();

                Report r;
                r.senderID  = rm.getSenderID();
                r.content   = rm.getContent();
                r.hashValue = rm.getHashValue();
                r.timestamp = rm.getTimestamp();
                r.publickey = rm.getSenderpublickey();

                // signature hex -> bin
                std::vector<unsigned char> binSig = hexToBin(rm.getSignature());
                r.signature = binSig;

                r.state = (NodeState)e.getState();
                synced.push_back(r);
            }

            // 4) 构造并追加 Block（注意用消息里的 hash，不重新 calculate）
            Block newB(sb->getBlockNum(), sb->getPreviousHash(), synced);
            newB.timestamp = sb->getTimestamp();
            newB.BlockHash = sb->getBlockHash(); // 覆盖成 orderer 的 hash

            ledger.addBlock(newB);

            EV_WARN << "Peer[" << getIndex() << "] synced block " << newB.blockheight
                    << " / " << sb->getLastHeight() << "\n";

            // 5) 若追到最新高度，同步完成
            if (newB.blockheight == sb->getLastHeight()) {
                EV_WARN << "Peer[" << getIndex() << "] full ledger sync DONE. height="
                        << newB.blockheight << "\n";
                if (joinTimingOn) {
                        joinSyncFinishTime = simTime();
                        simtime_t dt = joinSyncFinishTime - joinRequestTime;
                        Peer::globalJoinTimes.push_back(dt);
                        joinTimingOn = false;

                        EV_WARN << "Peer[" << getIndex() << "] JOIN total time (req->syncDone) = "
                                << dt << " (start=" << joinRequestTime
                                << ", end=" << joinSyncFinishTime << ")\n";
                    }
            }

            delete sb;
            return;
        }*/

        if (auto *sb = dynamic_cast<SyncBlockMsg*>(msg)) {
            int h = sb->getBlockNum();
            int curH = ledger.getLatestBlock().blockheight;

            // 过期块（已经有了）直接丢
            if (h <= curH) {
                delete sb;
                return;
            }

            // 重复块丢掉新来的
            if (pendingSyncBlocks.count(h)) {
                delete sb;
                return;
            }

            // ✅ 缓存，不做 out-of-order delete
            pendingSyncBlocks[h] = sb;

            // ✅ 尝试推进（如果 expect 正好到了就会启动 apply）
            tryStartNextSyncApply();
            return;
        }




        if (auto *ack = dynamic_cast<PeerLeaveAck*>(msg)) {
            if (ack->getPeerId() == getIndex()) {
                EV_WARN << "Peer[" << getIndex() << "] leave SUCCESS at " << ack->getAckTime()
                        << " sid=" << ack->getSenderID() << "\n";

                if (leaveTimingOn) {
                           simtime_t dt = simTime() - leaveRequestTime;
                           Peer::globalLeaveTimes.push_back(dt);
                           leaveTimingOn = false;

                           EV_WARN << "Peer[" << getIndex()
                                   << "] LEAVE time (req->ack) = " << dt << "\n";
                       }

                online = false;  // 或者 finish/cancel timers/停止处理外部消息
            }
            delete ack;
            return;
        }


        EV_WARN << "Peer received unknown msg\n";
        delete msg;
    }

    void startSyncApplyFromMsg(SyncBlockMsg *sb)
    {
        syncApplying = true;

        int blockNum = sb->getBlockNum();
        int lastH = sb->getLastHeight();

        // === 真实测时：解析 + 构造 Block（addBlock 放到 event 里）===
        auto t1 = std::chrono::high_resolution_clock::now();

        std::vector<Report> synced;
        int n = sb->getEntriesArraySize();
        synced.reserve(n);

        for (int i = 0; i < n; ++i) {
            const ReportEntry &e = sb->getEntries(i);
            const ReportMessage &rm = e.getRm();

            Report r;
            r.senderID  = rm.getSenderID();
            r.content   = rm.getContent();
            r.hashValue = rm.getHashValue();
            r.timestamp = rm.getTimestamp();
            r.publickey = rm.getSenderpublickey();

            r.signature = hexToBin(rm.getSignature());
            r.state = (NodeState)e.getState();

            synced.push_back(r);
        }

        Block newB(blockNum, sb->getPreviousHash(), synced);

        // ⚠️ 注意 timestamp 类型：
        // 你之前报过 long <- simtime_t/范围错误，
        // 所以这里建议不要把“Unix秒”塞进 SimTime。
        // 保持原来 long / time_t 即可：
        newB.timestamp = sb->getTimestamp();    // 确保两边类型一致（一般是 long / simtime_t 二选一）
        newB.BlockHash = sb->getBlockHash();

        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = t2 - t1;

        double scale = 1;  // ✅ 用 ini 里的 cpuScale
        simtime_t cpuDelay = mapRealToSim(diff.count(), scale);

        EV_WARN << "Peer[" << getIndex() << "] sync prep real=" << diff.count()
                << "s scale=" << scale << " -> simCpuDelay=" << cpuDelay
                << " block=" << blockNum << "/" << lastH << "\n";

        // 延迟后 apply
        scheduleAt(simTime() + cpuDelay, new SyncApplyDoneEvent(newB, lastH));

        delete sb; // ✅ sb 的数据都拷进 newB 了，可以删
    }

    void tryStartNextSyncApply()
    {
        if (syncApplying) return;

        int expect = ledger.getLatestBlock().blockheight + 1;

        auto it = pendingSyncBlocks.find(expect);
        if (it == pendingSyncBlocks.end())
            return;

        SyncBlockMsg *sb = it->second;
        pendingSyncBlocks.erase(it);

        startSyncApplyFromMsg(sb);
    }


    void handleLeaveReq(LeaveReq *lreq)
    {
        std::string senderID = lreq->getSenderID();   // 注意：这是 device 发来的 senderID
        int deviceId = lreq->getDeviceId();

        // 在 reports 里找对应 senderID 的报告
        auto it = std::find_if(reports.begin(), reports.end(),
                               [&](const Report& r){ return r.senderID == senderID; });

        if (it == reports.end()) {
            EV_WARN << "Peer[" << getIndex() << "] LeaveReq: no report found in reports for senderID="
                    << senderID << " (cannot build VerifyMessage)\n";
            return;
        }

        const Report& rep = *it;

        // 组装 ReportMessage（从 Report 提取）
        ReportMessage rm;
        rm.setSenderID(rep.senderID.c_str());
        rm.setContent(rep.content.c_str());
        rm.setHashValue(rep.hashValue.c_str());
        rm.setTimestamp(rep.timestamp);
        rm.setSenderpublickey(rep.publickey.c_str());
        rm.setSignature(binToHex(rep.signature).c_str());  // 你 Peer 里已有 hexToBin；binToHex 也要有或写一个

        // 组装 VerifyMessage，state=2 表示退出
        auto *vm = new VerifyMessage("VerifyMessage");
        vm->setRm(rm);
        vm->setState(2);

        EV_INFO << "Peer[" << getIndex() << "] LeaveReq for deviceId=" << deviceId
                << " senderID=" << senderID
                << " -> send VerifyMessage(state=2) to mapped orderer via out[0]\n";

        // 走 peer->orderer 已有连线
        send(vm, "out", 0);
    }


    void handleBlockMsg(BlockMsg *blkMsg)
    {
        int blockNum = blkMsg->getBlockNum();
        ReportMessage rm = blkMsg->getRm();
        simtime_t ts = blkMsg->getBlockTimestamp();
        int state = blkMsg->getState();



        EV << "Peer[" << getIndex() << "] received BlockMsg: blockNum=" << blockNum
           << " ts=" << ts << " reportSender=" << rm.getSenderID() << "\n";

        // 直接按 rm 更新区块链
        Block latestBlock = ledger.getLatestBlock();
        int currentHeight = latestBlock.blockheight;
        std::string currentHash = latestBlock.BlockHash;

        Report newReport;
        newReport.senderID  = rm.getSenderID();
        newReport.content   = rm.getContent();
        newReport.hashValue = rm.getHashValue();
        newReport.timestamp = rm.getTimestamp();

        // 把 state 映射到节点状态
        if (state == 1) newReport.state = NodeState::JOINED;
        else if (state == 2) newReport.state = NodeState::LEFT;
        else newReport.state = NodeState::NEW;

        // 如果你 Peer 也需要把 signature 转成二进制存储，就复用 hexToBin
        std::string sig = rm.getSignature();
        std::vector<unsigned char> binData = hexToBin(sig);
        newReport.signature = binData;

        // 有则替换，无则新增
        bool replaced = false;
        for (auto &r : reports) {
            if (r.senderID == newReport.senderID) {
                r = newReport;
                replaced = true;
                break;
            }
        }
        if (!replaced) reports.push_back(newReport);

        Block newBlock(currentHeight + 1, currentHash, reports);
        ledger.addBlock(newBlock);

        if (AttackTimingOn && state == 2) {

            if (!attackT1Queue.empty()) {
                simtime_t t1 = attackT1Queue.front();
                attackT1Queue.pop_front();

                simtime_t t2 = simTime();
                simtime_t dt = t2 - t1;

                Peer::globalAttackDelays.push_back(dt);

                EV_WARN << "Peer[" << getIndex()
                        << "] ATTACK dt recorded: " << dt
                        << " (t1=" << t1 << ", t2=" << t2 << ")\n";
            } else {
                EV_ERROR << "Peer[" << getIndex()
                         << "] ATTACK queue empty but state=2 block arrived!\n";
            }
        }

        // ===== 新增：端到端统计（开始验证 -> 收到区块并更新账本结束）=====
        simtime_t finishTime = simTime();
        std::string key = rm.getSenderID();

        auto it = verifyStartBySender.find(key);
        if (it != verifyStartBySender.end()) {
            simtime_t startTime = it->second;
            simtime_t e2e = finishTime - startTime;

            globalE2EDelays.push_back(e2e);

            if (globalE2EFirstStartTime == SIMTIME_ZERO || startTime < globalE2EFirstStartTime)
                globalE2EFirstStartTime = startTime;
            if (finishTime > globalE2ELastFinishTime)
                globalE2ELastFinishTime = finishTime;

            // 用完就删，避免 map 越来越大
            verifyStartBySender.erase(it);

            EV << "Peer[" << getIndex() << "] E2E delay for " << key
               << " = " << e2e << " (start=" << startTime << ", finish=" << finishTime << ")\n";
        } else {
            EV_WARN << "Peer[" << getIndex() << "] no verifyStartTime found for senderID=" << key
                    << " (cannot compute E2E)\n";
        }


        EV << "Peer[" << getIndex() << "] updated local ledger, new height="
           << newBlock.blockheight << " (from BlockMsg.blockNum=" << blockNum << ")\n";

        // ===== 如果这是 join 交易的区块，通知对应 device 加入成功 =====
        auto itj = pendingJoinDeviceBySender.find(rm.getSenderID());
        if (itj != pendingJoinDeviceBySender.end()) {
            int deviceId = itj->second;

            auto *ack = new JoinAck("JoinAck");
            ack->setDeviceId(deviceId);
            ack->setSuccess(state == 1);
            ack->setTime(simTime());
            ack->setPeerId(getIndex());

            cModule *devMod = getParentModule()->getSubmodule("dev", deviceId);
            if (!devMod) {
                EV_ERROR << "Peer[" << getIndex() << "]: dev[" << deviceId << "] not found\n";
                delete ack;
            } else {
                SendOptions opt;
                opt.propagationDelay(netDelay());
                EV_INFO << "Peer[" << getIndex() << "]: ledger updated for join tx sender="
                        << rm.getSenderID() << " -> notify dev[" << deviceId << "] JOIN success\n";
                sendDirect(ack, opt, devMod->gate("join"));
            }

            // ✅ 通知完就删，避免重复通知
            pendingJoinDeviceBySender.erase(itj);
        }
        // ===== 如果这是 leave 交易的区块，通知对应 device 退出成功 =====
        auto itl = pendingLeaveDeviceBySender.find(rm.getSenderID());
        if (state == 2 && itl != pendingLeaveDeviceBySender.end()) {
            int deviceId = itl->second;

            auto *ack = new LeaveAck("LeaveAck");
            ack->setSenderID(rm.getSenderID());   // 你也可以用 "peer_x" 等
            ack->setDeviceId(deviceId);
            ack->setAckTime(simTime());

            cModule *devMod = getParentModule()->getSubmodule("dev", deviceId);
            if (!devMod) {
                EV_ERROR << "Peer[" << getIndex() << "]: dev[" << deviceId << "] not found\n";
                delete ack;
            } else {
                SendOptions opt;
                opt.propagationDelay(netDelay());

                EV_INFO << "Peer[" << getIndex() << "]: ledger updated for LEAVE tx sender="
                        << rm.getSenderID() << " -> notify dev[" << deviceId << "] LEAVE ack\n";

                sendDirect(ack, opt, devMod->gate("join"));  // 你 device 的 join gate 用来收控制消息
            }

            // ✅ 通知完就删，避免重复通知
            pendingLeaveDeviceBySender.erase(itl);
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

    void processReceivedReport(ReportMessage& reportMsg, simtime_t reportStartTime) {
        // 记录当前报告的开始验证时间
        int state=0;
        EV << "Peer " << getIndex() << " 开始验证报告 " << reportMsg.getSenderID() << "，开始时间：" << reportStartTime << "\n";
            //EV << "Received report from " << reportMsg.getSenderID() << "\n";
            EV << "Content: " << reportMsg.getContent() << "\n";
            EV << "Hash: " << reportMsg.getHashValue() << "\n";
            EV << "Timestamp: " << reportMsg.getTimestamp() << "\n";
            EV << "signature: " << reportMsg.getSignature() << "\n";
            EV << "publickey: " << reportMsg.getSenderpublickey() << "\n";
            verifyStartBySender[reportMsg.getSenderID()] = reportStartTime;
            std::string sender_id = reportMsg.getSenderID();
            std::string sender_con = reportMsg.getContent();
            long sender_time = reportMsg.getTimestamp();
            std::string senderPub = reportMsg.getSenderpublickey();

            // ===== 身份绑定检查 =====
            auto it = boundPubkeyBySender.find(sender_id);
            if (it == boundPubkeyBySender.end()) {
                // 第一次看到这个 senderID：绑定公钥
                boundPubkeyBySender[sender_id] = senderPub;

                EV_WARN << "Peer[" << getIndex() << "] bind senderID="
                        << sender_id << " to pubkey (first seen)\n";
            } else {
                // senderID 已存在，检查公钥是否一致
                if (it->second != senderPub) {
                    EV_ERROR << "Peer[" << getIndex()
                             << "] senderID=" << sender_id
                             << " pubkey mismatch! possible Sybil/impersonation attack\n";

                    // 直接判失败
                    auto *vm = new VerifyMessage("VerifyMessage");
                    vm->setRm(reportMsg);
                    vm->setState(2);   // ❌ 拒绝
                    send(vm, "out", 0);
                    return;
                }
            }

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
                state = 1;
            }else{
                state = 2;
            }
            std::string isHashValid = VerifyHash(sender_id, sender_con, sender_time);
            if (isHashValid == hashvalue){
                EV << " correct Report verification at hashvalue check!" << "\n";
            }
            else{
                state = 2;
            }
            processReceivedReportTime(sender_time);
            // 记录当前报告的完成验证时间，更新统计
            simtime_t reportFinishTime = simTime();
            simtime_t reportDelay = reportFinishTime - reportStartTime;
            EV << "Peer " << getIndex() << " 完成验证报告 " << reportMsg.getSenderID() << "，完成时间：" << reportFinishTime << "，延迟：" << reportDelay << "\n";
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

            auto *vm = new VerifyMessage("VerifyMessage");
            vm->setRm(reportMsg);
            vm->setState(state);
            send(vm, "out", 0);
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

    std::string binToHex(const std::vector<unsigned char>& binData) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (unsigned char c : binData)
            ss << std::setw(2) << static_cast<int>(c);
        return ss.str();
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

   std::string VerifyHash(const std::string& senderID, const std::string& content, const long& timestamp){
       std::string data =senderID + content + std::to_string(timestamp);
       std::size_t stringHash = std::hash<std::string>{}(data);
       std::string hashvalue = std::to_string(stringHash);
       return hashvalue;
   }

   void processReceivedReportTime(const long sender_time) {
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
           return; // 拒绝该报告
       }

       if (timeDifference > MAX_AGE_SECONDS) {
           EV_ERROR << "Report verification failed: Timestamp is too old. Possible replay attack.\n";
           return; // 拒绝该报告
       }

       EV_INFO << "Timestamp verification passed.\n";

       // --- 如果通过，继续执行后续的哈希和签名验证 ---
       // ... (你的其他验证代码) ...
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
         // 示例：将公钥保存到文件（PEM 格式）
             FILE *pubKeyFile = fopen("peer_public.pem", "w");
             if (pubKeyFile && PEM_write_PUBKEY(pubKeyFile, evpPrivateKey)) {
                 EV_INFO << "Public key saved to device_public.pem\n";
             } else {
                 EV_ERROR << "Failed to save public key!\n";
                 ERR_print_errors_fp(stderr);
             }
             if (pubKeyFile) fclose(pubKeyFile);

             // 示例：将私钥保存到文件（PEM 格式）
             FILE *privKeyFile = fopen("peer_private.pem", "w");
             if (privKeyFile && PEM_write_PrivateKey(privKeyFile, evpPrivateKey, nullptr, nullptr, 0, nullptr, nullptr)) {
                 EV_INFO << "Private key saved to device_private.pem\n";
             } else {
                 EV_ERROR << "Failed to save private key!\n";
                 ERR_print_errors_fp(stderr);
             }
             if (privKeyFile) fclose(privKeyFile);
             //finish();
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

       cModule *peerMod = getParentModule()->getSubmodule("peer", 1);
       SendOptions opt;
       opt.propagationDelay(netDelay());

       sendDirect(msg, opt, peerMod->gate("rein"));
   }

   void finish(){
       // 仅最后一个Peer输出统计（getSubmoduleVectorSize()获取Peer总数）
       if (getIndex() == getParentModule()->getSubmoduleVectorSize("peer") - 1) {
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
           std::ofstream outFile("blockchain_performance.txt");
           outFile << "总验证报告数：" << globalTotalReports << endl;
           outFile << "总验证时间：" << totalVerifyTime << endl;
           outFile << "平均验证延迟：" << avgDelay << endl;
           outFile << "吞吐量：" << throughput << endl;

           // ===== 新增：端到端（验证开始 -> 共识结束并更新账本）统计 =====
           simtime_t e2eTotalTime = (globalE2EFirstStartTime == SIMTIME_ZERO) ? SIMTIME_ZERO
                                 : (globalE2ELastFinishTime - globalE2EFirstStartTime);
           simtime_t e2eAvgDelay = (globalE2EDelays.empty()) ? SIMTIME_ZERO
                                 : (std::accumulate(globalE2EDelays.begin(), globalE2EDelays.end(), SIMTIME_ZERO) / globalE2EDelays.size());

           EV << "E2E首份开始时间：" << globalE2EFirstStartTime << endl;
           EV << "E2E末份完成时间：" << globalE2ELastFinishTime << endl;
           EV << "E2E总时间(验证->共识->更新账本)：" << e2eTotalTime << endl;
           EV << "E2E平均延迟：" << e2eAvgDelay << endl;

           outFile << "E2E首份开始时间：" << globalE2EFirstStartTime << endl;
           outFile << "E2E末份完成时间：" << globalE2ELastFinishTime << endl;
           outFile << "E2E总时间(验证->共识->更新账本)：" << e2eTotalTime << endl;
           outFile << "E2E平均延迟：" << e2eAvgDelay << endl;

           outFile.close();

           // 释放 OpenSSL 资源（你原来的 finish() 不是 override，这里修正成 OMNeT++ 会调用的 finish）
           if (evpPrivateKey) EVP_PKEY_free(evpPrivateKey);
           evpPrivateKey = nullptr;

           EVP_cleanup();
           ERR_free_strings();

           std::ofstream out("peer_join_leave_time.txt", std::ios::app);
           if (!out.is_open()) return;

           // ===== JOIN =====
           if (!globalJoinTimes.empty()) {
               simtime_t sum = SIMTIME_ZERO;
               for (auto &t : globalJoinTimes)
                   sum += t;

               simtime_t avg = sum / globalJoinTimes.size();

               EV << "===== PEER JOIN STAT =====\n";
               EV << "Join count = " << globalJoinTimes.size() << "\n";
               EV << "Avg join time (req -> syncDone) = " << avg << "\n";

               out << "JOIN"
                   << " count=" << globalJoinTimes.size()
                   << " avg=" << avg << "\n";
           }

           // ===== LEAVE =====
           if (!globalLeaveTimes.empty()) {
               simtime_t sum = SIMTIME_ZERO;
               for (auto &t : globalLeaveTimes)
                   sum += t;

               simtime_t avg = sum / globalLeaveTimes.size();

               EV << "===== PEER LEAVE STAT =====\n";
               EV << "Leave count = " << globalLeaveTimes.size() << "\n";
               EV << "Avg leave time (req -> ack) = " << avg << "\n";

               out << "LEAVE"
                   << " count=" << globalLeaveTimes.size()
                   << " avg=" << avg << "\n";
           }

           std::ofstream outa("attack.txt", std::ios::app);
           if (!outa.is_open()) return;

           if (!Peer::globalAttackDelays.empty()) {
               simtime_t sum = SIMTIME_ZERO;
               for (auto &t : Peer::globalAttackDelays) sum += t;
               simtime_t avg = sum / Peer::globalAttackDelays.size();

               outa << "SYBIL_ATTACK_VERIFY_FAIL_TO_LEDGER"
                   << " count=" << Peer::globalAttackDelays.size()
                   << " avg=" << avg << "\n";
           }
           outa.close();
       }
   }
};
Define_Module(Peer);

// 新增2：初始化静态统计变量（类外定义，确保全局唯一）
simtime_t Peer::globalFirstStartTime = SIMTIME_ZERO;
simtime_t Peer::globalLastFinishTime = SIMTIME_ZERO;
int Peer::globalTotalReports = 0;
std::vector<simtime_t> Peer::globalDelays;
simtime_t Peer::globalE2EFirstStartTime = SIMTIME_ZERO;
simtime_t Peer::globalE2ELastFinishTime = SIMTIME_ZERO;
std::vector<simtime_t> Peer::globalE2EDelays;
std::vector<simtime_t> Peer::globalJoinTimes;
std::vector<simtime_t> Peer::globalLeaveTimes;
std::unordered_map<std::string, std::string> Peer::boundPubkeyBySender;
std::vector<simtime_t> Peer::globalAttackDelays;



