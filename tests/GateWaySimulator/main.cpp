#include "libSMGS.h"
#include "stdio.h"
#include "stdlib.h"
#include "argtable3.h"
#include "MQTTClient.h"
#include "libSMGS.h"
#include "time.h"
#include<chrono>
#include<thread>
#include<string>


/*
宏定义类型转换
*/
#define STRING(x)  #x
#define STR2(R) STRING(R)


/*
全局变量
*/
const char * TAG="GateWaySimulator";
char *MQTTBrokerServerAddr=NULL;
int  MQTTBrokerServerPort=1883;
std::string GateWaySerialNumber;
struct Network mqttserver= {0};
struct MQTTClient mqttclient= {0};
uint8_t mqtttxbuff[4096]= {0};
uint8_t mqttrxbuff[4096]= {0};
const int keepalive=120;

/*
CLI程序命令参数表
*/
static struct arg_str *arghost=arg_strn("h","host","ServerAddr",1,1,"MQTT Broker服务器地址");
static struct arg_int *argport=arg_intn("p", "port","ServerPort",1,1,"MQTT Broker服务器端口");
static struct arg_lit *help=arg_lit0(NULL,"help", "打印帮助");
static struct arg_str *argserialnumber=arg_strn("s","serialnumber","SerialNumber",0,1,"网关序列号（最少" STR2(CONFIG_SMGS_MIN_GATEWAY_SERIALNUMBER_LENGTH) "字节)");

static void * argtable[]=
{
    arghost,
    argport,
    argserialnumber,
    help,
    arg_end(20),
};

static void argtable_parse_arg(int argc,char *argv[])
{
    int nerrors = arg_parse(argc,argv,argtable);
    if(nerrors>0 || help->count>0)
    {
        if(nerrors>0 && help->count == 0)
        {
            printf("命令行参数有误!\r\n");
        }
        printf("%s:\r\n","帮助");
        printf("%s ",argv[0]);
        arg_print_syntax(stdout,argtable,"\r\n");
        arg_print_glossary(stdout,argtable,"%-40s %s\n");
        //退出程序
        exit(0);
    }


    if(arghost->count>0)
    {
        MQTTBrokerServerAddr=(char *)*arghost->sval;
    }

    if(argport->count>0)
    {
        MQTTBrokerServerPort=*argport->ival;
    }

    if(argserialnumber->count>0)
    {
        GateWaySerialNumber=*argserialnumber->sval;
        {
            //用0补足缺失的字节
            while(GateWaySerialNumber.length()<CONFIG_SMGS_MIN_GATEWAY_SERIALNUMBER_LENGTH)
            {
                GateWaySerialNumber+="0";
            }
        }
    }
    else
    {
        printf("未设置网关序列号,将自动生成序列号!\r\n");
        GateWaySerialNumber="Simulate";
        {
            //将时间戳作为后缀
            time_t current_time=time(NULL);
            char buff[10]= {0};
            sprintf(buff,"%08X",(uint32_t)current_time);
            GateWaySerialNumber+=buff;
        }

        {
            //用0补足缺失的字节
            while(GateWaySerialNumber.length()<CONFIG_SMGS_MIN_GATEWAY_SERIALNUMBER_LENGTH)
            {
                GateWaySerialNumber+="0";
            }
        }
    }

}




/*
MQTT消息回调
*/
extern SMGS_gateway_context_t gateway_context;
static void mqttmessageHandler(struct MessageData*msg)
{
    std::string topic(msg->topicName->lenstring.data,msg->topicName->lenstring.len);
    printf("%s:topic=%s,qos=%d,retain=%d\r\n",TAG,topic.c_str(),msg->message->qos,msg->message->retained);

    uint8_t buff[4096]= {0};
    SMGS_GateWay_Receive_MQTT_MSG(&gateway_context,msg->topicName->lenstring.data,msg->topicName->lenstring.len,(uint8_t *)msg->message->payload,msg->message->payloadlen,msg->message->qos,msg->message->retained,buff,sizeof(buff));
}

/*
MQTT Ping线程
*/

//执行ping
static bool mqtt_ping(MQTTClient * client)
{

    if(client==NULL || client->buf ==NULL || client->buf_size ==0 || client->ipstack==NULL || client->ipstack->mqttwrite==NULL)
    {
        return false;//参数有误
    }

    uint8_t buff[8]= {0};
    int len=MQTTSerialize_pingreq(buff,sizeof(buff));
    return len==client->ipstack->mqttwrite(client->ipstack,buff,len,1000);
}
static void MQTT_Ping_Thread()
{
    printf("MQTT Ping线程已启动!!\r\n");
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(keepalive));
        if(mqttclient.isconnected)
        {
            mqtt_ping(&mqttclient);
        }
    }
}

