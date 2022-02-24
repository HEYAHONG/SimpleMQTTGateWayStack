/***************************************************************
 * Name:      SMGSDeubgToolMain.cpp
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

#include "SMGSDeubgToolMain.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };



SMGSDeubgToolFrame::SMGSDeubgToolFrame(wxFrame *frame)
    : GUIFrame(frame)
{

}

SMGSDeubgToolFrame::~SMGSDeubgToolFrame()
{
}

void SMGSDeubgToolFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void SMGSDeubgToolFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void SMGSDeubgToolFrame::OnAbout(wxCommandEvent &event)
{
    
}
