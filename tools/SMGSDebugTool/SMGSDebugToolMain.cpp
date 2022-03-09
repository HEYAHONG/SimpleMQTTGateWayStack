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
#include <wx/textdlg.h>
#include <wx/base64.h>
#include "Res.h"
#include "GuiMainPage.h"
#include "GuiMQTTDialog.h"
#include "GuiGateWayPage.h"

#include "SMGSDebugToolMain.h"
#include "InternalDatabase.h"
#include "GuiGateWayDetector.h"
#include "libSMGS-Server.h"

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

    //设置工作区图标
    m_workspaceimagelist= new wxImageList(30,30,true,2);
    {
        wxImage icon(GateWay_xpm);
        m_workspaceimagelist->Add(icon.Scale(30,30));
    }
    {
        wxImage icon(Device_xpm);
        m_workspaceimagelist->Add(icon.Scale(30,30));
    }

    m_maintree->SetImageList(m_workspaceimagelist);
    m_maintree->AddRoot(_T("网关"));

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

bool SMGSDebugToolFrame::MQTTPublishMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain)
{
    bool ret=MQTTThread->MQTT_Publish(topic,payload,payloadlen,qos,retain);

    if(ret)
    {
        //将发布的消息也存入MQTTMessage表
        std::map<wxString,wxString> Dat;
        Dat[_T("Topic")]=topic;
        Dat[_T("Payload")]=wxBase64Encode(payload,payloadlen);
        Dat[_T("PayloadLen")]=std::to_string(payloadlen);
        Dat[_T("Qos")]=std::to_string(qos);
        Dat[_T("Retain")]=std::to_string(retain);
        InternalDatabase_Table_Insert_Data(_T(SMGSDebugToolMQTTMessage),Dat);
    }

    return ret;
}

void SMGSDebugToolFrame::AddMQTTGateWayToWorkSpace(wxString Addr)
{
    if(!InternalDatabase_Is_Table_Valied(_T(SMGSDebugToolWorkSpaceGateWayList)))
    {
        wxArrayString header;
        header.Add(_T("Addr"));
        header.Add(_T("IsOpen"));
        InternalDatabase_Create_Table(_T(SMGSDebugToolWorkSpaceGateWayList),header);
    }

    {
        //检查是否已添加
        std::map<wxString,wxString> con;
        con[_T("Addr")]=Addr;
        std::map<wxString,wxArrayString> Dat=InternalDatabase_Table_Get_AllData(_T(SMGSDebugToolWorkSpaceGateWayList),con);
        if(!Dat.empty() && !Dat[_T("Addr")].empty())
        {
            wxLogMessage(_T("%s已添加到工作区,无需重复添加!"),Addr);
            return;
        }
    }

    {
        std::map<wxString,wxString> dat;
        dat[_T("Addr")]=Addr;
        dat[_T("IsOpen")]=_T("0");
        InternalDatabase_Table_Insert_Data(_T(SMGSDebugToolWorkSpaceGateWayList),dat);
    }

    {
        //添加至工作区
        wxTreeCtrl *m_tree=m_maintree;
        auto cb=[Addr,m_tree]()
        {
            m_tree->InsertItem(m_tree->GetRootItem(),0,Addr,0);
        };
        UpdateUIMsgQueue.Post(cb);
    }

    wxLogMessage(_T("%s已添加到工作区。"),Addr);
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
        //创建MQTT消息数据库
        wxString Table_Name=_T(SMGSDebugToolMQTTMessage);
        if(!InternalDatabase_Is_Table_Valied(Table_Name))
        {
            //创建表
            wxArrayString header;
            header.Add(_T("Topic"));
            header.Add(_T("Payload"));
            header.Add(_T("PayloadLen"));
            header.Add(_T("Qos"));
            header.Add(_T("Retain"));
            InternalDatabase_Create_Table(Table_Name,header);
        }
    }

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

    //保存MQTT消息到数据库（本工具不能长时间运行）



    {
        std::map<wxString,wxString> Dat;
        Dat[_T("Topic")]=topic;
        Dat[_T("Payload")]=wxBase64Encode(payload,payloadlen);
        Dat[_T("PayloadLen")]=std::to_string(payloadlen);
        Dat[_T("Qos")]=std::to_string(qos);
        Dat[_T("Retain")]=std::to_string(retain);
        InternalDatabase_Table_Insert_Data(_T(SMGSDebugToolMQTTMessage),Dat);
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

void SMGSDebugToolFrame::OnMenuGateWayDetector( wxCommandEvent& event )
{
    GuiGateWayDetector dlg(this);
    dlg.SetIcon(logo_xpm);
    dlg.ShowModal();
}

void SMGSDebugToolFrame::OnMenuAddGateWay( wxCommandEvent& event )
{
    wxTextEntryDialog dlg(this,_T("输入网关地址(序列号):"));
    dlg.SetTitle(_T("输入"));
    dlg.SetIcon(logo_xpm);
    dlg.FindWindow(wxID_OK)->SetLabel(_T("确定"));
    dlg.FindWindow(wxID_CANCEL)->SetLabel(_T("取消"));
    if(dlg.ShowModal()==wxID_OK)
    {
        wxString addr=dlg.GetValue();
        if(addr.length()<CONFIG_SMGS_MIN_GATEWAY_SERIALNUMBER_LENGTH)
        {
            wxLogMessage(_T("网关地址(序列号) %s 过短！"),(const char *)addr);
        }
        else
        {
            AddMQTTGateWayToWorkSpace(addr);
        }
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

void SMGSDebugToolFrame::OnMaintreeItemActivated( wxTreeEvent& event )
{
    //工作区中网关被选中

    wxString Addr=m_maintree->GetItemText(event.GetItem());

    {
        std::map<wxString,wxString> Con;
        Con[_T("Addr")]=Addr;
        std::map<wxString,wxArrayString> dat=InternalDatabase_Table_Get_AllData(_T(SMGSDebugToolWorkSpaceGateWayList),Con);
        if(dat.empty() || dat[_T("Addr")].empty())
        {
            wxLogMessage(_T("%s 未在工作区!"),(const char *)Addr);
            return;
        }
        else
        {
            if(dat[_T("IsOpen")][0] != wxString(_T("0")))
            {
                wxLogMessage(_T("%s 已打开!"),(const char *)Addr);
                return;
            }
        }
    }

    GuiGateWayPage *page=new GuiGateWayPage(Addr,this);
    m_notebook_workspace->InsertPage(0,page,Addr,true);

}

void SMGSDebugToolFrame::OnMaintreeItemRightClick( wxTreeEvent& event )
{

}

SMGSDebugToolFrame::~SMGSDebugToolFrame()
{
    //清空打开的窗口
    m_notebook_workspace->DeleteAllPages();

    //关闭Log
    wxLog::SetActiveTarget(NULL);

    MQTTThread->MQTT_StopConnect();
    MQTTThread->MQTT_ForceCloseConnect();
    MQTTThread->Delete();

    delete m_workspaceimagelist;

    //反初始化内部数据库
    InternalDatabase_Deinit();
}
