/*
 * TopologyManager.h
 *
 *  Created on: Jan 21, 2026
 *      Author: exp
 */

#ifndef TOPOLOGYMANAGER_H_
#define TOPOLOGYMANAGER_H_

#include <omnetpp.h>
#include "message_m.h"
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
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include "Block.h"
#include "Ledger.h"
#include "Report.h"


using namespace omnetpp;

class TopologyManager : public cSimpleModule
{
  protected:
    int peerNum = 0;
    int deviceNum = 0;
    Ledger ledger;
    std::vector<Report> reports;

    enum NodeClass {
        DEVICE_LAYER = 0,
        CONTROL_LAYER = 1
    };

    // ===== 记录：活跃/已加入的 peer、device =====
    std::unordered_set<int> joinedPeerIdx;     // 已加入/可用的 peer index 集合
    std::unordered_set<int> joinedDeviceIdx;   // 已加入的 device index 集合

    // ===== 映射：peer -> devices；device -> peer =====
    std::unordered_map<int, std::vector<int>> peerDevices;  // peerIdx -> deviceIdx list
    std::unordered_map<int, int> deviceToPeer;              // deviceIdx -> peerIdx

    std::unordered_map<std::string, int> pendingPeerJoinBySid;
    std::unordered_map<std::string, int> pendingPeerLeaveBySid;


  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    int nearestPeer(int deviceId) const;
    void DealJoin(JoinReq *req);
    void DealPeerJoin(PeerJoinReq *req);
    double computeScore(int cpu, int io, int memory) const;
    NodeClass classifyNode(double score) const;
    simtime_t netDelay() const;

    // ===== 新增：维护记录的工具函数 =====
    void recordDeviceJoin(int deviceId, int peerId);
    void recordDeviceLeave(int deviceId);
    void recordPeerLeave(int peerId);          // peer 退出（后续重组用）
    void removeDeviceFromPeer(int deviceId, int peerId);
    void handleBlockMsg(BlockMsg *blkMsg);
    bool findReportMessageBySid(const std::string& sid, ReportMessage& outRm) const;
  public:
    // 可选：给你调试用
    void dumpMapping() const;
};

#endif /* TOPOLOGYMANAGER_H_ */