/*
协议栈相关
*/

SMGS_device_context_t device_context;

bool  SMGS_Device_IsOnline(SMGS_device_context_t *ctx)
{
    //默认返回真
    return true;
}

bool SMGS_Device_Command(SMGS_device_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_cmdid_t *cmdid,uint8_t *cmddata,size_t cmddata_length,uint8_t *retbuff,size_t *retbuff_length,SMGS_payload_retcode_t *ret)
{
    bool _ret=false;
    printf("%s:设备特殊命令(CmdID=%04X)\r\n",TAG,(uint32_t)(*cmdid));
    return _ret;
}

bool SMGS_Device_ReadRegister(SMGS_device_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag)
{
    bool ret=false;
    printf("%s:设备读取寄存器(Addr=%04X)\r\n",TAG,(uint32_t)addr);
    return ret;
}

bool SMGS_Device_WriteRegister(SMGS_device_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag)
{
    bool ret=false;
    printf("%s:设备写入寄存器(Addr=%04X,Data=%016llX,Flag=%02X)\r\n",TAG,(uint32_t)addr,(*dat),(uint32_t)(flag->val));
    return ret;
}

bool SMGS_Device_ReadSensor(SMGS_device_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_sensor_address_t addr,uint64_t *dat,SMGS_payload_sensor_flag_t *flag)
{
    bool ret=false;
    printf("%s:设备读取传感器(Addr=%04X,Flag=%02X)\r\n",TAG,(uint32_t)addr,(uint32_t)(flag->val));
    return ret;
}



SMGS_gateway_context_t gateway_context;

bool SMGS_GateWay_Command(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_cmdid_t *cmdid,uint8_t *cmddata,size_t cmddata_length,uint8_t *retbuff,size_t *retbuff_length,SMGS_payload_retcode_t *ret)
{
    bool _ret=false;
    printf("%s:网关特殊命令(CmdID=%04X)\r\n",TAG,(uint32_t)(*cmdid));
    return _ret;
}

bool SMGS_GateWay_ReadRegister(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag)
{
    bool ret=false;
    printf("%s:网关读取寄存器(Addr=%04X)\r\n",TAG,(uint32_t)addr);
    return ret;
}

bool SMGS_GateWay_WriteRegister(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag)
{
    bool ret=false;
    printf("%s:网关写入寄存器(Addr=%04X,Data=%016llX,Flag=%02X)\r\n",TAG,(uint32_t)addr,(*dat),(uint32_t)(flag->val));
    return ret;
}

bool SMGS_GateWay_ReadSensor(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_sensor_address_t addr,uint64_t *dat,SMGS_payload_sensor_flag_t *flag)
{
    bool ret=false;
    printf("%s:网关读取传感器(Addr=%04X,Flag=%02X)\r\n",TAG,(uint32_t)addr,(uint32_t)(flag->val));
    return ret;
}


//设备查询函数
SMGS_device_context_t * SMGS_Device_Next(struct __SMGS_gateway_context_t *ctx,SMGS_device_context_t * devctx)
{
    if(devctx==NULL)
    {
        return &device_context;//返回第一个设备
    }

    //由于只有一个设备，直接返回NULL

    return NULL;
}


static bool SMGS_MessagePublish(struct __SMGS_gateway_context_t *ctx,const char * topic,void * payload,size_t payloadlen,uint8_t qos,int retain)
{
    if(mqttclient.isconnected==0)
    {
        return false;
    }

    QoS Qos=QOS0;
    switch(qos)
    {
    default:
        break;
    case 0:
        Qos=QOS0;
        break;
    case 1:
        Qos=QOS1;
        break;
    case 2:
        Qos=QOS2;
        break;

    }

    MQTTMessage msg;
    memset(&msg,0,sizeof(msg));
    msg.payload=payload;
    msg.payloadlen=payloadlen;
    msg.qos=Qos;
    msg.retained=retain;
    return MQTTPublish(&mqttclient,topic,&msg)==0;
}

