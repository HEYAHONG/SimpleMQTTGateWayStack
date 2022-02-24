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

#include "SMGSDebugToolMain.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };



SMGSDebugToolFrame::SMGSDebugToolFrame(wxFrame *frame)
    : GUIFrame(frame)
{

}

SMGSDebugToolFrame::~SMGSDebugToolFrame()
{
}

void SMGSDebugToolFrame::OnMenuFileExit( wxCommandEvent& event )
{
    Close();
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
