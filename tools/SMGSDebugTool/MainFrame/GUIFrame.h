///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
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

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrame
///////////////////////////////////////////////////////////////////////////////
class GUIFrame : public wxFrame
{
	private:

	protected:
		enum
		{
			ID_Save = 1000,
			ID_Menu_File_Exit,
			ID_Menu_MQTT,
			ID_Menu_MQTT_Start,
			ID_Menu_MQTT_Stop,
			ID_Menu_About,
			wxID_InitTimer,
			wxID_MQTTPingTimer,
			wxID_UpdateUItimer
		};

		wxStatusBar* m_statusBar;
		wxMenuBar* m_menubar;
		wxMenu* Menu_File;
		wxMenu* Menu_Net;
		wxMenu* Menu_Help;
		wxTimer m_Inittimer;
		wxAuiNotebook* m_notebook_workspace;
		wxTreeCtrl* m_maintree;
		wxTimer m_MQTTPingtimer;
		wxTimer m_UpdateUItimer;

		// Virtual event handlers, override them in your derived class
		virtual void OnMenuFileSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuFileExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuMQTT( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuMQTTStart( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuMQTTStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLogPanelSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnInitTimer( wxTimerEvent& event ) { event.Skip(); }
		virtual void OnMQTTPingTimer( wxTimerEvent& event ) { event.Skip(); }
		virtual void OnUpdateUITimer( wxTimerEvent& event ) { event.Skip(); }


	public:
		wxPanel* m_panel_log;
		wxTextCtrl* m_textCtrl_log;

		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;

		~GUIFrame();

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

