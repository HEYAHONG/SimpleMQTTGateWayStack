#ifndef GUISENDMQTTRAWMESSAGEDIALOG_H
#define GUISENDMQTTRAWMESSAGEDIALOG_H

#include "GUIFrame.h"

class GuiSendMQTTRawMessageDialog:public SendMQTTRawMessageDialog
{
    public:
        GuiSendMQTTRawMessageDialog(wxWindow* parent, wxWindowID id = wxID_ANY);
        virtual ~GuiSendMQTTRawMessageDialog();

    protected:
    virtual void OnButtonSendClick( wxCommandEvent& event );
    virtual void OnButtonExitClick( wxCommandEvent& event );


    private:
};

#endif // GUISENDMQTTRAWMESSAGEDIALOG_H
