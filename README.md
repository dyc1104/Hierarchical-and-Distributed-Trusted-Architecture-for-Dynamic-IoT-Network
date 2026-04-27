# Hierarchical-and-Distributed-Trusted-Architecture-for-Dynamic-IoT-Network

论文整体思路：
物联网设备数量激增，频繁加入退出网络引入安全问题，需要对设备的可信状态进行持续的管理。传统集中式效率低且与物联网天然的分布式结构不适配。

研究人员考虑引入区块链与分层想法。在此基础上提出基于区块链的分层架构，并进一步的借助可信平台模块作为可信根，设计管理节点加入、退出以及周期性验证节点可信状态的管理机制。

除此之外，为了保护分层架构下节点信息传输过程中的隐私，设计了结合无证书公钥密码体制与假名的匿名聚合签名机制。在提升海量设备并发验证效率的同时，保护节点身份与具体的硬件信息。

***论文章节介绍***

1. introduction

    论文背景介绍，与整体思路基本一致

2. Related Work

    现有研究综述，分为框架部分与签名算法部分

3. System Model Description

    层次化分布式架构的形式化描述。控制层、设备层、KGC、CA、IPFS

4. Anonymous Aggregated Verification

    匿名聚合签名流程介绍。分为TPM信息采集、隐私凭证生成、可信报告生成、聚合签名生成

    具体流程可以看图

5. Dynamic Node Management Mechanisms

    节点动态管理机制介绍。分为节点加入、节点退出与节点可信状态更新。

    具体流程可以看图

6. Security And Efficiency Analysis

    安全性证明与效率分析。分为理论效率分析和安全性证明。
    
    安全性证明从三个方面展开：确保隐私凭证由可信CA签发；确保可信报告来自正确设备层节点；确保可信报告没有被篡改。

7. Experiment Results
    
    软硬件结合实验环境介绍与实验结果。
