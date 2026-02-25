simple device
{
    parameters:
        int device_id;
        int clusterSize = default(1);

        bool enableVerify = default(true);
        int verifyTime = default(20);   // 10s 触发发送验证报告

        bool enableExit = default(false);
        int exitTime = default(50);

        @display("i=device/pc_s,black");
        //bool sendMsgOninit = default(false);
    gates:
        input in[];
        output out[];

        input join @loose;
}

simple Peer
{
    parameters:
        int peer_id;
        @display("i=device/server,blue");
    gates:
        input in[];
        //input in[1];
        output out[];

        input join @loose;
        input rein @loose;
}

simple Orderer
{
    parameters:
        int orderId;
        bool isPrimary;
        @display("i=device/server2,red");
    gates:
        input orderin[];//n-1
        output orderout[];
        input join @loose;
        input syn @loose;
}

simple TopologyManager
{
    parameters:
        int peer_num;
        int dev_num;
        @display("i=block/control,green");
    gates:
        input join @loose;
        input in @loose;
}

simple Orderer_P extends Orderer
{
    parameters:
        int totalNodes ;
    gates:
        input in[];
        output out[];
}

simple Orderer_nP extends Orderer
{
    parameters:
        int totalNodes ;
    gates:
        input in[];
        output out[];
}

network block_gen
{
    parameters:
        int dev_num = default(10);
        int peer_num = default(5);
        int orderer_num = default(5);
    types:
        channel DChannel extends ned.DelayChannel { delay = uniform(10ms, 12ms); }
        channel Channel extends ned.DelayChannel { delay = uniform(10ms, 12ms); }
    submodules:
        dev[dev_num] : device { device_id = index; clusterSize = 1;}
        peer[peer_num] : Peer { peer_id = index; }
        topoMgr: TopologyManager { peer_num = parent.peer_num; dev_num = parent.dev_num;}

        orderer_0 : Orderer_P {
            orderId = 0;
            isPrimary = true;
            totalNodes = parent.orderer_num;
        }
        orderer_np[orderer_num-1] : Orderer_nP {
            orderId = index + 1;
            isPrimary = false;
            totalNodes = parent.orderer_num;
        }

    connections:
        // 1) device -> peer：轮询均匀分配
        for d=0..dev_num-1 {
            dev[d].out++ --> DChannel --> peer[d % peer_num].in++;
        }

        // 2) peer -> orderer：均匀分配给 3 个 orderer（每个 50 个）
        for k=0..peer_num-1 {
            peer[k].out++ --> Channel --> orderer_0.in++ if (k % orderer_num) == 0;
            peer[k].out++ --> Channel --> orderer_np[(k % orderer_num) - 1].in++ if (k % orderer_num) != 0;

    		orderer_0.out++ --> Channel --> peer[k].in++ if (k % orderer_num) == 0;
    		orderer_np[(k % orderer_num) - 1].out++ --> Channel --> peer[k].in++ if (k % orderer_num) != 0;
        }

        // 3) 主 <-> 从：共识端口 orderin/orderout
        for i=0..orderer_num-2 {
            orderer_0.orderout++ --> Channel --> orderer_np[i].orderin++;
            orderer_np[i].orderout++ --> Channel --> orderer_0.orderin++;
        }

        // 4) 从节点之间全互联
        for i=0..orderer_num-2, for j=0..orderer_num-2 {
            orderer_np[i].orderout++ --> Channel --> orderer_np[j].orderin++ if i != j;
        }
}


