#ifndef GUIGATEWAYPAGE_H
#define GUIGATEWAYPAGE_H

#include "GUIFrame.h"
#include <wx/msgqueue.h>
#include <functional>
#include <wx/datetime.h>

class GuiGateWayPage:public GateWayPage
{
    public:
        GuiGateWayPage(wxString Addr,wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
        virtual ~GuiGateWayPage();


        void AppendLogText(wxString _log,wxDateTime now = wxDateTime::Now());

        void OnMQTTMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain,time_t timestamp);


    protected:

        void AddMQTTMessagePage();
        void AddLogPage();
        virtual void OnUpdateGateWayPagetimer( wxTimerEvent& event );

    private:
        wxString GateWayAddr;
        GateWayPageLogPage *log;
        wxMessageQueue<std::function<void()>> UpdateUIMsgQueue;

        void SetOnline(bool IsOnline=true);
};

#endif // GUIGATEWAYPAGE_H