/*
主程序
*/
int main(int argc,char *argv[])
{
    //关闭输出缓冲
    setbuf(stdout,NULL);

    //检查命令参数
    argtable_parse_arg(argc,argv);

    //启动MQTT Ping线程
    std::thread ping(MQTT_Ping_Thread);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));


    {
        //初始化设备上下文
        SMGS_Device_Context_Init(&device_context);

        //填写设备上下文
        device_context.DeviceName=TAG;
        device_context.DevicePosNumber=1;
        device_context.DeviceSerialNumber=GateWaySerialNumber.c_str();//默认序列号同网关
        device_context.IsOnline=SMGS_Device_IsOnline;
        device_context.Command=SMGS_Device_Command;
        device_context.ReadRegister=SMGS_Device_ReadRegister;
        device_context.WriteRegister=SMGS_Device_WriteRegister;
        device_context.ReadSensor=SMGS_Device_ReadSensor;

    }

    {

        //初始化网关上下文
        SMGS_GateWay_Context_Init(&gateway_context,GateWaySerialNumber.c_str(),SMGS_MessagePublish);

        //填写网关上下文
        gateway_context.GateWayName=TAG;
        gateway_context.Command=SMGS_GateWay_Command;
        gateway_context.ReadRegister=SMGS_GateWay_ReadRegister;
        gateway_context.WriteRegister=SMGS_GateWay_WriteRegister;
        gateway_context.ReadSensor=SMGS_GateWay_ReadSensor;
        gateway_context.Device_Next=SMGS_Device_Next;
    }

    while(true)
    {
        printf("%s:序列号是%s!!\r\n",TAG,gateway_context.GateWaySerialNumber);

        //MQTT连接
        printf("%s:MQTT连接开始(%s:%d)!!\r\n",TAG,MQTTBrokerServerAddr,MQTTBrokerServerPort);


        NetworkInit(&mqttserver);

        while(0!=NetworkConnect(&mqttserver,MQTTBrokerServerAddr,MQTTBrokerServerPort))
        {
            printf("%s:连接MQTT中!\r\n",TAG);
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }

        MQTTClientInit(&mqttclient,&mqttserver,3000,mqtttxbuff,sizeof(mqtttxbuff),mqttrxbuff,sizeof(mqttrxbuff));


        MQTTPacket_connectData cfg=MQTTPacket_connectData_initializer;

        //使用keepalive选项
        cfg.keepAliveInterval=keepalive;

        //填写clientID
        cfg.clientID.cstring=(char *)GateWaySerialNumber.c_str();

        //填写cleansession
        cfg.cleansession=1;

        //填写用户名与密码
        cfg.username.cstring=(char *)GateWaySerialNumber.c_str();
        cfg.password.cstring=(char *)GateWaySerialNumber.c_str();

        //填写will
        uint8_t willbuff[256]= {0};
        SMGS_gateway_will_t will= {0};
        SMGS_GateWay_Will_Encode(&gateway_context,&will,willbuff,sizeof(willbuff));

        cfg.will.topicName.cstring=(char *)will.topic;
        cfg.will.qos=will.qos;
        cfg.will.message.lenstring.data=(char *)will.payload;
        cfg.will.message.lenstring.len=will.payloadlen;
        cfg.will.retained=will.ratain;
        cfg.willFlag=1;


        if(SUCCESS!=MQTTConnect(&mqttclient,&cfg))
        {
            printf("%s:MQTT连接失败!!\r\n",TAG);
            NetworkDisconnect(&mqttserver);
            continue;
        }

        //订阅主题
        std::string subtoic=GateWaySerialNumber+"/#";
        if(SUCCESS!=MQTTSubscribe(&mqttclient,subtoic.c_str(),QOS0,mqttmessageHandler))
        {
            printf("%s:MQTT订阅失败!!\r\n",TAG);
            NetworkDisconnect(&mqttserver);
            continue;
        }

        printf("%s:连接成功!!\r\n",TAG);
        {
            //发送网关上线消息
            uint8_t buff[512]= {0};
            SMGS_GateWay_Online(&gateway_context,buff,sizeof(buff),0,0);
        }

        {
            while(true)
            {
                if(SUCCESS!=MQTTYield(&mqttclient,10))
                {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }


        printf("%s:MQTTYield执行失败!!重启中!!!\r\n",TAG);

        //将连接标志置为0
        mqttclient.isconnected=0;

        if(mqttserver.disconnect!=NULL)
        {
            mqttserver.disconnect(&mqttserver);
        }

    }

    return 0;
}
