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
	this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );
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

	m_panel_log = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	m_panel_log->SetMinSize( wxSize( 800,100 ) );

	m_mgr.AddPane( m_panel_log, wxAuiPaneInfo() .Bottom() .CaptionVisible( false ).CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).TopDockable( false ).LeftDockable( false ).RightDockable( false ).MinSize( wxSize( 800,100 ) ) );

	wxWrapSizer* wSizer1;
	wSizer1 = new wxWrapSizer( wxHORIZONTAL, wxEXTEND_LAST_ON_EACH_LINE|wxREMOVE_LEADING_SPACES|wxWRAPSIZER_DEFAULT_FLAGS );

	m_textCtrl_log = new wxTextCtrl( m_panel_log, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	#ifdef __WXGTK__
	if ( !m_textCtrl_log->HasFlag( wxTE_MULTILINE ) )
	{
	m_textCtrl_log->SetMaxLength( 1000000 );
	}
	#else
	m_textCtrl_log->SetMaxLength( 1000000 );
	#endif
	m_textCtrl_log->SetMinSize( wxSize( 800,100 ) );

	wSizer1->Add( m_textCtrl_log, 0, wxALL, 5 );


	m_panel_log->SetSizer( wSizer1 );
	m_panel_log->Layout();
	wSizer1->Fit( m_panel_log );
	m_timer_init.SetOwner( this, wxID_ANY );
	m_timer_init.Start( 50, true );

	m_notebook_workspace = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_mgr.AddPane( m_notebook_workspace, wxAuiPaneInfo() .Center() .CloseButton( false ).PaneBorder( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).TopDockable( false ).LeftDockable( false ).RightDockable( false ).Floatable( false ).CentrePane() );



	m_mgr.Update();
	this->Centre( wxBOTH );

	// Connect Events
	Menu_File->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuFileExit ), this, Menu_File_Exit->GetId());
	m_panel_log->Connect( wxEVT_SIZE, wxSizeEventHandler( GUIFrame::OnLogPanelSize ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( GUIFrame::OnInitTimer ) );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	m_panel_log->Disconnect( wxEVT_SIZE, wxSizeEventHandler( GUIFrame::OnLogPanelSize ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( GUIFrame::OnInitTimer ) );

	m_mgr.UnInit();

}
