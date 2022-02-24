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

#include "SMGSDebugToolMain.h"

class SMGSLogFormatter : public wxLogFormatter
{
    virtual wxString Format(wxLogLevel level,
                            const wxString& msg,
                            const wxLogRecordInfo& info) const
    {
        wxDateTime now = wxDateTime::Now();
        return wxString::Format("%02d-%02d-%02d %02d:%02d:%02d %03d %s",now.GetYear(),now.GetMonth(),now.GetDay(),now.GetHour(),now.GetMinute(),now.GetSecond(),now.GetMillisecond(),msg);
    }
};


SMGSDebugToolFrame::SMGSDebugToolFrame(wxFrame *frame)
    : GUIFrame(frame)
{

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
}

SMGSDebugToolFrame::~SMGSDebugToolFrame()
{
}

void SMGSDebugToolFrame::OnMenuFileExit( wxCommandEvent& event )
{
    //退出程序
    Close();
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
