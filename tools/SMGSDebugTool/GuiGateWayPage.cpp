#include "GuiGateWayPage.h"
#include "InternalDatabase.h"
#include "SMGSDebugToolMain.h"

GuiGateWayPage::GuiGateWayPage(wxString Addr,wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):GateWayPage(parent,id,pos,size,style,name),GateWayAddr(Addr)
{
    //ctor
    std::map<wxString,wxString> Con,Dat;
    Con[_T("Addr")]=GateWayAddr;
    Dat[_T("IsOpen")]=_T("1");
    InternalDatabase_Table_Update_Data(_T(SMGSDebugToolWorkSpaceGateWayList),Dat,Con);

}

GuiGateWayPage::~GuiGateWayPage()
{
    //dtor
    std::map<wxString,wxString> Con,Dat;
    Con[_T("Addr")]=GateWayAddr;
    Dat[_T("IsOpen")]=_T("0");
    InternalDatabase_Table_Update_Data(_T(SMGSDebugToolWorkSpaceGateWayList),Dat,Con);
}
