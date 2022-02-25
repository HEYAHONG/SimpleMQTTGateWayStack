/***************************************************************
 * Name:      SMGSDebugToolMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2022-02-24
 * Copyright: HYH (hyhsystem.cn)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/log.h>
#include <wx/datetime.h>
#include <wx/filedlg.h>
#include "Res.h"

#include "SMGSDebugToolMain.h"
#include "InternalDatabase.h"

class SMGSLogFormatter : public wxLogFormatter
{
    virtual wxString Format(wxLogLevel level,
                            const wxString& msg,
                            const wxLogRecordInfo& info) const
    {
        wxDateTime now = wxDateTime::Now();
        return wxString::Format("%02d-%02d-%02d %02d:%02d:%02d  %s",now.GetYear(),now.GetMonth()-wxDateTime::Month::Jan+1,now.GetDay(),now.GetHour(),now.GetMinute(),now.GetSecond(),msg);
    }
};


SMGSDebugToolFrame::SMGSDebugToolFrame(wxFrame *frame)
    : GUIFrame(frame)
{
    //设置icon
    SetIcon(logo_xpm);

    //设置标题
    SetTitle(_T("SMGSDebugTool"));
}

void SMGSDebugToolFrame::OnInitTimer( wxTimerEvent& event )
{

    wxLog::EnableLogging(true);
    {
        //设置日志窗口
        wxLogTextCtrl *logger=new wxLogTextCtrl(m_textCtrl_log);
        wxLog::SetActiveTarget(logger);
        SMGSLogFormatter *formatter=new SMGSLogFormatter();
        logger->SetFormatter(formatter);

    }

    wxLogMessage(_T("程序已启动!"));

    //初始化内部数据
    InternalDatabase_Init();

    InternalDatebase_ProgramInfo_Set(_T("Name"),_T("SMGSDebugTool"));

}


void SMGSDebugToolFrame::OnMenuFileExit( wxCommandEvent& event )
{
    //退出程序
    Close();
}

void SMGSDebugToolFrame::OnMenuFileSave( wxCommandEvent& event )
{
    wxFileDialog saveFileDialog(this, _T("保存内部数据"), "", "","db files (*.db)|*.db", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if(saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    if(InternalDatabase_Backup(saveFileDialog.GetPath()))
    {
        wxLogMessage(_T("保存内部数据成功!"));
    }
    else
    {
        wxLogMessage(_T("保存内部数据失败!"));
    }

}

void SMGSDebugToolFrame::OnLogPanelSize( wxSizeEvent& event )
{
    m_textCtrl_log->SetSize(event.GetSize());
}


void SMGSDebugToolFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void SMGSDebugToolFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void SMGSDebugToolFrame::OnAbout(wxCommandEvent &event)
{

}

SMGSDebugToolFrame::~SMGSDebugToolFrame()
{
    //反初始化内部数据库
    InternalDatabase_Deinit();
}
