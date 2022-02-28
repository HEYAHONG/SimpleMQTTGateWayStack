#include "MQTTClientThread.h"
#include "InternalDatabase.h"
#include "libSMGS-Server.h"
#include "wx/log.h"

static void mqttconnectstatechange(bool IsConnect)
{
    wxLogMessage(_T("MQTT客户端默认回调:%s"),IsConnect?_T("已连接"):_T("未连接"));
}

static void __OnMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain)
{
    wxLogMessage(_T("MQTT客户端默认消息回调:%s"),topic.ToAscii());
}

MQTTClientThread::MQTTClientThread(): wxThread(wxTHREAD_DETACHED)
{
    //ctor
    memset(&mqttclient,0,sizeof(mqttclient));
    memset(&mqttserver,0,sizeof(mqttserver));
    memset(mqttrxbuff,0,sizeof(mqttrxbuff));
    memset(mqtttxbuff,0,sizeof(mqtttxbuff));

    NetworkInit(&mqttserver);
    MQTTClientInit(&mqttclient,&mqttserver,3000,mqtttxbuff,sizeof(mqtttxbuff),mqttrxbuff,sizeof(mqttrxbuff));

    //标志位清零
    IsRunning=false;
    IsPendingStart=false;
    IsPendingStop=false;

    //设定默认回调
    ConnectStateChange_Callback=mqttconnectstatechange;
    OnMessage=__OnMessage;

}

void MQTTClientThread::MQTT_StartConnect(wxString _Server,wxString _Port,wxString _Name)
{
    if(_Name.empty())
    {
        Name=InternalDatebase_ProgramInfo_Get(_T("Name"));
    }
    else
    {
        Name=_Name;
    }

    if(_Server.empty())
    {
        ServerAddr=InternalDatebase_ProgramInfo_Get(_T("Server"));
    }
    else
    {
        ServerAddr=_Server;
    }

    if(_Port.empty())
    {
        ServerPort=InternalDatebase_ProgramInfo_Get(_T("Port"));
    }
    else
    {
        ServerPort=_Port;
    }



    IsPendingStop=false;
    IsPendingStart=true;
}

bool MQTTClientThread::MQTT_IsConnected()
{
    return MQTTIsConnected(&mqttclient);
}

bool MQTTClientThread::MQTT_Ping()
{
    if( mqttclient.buf ==NULL || mqttclient.buf_size ==0 || mqttclient.ipstack==NULL || mqttclient.ipstack->mqttwrite==NULL)
    {
        return false;//参数有误
    }

    if(!MQTT_IsConnected())
    {
        return false;
    }

    uint8_t buff[8]= {0};
    int len=MQTTSerialize_pingreq(buff,sizeof(buff));
    return len==mqttclient.ipstack->mqttwrite(mqttclient.ipstack,buff,len,1000);
}

bool MQTTClientThread::MQTT_Publish(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain)
{
    if( mqttclient.buf ==NULL || mqttclient.buf_size ==0 || mqttclient.ipstack==NULL || mqttclient.ipstack->mqttwrite==NULL)
    {
        return false;//参数有误
    }

    if(!MQTT_IsConnected())
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

    wxMutexLocker Lock(MQTTLock);

    return MQTTPublish(&mqttclient,topic,&msg)==0;

}

void MQTTClientThread::MQTT_StopConnect()
{
    IsPendingStop=true;
    IsPendingStart=false;
}

void MQTTClientThread::SetConnectStateCallback(std::function<void(bool)> cb)
{
    if(cb!=NULL)
    {
        ConnectStateChange_Callback=cb;
    }
    else
    {
        ConnectStateChange_Callback=mqttconnectstatechange;
    }
}

void MQTTClientThread::SetOnMessageCallback(std::function<void(wxString,void *,size_t,uint8_t,int)> cb)
{
    if(cb!=NULL)
    {
        OnMessage=cb;
    }
    else
    {
        OnMessage=__OnMessage;
    }
}

