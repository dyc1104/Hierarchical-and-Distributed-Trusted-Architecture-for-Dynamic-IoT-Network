/*
 * TopologyManager.cc
 *
 *  Created on: Jan 21, 2026
 *      Author: exp
 */
#include "TopologyManager.h"

Define_Module(TopologyManager);

static std::vector<unsigned char> hexToBin(const std::string& hexString);
static std::string binToHex(const std::vector<unsigned char>& binData);

class DealJoinDoneEvent : public cMessage {
public:
    JoinReq *req = nullptr;          // 用来最后 delete（或你也可以不保存它，直接在 DealJoin 里 delete）
    int peerId = -1;
    int deviceId = -1;
    bool recorded = false;

    // ✅存数据
    ReportMessage rm;
    simtime_t sendTime;

    bool needForward = false;        // DEVICE_LAYER 才需要 forward

    DealJoinDoneEvent(JoinReq *r, bool forward,
                      int pid, int did, bool rec,
                      const ReportMessage& _rm,
                      simtime_t st)
        : cMessage("DealJoinDoneEvent"),
          req(r), peerId(pid), deviceId(did), recorded(rec),
          rm(_rm), sendTime(st), needForward(forward) {}
};


class DealPeerJoinDoneEvent : public cMessage {
public:
    PeerJoinReq *req = nullptr;        // 仍然可以保留指针，最后统一 delete
    int verifyPeerId = -1;
    int pid = -1;
    std::string sid;

    // 关键：存数据，不存 verifyReq 指针
    ReportMessage rm;
    simtime_t sendTime;

    bool needForward = false; // CONTROL_LAYER 才需要 forward

    DealPeerJoinDoneEvent(PeerJoinReq *r, bool forward,
                          int vpid, int joinPid,
                          const std::string& s,
                          const ReportMessage& _rm,
                          simtime_t st)
        : cMessage("DealPeerJoinDoneEvent"),
          req(r), verifyPeerId(vpid), pid(joinPid),
          sid(s), rm(_rm), sendTime(st), needForward(forward) {}
};


static simtime_t mapRealToSim(double realSeconds, double scale)
{
    double simSeconds = realSeconds * scale;

    // 防止 0（可选）：给一个最小延迟，避免完全看不到差异
    if (simSeconds < 1e-9) simSeconds = 1e-9;

    return SimTime(simSeconds, SIMTIME_S);
}

void TopologyManager::initialize()
{
    Block genesisBlock = Block::createGenesisBlock();
    ledger.addBlock(genesisBlock);

    peerNum = par("peer_num").intValue();
    deviceNum = par("dev_num").intValue();

    if (peerNum <= 0) EV_ERROR << "TopologyManager: peer_num must be > 0\n";
    if (deviceNum < 0) EV_ERROR << "TopologyManager: dev_num must be >= 0\n";

    // 初始时把所有 peer 都视作可用
    for (int i = 0; i < peerNum; ++i)
        if(i!=3){
        joinedPeerIdx.insert(i);
        }

    // 初始化 peerDevices 的 key
    for (int i = 0; i < peerNum; ++i)
        peerDevices[i] = {};

    EV_INFO << "TopologyManager init: peerNum=" << peerNum
            << " deviceNum=" << deviceNum << "\n";
}

simtime_t TopologyManager::netDelay() const {
    return uniform(0.01, 0.012); // 10–12ms
}

double TopologyManager::computeScore(int cpu, int io, int memory) const
{
    // Score = 0.5CPU + 0.2IO + 0.3Memory
    return 0.5 * cpu + 0.2 * io + 0.3 * memory;
}

TopologyManager::NodeClass TopologyManager::classifyNode(double score) const
{
    if (score < 2.0)
        return DEVICE_LAYER;
    else
        return CONTROL_LAYER;
}


int TopologyManager::nearestPeer(int deviceId) const
{
    if (peerNum <= 0 || joinedPeerIdx.empty())
        return -1;   // 表示没有可用 peer，调用方要处理

    // 1) 计算初始 peerId
    int base = deviceId % peerNum;
    if (base < 0) base += peerNum;

    // 2) 从 base 开始，循环查找可用 peer
    for (int offset = 0; offset < peerNum; ++offset) {
        int pid = (base + offset) % peerNum;
        if (joinedPeerIdx.count(pid) > 0) {
            return pid;
        }
    }

    // 3) 理论上不会走到这里（除非 joinedPeerIdx 为空）
    return -1;
}

