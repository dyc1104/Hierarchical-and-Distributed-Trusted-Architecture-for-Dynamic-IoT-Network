simple device
{
    parameters:
        int device_id;
        int clusterSize = default(1);
        @display("i=device/pc_s,black");
        //bool sendMsgOninit = default(false);
    gates:
        input in[];
        output out[1];
}
simple Center
{
    parameters:
        int center_id;
        @display("i=device/server,blue");
    gates:
        input in[100];
        output out[];
}
network centralized
{
    parameters:
        int dev_num = default(100);
        int cen_num = default(1);
    types:
        channel Channel extends ned.DelayChannel{
            delay = 5ms;
        }
    submodules:
        dev[dev_num] : device{
            device_id = index;
            clusterSize = 20;
        }
        center_1 : Center{
            center_id = 1;
        }
     connections:
         for i=0..dev_num-1 {
             dev[i].out[0] --> Channel --> center_1.in[i];
         }
}