/***************************************************************
 * Name:      SMGSDebugToolMain.h
 * Purpose:   Defines Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2022-02-24
 * Copyright: HYH (hyhsystem.cn)
 * License:
 **************************************************************/

#ifndef SMGSDebugTOOLMAIN_H
#define SMGSDebugTOOLMAIN_H



#include "SMGSDebugToolApp.h"
#include "MQTTClientThread.h"
#include "GUIFrame.h"
#include <wx/msgqueue.h>
#include "functional"
#include <wx/thread.h>
#include <wx/vector.h>


class SMGSDebugToolFrame: public GUIFrame
{
public:
    SMGSDebugToolFrame(wxFrame *frame);
    ~SMGSDebugToolFrame();

    //MQTT消息回调函数注册与反注册
    void MQTTOnMessageRegister(void *obj,std::function<void(wxString,void *,size_t,uint8_t,int)> OnMessage);
    void MQTTOnMessageUnRegister(void *obj);

    //添加网关到工作区,
    void AddMQTTGateWayToWorkSpace(wxString Addr);

private:
    //菜单退出
    virtual void OnMenuFileExit( wxCommandEvent& event );

    //菜单保存
    virtual void OnMenuFileSave( wxCommandEvent& event );

    //日志窗口改变大小
    virtual void OnLogPanelSize( wxSizeEvent& event );

    /*
    初始化 定时器触发
    使用定时器初始化可防止窗口对象未创建导致的错误
    */
    virtual void OnInitTimer( wxTimerEvent& event );

    //MQTT定时器
    virtual void OnMQTTPingTimer( wxTimerEvent& event );

    //UI更新定时器。
    virtual void OnUpdateUITimer( wxTimerEvent& event );

    //MQTT连接状态改变回调
    void OnMQTTConnectStateChange(bool IsConnect);

    //MQTT消息回调
    void OnMQTTMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain);

    typedef struct
    {
        void * obj;//关联的对象的指针,每个对象只能关联一个回调函数
        std::function<void(wxString,void *,size_t,uint8_t,int)> OnMessage;
    }
    MQTTOnMessageCallback_t;

    struct
    {
        wxVector<MQTTOnMessageCallback_t> List;
        wxMutex Lock;
    } MQTTOnMessage;

    /*
    MQTT菜单回调
    */
    virtual void OnMenuMQTT( wxCommandEvent& event );
    virtual void OnMenuMQTTStart( wxCommandEvent& event );
    virtual void OnMenuMQTTStop( wxCommandEvent& event );

    virtual void OnMenuGateWayDetector( wxCommandEvent& event );

    virtual void OnClose(wxCloseEvent& event);
    virtual void OnQuit(wxCommandEvent& event);
    virtual void OnAbout(wxCommandEvent& event);

    //MQTT线程
    MQTTClientThread * MQTTThread;
    friend class MQTTClientThread;


    static wxMessageQueue<std::function<void()>> UpdateUIMsgQueue;

    wxImageList * m_workspaceimagelist;
};

#endif // SMGSDebugTOOLMAIN_H