void TopologyManager::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()){
        if (auto *evt = dynamic_cast<DealJoinDoneEvent*>(msg)) {

            if (!evt->needForward) {  // 非 DEVICE_LAYER：只延迟 delete
                delete evt->req;
                delete evt;
                return;
            }

            cModule *peerMod = getParentModule()->getSubmodule("peer", evt->peerId);
            if (!peerMod) {
                EV_ERROR << "TopoMgr: peer[" << evt->peerId << "] not found at send-time\n";
                if (evt->recorded) recordDeviceLeave(evt->deviceId);
                delete evt->req;
                delete evt;
                return;
            }

            // ✅重新组装 JoinVerifyReq
            auto *v = new JoinVerifyReq("JoinVerifyReq");
            v->setDeviceId(evt->deviceId);
            v->setAssignedPeerId(evt->peerId);
            v->setRm(evt->rm);
            v->setSendTime(evt->sendTime);

            SendOptions opt;
            opt.propagationDelay(netDelay());

            EV_INFO << "TopoMgr: (after cpuDelay) send JoinVerifyReq dev=" << evt->deviceId
                    << " -> peer[" << evt->peerId << "]\n";

            sendDirect(v, opt, peerMod->gate("join"));

            delete evt->req;
            delete evt;
            return;
        }


        if (auto *evt = dynamic_cast<DealPeerJoinDoneEvent*>(msg)) {

                if (!evt->needForward) {   // 非 CONTROL_LAYER：只延迟删除即可
                    delete evt->req;
                    delete evt;
                    return;
                }

                cModule *peerMod = getParentModule()->getSubmodule("peer", evt->verifyPeerId);
                if (!peerMod) {
                    EV_ERROR << "TopoMgr: peer[" << evt->verifyPeerId << "] not found at send-time\n";
                    if (!evt->sid.empty()) pendingPeerJoinBySid.erase(evt->sid);
                    delete evt->req;
                    delete evt;
                    return;
                }

                // ✅重新组装要发送的验证请求
                auto *v = new PeerJoinVerifyReq("PeerJoinVerifyReq");
                v->setPeerId(evt->pid);
                v->setRm(evt->rm);
                v->setSendTime(evt->sendTime);

                SendOptions opt;
                opt.propagationDelay(netDelay());

                EV_INFO << "TopoMgr: (after cpuDelay) forward PeerJoinVerifyReq(peerId="
                        << evt->pid << ") -> peer[" << evt->verifyPeerId << "]\n";

                // 建议加 gate 存在检查（防止 gate 名不对直接炸）
                cGate *g = peerMod->gate("join");
                if (!g) throw cRuntimeError("peer[%d] has no gate 'join'", evt->verifyPeerId);

                sendDirect(v, opt, g);

                delete evt->req;
                delete evt;
                return;
            }
    }

    // 1) device -> topoMgr : JoinReq
    if (auto *req = dynamic_cast<JoinReq*>(msg)) {
        DealJoin(req);   // 所有权交给 sendDirect，不 delete
        return;
    }

    if (auto *req = dynamic_cast<PeerJoinReq*>(msg)) {
        DealPeerJoin(req);   // 所有权交给 sendDirect，不 delete
        return;
    }

    if (auto *pl = dynamic_cast<PeerLeaveReq*>(msg)) {
        int pid = pl->getPeerId();
        std::string sid = pl->getSenderID();

        // 1) 从 topoMgr 本地账本/报告列表找出该 peer 的 ReportMessage
        ReportMessage rm;
        if (!findReportMessageBySid(sid, rm)) {
            EV_ERROR << "TopoMgr: cannot find report for sid=" << sid
                     << " (peerId=" << pid << "), cannot build leave tx\n";
            delete pl;
            return;
        }

        // 2) 记 pending：等区块确认后再真正 erase + 回执
        pendingPeerLeaveBySid[sid] = pid;

        // 3) 构造 VerifyMessage(state=2) 发给主 orderer
        auto *vm = new VerifyMessage("PeerLeaveVerify");
        vm->setRm(rm);
        vm->setState(2);

        cModule *ord0 = getParentModule()->getSubmodule("orderer_0");
        if (!ord0) {
            EV_ERROR << "TopoMgr: orderer_0 not found\n";
            pendingPeerLeaveBySid.erase(sid);
            delete vm;
            delete pl;
            return;
        }

        SendOptions opt;
        opt.propagationDelay(netDelay());

        EV_WARN << "TopoMgr: send peer leave tx to orderer_0, sid=" << sid
                << " peerId=" << pid << " at " << simTime() << "\n";

        // 发给 orderer_0 的 join 端口
        sendDirect(vm, opt, ord0->gate("join"));

        delete pl;
        return;
    }


    // if (auto *pl = dynamic_cast<PeerLeave*>(msg)) { recordPeerLeave(pl->getPeerId()); ... }

    if (auto *blk = dynamic_cast<BlockMsg*>(msg)) {
        handleBlockMsg(blk);
        delete blk;
        return;
    }

    EV_WARN << "TopologyManager: unknown msg type, drop\n";
    delete msg;
}


