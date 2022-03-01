/***************************************************************
 * Name:      SMGSDebugToolMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2022-02-24
 * Copyright: HYH (hyhsystem.cn)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/log.h>
#include <wx/datetime.h>
#include <wx/filedlg.h>
#include "Res.h"
#include "GuiMainPage.h"
#include "GuiMQTTDialog.h"

#include "SMGSDebugToolMain.h"
#include "InternalDatabase.h"

class SMGSLogFormatter : public wxLogFormatter
{
    virtual wxString Format(wxLogLevel level,
                            const wxString& msg,
                            const wxLogRecordInfo& info) const
    {
        wxDateTime now = wxDateTime::Now();
        return wxString::Format("%02d-%02d-%02d %02d:%02d:%02d  %s",now.GetYear(),now.GetMonth()-wxDateTime::Month::Jan+1,now.GetDay(),now.GetHour(),now.GetMinute(),now.GetSecond(),msg);
    }
};

//UI刷新队列
wxMessageQueue<std::function<void()>> SMGSDebugToolFrame::UpdateUIMsgQueue;

SMGSDebugToolFrame::SMGSDebugToolFrame(wxFrame *frame)
    : GUIFrame(frame)
{
    //设置icon
    SetIcon(logo_xpm);

    //设置标题
    SetTitle(_T("SMGSDebugTool"));



    //创建MQTTThread
    MQTTThread=new MQTTClientThread();
    MQTTThread->Run();

    //关联回调函数
    MQTTThread->SetConnectStateCallback(std::bind(&SMGSDebugToolFrame::OnMQTTConnectStateChange,this,std::placeholders::_1));
    MQTTThread->SetOnMessageCallback(std::bind(&SMGSDebugToolFrame::OnMQTTMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5));
}

void SMGSDebugToolFrame::MQTTOnMessageRegister(void *obj,std::function<void(wxString,void *,size_t,uint8_t,int)> OnMessage)
{
    if(obj==NULL || OnMessage == NULL)
    {
        return;
    }

    MQTTOnMessageUnRegister(obj);

    wxMutexLocker Lock(MQTTOnMessage.Lock);

    MQTTOnMessageCallback_t cb;
    cb.obj=obj;
    cb.OnMessage=OnMessage;

    MQTTOnMessage.List.push_back(cb);
}
void SMGSDebugToolFrame::MQTTOnMessageUnRegister(void *obj)
{
    if(obj==NULL)
    {
        return;
    }

    wxMutexLocker Lock(MQTTOnMessage.Lock);
    for(auto it=MQTTOnMessage.List.begin(); it!=MQTTOnMessage.List.end(); it++)
    {
        if(it->obj==obj)
        {
            MQTTOnMessage.List.erase(it);
            break;
        }
    }
}

void SMGSDebugToolFrame::OnInitTimer( wxTimerEvent& event )
{

    wxLog::EnableLogging(true);
    {
        //设置日志窗口
        wxLogTextCtrl *logger=new wxLogTextCtrl(m_textCtrl_log);
        wxLog::SetActiveTarget(logger);
        SMGSLogFormatter *formatter=new SMGSLogFormatter();
        logger->SetFormatter(formatter);

    }

    wxLogMessage(_T("程序已启动!"));

    //初始化内部数据
    InternalDatabase_Init();

    //设定内部数据
    InternalDatebase_ProgramInfo_Set(_T("Name"),_T("SMGSDebugTool"));
    InternalDatebase_ProgramInfo_Set(_T("Server"),_T("mqtt.hyhsystem.cn"));
    InternalDatebase_ProgramInfo_Set(_T("Port"),_T("1883"));

    {
        //打开首页
        GuiMainPage *page=new GuiMainPage(m_notebook_workspace);
        m_notebook_workspace->InsertPage(0,page,_T("首页"));

    }

    {
        //打开MQTT设置窗口
        GuiMQTTDialog dlg(this);
        dlg.SetIcon(logo_xpm);
        dlg.ShowModal();
    }
    //连接MQTT
    MQTTThread->MQTT_StartConnect();
}


void SMGSDebugToolFrame::OnMQTTPingTimer( wxTimerEvent& event )
{
    MQTTThread->MQTT_Ping();
}

void SMGSDebugToolFrame::OnUpdateUITimer( wxTimerEvent& event )
{
    std::function<void()> cb=NULL;
    while( wxMSGQUEUE_NO_ERROR==UpdateUIMsgQueue.ReceiveTimeout(1,cb))
    {
        //处理刷新事件
        if(cb!=NULL)
        {
            cb();
        }
    }
}

void SMGSDebugToolFrame::OnMQTTConnectStateChange(bool IsConnect)
{

    //Lambda表达式
    wxMenu* _Menu_Net=Menu_Net;
    wxStatusBar* _m_statusBar=m_statusBar;

    auto cb=[IsConnect,_Menu_Net,_m_statusBar]
    {
        if(IsConnect)
        {
            _Menu_Net->FindChildItem(ID_Menu_MQTT_Start)->Enable(false);
            _Menu_Net->FindChildItem(ID_Menu_MQTT_Stop)->Enable(true);
            _m_statusBar->SetStatusText(_T("已连接MQTT"),1);
        }
        else
        {
            _Menu_Net->FindChildItem(ID_Menu_MQTT_Start)->Enable(true);
            _Menu_Net->FindChildItem(ID_Menu_MQTT_Stop)->Enable(false);
            _m_statusBar->SetStatusText(_T("未连接MQTT"),1);
        }
    };

    UpdateUIMsgQueue.Post(cb);//将刷新事件加入队列

}

void SMGSDebugToolFrame::OnMQTTMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain)
{
    //MQTT消息,调用回调函数
    wxMutexLocker Lock(MQTTOnMessage.Lock);
    for(size_t i=0; i<MQTTOnMessage.List.size(); i++)
    {
        if(MQTTOnMessage.List[i].OnMessage!=NULL)
        {
            MQTTOnMessage.List[i].OnMessage(topic,payload,payloadlen,qos,retain);
        }
    }

}

void SMGSDebugToolFrame::OnMenuMQTT( wxCommandEvent& event )
{
    GuiMQTTDialog dlg(this);
    dlg.SetIcon(logo_xpm);
    if(dlg.ShowModal()==wxID_OK)
    {
        if(MQTTThread->MQTT_IsConnected())
        {
            //重启连接
            MQTTThread->MQTT_StopConnect();
            MQTTThread->MQTT_Ping();
            wxThread::Sleep(10);
            MQTTThread->MQTT_StartConnect();
        }
    }
}
void SMGSDebugToolFrame::OnMenuMQTTStart( wxCommandEvent& event )
{
    MQTTThread->MQTT_StartConnect();
}
void SMGSDebugToolFrame::OnMenuMQTTStop( wxCommandEvent& event )
{
    MQTTThread->MQTT_StopConnect();
    MQTTThread->MQTT_Ping();
}

void SMGSDebugToolFrame::OnMenuFileExit( wxCommandEvent& event )
{
    //退出程序
    Close();
}

void SMGSDebugToolFrame::OnMenuFileSave( wxCommandEvent& event )
{
    wxFileDialog saveFileDialog(this, _T("保存内部数据"), "", "","db files (*.db)|*.db", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if(saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    if(InternalDatabase_Backup(saveFileDialog.GetPath()))
    {
        wxLogMessage(_T("保存内部数据成功!"));
    }
    else
    {
        wxLogMessage(_T("保存内部数据失败!"));
    }

}

void SMGSDebugToolFrame::OnLogPanelSize( wxSizeEvent& event )
{
    m_textCtrl_log->SetSize(event.GetSize());
}


void SMGSDebugToolFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void SMGSDebugToolFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void SMGSDebugToolFrame::OnAbout(wxCommandEvent &event)
{
    AboutDialog dlg(this);
    dlg.Centre();
    dlg.SetTitle(_T("关于SMGSDebugTool"));
    dlg.SetIcon(logo_xpm);
    dlg.ShowModal();
}

SMGSDebugToolFrame::~SMGSDebugToolFrame()
{
    //关闭Log
    wxLog::SetActiveTarget(NULL);

    MQTTThread->MQTT_StopConnect();
    MQTTThread->MQTT_Ping();
    MQTTThread->Delete();

    //反初始化内部数据库
    InternalDatabase_Deinit();
}
