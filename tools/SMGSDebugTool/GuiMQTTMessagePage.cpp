#include "GuiMQTTMessagePage.h"
#include "string"
#include "SMGSDebugToolMain.h"
#include "InternalDatabase.h"
#include "wx/base64.h"

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

void GuiMQTTMessagePage::OnMQTTMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain,time_t timestamp)
{
    if(timestamp==0)
    {
        timestamp=wxDateTime::GetTimeNow();
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

GuiMQTTMessagePage::~GuiMQTTMessagePage()
{
    //dtor
    SMGSDebugToolFrame *Frame=SMGSDebugToolApp_GetMainFrame();
    if(Frame!=NULL)
    {
        Frame->MQTTOnMessageUnRegister(this);
    }
}