void mqttmessageHandler(struct MQTTClient *client,struct MessageData*msg)
{
    if(client==NULL || client->UserPtr==NULL)
    {
        return;
    }

    wxString topic(msg->topicName->lenstring.data,msg->topicName->lenstring.len);

    MQTTClientThread &self=*(MQTTClientThread *)client->UserPtr;

    if(self.OnMessage!=NULL)
    {
        self.OnMessage(topic,msg->message->payload,msg->message->payloadlen,msg->message->qos,msg->message->retained);
    }
}


MQTTClientThread::ExitCode MQTTClientThread::Entry()
{
    const char *TAG="MQTTClient";

    while(!TestDestroy())
    {
        if(!IsPendingStart)
        {
            //未启动
            This()->Sleep(1);
            continue;
        }

        IsRunning=false;
        if(ConnectStateChange_Callback!=NULL)
            ConnectStateChange_Callback(IsRunning);

        NetworkInit(&mqttserver);

        {
            long port=1883;
            ServerPort.ToCLong(&port);

            while(0!=NetworkConnect(&mqttserver,(char *)(const char *)ServerAddr.ToAscii(),port))
            {
                wxLogMessage(_T("%s:连接MQTT中(%s:%s)!"),TAG,ServerAddr.ToAscii(),ServerPort.ToAscii());
                This()->Sleep(3000);
            }
        }

        MQTTClientInit(&mqttclient,&mqttserver,3000,mqtttxbuff,sizeof(mqtttxbuff),mqttrxbuff,sizeof(mqttrxbuff));

        mqttclient.UserPtr=this;//设定指针,用于接收订阅消息

        MQTTPacket_connectData cfg=MQTTPacket_connectData_initializer;

        //使用keepalive选项
        cfg.keepAliveInterval=120;

        //填写clientID
        cfg.clientID.cstring=(char *)(const char *)Name.ToAscii();

        //填写cleansession
        cfg.cleansession=1;

        //填写用户名与密码
        cfg.username.cstring=(char *)(const char *)Name.ToAscii();
        cfg.password.cstring=(char *)(const char *)Name.ToAscii();


        if(SUCCESS!=MQTTConnect(&mqttclient,&cfg))
        {
            wxLogMessage(_T("%s:MQTT连接失败!!"),TAG);
            NetworkDisconnect(&mqttserver);
            continue;
        }

        IsRunning=true;
        if(ConnectStateChange_Callback!=NULL)
            ConnectStateChange_Callback(IsRunning);

        //订阅主题
        wxString subtoic=Name+"/#";
        if(SUCCESS!=MQTTSubscribe(&mqttclient,subtoic,QOS0,mqttmessageHandler))
        {
            wxLogMessage(_T("%s:MQTT订阅失败!!"),TAG);
            NetworkDisconnect(&mqttserver);
            continue;
        }

        if(SUCCESS!=MQTTSubscribe(&mqttclient,CONFIG_SMGS_SERVER_DEFAULT_NAME"/#",QOS0,mqttmessageHandler))
        {
            wxLogMessage(_T("%s:MQTT订阅失败!!"),TAG);
            NetworkDisconnect(&mqttserver);
            continue;
        }

        wxLogMessage(_T("%s:连接成功!!"),TAG);
        {
            while(!IsPendingStop)
            {
                if(SUCCESS!=MQTTYield(&mqttclient,10))
                {
                    wxLogMessage(_T("%s:MQTTYield执行失败!!重启中!!!"),TAG);
                    break;
                }
                This()->Sleep(1);
            }
        }

        if(IsPendingStop)
        {
            wxLogMessage(_T("%s:已断开连接!!"),TAG);
            IsRunning=false;
            if(ConnectStateChange_Callback!=NULL)
                ConnectStateChange_Callback(IsRunning);
        }


        //将连接标志置为0
        mqttclient.isconnected=0;


        if(mqttserver.disconnect!=NULL)
        {
            mqttserver.disconnect(&mqttserver);
        }


    }

    return (ExitCode)0;
}

MQTTClientThread::~MQTTClientThread()
{
    //dtor
}
