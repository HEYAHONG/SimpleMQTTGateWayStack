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

#include "SMGSDeubgToolApp.h"
#include "SMGSDeubgToolMain.h"

IMPLEMENT_APP(SMGSDeubgToolApp);

bool SMGSDeubgToolApp::OnInit()
{
    SMGSDeubgToolFrame* frame = new SMGSDeubgToolFrame(0L);
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();
    
    return true;
}
