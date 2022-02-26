#ifndef GUIMAINPAGE_H
#define GUIMAINPAGE_H

#include "GuiFrame.h"
#include <wx/webview.h>

class GuiMainPage: public MainPage
{
public:
    GuiMainPage(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
    virtual ~GuiMainPage();

protected:

    wxWebView * m_html;

    virtual void OnInit( wxInitDialogEvent& event );
    virtual void OnSize( wxSizeEvent& event );

private:
};

#endif // GUIMAINPAGE_H
