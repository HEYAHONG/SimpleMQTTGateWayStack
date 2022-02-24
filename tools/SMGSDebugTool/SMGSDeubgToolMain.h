/***************************************************************
 * Name:      SMGSDeubgToolMain.h
 * Purpose:   Defines Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2022-02-24
 * Copyright: HYH (hyhsystem.cn)
 * License:
 **************************************************************/

#ifndef SMGSDEUBGTOOLMAIN_H
#define SMGSDEUBGTOOLMAIN_H



#include "SMGSDeubgToolApp.h"


#include "GUIFrame.h"

class SMGSDeubgToolFrame: public GUIFrame
{
    public:
        SMGSDeubgToolFrame(wxFrame *frame);
        ~SMGSDeubgToolFrame();
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
};

#endif // SMGSDEUBGTOOLMAIN_H
