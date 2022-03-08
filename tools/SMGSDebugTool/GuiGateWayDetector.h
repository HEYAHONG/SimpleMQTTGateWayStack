#ifndef GUIGATEWAYDETECTOR_H
#define GUIGATEWAYDETECTOR_H

#include "GuiFrame.h"
#include "wx/msgqueue.h"
#include "wx/arrstr.h"

class GuiGateWayDetector:public GateWayDetectorDialog
{
public:
    GuiGateWayDetector(wxWindow* parent, wxWindowID id = wxID_ANY);
    virtual ~GuiGateWayDetector();

    void OnMQTTMessage(wxString topic,void *payload,size_t payloadlen,uint8_t qos,int retain);
protected:

    virtual void OnGateWayDetectorUpdatetimer( wxTimerEvent& event );
    virtual void OnListItemRightClick( wxListEvent& event );
    virtual void OnMenuItemCopy( wxCommandEvent& event );
    virtual void OnMenuItemAddGateWayToWorkSpace( wxCommandEvent& event );

private:
    wxMessageQueue<wxArrayString> Msg;
    wxString CurrentAddr;
    wxImageList *imagelist;
};

#endif // GUIGATEWAYDETECTOR_H
