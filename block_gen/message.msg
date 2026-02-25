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
namespace omnetpp;
message ReportMessage{
    string SenderID;          // 发送者ID
    string Content;           // 报告内容
    string HashValue;         // 哈希值
    long Timestamp;           // 时间戳
    string Signature;
    string Senderpublickey;
}

// =======================
// Join / Leave control messages
// =======================

// device -> TopoManager：加入请求（携带设备ID + 报告）
message JoinReq {
    int deviceId;
    ReportMessage rm;
    int cpu;
    int memory;
    int IO;
    simtime_t sendTime;   // 发送时刻（用于统计加入耗时）
}

// TopoManager -> nearest peer：验证请求（把加入请求转发给最近 peer）
message JoinVerifyReq {
    int deviceId;
    int assignedPeerId;   // 最近 peer id（deviceId % peer_num）
    ReportMessage rm;
    simtime_t sendTime;   // 保留原始发送时刻（便于链路端到端统计）
}

message JoinAck {
    int deviceId;
    int peerId;
    bool success;
    simtime_t time;
}

message PeerJoinReq {
    int peerId;          // 3
    ReportMessage rm;
    int cpu;
    int memory;
    int IO;
    simtime_t sendTime;
}


message PeerJoinVerifyReq {
    int peerId;
    ReportMessage rm;
    simtime_t sendTime;   // 保留原始发送时刻（便于链路端到端统计）
}

message PeerJoinAck {
    int peerId;
    bool success;
    simtime_t time;
}


message SyncLedgerReq {
    int peerId;
    int fromHeight;
    simtime_t sendTime;
}

message ReportEntry {
    ReportMessage rm;
    int state;   // 1=JOINED, 2=LEFT, 0/其他=NEW（按你现有映射）
}

message SyncBlockMsg {
    int peerId;
    int blockNum;
    string previousHash;
    string blockHash;
    long timestamp;
    int lastHeight;              // orderer 告诉对方目前链最新高度，便于判断何时结束
    ReportEntry entries[];       // 这个 block 的全部 reports（全量快照）
}

// device -> TopoManager：退出请求（可选）
message LeaveReq {
    string SenderID;
    int deviceId;
    simtime_t sendTime;
}

// TopoManager -> device：退出确认（可选）
message LeaveAck {
    string SenderID;
    int deviceId;
    simtime_t ackTime;
}

// peer -> TopoManager：退出请求（可选）
message PeerLeaveReq {
    string SenderID;
    int peerId;
    simtime_t sendTime;
}

message ReJoin {
    int deviceId;
    simtime_t sendTime;   // 发送时刻（用于统计加入耗时）
}

// TopoManager -> peer：退出确认（可选）
message PeerLeaveAck {
    string SenderID;
    int peerId;
    simtime_t ackTime;
}

message VerifyMessage{
    ReportMessage rm;
    int state;
}

message PrePrepareMsg {
    int viewNum;
    int seqNum;
    ReportMessage rm;
    int state;
}

message PrepareMsg {
    int viewNum;
    int seqNum;
    int orderId;
    ReportMessage rm;
    int state;
}

message CommitMsg {
    int viewNum;
    int seqNum;
    int orderId;
    ReportMessage rm;
    int state;
}

message BlockMsg {
    int blockNum;
    ReportMessage rm;
    simtime_t BlockTimestamp;
    int state;
}
