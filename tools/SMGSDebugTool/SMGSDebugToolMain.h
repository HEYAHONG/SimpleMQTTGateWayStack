/***************************************************************
 * Name:      SMGSDebugToolMain.h
 * Purpose:   Defines Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2022-02-24
 * Copyright: HYH (hyhsystem.cn)
 * License:
 **************************************************************/

#ifndef SMGSDebugTOOLMAIN_H
#define SMGSDebugTOOLMAIN_H



#include "SMGSDebugToolApp.h"


#include "GUIFrame.h"

class SMGSDebugToolFrame: public GUIFrame
{
    public:
        SMGSDebugToolFrame(wxFrame *frame);
        ~SMGSDebugToolFrame();
    private:
        void OnMenuFileExit( wxCommandEvent& event );
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
};

#endif // SMGSDebugTOOLMAIN_H
