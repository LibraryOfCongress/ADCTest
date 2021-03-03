/***************************************************************
 * Name:      FDEngineTestMain.h
 * Purpose:   Defines Application Frame
 * Author:    clandone ()
 * Created:   2018-08-02
 * Copyright: clandone ()
 * License:
 **************************************************************/

#ifndef FDENGINETESTMAIN_H
#define FDENGINETESTMAIN_H

//(*Headers(FDEngineTestDialog)
#include "AudioDevicesPanel.h"
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/statline.h>
//*)

#include "AudioIO/FDAudioEngine.h"

class FDEngineTestDialog: public wxDialog
{
    public:

        FDEngineTestDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~FDEngineTestDialog();

    private:

        //(*Handlers(FDEngineTestDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(FDEngineTestDialog)
        static const long ID_BUTTON1;
        static const long ID_STATICLINE1;
        static const long ID_BUTTON2;
        static const long ID_ADEVS_PANEL;
        //*)

        //(*Declarations(FDEngineTestDialog)
        AudioDevicesPanel* mADPanel;
        wxBoxSizer* BoxSizer1;
        wxBoxSizer* BoxSizer2;
        wxButton* Button1;
        wxButton* Button2;
        wxStaticLine* StaticLine1;
        //*)

        DECLARE_EVENT_TABLE()

    protected:
};

#endif // FDENGINETESTMAIN_H
