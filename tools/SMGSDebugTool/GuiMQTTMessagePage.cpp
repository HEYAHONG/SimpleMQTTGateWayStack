#include "GuiMQTTMessagePage.h"
#include "string"
#include "SMGSDebugToolMain.h"
#include "InternalDatabase.h"
#include "wx/base64.h"
#include "wx/msgdlg.h"
#include "libSMGS-Server.h"

GuiMQTTMessagePage::GuiMQTTMessagePage(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,long style, const wxString& name):MQTTMessagePage(parent,id,pos,size,style,name)
{
    //ctor

    SMGSDebugToolFrame *Frame=SMGSDebugToolApp_GetMainFrame();
    if(Frame!=NULL)
    {
        Frame->MQTTOnMessageRegister(this,std::bind(&GuiMQTTMessagePage::OnMQTTMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,0),true);
    }

    {
        wxDataViewColumn *col=m_MQTTMessagedataViewList->AppendTextColumn(_T("Topic"));
        col->SetWidth(512);
    }

    {
        wxDataViewColumn *col=m_MQTTMessagedataViewList->AppendTextColumn(_T("Payload"));
        col->SetWidth(512);
    }

    {
        wxDataViewColumn *col=m_MQTTMessagedataViewList->AppendTextColumn(_T("Qos"));
        col->SetWidth(80);
    }

    {
        wxDataViewColumn *col=m_MQTTMessagedataViewList->AppendTextColumn(_T("Retain"));
        col->SetWidth(80);
    }

    {
        wxDataViewColumn *col=m_MQTTMessagedataViewList->AppendTextColumn(_T("TimeStamp"));
        col->SetWidth(128);
    }


}

void GuiMQTTMessagePage::OnMQTTMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain,time_t timestamp)
{
    if(timestamp==0)
    {
        timestamp=wxDateTime::GetTimeNow();
    }

    if(!Addr.empty())
    {
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

        if(wxString(plies[SMGS_TOPIC_PLY_DESTADDR])!=Addr && wxString(plies[SMGS_TOPIC_PLY_SRCADDR])!=Addr)
        {
            return;
        }
    }

    wxVector<wxVariant> Data;
    Data.push_back(topic);
    {
        //payload

        wxString PayStr;

        uint8_t *p=(uint8_t *)payload;
        for(size_t i=0; i<payloadlen; i++)
        {
            char buff[5]= {0};
            sprintf(buff,"%02X ",p[i]);
            PayStr+=buff;
        }

        Data.push_back(PayStr);
    }
    Data.push_back(wxString(std::to_string(qos)));
    Data.push_back(wxString(std::to_string(retain)));
    Data.push_back(wxString(std::to_string(timestamp)));

    m_MQTTMessagedataViewList->InsertItem(0,Data);
}


void GuiMQTTMessagePage::SetAddr(wxString _Addr)
{
    Addr=_Addr;
}

void GuiMQTTMessagePage::OnMQTTMessageItemActivated( wxDataViewEvent& event )
{
    wxDataViewListStore *data=dynamic_cast<wxDataViewListStore*>(event.GetModel());
    wxDataViewItem item=event.GetItem();
    int row=data->GetRow(item);
    int col=event.GetColumn();
    if(row < 0 || (col < 0 && row < 0))//在Linux下col可能为-1
    {
        wxLogMessage(_T("内部错误,行=%d,列=%d"),row,col);
        return;
    }

    {
        //输出消息
        wxVariant var;
        wxString msg;
        data->GetValueByRow(var,row,0);
        wxString Topic=var;

        unsigned long long timestamp=0;
        {
            data->GetValueByRow(var,row,4);
            wxString Str=var;
            if(Str.ToULongLong(&timestamp))
            {
                msg+=wxString(_T("时间戳: "))+wxDateTime((time_t)timestamp).Format()+_T("\r\n");
            }
        }

        {
            uint8_t buff[4096]= {0};
            SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END]= {0};
            SMGS_Topic_Plies_Decode(plies,SMGS_TOPIC_PLY_END,buff,sizeof(buff),Topic.ToAscii(),Topic.length());

            for(size_t i=0; i<SMGS_TOPIC_PLY_END; i++)
            {
                if(plies[i]==NULL)
                {
                    plies[i]="";//将NULL变为空字符串，防止出现字符串拼接错误
                }
            }

            msg+=wxString(_T("目的地址: "))+plies[SMGS_TOPIC_PLY_DESTADDR]+_T("\r\n");
            msg+=wxString(_T("源地址: "))+plies[SMGS_TOPIC_PLY_SRCADDR]+_T("\r\n");
            msg+=wxString(_T("通信类型: "))+plies[SMGS_TOPIC_PLY_COMTYPE]+_T("\r\n");
            msg+=wxString(_T("模块: "))+plies[SMGS_TOPIC_PLY_MODULE]+_T("\r\n");
            msg+=wxString(_T("命令: "))+plies[SMGS_TOPIC_PLY_CMD]+_T("\r\n");
            msg+=wxString(_T("命令参数1: "))+plies[SMGS_TOPIC_PLY_CMD_PARA_1]+_T("\r\n");
            msg+=wxString(_T("命令参数2: "))+plies[SMGS_TOPIC_PLY_CMD_PARA_2]+_T("\r\n");
            msg+=wxString(_T("命令参数3: "))+plies[SMGS_TOPIC_PLY_CMD_PARA_3]+_T("\r\n");

        }

        wxMessageBox(msg,_T("详情"));
    }
}

void GuiMQTTMessagePage::OnInitMQTTMessagePagetimer( wxTimerEvent& event )
{
    {
        std::map<wxString,wxArrayString> dat=InternalDatabase_Table_Get_AllData(SMGSDebugToolMQTTMessage);
        for(size_t i=0; i<dat[_T("Topic")].size(); i++)
        {
            wxString Topic=dat[_T("Topic")][i];
            auto Payload=wxBase64Decode(dat[_T("Payload")][i]);
            long payloadlen=0;
            {
                wxString PayloadLenStr=dat[_T("PayloadLen")][i];
                PayloadLenStr.ToCLong(&payloadlen);
            }
            long qos=0;
            {
                wxString QosStr=dat[_T("Qos")][i];
                QosStr.ToCLong(&qos);
            }
            long retain=0;
            {
                wxString RetainStr=dat[_T("Retain")][i];
                RetainStr.ToCLong(&retain);
            }

            unsigned long long timestamp=0;
            {
                wxString TimeStampStr=dat[_T("TimeStamp")][i];
                TimeStampStr.ToULongLong(&timestamp);
            }

            OnMQTTMessage(Topic,Payload.GetData(),payloadlen,qos,retain,timestamp);

        }
    }
}

GuiMQTTMessagePage::~GuiMQTTMessagePage()
{
    //dtor
    SMGSDebugToolFrame *Frame=SMGSDebugToolApp_GetMainFrame();
    if(Frame!=NULL)
    {
        Frame->MQTTOnMessageUnRegister(this);
    }
}
