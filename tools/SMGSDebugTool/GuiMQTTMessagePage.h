#ifndef GUIMQTTMESSAGEPAGE_H
#define GUIMQTTMESSAGEPAGE_H

#include "GuiFrame.h"
#include <wx/datetime.h>

class GuiMQTTMessagePage:public MQTTMessagePage
{
    public:
        GuiMQTTMessagePage(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
        virtual ~GuiMQTTMessagePage();


        void OnMQTTMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain,time_t timestamp);

    protected:


    private:
};

#endif // GUIMQTTMESSAGEPAGE_H
