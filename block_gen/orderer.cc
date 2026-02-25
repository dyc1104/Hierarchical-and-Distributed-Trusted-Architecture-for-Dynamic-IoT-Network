/*
 * orderer.cc
 *
 *  Created on: Nov 14, 2025
 *      Author: helen
 */
#include "Block.h"
#include "Ledger.h"
#include "message_m.h"
#include "Report.h"
#include <omnetpp.h>
#include <string>
#include <map>
#include <sstream>
#include <stdexcept> // 用于抛出异常
#include <iomanip>
#include <vector>

using namespace omnetpp;

class Orderer:public cSimpleModule{
private:
    Ledger ledger;
    std::vector<Report> reports;

    int OrderId;
    bool isPrimary;
    int viewNum;
    int seqNum;
    int totalNodes;

    bool malice = false;

    // PBFT阶段记录
    std::map<int, std::vector<int>> prepareCount;  // seqNum -> list of senderIds
    std::map<int, std::vector<int>> commitCount;   // seqNum -> list of senderIds
    // seq -> digest -> state -> set(orderId)
    std::map<int, std::map<std::string, std::map<int, std::set<int>>>> commitVotes;

    // 新增去重集合
    std::set<int> preparedSeq;   // 已发送 prepared 的 seq
    std::set<int> commitSeq;   // 已发送 commit 的 seq
    std::set<int> finalizedSeq;  // 已 finalize Block 的 seq


protected:
    virtual void initialize() override{

        OrderId = par("orderId");
        isPrimary = par("isPrimary");
        totalNodes = par("totalNodes");
        viewNum = 0;
        seqNum = 0;

        if(OrderId == 5){
            malice = true;
        }

        // 初始化去重集合
        preparedSeq.clear();
        finalizedSeq.clear();

        Block genesisBlock = Block::createGenesisBlock();
        ledger.addBlock(genesisBlock);

        EV << "Orderer initialized.\n";
    }

    inline simtime_t netDelay() {
        return uniform(0.01, 0.012); // 10–12ms
    }

    virtual void handleMessage(cMessage *msg) override{
        if (msg->isSelfMessage()) {
            EV << "selfMessage " << endl;
        }
        else if (auto *vm = dynamic_cast<VerifyMessage*>(msg)) {
            handleVerifyMsg(vm);   // 👈 抽成函数，逻辑更清晰
            delete vm;
            return;
        }
        else if (auto preMsg = dynamic_cast<PrePrepareMsg*>(msg)) {
            sendPrepare(preMsg);
            delete preMsg;
            return;
        }
        // Prepare → 收集并发送 Commit
        else if (auto prepMsg = dynamic_cast<PrepareMsg*>(msg)) {
            sendCommit(prepMsg);
            delete prepMsg;
            return;
        }
        // Commit → 收集 finalize Block
        else if (auto cmtMsg = dynamic_cast<CommitMsg*>(msg)) {
            finalizeBlock(cmtMsg);
            delete cmtMsg;
            return;
        }
        else if (auto *sreq = dynamic_cast<SyncLedgerReq*>(msg)) {
            handleSyncLedgerReq(sreq);
            delete sreq;
            return;
        }
        else {
            EV_WARN << "Orderer received unknown msg\n";
            delete msg;
        }
    }

    void handleSyncLedgerReq(SyncLedgerReq *req)
    {
        if (!isPrimary) {
            EV_WARN << "Orderer[" << OrderId << "] not primary, ignore SyncLedgerReq\n";
            return;
        }

        int pid = req->getPeerId();
        int fromH = req->getFromHeight();
        int lastH = ledger.getLatestBlock().blockheight;

        cModule *peerMod = getParentModule()->getSubmodule("peer", pid);
        if (!peerMod) {
            EV_ERROR << "Orderer[" << OrderId << "]: peer[" << pid << "] not found\n";
            return;
        }

        EV_WARN << "Orderer[" << OrderId << "] sync to peer[" << pid
                << "] fromHeight=" << fromH << " lastHeight=" << lastH << "\n";

        for (int h = fromH; h <= lastH; ++h) {
            Block b = ledger.getBlock(h);

            auto *m = new SyncBlockMsg("SyncBlockMsg");
            m->setPeerId(pid);
            m->setBlockNum(b.blockheight);
            m->setPreviousHash(b.previousBlockHash.c_str());
            m->setBlockHash(b.BlockHash.c_str());
            m->setTimestamp(b.timestamp);
            m->setLastHeight(lastH);

            // entries[] = block.reports（全量）
            m->setEntriesArraySize(b.reports.size());
            for (int i = 0; i < (int)b.reports.size(); ++i) {
                const Report &r = b.reports[i];

                ReportMessage rm;
                rm.setSenderID(r.senderID.c_str());
                rm.setContent(r.content.c_str());
                rm.setHashValue(r.hashValue.c_str());
                rm.setTimestamp(r.timestamp);
                rm.setSenderpublickey(r.publickey.c_str());
                rm.setSignature(binToHex(r.signature).c_str()); // 你 Orderer 里已有 binToHex/hexToBin

                ReportEntry e;
                e.setRm(rm);
                // 注意：r.state 是 enum NodeState；msg 里是 int，所以转 int
                e.setState((int)r.state);

                m->setEntries(i, e);
            }

            SendOptions opt;
            opt.propagationDelay(SimTime(12, SIMTIME_MS));
            sendDirect(m, opt, peerMod->gate("rein"));   // ✅ 发给 peer 的 rein（你用于控制消息）
        }
    }



