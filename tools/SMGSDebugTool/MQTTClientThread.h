#ifndef MQTTCLIENTTHREAD_HPP
#define MQTTCLIENTTHREAD_HPP
#include <wx/thread.h>
#include "MQTTClient.h"
#include <wx/string.h>
#include "stdint.h"
#include "string.h"
#include "functional"

class MQTTClientThread:public wxThread
{
public:
    MQTTClientThread();
    virtual ~MQTTClientThread();

    /*
    开始连接MQTT,线程启动后有效
    */
    void MQTT_StartConnect(wxString Server=wxString(),wxString Port=wxString(),wxString Name=wxString());


    /*
    是否已连接
    */
    bool MQTT_IsConnected();

    /*
    执行Ping
    */
    bool MQTT_Ping();

    /*
    MQTT发布
    */
    bool MQTT_Publish(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain);

    /*
    停止连接
    */
    void MQTT_StopConnect();


    /*
    强制关闭本次连接，在需要删除线程时使用
    */
    void MQTT_ForceCloseConnect();

    /*
    设定连接状态变化的回调。(注意:连接断开后可能多次调用断开连接的回调)
    */
    void SetConnectStateCallback(std::function<void(bool)> cb);

    /*
    设定数据回调
    */
    void SetOnMessageCallback(std::function<void(wxString,void *,size_t,uint8_t,int)> cb);

protected:
    virtual wxThread::ExitCode Entry();

    //相关标志
    bool IsRunning;
    bool IsPendingStart;
    bool IsPendingStop;

    //相关回调
    std::function<void(bool)> ConnectStateChange_Callback;//连接状态
    std::function<void(wxString,void *,size_t,uint8_t,int)> OnMessage;


private:
    wxMutex MQTTLock;

    struct Network mqttserver;
    struct MQTTClient mqttclient;
    uint8_t mqtttxbuff[4096];
    uint8_t mqttrxbuff[4096];

    wxString Name;
    wxString ServerAddr;
    wxString ServerPort;

    friend  void mqttmessageHandler(struct MQTTClient *client,struct MessageData*msg);
};

#endif // MQTTCLIENTTHREAD_HPP
