/***************************************************************
 * Name:      SMGSDeubgToolApp.cpp
 * Purpose:   Code for Application Class
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

#include "SMGSDebugToolApp.h"
#include "SMGSDebugToolMain.h"

IMPLEMENT_APP(SMGSDebugToolApp);

static SMGSDebugToolFrame * MainFrame=NULL;

bool SMGSDebugToolApp::OnInit()
{
    SMGSDebugToolFrame* frame = new SMGSDebugToolFrame(0L);
    frame->Show();

    MainFrame=frame;

    return true;
}

SMGSDebugToolApp::SMGSDebugToolApp()
{

}
SMGSDebugToolApp::~SMGSDebugToolApp()
{
    MainFrame=NULL;
}

SMGSDebugToolFrame * SMGSDebugToolApp_GetMainFrame()
{
    return MainFrame;
}
