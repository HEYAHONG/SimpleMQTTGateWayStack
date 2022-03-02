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

	Menu_Net = new wxMenu();
	wxMenuItem* Menu_Net_MQTT;
	Menu_Net_MQTT = new wxMenuItem( Menu_Net, ID_Menu_MQTT, wxString( wxT("MQTT") ) + wxT('\t') + wxT("Ctrl+M"), wxT("MQTT设置"), wxITEM_NORMAL );
	Menu_Net->Append( Menu_Net_MQTT );

	Menu_Net->AppendSeparator();

	wxMenuItem* Menu_Net_MQTT_Start;
	Menu_Net_MQTT_Start = new wxMenuItem( Menu_Net, ID_Menu_MQTT_Start, wxString( wxT("MQTT开始") ) , wxT("开始MQTT连接"), wxITEM_NORMAL );
	Menu_Net->Append( Menu_Net_MQTT_Start );

	wxMenuItem* Menu_Net_MQTT_Stop;
	Menu_Net_MQTT_Stop = new wxMenuItem( Menu_Net, ID_Menu_MQTT_Stop, wxString( wxT("MQTT停止") ) , wxT("停止MQTT连接"), wxITEM_NORMAL );
	Menu_Net->Append( Menu_Net_MQTT_Stop );

	m_menubar->Append( Menu_Net, wxT("网络") );

	Menu_GateWay = new wxMenu();
	wxMenuItem* GateWayDetector;
	GateWayDetector = new wxMenuItem( Menu_GateWay, ID_Menu_GateWayDetector, wxString( wxT("网关发现") ) + wxT('\t') + wxT("Ctrl+F"), wxT("打开网关发现窗口监测网关"), wxITEM_NORMAL );
	Menu_GateWay->Append( GateWayDetector );

	m_menubar->Append( Menu_GateWay, wxT("网关") );

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
	m_Inittimer.SetOwner( this, wxID_InitTimer );
	m_Inittimer.Start( 50, true );

	m_notebook_workspace = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_CLOSE_ON_ACTIVE_TAB|wxAUI_NB_CLOSE_ON_ALL_TABS );
	m_mgr.AddPane( m_notebook_workspace, wxAuiPaneInfo() .Center() .CloseButton( false ).PaneBorder( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).TopDockable( false ).LeftDockable( false ).RightDockable( false ).Floatable( false ).CentrePane() );


	m_maintree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	m_maintree->SetMinSize( wxSize( 200,-1 ) );

	m_mgr.AddPane( m_maintree, wxAuiPaneInfo() .Left() .CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).TopDockable( false ).MinSize( wxSize( 200,-1 ) ) );

	m_MQTTPingtimer.SetOwner( this, wxID_MQTTPingTimer );
	m_MQTTPingtimer.Start( 120000 );

	m_UpdateUItimer.SetOwner( this, wxID_UpdateUItimer );
	m_UpdateUItimer.Start( 10 );


	m_mgr.Update();
	this->Centre( wxBOTH );

	// Connect Events
	Menu_File->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuFileSave ), this, Menu_File_Save->GetId());
	Menu_File->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuFileExit ), this, Menu_File_Exit->GetId());
	Menu_Net->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuMQTT ), this, Menu_Net_MQTT->GetId());
	Menu_Net->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuMQTTStart ), this, Menu_Net_MQTT_Start->GetId());
	Menu_Net->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuMQTTStop ), this, Menu_Net_MQTT_Stop->GetId());
	Menu_GateWay->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuGateWayDetector ), this, GateWayDetector->GetId());
	Menu_Help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ), this, Menu_About->GetId());
	m_panel_log->Connect( wxEVT_SIZE, wxSizeEventHandler( GUIFrame::OnLogPanelSize ), NULL, this );
	this->Connect( wxID_InitTimer, wxEVT_TIMER, wxTimerEventHandler( GUIFrame::OnInitTimer ) );
	this->Connect( wxID_MQTTPingTimer, wxEVT_TIMER, wxTimerEventHandler( GUIFrame::OnMQTTPingTimer ) );
	this->Connect( wxID_UpdateUItimer, wxEVT_TIMER, wxTimerEventHandler( GUIFrame::OnUpdateUITimer ) );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	m_panel_log->Disconnect( wxEVT_SIZE, wxSizeEventHandler( GUIFrame::OnLogPanelSize ), NULL, this );
	this->Disconnect( wxID_InitTimer, wxEVT_TIMER, wxTimerEventHandler( GUIFrame::OnInitTimer ) );
	this->Disconnect( wxID_MQTTPingTimer, wxEVT_TIMER, wxTimerEventHandler( GUIFrame::OnMQTTPingTimer ) );
	this->Disconnect( wxID_UpdateUItimer, wxEVT_TIMER, wxTimerEventHandler( GUIFrame::OnUpdateUITimer ) );

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

