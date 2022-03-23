#ifndef GUIGATEWAYPAGE_H
#define GUIGATEWAYPAGE_H

#include "GUIFrame.h"
#include <wx/msgqueue.h>
#include <functional>

class GuiGateWayPage:public GateWayPage
{
    public:
        GuiGateWayPage(wxString Addr,wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
        virtual ~GuiGateWayPage();


        void AppendLogText(wxString _log);


    protected:

        void AddMQTTMessagePage();
        void AddLogPage();
        virtual void OnUpdateGateWayPagetimer( wxTimerEvent& event );

    private:
        wxString GateWayAddr;
        GateWayPageLogPage *log;
        wxMessageQueue<std::function<void()>> UpdateUIMsgQueue;
};

#endif // GUIGATEWAYPAGE_H
