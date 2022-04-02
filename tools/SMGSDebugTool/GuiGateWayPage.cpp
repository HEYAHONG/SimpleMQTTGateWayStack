#include "GuiGateWayPage.h"
#include "InternalDatabase.h"
#include "SMGSDebugToolMain.h"
#include "GuiMQTTMessagePage.h"
#include <wx/datetime.h>
#include <libSMGS-Server.h>

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

void GuiGateWayPage::AppendLogText(wxString _log,wxDateTime now)
{

    auto cb=[=]()
    {
        if(log!=NULL)
        {
            log->m_textCtrl_log->AppendText(wxString::Format("%02d-%02d-%02d %02d:%02d:%02d  %s\r\n",now.GetYear(),now.GetMonth()-wxDateTime::Month::Jan+1,now.GetDay(),now.GetHour(),now.GetMinute(),now.GetSecond(),_log));
        }
    };
    UpdateUIMsgQueue.Post(cb);
}

void GuiGateWayPage::OnMQTTMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain,time_t timestamp)
{
    //处理网关历史消息及实时消息
    uint8_t buff[4096]= {0};
    SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END]= {0};
    SMGS_Topic_Plies_Decode(plies,SMGS_TOPIC_PLY_END,buff,sizeof(buff),topic.ToAscii(),topic.length());

    for(size_t i=0; i<SMGS_TOPIC_PLY_END; i++)
    {
        if(plies[i]==NULL)
        {
            plies[i]="";//将NULL变为空字符串，防止出现字符串拼接错误
        }
    }



    if(GateWayAddr==plies[SMGS_TOPIC_PLY_SRCADDR])
    {
        //已接收消息
        switch(SMGS_Get_Topic_Ply_ComType(plies[SMGS_TOPIC_PLY_COMTYPE]))
        {
        case SMGS_TOPIC_PLY_COMTYPE_BINEVT:
        {
            //上报数据(仅使用CMD命令)
            if(SMGS_Get_Topic_Ply_Module(plies[SMGS_TOPIC_PLY_MODULE])==SMGS_TOPIC_PLY_MODULE_GATEWAY)
            {
                //网关模块
                if(wxString(SMGS_Get_Topic_Ply_CMD_String(SMGS_TOPIC_PLY_CMD_COMMAND))==plies[SMGS_TOPIC_PLY_CMD])
                {
                    //特殊命令

                    if(payloadlen>=2)
                    {
                        uint16_t cmdid=0;
                        cmdid+=((uint8_t*)payload)[1];
                        cmdid<<=8;
                        cmdid+=((uint8_t*)payload)[0];
                        switch(cmdid)
                        {
                        case SMGS_GATEWAY_CMDID_ONLINE:
                        {
                            //上线
                            AppendLogText(GateWayAddr+_T("已上线!"),timestamp);
                        }
                        break;
                        case SMGS_GATEWAY_CMDID_OFFLINE:
                        {
                            //下线
                            AppendLogText(GateWayAddr+_T("已离线!"),timestamp);
                        }
                        break;
                        case SMGS_GATEWAY_CMDID_REPORT_DEVICETABLE_ONLINE:
                        {
                            if(payloadlen<4)
                            {
                                break;//负载不合法
                            }
                            //上报设备表
                            int pos=(((uint8_t*)payload)[2]);
                            char * SerialNumber=(char *)&(((uint8_t*)payload)[3]);
                            AppendLogText(wxString::Format(_T("上线上报设备%d序列号为%s!"),pos,SerialNumber),timestamp);
                        }
                        break;
                        default:
                            break;
                        }
                    }

                }

            }
            else
            {
                //设备模块
            }
        }
        break;
        case SMGS_TOPIC_PLY_COMTYPE_BINRESP:
        {
            //回复数据
        }
        break;
        default:
            break;
        }
    }

    if(GateWayAddr==plies[SMGS_TOPIC_PLY_DESTADDR])
    {
        //已发送消息
        switch(SMGS_Get_Topic_Ply_ComType(plies[SMGS_TOPIC_PLY_MODULE]))
        {
        case SMGS_TOPIC_PLY_COMTYPE_BINREQ:
        {
            //上报数据
        }
        break;
        default:
            break;
        }

    }

}


void GuiGateWayPage::AddMQTTMessagePage()
{
    GuiMQTTMessagePage *page=new GuiMQTTMessagePage(m_auinotebook_gateway_workspace);
    page->SetAddr(GateWayAddr);
    page->SetOnMQTTMessageCallback(std::bind(&GuiGateWayPage::OnMQTTMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6));
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
    m_auinotebook_gateway_workspace->DeleteAllPages();
}
