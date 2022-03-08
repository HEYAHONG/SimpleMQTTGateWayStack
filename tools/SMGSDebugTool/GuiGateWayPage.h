#ifndef GUIGATEWAYPAGE_H
#define GUIGATEWAYPAGE_H

#include "GuiFrame.h"

class GuiGateWayPage:public GateWayPage
{
    public:
        GuiGateWayPage(wxString Addr,wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
        virtual ~GuiGateWayPage();

    protected:

    private:
        wxString GateWayAddr;
};

#endif // GUIGATEWAYPAGE_H
