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

	m_statusBar = this->CreateStatusBar( 3, wxSTB_DEFAULT_STYLE|wxSTB_SIZEGRIP, wxID_ANY );
	m_menubar = new wxMenuBar( 0 );
	Menu_File = new wxMenu();
	wxMenuItem* Menu_File_Save;
	Menu_File_Save = new wxMenuItem( Menu_File, ID_Save, wxString( wxT("保存") ) + wxT('\t') + wxT("Ctrl+S"), wxT("保存内部数据"), wxITEM_NORMAL );
	Menu_File->Append( Menu_File_Save );

	Menu_File->AppendSeparator();

	wxMenuItem* Menu_File_Exit;
	Menu_File_Exit = new wxMenuItem( Menu_File, ID_Menu_File_Exit, wxString( wxT("退出") ) + wxT('\t') + wxT("Alt+F4"), wxT("退出程序"), wxITEM_NORMAL );
	Menu_File->Append( Menu_File_Exit );

	m_menubar->Append( Menu_File, wxT("文件") );

	Menu_Help = new wxMenu();
	wxMenuItem* Menu_About;
	Menu_About = new wxMenuItem( Menu_Help, ID_Menu_About, wxString( wxT("关于本程序") ) , wxT("关于本程序"), wxITEM_NORMAL );
	Menu_Help->Append( Menu_About );

	m_menubar->Append( Menu_Help, wxT("帮助") );

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

	m_notebook_workspace = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_CLOSE_ON_ACTIVE_TAB|wxAUI_NB_CLOSE_ON_ALL_TABS );
	m_mgr.AddPane( m_notebook_workspace, wxAuiPaneInfo() .Center() .CloseButton( false ).PaneBorder( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).TopDockable( false ).LeftDockable( false ).RightDockable( false ).Floatable( false ).CentrePane() );


	m_maintree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	m_maintree->SetMinSize( wxSize( 200,-1 ) );

	m_mgr.AddPane( m_maintree, wxAuiPaneInfo() .Left() .CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).TopDockable( false ).MinSize( wxSize( 200,-1 ) ) );


	m_mgr.Update();
	this->Centre( wxBOTH );

	// Connect Events
	Menu_File->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuFileSave ), this, Menu_File_Save->GetId());
	Menu_File->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuFileExit ), this, Menu_File_Exit->GetId());
	Menu_Help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ), this, Menu_About->GetId());
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

MainPage::MainPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	bSizer1 = new wxBoxSizer( wxVERTICAL );


	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( MainPage::OnInit ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( MainPage::OnSize ) );
}

MainPage::~MainPage()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( MainPage::OnInit ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( MainPage::OnSize ) );

}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,150 ), wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("SMGSDebugTool BY 何亚红"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText1->Wrap( -1 );
	bSizer2->Add( m_staticText1, 0, wxALL|wxEXPAND, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer2->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	m_hyperlink1 = new wxHyperlinkCtrl( this, wxID_ANY, wxT("项目地址:https://github.com/HEYAHONG/SimpleMQTTGateWayStack"), wxT("https://github.com/HEYAHONG/SimpleMQTTGateWayStack"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer2->Add( m_hyperlink1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_hyperlink2 = new wxHyperlinkCtrl( this, wxID_ANY, wxT("备用地址:https://gitee.com/HEYAHONG/SimpleMQTTGateWayStack"), wxT("https://gitee.com/HEYAHONG/SimpleMQTTGateWayStack"), wxDefaultPosition, wxDefaultSize, wxHL_ALIGN_CENTRE|wxHL_DEFAULT_STYLE );
	bSizer2->Add( m_hyperlink2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_hyperlink3 = new wxHyperlinkCtrl( this, wxID_ANY, wxT("个人网址:http://hyhsystem.cn"), wxT("http://hyhsystem.cn"), wxDefaultPosition, wxDefaultSize, wxHL_ALIGN_CENTRE|wxHL_DEFAULT_STYLE );
	bSizer2->Add( m_hyperlink3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );

	this->Centre( wxBOTH );
}

AboutDialog::~AboutDialog()
{
}