    void processReceivedReport(const ReportMessage& reportMsg) {
        Block latestBlock = ledger.getLatestBlock();
        int currentHeight = latestBlock.blockheight;
        std::string currentHash = latestBlock.BlockHash;
        Report newReport;
        newReport.senderID = reportMsg.getSenderID();
        newReport.content = reportMsg.getContent();
        newReport.hashValue = reportMsg.getHashValue();
        newReport.timestamp = reportMsg.getTimestamp();
        std::string sig= reportMsg.getSignature();
        std::vector<unsigned char> binData = hexToBin(sig);
        newReport.signature = binData;
        reports.push_back(newReport);
        Block newBlock = Block(currentHeight+1, currentHash, reports);
        ledger.addBlock(newBlock);
        EV << "Generated a new block" << endl;
    }

    void handleVerifyMsg(VerifyMessage *vm)
    {
        ReportMessage rm = vm->getRm();
        int state = vm->getState();
        EV << "Orderer received txId=" << rm.getSenderID() << " hash=" << rm.getHashValue() << "\n";
        if (auto vMsg = dynamic_cast<VerifyMessage*>(vm)) {
            if (isPrimary) {
                broadcastPrePrepare(vMsg);
                return;
            }
            else{
                sendtoPrimary(vMsg);
            }
        }
    }

    // 非主节点：将收到的 ReportMessage 转发给主节点（从 orderout[0] 发出）
    void sendtoPrimary(VerifyMessage *vMsg)
    {
        if (vMsg == nullptr) {
            EV << "sendtoPrimary: nullptr rMsg, skip.\n";
            return;
        }


        // 确保 orderout[0] 存在且已连接
        if (gateSize("orderout") <= 0) {
            EV_WARN << "sendtoPrimary: gate 'orderout' not found or not an array.\n";
            return;
        }
        cGate *g = gate("orderout", 0);
        if (g == nullptr || !g->isConnected()) {
            EV_WARN << "sendtoPrimary: orderout[0] not connected, cannot forward.\n";
            return;
        }

        // 转发必须 dup（原消息会在 handleMessage 里 delete）
        VerifyMessage *fwd = vMsg->dup();
        // 可选：改个名字方便调试
        fwd->setName("report_to_primary");

        EV << "Node[" << OrderId << "] forward Report to Primary via orderout[0], "
           << "sender=" << vMsg->getRm().getSenderID()
           << " hash=" << vMsg->getRm().getHashValue() << "\n";

        send(fwd, "orderout", 0); // 所有权交给 OMNeT++，不要 delete fwd
    }


    // 广播给所有连接在 out[] 上的 peer（Orderer <-> Peer 端口）
    void broadcastToPeers(cMessage *msg)
    {
        if (msg == nullptr) {
            EV_WARN << "broadcastToPeers: nullptr msg, skip.\n";
            return;
        }

        int n = gateSize("out");
        EV << "[BroadcastToPeers] t=" << simTime()
           << " msg=" << msg->getName()
           << " outSize=" << n << "\n";

        if (n <= 0) {
            EV_WARN << "broadcastToPeers: gate 'out' has size=" << n
                    << " (no peer connected?)\n";
            delete msg;   // 避免内存泄漏
            return;
        }

        for (int i = 0; i < n; ++i) {
            cGate *g = gate("out", i);
            if (g && g->isConnected()) {
                EV << "  -> send to peer via out[" << i << "]\n";
                send(msg->dup(), "out", i);
            } else {
                EV << "  -> out[" << i << "] not connected, skip\n";
            }
        }

        if(isPrimary){
            cModule *mgr = getParentModule()->getSubmodule("topoMgr");
            if (!mgr) {
                EV_WARN << "broadcastToPeers: topoMgr not found, skip\n";
            } else {
                // ⚠️ 一定 dup，因为下面会 delete msg
                cMessage *dupMsg = msg->dup();

                SendOptions opt;
                opt.propagationDelay(netDelay());

                EV << "  -> sendDirect to topoMgr via gate 'in' (5ms)\n";
                sendDirect(dupMsg, opt, mgr->gate("in"));
            }
        }

        delete msg; // 原消息删掉，dup 的交给 OMNeT++
    }


