#include "GuiGateWayPage.h"
#include "InternalDatabase.h"
#include "SMGSDebugToolMain.h"
#include "GuiMQTTMessagePage.h"
#include <wx/datetime.h>

GuiGateWayPage::GuiGateWayPage(wxString Addr,wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):GateWayPage(parent,id,pos,size,style,name),GateWayAddr(Addr)
{
    //ctor
    log=NULL;
    std::map<wxString,wxString> Con,Dat;
    Con[_T("Addr")]=GateWayAddr;
    Dat[_T("IsOpen")]=_T("1");
    InternalDatabase_Table_Update_Data(_T(SMGSDebugToolWorkSpaceGateWayList),Dat,Con);

    AddMQTTMessagePage();
    AddLogPage();

    AppendLogText(wxString(_T("网关"))+GateWayAddr+_T("已打开!"));

}

void GuiGateWayPage::AppendLogText(wxString _log)
{
    wxDateTime now = wxDateTime::Now();

    auto cb=[=]()
    {
        if(log!=NULL)
        {
            log->m_textCtrl_log->AppendText(wxString::Format("%02d-%02d-%02d %02d:%02d:%02d  %s\r\n",now.GetYear(),now.GetMonth()-wxDateTime::Month::Jan+1,now.GetDay(),now.GetHour(),now.GetMinute(),now.GetSecond(),_log));
        }
    };
    UpdateUIMsgQueue.Post(cb);
}

void GuiGateWayPage::AddMQTTMessagePage()
{
    GuiMQTTMessagePage *page=new GuiMQTTMessagePage(m_auinotebook_gateway_workspace);
    page->SetAddr(GateWayAddr);
    m_auinotebook_gateway_workspace->InsertPage(0,page,_T("MQTT消息"),true);
}

void GuiGateWayPage::AddLogPage()
{
    log=new GateWayPageLogPage(m_auinotebook_gateway_workspace);
    m_auinotebook_gateway_workspace->InsertPage(0,log,_T("网关日志"),true);
}


void GuiGateWayPage::OnUpdateGateWayPagetimer( wxTimerEvent& event )
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

GuiGateWayPage::~GuiGateWayPage()
{
    //dtor
    std::map<wxString,wxString> Con,Dat;
    Con[_T("Addr")]=GateWayAddr;
    Dat[_T("IsOpen")]=_T("0");
    InternalDatabase_Table_Update_Data(_T(SMGSDebugToolWorkSpaceGateWayList),Dat,Con);
    log=NULL;
}