MQTTDialog::MQTTDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("名称:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetMinSize( wxSize( 100,-1 ) );

	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_textCtrl_Name = new wxTextCtrl( this, wxID_ANY, wxT("SMGSDebugTool"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_Name->SetMinSize( wxSize( 150,-1 ) );

	fgSizer1->Add( m_textCtrl_Name, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer3->Add( fgSizer1, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("MQTT服务器地址:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText4->Wrap( -1 );
	m_staticText4->SetMinSize( wxSize( 100,-1 ) );

	fgSizer2->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_ServerAddr = new wxTextCtrl( this, wxID_ANY, wxT("mqtt.hyhsystem.cn"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_ServerAddr->SetMinSize( wxSize( 150,-1 ) );

	fgSizer2->Add( m_textCtrl_ServerAddr, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer3->Add( fgSizer2, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("MQTT服务器端口:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText5->Wrap( -1 );
	m_staticText5->SetMinSize( wxSize( 100,-1 ) );

	fgSizer3->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_ServerPort = new wxTextCtrl( this, wxID_ANY, wxT("1883"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_ServerPort->SetMinSize( wxSize( 150,-1 ) );

	fgSizer3->Add( m_textCtrl_ServerPort, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer3->Add( fgSizer3, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_button1 = new wxButton( this, wxID_ANY, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizer3->Add( bSizer6, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MQTTDialog::OnButtonOk ), NULL, this );
}

MQTTDialog::~MQTTDialog()
{
	// Disconnect Events
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MQTTDialog::OnButtonOk ), NULL, this );

}

GateWayDetectorDialog::GateWayDetectorDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	bSizer5->SetMinSize( wxSize( 300,500 ) );
	m_list = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_LIST );
	bSizer5->Add( m_list, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer5 );
	this->Layout();
	bSizer5->Fit( this );
	m_GateWayDetectorUpdatetimer.SetOwner( this, wxID_GateWayDetectorUpdatetimer );
	m_GateWayDetectorUpdatetimer.Start( 100 );

	m_RightClickMenu = new wxMenu();
	wxMenuItem* m_MenuItemCopy;
	m_MenuItemCopy = new wxMenuItem( m_RightClickMenu, wxID_MenuItemCopy, wxString( wxT("复制") ) , wxEmptyString, wxITEM_NORMAL );
	m_RightClickMenu->Append( m_MenuItemCopy );

	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GateWayDetectorDialog::GateWayDetectorDialogOnContextMenu ), NULL, this );


	this->Centre( wxBOTH );

	// Connect Events
	m_list->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( GateWayDetectorDialog::OnListItemRightClick ), NULL, this );
	this->Connect( wxID_GateWayDetectorUpdatetimer, wxEVT_TIMER, wxTimerEventHandler( GateWayDetectorDialog::OnGateWayDetectorUpdatetimer ) );
	m_RightClickMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GateWayDetectorDialog::OnMenuItemCopy ), this, m_MenuItemCopy->GetId());
}

GateWayDetectorDialog::~GateWayDetectorDialog()
{
	// Disconnect Events
	m_list->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( GateWayDetectorDialog::OnListItemRightClick ), NULL, this );
	this->Disconnect( wxID_GateWayDetectorUpdatetimer, wxEVT_TIMER, wxTimerEventHandler( GateWayDetectorDialog::OnGateWayDetectorUpdatetimer ) );

	delete m_RightClickMenu;
}
