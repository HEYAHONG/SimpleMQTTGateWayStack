///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/cshelp.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/wrapsizer.h>
#include <wx/panel.h>
#include <wx/timer.h>
#include <wx/aui/auibook.h>
#include <wx/treectrl.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/hyperlink.h>
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/dataview.h>
#include <wx/checkbox.h>
#include <wx/radiobox.h>

///////////////////////////////////////////////////////////////////////////

#define ID_Save 1000
#define ID_Menu_File_Exit 1001
#define ID_Menu_MQTT 1002
#define ID_Menu_MQTT_Start 1003
#define ID_Menu_MQTT_Stop 1004
#define ID_Menu_MQTTMessage 1005
#define ID_Menu_SendMQTTRawMessage 1006
#define ID_Menu_GateWayDetector 1007
#define ID_Menu_Add_GateWay 1008
#define ID_Menu_About 1009
#define wxID_InitTimer 1010
#define wxID_MQTTPingTimer 1011
#define wxID_UpdateUItimer 1012
#define wxID_Menu_maintree_CopySerialNumber 1013
#define wxID_Menu_maintree_SendMQTTRawMessage_DefaultName 1014
#define wxID_Menu_maintree_SendMQTTRawMessage_ToolName 1015
#define wxID_GateWayDetectorUpdatetimer 1016
#define wxID_MenuItemCopy 1017
#define wxID_MenuItemAddGateWayToWorkSpace 1018
#define wxID_InitMQTTMessagePage 1019

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrame
///////////////////////////////////////////////////////////////////////////////
class GUIFrame : public wxFrame
{
	private:

	protected:
		wxStatusBar* m_statusBar;
		wxMenuBar* m_menubar;
		wxMenu* Menu_File;
		wxMenu* Menu_Net;
		wxMenu* Menu_GateWay;
		wxMenu* Menu_Help;
		wxTimer m_Inittimer;
		wxAuiNotebook* m_notebook_workspace;
		wxTreeCtrl* m_maintree;
		wxTimer m_MQTTPingtimer;
		wxTimer m_UpdateUItimer;
		wxMenu* m_menu_maintree;

