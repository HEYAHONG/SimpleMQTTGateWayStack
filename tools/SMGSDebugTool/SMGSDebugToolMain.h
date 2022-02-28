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


class SMGSDebugToolFrame: public GUIFrame
{
    public:
        SMGSDebugToolFrame(wxFrame *frame);
        ~SMGSDebugToolFrame();
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
        bool IsMQTTConnect;
        bool IsMQTTConnect_Last;
        virtual void OnUpdateUITimer( wxTimerEvent& event );

        void OnMQTTConnectStateChange(bool IsConnect);

        /*
        MQTT菜单回调
        */
        virtual void OnMenuMQTT( wxCommandEvent& event );
		virtual void OnMenuMQTTStart( wxCommandEvent& event );
		virtual void OnMenuMQTTStop( wxCommandEvent& event );

        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);

        //MQTT线程
        MQTTClientThread * MQTTThread;
        friend class MQTTClientThread;


};

#endif // SMGSDebugTOOLMAIN_H