/*void TopologyManager::DealJoin(JoinReq *req)
{
    int cpu    = req->getCpu();
    int memory = req->getMemory();
    int io     = req->getIO();

    // ===== 1. 计算 Score 并分类 =====
    double score = computeScore(cpu, io, memory);
    NodeClass cls = classifyNode(score);



    // ===== 2. 按分类执行不同操作 =====

    // ---------- 情况 A：设备层（继续你原有逻辑，转发给 peer） ----------
    if (cls == DEVICE_LAYER) {
        int deviceId = req->getDeviceId();
        int peerId   = nearestPeer(deviceId);

        if (peerId < 0) {
            EV_ERROR << "TopoMgr: no available peer for device " << deviceId << "\n";
            delete req;
            return;
        }

        recordDeviceJoin(deviceId, peerId);

        EV_INFO << "TopoMgr: device " << deviceId
                << " CPU=" << cpu
                << " IO=" << io
                << " MEM=" << memory
                << " Score=" << score
                << " Class=" << (cls == DEVICE_LAYER ? "DEVICE_LAYER" : "CONTROL_LAYER")
                << "\n";

        auto *v = new JoinVerifyReq("JoinVerifyReq");
        v->setDeviceId(deviceId);
        v->setAssignedPeerId(peerId);
        v->setRm(req->getRm());
        v->setSendTime(req->getSendTime());

        cModule *peerMod = getParentModule()->getSubmodule("peer", peerId);
        if (!peerMod) {
            EV_ERROR << "TopologyManager: peer[" << peerId << "] not found\n";
            recordDeviceLeave(deviceId);
            delete v;
            delete req;
            return;
        }

        SendOptions opt;
        opt.propagationDelay(netDelay());

        EV_INFO << "TopoMgr: DEVICE_LAYER node "
                << deviceId << " -> peer[" << peerId << "] verify\n";

        sendDirect(v, opt, peerMod->gate("join"));
        delete req; // JoinReq 生命周期到此结束
            return;
    }

    else {
        delete req;
        return;
    }
}*/