		// Virtual event handlers, override them in your derived class
		virtual void OnMenuFileSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuFileExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuMQTT( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuMQTTStart( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuMQTTStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuMQTTMessage( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSendMQTTRawMessage( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuGateWayDetector( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuAddGateWay( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLogPanelSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnInitTimer( wxTimerEvent& event ) { event.Skip(); }
		virtual void OnMaintreeItemActivated( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnMaintreeItemRightClick( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnMQTTPingTimer( wxTimerEvent& event ) { event.Skip(); }
		virtual void OnUpdateUITimer( wxTimerEvent& event ) { event.Skip(); }
		virtual void OnMenuMaintreeCopySerialNumber( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuMaintreeSendMQTTRawMessageDefaultName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuMaintreeSendMQTTRawMessageToolName( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxPanel* m_panel_log;
		wxTextCtrl* m_textCtrl_log;

		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;

		~GUIFrame();

		void GUIFrameOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( m_menu_maintree, event.GetPosition() );
		}

};

///////////////////////////////////////////////////////////////////////////////
/// Class MainPage
///////////////////////////////////////////////////////////////////////////////
class MainPage : public wxPanel
{
	private:

	protected:

		// Virtual event handlers, override them in your derived class
		virtual void OnInit( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }


	public:
		wxBoxSizer* bSizer1;

		MainPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~MainPage();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialog
///////////////////////////////////////////////////////////////////////////////
class AboutDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText1;
		wxStaticLine* m_staticline1;
		wxHyperlinkCtrl* m_hyperlink1;
		wxHyperlinkCtrl* m_hyperlink2;
		wxHyperlinkCtrl* m_hyperlink3;

	public:

		AboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );

		~AboutDialog();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MQTTDialog
///////////////////////////////////////////////////////////////////////////////
class MQTTDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrl_Name;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textCtrl_ServerAddr;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_textCtrl_ServerPort;
		wxButton* m_button1;

		// Virtual event handlers, override them in your derived class
		virtual void OnButtonOk( wxCommandEvent& event ) { event.Skip(); }


	public:

		MQTTDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("MQTT"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );

		~MQTTDialog();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GateWayDetectorDialog
///////////////////////////////////////////////////////////////////////////////
class GateWayDetectorDialog : public wxDialog
{
	private:

	protected:
		wxListCtrl* m_list;
		wxTimer m_GateWayDetectorUpdatetimer;
		wxMenu* m_RightClickMenu;

		// Virtual event handlers, override them in your derived class
		virtual void OnListItemRightClick( wxListEvent& event ) { event.Skip(); }
		virtual void OnGateWayDetectorUpdatetimer( wxTimerEvent& event ) { event.Skip(); }
		virtual void OnMenuItemCopy( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuItemAddGateWayToWorkSpace( wxCommandEvent& event ) { event.Skip(); }


	public:

		GateWayDetectorDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("网关发现"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );

		~GateWayDetectorDialog();

		void GateWayDetectorDialogOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( m_RightClickMenu, event.GetPosition() );
		}

};

///////////////////////////////////////////////////////////////////////////////
/// Class GateWayPage
///////////////////////////////////////////////////////////////////////////////
class GateWayPage : public wxPanel
{
	private:

	protected:
		wxAuiNotebook* m_auinotebook_gateway_workspace;
		wxPanel* m_operatepanel;
		wxDataViewTreeCtrl* m_dataViewTreeCtrl_GateWay;
		wxTimer m_UpdateGateWayPagetimer;

		// Virtual event handlers, override them in your derived class
		virtual void OnUpdateGateWayPagetimer( wxTimerEvent& event ) { event.Skip(); }


	public:

		GateWayPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		wxAuiManager m_mgr;

		~GateWayPage();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GateWayPageLogPage
///////////////////////////////////////////////////////////////////////////////
class GateWayPageLogPage : public wxPanel
{
	private:

	protected:

	public:
		wxTextCtrl* m_textCtrl_log;

		GateWayPageLogPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~GateWayPageLogPage();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MQTTMessagePage
///////////////////////////////////////////////////////////////////////////////
class MQTTMessagePage : public wxPanel
{
	private:

	protected:
		wxDataViewListCtrl* m_MQTTMessagedataViewList;
		wxTimer m_InitMQTTMessagePagetimer;

		// Virtual event handlers, override them in your derived class
		virtual void OnMQTTMessageItemActivated( wxDataViewEvent& event ) { event.Skip(); }
		virtual void OnInitMQTTMessagePagetimer( wxTimerEvent& event ) { event.Skip(); }


	public:

		MQTTMessagePage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~MQTTMessagePage();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MQTTMessageDataDetail
///////////////////////////////////////////////////////////////////////////////
class MQTTMessageDataDetail : public wxDialog
{
	private:

	protected:

	public:
		wxTextCtrl* m_textDetail;

		MQTTMessageDataDetail( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );

		~MQTTMessageDataDetail();

};

///////////////////////////////////////////////////////////////////////////////
/// Class SendMQTTRawMessageDialog
///////////////////////////////////////////////////////////////////////////////
class SendMQTTRawMessageDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText5;
		wxStaticText* m_staticText6;
		wxCheckBox* m_checkBox_IsHex;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText9;
		wxButton* m_button_Send;
		wxButton* m_button_Exit;

		// Virtual event handlers, override them in your derived class
		virtual void OnButtonSendClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonExitClick( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxTextCtrl* m_textCtrl_topic;
		wxTextCtrl* m_textCtrl_payload;
		wxRadioBox* m_radioBox_Qos;
		wxCheckBox* m_checkBox_Retain;

		SendMQTTRawMessageDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("发送MQTT消息"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );

		~SendMQTTRawMessageDialog();

};

