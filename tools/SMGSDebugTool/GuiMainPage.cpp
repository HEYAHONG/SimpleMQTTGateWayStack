#include "GuiMainPage.h"
#include "wx/fs_mem.h"
#include <wx/webviewfshandler.h>

GuiMainPage::GuiMainPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):MainPage(parent,id,pos,size,style,name)
{
    //ctor
    m_html=wxWebView::New(this, wxID_ANY);
    bSizer1->Add(m_html, 1, wxEXPAND | wxALL, 5 );
    Layout();
    bSizer1->Fit( this );

    m_html->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewFSHandler("memory")));

    m_html->LoadURL(_T("memory:about.htm"));
}

void GuiMainPage::OnInit( wxInitDialogEvent& event )
{

}
void GuiMainPage::OnSize( wxSizeEvent& event )
{
    //设置html空间大小
    m_html->SetSize(event.GetSize());
}

GuiMainPage::~GuiMainPage()
{
    //dtor
}
