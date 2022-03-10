#ifndef GUIMQTTDIALOG_H
#define GUIMQTTDIALOG_H

#include "GUIFrame.h"

class GuiMQTTDialog: public MQTTDialog
{
    public:
        GuiMQTTDialog(wxWindow* parent, wxWindowID id = wxID_ANY);
        virtual ~GuiMQTTDialog();

    protected:
        virtual void OnButtonOk( wxCommandEvent& event );

    private:
};

#endif // GUIMQTTDIALOG_H