void TopologyManager::DealJoin(JoinReq *req)
{
    // ====== 真实测时开始：我们把“处理逻辑”全部做完，但不立刻 sendDirect ======
    auto t1 = std::chrono::high_resolution_clock::now();

    int cpu    = req->getCpu();
    int memory = req->getMemory();
    int io     = req->getIO();

    double score = computeScore(cpu, io, memory);
    NodeClass cls = classifyNode(score);

    int deviceId = req->getDeviceId();
    int peerId   = nearestPeer(deviceId);

    if (peerId < 0) {
        EV_ERROR << "TopoMgr: no available peer for device " << deviceId << "\n";
        delete req;
        return;
    }

    // 你要求“发送 verifyreq 前更新映射”，可以保留
    recordDeviceJoin(deviceId, peerId);
    bool recorded = true;

    auto *v = new JoinVerifyReq("JoinVerifyReq");
    v->setDeviceId(deviceId);
    v->setAssignedPeerId(peerId);
    v->setRm(req->getRm());
    v->setSendTime(req->getSendTime());

    cModule *peerMod = getParentModule()->getSubmodule("peer", peerId);
    if (!peerMod) {
        EV_ERROR << "TopologyManager: peer[" << peerId << "] not found\n";
        if (recorded) recordDeviceLeave(deviceId);
        delete v;
        delete req;
        return;
    }

    // ====== 真实测时结束（到这里为止，等价于 DealJoin 的“计算/决策/构造消息”部分）======
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t2 - t1;
    double realSeconds = diff.count();

    double scale = 1;
    simtime_t delay = mapRealToSim(realSeconds, scale);

    EV_INFO << "TopoMgr: DealJoin real=" << realSeconds
            << "s scale=" << scale << " simDelay=" << delay
            << " (device=" << deviceId << " -> peer=" << peerId << ")\n";

    // ... 真实测时 diff -> delay

    bool forward = (cls == DEVICE_LAYER);

    // 你在 DealJoin 里已经算出来 peerId/deviceId 并 recordDeviceJoin 了
    scheduleAt(simTime() + delay,
        new DealJoinDoneEvent(req, forward, peerId, deviceId, recorded, req->getRm(), req->getSendTime()));

}


/*void TopologyManager::DealPeerJoin(PeerJoinReq *req)
{
    int cpu    = req->getCpu();
    int memory = req->getMemory();
    int io     = req->getIO();

    // ===== 1. 计算 Score 并分类 =====
    double score = computeScore(cpu, io, memory);
    NodeClass cls = classifyNode(score);



    // ===== 2. 按分类执行不同操作 =====

    // ---------- 情况 B：控制层 ----------
    if (cls == CONTROL_LAYER) {
        int pid = req->getPeerId();
        ReportMessage rm = req->getRm();
        simtime_t st = req->getSendTime();

        // 0) 如果 peer 已经是活跃的，就忽略/拒绝（防重复）
        if (joinedPeerIdx.find(pid) != joinedPeerIdx.end()) {
            EV_WARN << "TopoMgr: peer[" << pid << "] already joined, ignore PeerJoinReq\n";
            delete req;
            return;
        }

        // 1) 放入“预加入队列”
        std::string sid = rm.getSenderID();
        pendingPeerJoinBySid[sid] = pid;

        EV_INFO << "TopoMgr: peer[" << pid << "] pending-join, senderID="
                << rm.getSenderID() << " at " << simTime() << "\n";

        // 2) 构造 PeerJoinVerifyReq
        auto *v = new PeerJoinVerifyReq("PeerJoinVerifyReq");
        v->setPeerId(pid);
        v->setRm(rm);
        v->setSendTime(st);

        // 3) 固定发给 peer[1]
        int verifyPeerId = 1;
        cModule *peerMod = getParentModule()->getSubmodule("peer", verifyPeerId);
        if (!peerMod) {
            EV_ERROR << "TopoMgr: peer[" << verifyPeerId << "] not found, cannot verify peer join\n";
            pendingPeerJoinBySid.erase(sid);
            delete v;
            delete req;
            return;
        }

        SendOptions opt;
        opt.propagationDelay(netDelay());

        EV_INFO << "TopoMgr: forward PeerJoinVerifyReq(peerId=" << pid
                << ") -> peer[" << verifyPeerId << "]\n";

        sendDirect(v, opt, peerMod->gate("join")); // peer 的 join gate 负责收验证请求

        delete req;
        return;
    }
    else{
        delete req;
        return;
    }
}*/

