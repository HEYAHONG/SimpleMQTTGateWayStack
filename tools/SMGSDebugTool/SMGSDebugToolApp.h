/***************************************************************
 * Name:      SMGSDeubgToolApp.h
 * Purpose:   Defines Application Class
 * Author:    HYH (2229388563@qq.com)
 * Created:   2022-02-24
 * Copyright: HYH (hyhsystem.cn)
 * License:
 **************************************************************/

#ifndef SMGSDEUBUGTOOLAPP_H
#define SMGSDEUBUGTOOLAPP_H

#include <wx/app.h>

class SMGSDebugToolApp : public wxApp
{
public:
    virtual bool OnInit();
    SMGSDebugToolApp();
    ~SMGSDebugToolApp();
};

class SMGSDebugToolFrame;

/*
获取主Frame指针，失败返回NULL
*/
SMGSDebugToolFrame * SMGSDebugToolApp_GetMainFrame();

#endif // SMGSDEUBGTOOLAPP_H
