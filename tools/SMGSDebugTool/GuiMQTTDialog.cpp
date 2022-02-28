#include "GuiMQTTDialog.h"
#include "InternalDatabase.h"
#include "libSMGS-Server.h"
#include <wx/msgdlg.h>

GuiMQTTDialog::GuiMQTTDialog(wxWindow* parent, wxWindowID id):MQTTDialog(parent,id)
{
    //ctor
    wxString Name=InternalDatebase_ProgramInfo_Get(_T("Name"));
    m_textCtrl_Name->SetValue(Name);

    wxString ServerAddr=InternalDatebase_ProgramInfo_Get(_T("Server"));
    m_textCtrl_ServerAddr->SetValue(ServerAddr);

    wxString ServerPort=InternalDatebase_ProgramInfo_Get(_T("Port"));
    m_textCtrl_ServerPort->SetValue(ServerPort);
}

void GuiMQTTDialog::OnButtonOk( wxCommandEvent& event )
{
    wxString Name=m_textCtrl_Name->GetValue();
    if(Name.empty() || Name.length() >= CONFIG_SMGS_MIN_GATEWAY_SERIALNUMBER_LENGTH)
    {
        wxMessageBox(_T("名称过长或过短!"),_T("错误"));
        return;
    }

    wxString ServerAddr=m_textCtrl_ServerAddr->GetValue();
    wxString ServerPort=m_textCtrl_ServerPort->GetValue();

    {
        long i=0;
        if( !ServerPort.ToLong(&i) ||i<=0 || i>65535)
        {
            wxMessageBox(_T("端口输入错误!"),_T("错误"));
            return;
        }
    }

    InternalDatebase_ProgramInfo_Set(_T("Name"),Name);
    InternalDatebase_ProgramInfo_Set(_T("Server"),ServerAddr);
    InternalDatebase_ProgramInfo_Set(_T("Port"),ServerPort);

    EndModal(wxID_OK);
}

GuiMQTTDialog::~GuiMQTTDialog()
{
    //dtor
}