void TopologyManager::DealPeerJoin(PeerJoinReq *req)
{
    auto t1 = std::chrono::high_resolution_clock::now();

    int cpu    = req->getCpu();
    int memory = req->getMemory();
    int io     = req->getIO();

    double score = computeScore(cpu, io, memory);
    NodeClass cls = classifyNode(score);

    int pid = req->getPeerId();
    ReportMessage rm = req->getRm();
    simtime_t st = req->getSendTime();

    // 0) 已经 join 过：拒绝
    if (joinedPeerIdx.find(pid) != joinedPeerIdx.end()) {
        EV_WARN << "TopoMgr: peer[" << pid << "] already joined, ignore PeerJoinReq\n";
        delete req;
        return;
    }

    // 1) 放 pending
    std::string sid = rm.getSenderID();
    pendingPeerJoinBySid[sid] = pid;

    // 2) 构造 PeerJoinVerifyReq
    auto *v = new PeerJoinVerifyReq("PeerJoinVerifyReq");
    v->setPeerId(pid);
    v->setRm(rm);
    v->setSendTime(st);

    // 3) 固定发给 peer[1]
    int verifyPeerId = 1;
    cModule *peerMod = getParentModule()->getSubmodule("peer", verifyPeerId);
    if (!peerMod) {
        EV_ERROR << "TopoMgr: peer[" << verifyPeerId << "] not found, cannot verify peer join\n";
        pendingPeerJoinBySid.erase(sid);
        delete v;
        delete req;
        return;
    }

    // === 真实测时结束：把这段真实耗时映射进仿真时间 ===
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t2 - t1;

    double scale = 1;
    simtime_t delay = mapRealToSim(diff.count(), scale);

    EV_INFO << "TopoMgr: peer[" << pid << "] pending-join sid=" << sid
            << " real=" << diff.count() << "s scale=" << scale
            << " simDelay=" << delay << "\n";

    bool forward = (cls == CONTROL_LAYER);
    // 如果非 CONTROL_LAYER，就 forward=false，其他字段随便填
    scheduleAt(simTime() + delay,
        new DealPeerJoinDoneEvent(req, forward, verifyPeerId, pid, sid, rm, st));
}



// ===== 维护记录的工具函数 =====

void TopologyManager::recordDeviceJoin(int deviceId, int peerId)
{
    joinedDeviceIdx.insert(deviceId);

    // 如果 device 已经在别的 peer 下，先移除旧关系
    auto it = deviceToPeer.find(deviceId);
    if (it != deviceToPeer.end() && it->second != peerId) {
        removeDeviceFromPeer(deviceId, it->second);
    }

    deviceToPeer[deviceId] = peerId;

    // 去重插入
    auto &vec = peerDevices[peerId];
    if (std::find(vec.begin(), vec.end(), deviceId) == vec.end())
        vec.push_back(deviceId);

    EV_INFO << "TopoMgr record: dev[" << deviceId << "] -> peer[" << peerId << "]\n";
}

void TopologyManager::removeDeviceFromPeer(int deviceId, int peerId)
{
    auto pit = peerDevices.find(peerId);
    if (pit == peerDevices.end()) return;

    auto &vec = pit->second;
    vec.erase(std::remove(vec.begin(), vec.end(), deviceId), vec.end());
}

void TopologyManager::recordDeviceLeave(int deviceId)
{
    joinedDeviceIdx.erase(deviceId);

    auto it = deviceToPeer.find(deviceId);
    if (it != deviceToPeer.end()) {
        int peerId = it->second;
        removeDeviceFromPeer(deviceId, peerId);
        deviceToPeer.erase(it);
        EV_INFO << "TopoMgr record: dev[" << deviceId << "] left (was under peer[" << peerId << "])\n";
    }
}

void TopologyManager::recordPeerLeave(int peerId)
{
    joinedPeerIdx.erase(peerId);

    // 以后你重组时要用：把 peerId 下的所有 device 重新分配到其它活跃 peer
    EV_WARN << "TopoMgr record: peer[" << peerId << "] left, devices need remap\n";
}

void TopologyManager::dumpMapping() const
{
    EV_INFO << "==== TopoMgr mapping dump ====\n";
    for (const auto &kv : peerDevices) {
        int peerId = kv.first;
        EV_INFO << "peer[" << peerId << "] has devices:";
        for (int d : kv.second) EV_INFO << " " << d;
        EV_INFO << "\n";
    }
    EV_INFO << "==============================\n";
}

