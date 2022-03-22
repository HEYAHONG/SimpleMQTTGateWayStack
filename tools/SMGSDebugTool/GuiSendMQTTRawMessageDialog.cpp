#include "GuiSendMQTTRawMessageDialog.h"
#include "SMGSDebugToolMain.h"
#include "wx/msgdlg.h"

GuiSendMQTTRawMessageDialog::GuiSendMQTTRawMessageDialog(wxWindow* parent, wxWindowID id):SendMQTTRawMessageDialog(parent,id)
{
    //ctor
}

void GuiSendMQTTRawMessageDialog::OnButtonSendClick( wxCommandEvent& event )
{
    wxString topic=m_textCtrl_topic->GetValue();

    if(topic.empty())
    {
        wxMessageBox(_T("主题不能为空!"),_T("错误"));
        return;
    }

    wxString payload=m_textCtrl_payload->GetValue();

    if(m_checkBox_IsHex->IsChecked() && !payload.empty())
    {
        wxString _payload=payload;
        _payload.Replace(" ","");
        _payload.Replace("\r","");
        _payload.Replace("\n","");
        for(size_t i=0; i<_payload.length(); i++)
        {
            char dat=((const char *)_payload)[i];
            if(!((dat >= '0' && dat <= '9') || (dat >= 'A' && dat <= 'F') || (dat >= 'a' && dat <= 'F')))
            {
                wxMessageBox(_T("Payload不是Hex字符串!"),_T("错误"));
                return;
            }
        }

        payload.clear();

        for(size_t i=0; i<_payload.length(); i+=2)
        {
            wxString DatStr=_payload.substr(i,2);
            long dat=0;
            if(DatStr.ToCLong(&dat,16))
            {
                payload.Append((char)dat);
            }
        }


    }

    SMGSDebugToolFrame *frame=SMGSDebugToolApp_GetMainFrame();
    if(frame!=NULL)
    {
        if(!frame->MQTTPublishMessage(topic,(void *)(const char *)payload,payload.length(),m_radioBox_Qos->GetSelection(),m_checkBox_Retain->IsChecked()?1:0))
        {
            wxMessageBox(_T("发送失败!"),_T("错误"));
            return;
        }
    }
    EndModal(wxID_OK);
}
void GuiSendMQTTRawMessageDialog::OnButtonExitClick( wxCommandEvent& event )
{
    EndModal(wxID_CANCEL);
}

GuiSendMQTTRawMessageDialog::~GuiSendMQTTRawMessageDialog()
{
    //dtor
}
