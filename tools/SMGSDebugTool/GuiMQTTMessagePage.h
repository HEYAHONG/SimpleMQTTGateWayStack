#ifndef GUIMQTTMESSAGEPAGE_H
#define GUIMQTTMESSAGEPAGE_H

#include "GUIFrame.h"
#include <wx/datetime.h>

class GuiMQTTMessagePage:public MQTTMessagePage
{
    public:
        GuiMQTTMessagePage(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
        virtual ~GuiMQTTMessagePage();


        void OnMQTTMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain,time_t timestamp);

        void SetAddr(wxString _Addr);//设置地址,当此值不为空时,将筛选地址为_Addr的消息

    protected:

        virtual void OnMQTTMessageItemActivated( wxDataViewEvent& event );
        virtual void OnInitMQTTMessagePagetimer( wxTimerEvent& event );


    private:
        wxString Addr;
};

#endif // GUIMQTTMESSAGEPAGE_H
