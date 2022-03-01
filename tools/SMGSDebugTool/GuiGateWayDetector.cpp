#include "GuiGateWayDetector.h"
#include "SMGSDebugToolApp.h"
#include "SMGSDebugToolMain.h"
#include "functional"
#include "wx/thread.h"
#include "wx/arrstr.h"
#include "libSMGS-Server.h"

GuiGateWayDetector::GuiGateWayDetector(wxWindow* parent, wxWindowID id):GateWayDetectorDialog(parent,id)
{
    //ctor
    SMGSDebugToolFrame *Frame=SMGSDebugToolApp_GetMainFrame();
    if(Frame!=NULL)
    {
        Frame->MQTTOnMessageRegister(this,std::bind(GuiGateWayDetector::OnMQTTMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5));
    }
}

void GuiGateWayDetector::OnMQTTMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain)
{
    uint8_t buff[4096]={0};
    SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END]={0};
    SMGS_Topic_Plies_Decode(plies,SMGS_TOPIC_PLY_END,buff,sizeof(buff),topic,topic.length());

    wxArrayString topic_plies;

    for(size_t i=0;i<SMGS_TOPIC_PLY_END;i++)
    {
        topic_plies.Add(plies[i]);
    }

    Msg.Post(topic_plies);
}

 void GuiGateWayDetector::OnGateWayDetectorUpdatetimer( wxTimerEvent& event )
 {
     wxArrayString topic_plies;
     while( wxMSGQUEUE_NO_ERROR==Msg.ReceiveTimeout(1,topic_plies))
     {
         if(wxString("online")==topic_plies[SMGS_TOPIC_PLY_CMD_PARA_1])
         {
             wxLogMessage(_T("%s已上线"),(const char *)topic_plies[SMGS_TOPIC_PLY_SRCADDR]);
             m_listBox->Insert(topic_plies[SMGS_TOPIC_PLY_SRCADDR],0);
         }
     }
 }

GuiGateWayDetector::~GuiGateWayDetector()
{
    //dtor
    SMGSDebugToolFrame *Frame=SMGSDebugToolApp_GetMainFrame();
    if(Frame!=NULL)
    {
        Frame->MQTTOnMessageUnRegister(this);
    }
}