    void broadcastPrePrepare(VerifyMessage *vMsg)
    {
        if (vMsg == nullptr) {
            EV << "Error: broadcastPrePrepare got nullptr rMsg! Skip broadcast.\n";
            return; // 直接返回，不执行后续逻辑
        }

        seqNum++;
        auto *preMsg = new PrePrepareMsg();
        preMsg->setViewNum(viewNum);
        preMsg->setSeqNum(seqNum);
        preMsg->setRm(vMsg->getRm());
        preMsg->setState(vMsg->getState());

        EV << "Primary[" << OrderId << "] broadcast PrePrepare seq=" << seqNum << "\n";

        broadcast(preMsg);
    }

    void sendPrepare(PrePrepareMsg *preMsg)
    {
        int seq = preMsg->getSeqNum();

        // 去重：只处理一次相同 seq 的 PrePrepare
        if (preparedSeq.find(seq) == preparedSeq.end()) {
            preparedSeq.insert(seq);

            auto *prep = new PrepareMsg();
            prep->setViewNum(preMsg->getViewNum());
            prep->setSeqNum(seq);
            prep->setOrderId(OrderId);
            prep->setRm(preMsg->getRm());
            prep->setState(preMsg->getState());

            EV << "Node[" << OrderId << "] send Prepare seq=" << seq << "\n";
            broadcast(prep);
        }
    }

    void sendCommit(PrepareMsg *prepMsg)
    {
        int seq = prepMsg->getSeqNum();
        int sender = prepMsg->getOrderId();

        // 防止同一节点重复计数
        auto &vec = prepareCount[seq];
        if (std::find(vec.begin(), vec.end(), sender) == vec.end()) {
            vec.push_back(sender);
        }

        // 已经发送过 Commit 就跳过
        if (commitSeq.find(seq) != commitSeq.end())
            return;

        // 2f+1 Prepare 收集后，发送 Commit
        if (vec.size() >= (2 * ((totalNodes - 1) / 3))) {
            auto *cmt = new CommitMsg();
            cmt->setViewNum(prepMsg->getViewNum());
            cmt->setSeqNum(seq);
            cmt->setOrderId(OrderId);
            cmt->setRm(prepMsg->getRm());
            int temstate = prepMsg->getState();

            if(malice&&temstate==1){
                temstate = 2;
            }
            if(malice&&temstate==2){
                temstate = 1;
            }

            cmt->setState(temstate);

            EV << "Node[" << OrderId << "] collected enough Prepare, send Commit seq=" << seq << "\n";

            // ✅ 自己也先投 commit 票（关键）
            finalizeBlock(cmt->dup());

            broadcast(cmt);
            commitSeq.insert(seq); // 标记该 seq 已发送 Commit
        }
    }

    void finalizeBlock(CommitMsg *cmtMsg) {
        int seq = cmtMsg->getSeqNum();
        int sender = cmtMsg->getOrderId();
        const ReportMessage& rm = cmtMsg->getRm();
        int state = cmtMsg->getState();

        if (finalizedSeq.count(seq)) return;

        // 用 digest 区分“不同提案”
        std::string digest = rm.getHashValue();   // 或你自己的 requestDigest

        // 1) 计票：按 (seq, digest, state) 分桶
        auto &voters = commitVotes[seq][digest][state];
        voters.insert(sender);  // set 自动去重

        int f = (totalNodes - 1) / 3;
        int quorum = 2 * f + 1;

        // 2) 如果该桶达到 2f+1，就用这个 state finalize
        if ((int)voters.size() < quorum) return;

        // ======= finalize：用“达到阈值的 state” =======
        Block latestBlock = ledger.getLatestBlock();
        int currentHeight = latestBlock.blockheight;
        std::string currentHash = latestBlock.BlockHash;

        Report newReport;
        newReport.senderID  = rm.getSenderID();
        newReport.content   = rm.getContent();
        newReport.hashValue = rm.getHashValue();
        newReport.timestamp = rm.getTimestamp();

        if (state == 1) newReport.state = NodeState::JOINED;
        else if (state == 2) newReport.state = NodeState::LEFT;
        else newReport.state = NodeState::NEW; // 可选兜底

        // signature hex -> bin
        std::vector<unsigned char> binData = hexToBin(rm.getSignature());
        newReport.signature = binData;

        // reports 替换/新增
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

        finalizedSeq.insert(seq);

        EV_WARN << "Node[" << OrderId << "] FINALIZE seq=" << seq
                << " ✅ quorum=" << quorum
                << " digest=" << digest
                << " state=" << state << "\n";

        // 广播给 peers
        auto *blkMsg = new BlockMsg("block_broadcast_to_peers");
        blkMsg->setBlockNum(newBlock.blockheight);
        blkMsg->setRm(rm);
        blkMsg->setBlockTimestamp(simTime());
        blkMsg->setState(state);

        broadcastToPeers(blkMsg);
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

    void broadcast(cMessage *msg)
    {
        EV << "[Broadcast] time=" << simTime()
           << " msg=" << msg->getName()
           << " outSize=" << gateSize("orderout") << endl;

        for (int i = 0; i < gateSize("orderout"); ++i) {
            EV << "  -> sending to out[" << i << "] at t=" << simTime() << endl;
            send(msg->dup(), "orderout", i);
        }

        delete msg;
    }

};
Define_Module(Orderer);

