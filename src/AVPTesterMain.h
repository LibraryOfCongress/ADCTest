/***************************************************************
 * Name:      AVPTesterMain.h
 * Purpose:   Defines Application Frame
 * Author:    clandone (administrator@aworldofhertz.com)
 * Created:   2017-01-11
 * Copyright: clandone (www.aworldofhertz.com)
 * License:
 **************************************************************/

#ifndef AVPTESTERMAIN_H
#define AVPTESTERMAIN_H

//(*Headers(AVPTesterFrame)
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

#include "AudioDevicesPanel.h"

class AVPTesterFrame: public wxFrame
{
    public:

        AVPTesterFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~AVPTesterFrame();

        void TestStuff();
		void BuildUI();

    private:

        //(*Handlers(AVPTesterFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnMenuItemDevicesSelected(wxCommandEvent& event);
        //*)

        //(*Identifiers(AVPTesterFrame)
        static const long ID_PANEL_MAIN_UI;
        static const long idMenuQuit;
        static const long ID_MENUITEM_ADEVICES;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        static const long ID_TOOLBAR1;
        //*)

        //(*Declarations(AVPTesterFrame)
        wxToolBar* ToolBar1;
        wxMenu* Menu3;
        wxPanel* PanelMainUI;
        wxBoxSizer* BoxSizerMain;
        wxStatusBar* StatusBar1;
        wxMenuItem* MenuItemDevices;
        //*)

		AudioDevicesPanel* mADevicesPanel;

        DECLARE_EVENT_TABLE()
};

#endif // AVPTESTERMAIN_H