void TopologyManager::handleBlockMsg(BlockMsg *blkMsg)
 {
     int blockNum = blkMsg->getBlockNum();
     ReportMessage rm = blkMsg->getRm();
     simtime_t ts = blkMsg->getBlockTimestamp();
     int state = blkMsg->getState();


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

     // ===== 区块确认 peer join：state==1 且 senderID 在 pendingPeerJoinBySid 里 =====
     if (state == 1) {
         std::string sid = rm.getSenderID();
         auto it = pendingPeerJoinBySid.find(sid);
         if (it != pendingPeerJoinBySid.end()) {
             int pid = it->second;

             // 1) 现在才真正把 peer 标记为活跃（加入集合）
             joinedPeerIdx.insert(pid);

             EV_WARN << "TopoMgr: peer join confirmed on-chain, peer[" << pid
                     << "] sid=" << sid << " at " << simTime() << "\n";

             // 2) 通知 peer join 成功
             auto *ack = new PeerJoinAck("PeerJoinAck");
             ack->setPeerId(pid);
             ack->setSuccess(true);
             ack->setTime(simTime());

             cModule *peerMod = getParentModule()->getSubmodule("peer", pid);
             if (!peerMod) {
                 EV_ERROR << "TopoMgr: peer[" << pid << "] not found for PeerJoinAck\n";
                 delete ack;
             } else {
                 SendOptions opt;
                 opt.propagationDelay(netDelay());

                 // peer 需要有一个 gate 接 topoMgr 控制消息，比如 rein
                 sendDirect(ack, opt, peerMod->gate("rein"));
             }

             // 3) 清 pending
             pendingPeerJoinBySid.erase(it);
         }
     }

     // ===== 区块确认 peer leave：state==2 且 senderID 在 pending 里 =====
     if (state == 2) {
         std::string sid = rm.getSenderID();
         auto it = pendingPeerLeaveBySid.find(sid);
         if (it != pendingPeerLeaveBySid.end()) {
             int pid = it->second;

             // 先取出该 peer 管辖的 devices（拷贝一份，防止后面修改结构）
             std::vector<int> managedDevs;
             auto pit = peerDevices.find(pid);
             if (pit != peerDevices.end()) {
                 managedDevs = pit->second;
             }

             // 1) 现在才真正从活跃集合中移除 peer
             joinedPeerIdx.erase(pid);

             // 2) 通知 peer 退出成功
             auto *ack = new PeerLeaveAck("PeerLeaveAck");
             ack->setSenderID(sid.c_str());
             ack->setPeerId(pid);
             ack->setAckTime(simTime());

             cModule *peerMod = getParentModule()->getSubmodule("peer", pid);
             if (!peerMod) {
                 EV_ERROR << "TopoMgr: peer[" << pid << "] not found for ack\n";
                 delete ack;
             } else {
                 SendOptions opt;
                 opt.propagationDelay(netDelay());
                 sendDirect(ack, opt, peerMod->gate("rein"));
             }

             // 3) 给该 peer 管辖的所有 device 发 ReJoin
             for (int devId : managedDevs) {
                 // 先把旧映射清理掉
                 recordDeviceLeave(devId);

                 auto *rj = new ReJoin("ReJoin");
                 rj->setDeviceId(devId);
                 rj->setSendTime(simTime());

                 cModule *devMod = getParentModule()->getSubmodule("dev", devId);
                 if (!devMod) {
                     EV_ERROR << "TopoMgr: dev[" << devId << "] not found for ReJoin\n";
                     delete rj;
                     continue;
                 }

                 SendOptions opt;
                 opt.propagationDelay(netDelay());

                 EV_WARN << "TopoMgr: trigger dev[" << devId << "] ReJoin due to peer[" << pid << "] leave\n";
                 sendDirect(rj, opt, devMod->gate("join"));  // ✅ device.join gate
             }

             // 4) 清 pending
             pendingPeerLeaveBySid.erase(it);
         }
     }
 }

bool TopologyManager::findReportMessageBySid(const std::string& sid, ReportMessage& outRm) const
{
    for (const auto& r : reports) {
        if (r.senderID == sid) {
            outRm.setSenderID(r.senderID.c_str());
            outRm.setContent(r.content.c_str());
            outRm.setHashValue(r.hashValue.c_str());
            outRm.setTimestamp(r.timestamp);
            outRm.setSenderpublickey(r.publickey.c_str());
            outRm.setSignature(binToHex(r.signature).c_str());
            return true;
        }
    }
    return false;
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



