///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GUIFrame.h"

///////////////////////////////////////////////////////////////////////////

GUIFrame::GUIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);

	m_statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	m_menubar = new wxMenuBar( 0 );
	Menu_File = new wxMenu();
	wxMenuItem* Menu_File_Exit;
	Menu_File_Exit = new wxMenuItem( Menu_File, ID_Menu_File_Exit, wxString( wxT("退出") ) + wxT('\t') + wxT("Alt+F4"), wxT("退出程序"), wxITEM_NORMAL );
	Menu_File->Append( Menu_File_Exit );

	m_menubar->Append( Menu_File, wxT("文件") );

	this->SetMenuBar( m_menubar );


	m_mgr.Update();
	this->Centre( wxBOTH );

	// Connect Events
	Menu_File->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuFileExit ), this, Menu_File_Exit->GetId());
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events

	m_mgr.UnInit();

}
