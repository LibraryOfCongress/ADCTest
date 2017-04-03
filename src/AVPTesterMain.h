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
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

#include "AudioDevicesDialog.h"
#include "Widgets\GButton.h"
#include "TestManager.h"
#include "ResultsDialog.h"

//threads ID
enum {
	AUDIOT_EVENT = wxID_HIGHEST + 1
};

class AVPTesterFrame: public wxFrame
{
    public:

        AVPTesterFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~AVPTesterFrame();

		void BuildUI();

    private:

        //(*Handlers(AVPTesterFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnMenuItemDevicesSelected(wxCommandEvent& event);
        void OnMenuItemOpenManualSelected(wxCommandEvent& event);
        void OnButtonStartTestsClick(wxCommandEvent& event);
        void OnListViewTestsItemSelect(wxListEvent& event);
        void OnListViewTestsItemActivated(wxListEvent& event);
        void OnListViewParametersItemSelect(wxListEvent& event);
        void OnListViewTestsItemRClick(wxListEvent& event);
        void OnListViewParametersItemActivated(wxListEvent& event);
        void OnMenuItemFOpenSelected(wxCommandEvent& event);
        void OnMenuItemFSaveSelected(wxCommandEvent& event);
        //*)
		void OnTestMenuPopupClick(wxCommandEvent &evt);

		void OnAudioThreadEvent(wxThreadEvent& event);
		void PopulateTestsList();
		void PopulateTestParametersList();
		void EnableTestUI(bool enabled);

		void WriteLogMsg(wxString msg);

        //(*Identifiers(AVPTesterFrame)
        static const long ID_STATICBITMAP_FADGI;
        static const long ID_STATICTEXT_PROC_TTL;
        static const long ID_LISTVIEW_TESTS;
        static const long ID_STATICTEXT_LINE1;
        static const long ID_BUTTON_START_TESTS;
        static const long ID_PANEL_CTRL_BTNS;
        static const long ID_PANEL_TST_LST;
        static const long ID_STATICTEXT_PARAMS_TTL;
        static const long ID_LISTVIEW_PARAMETERS;
        static const long ID_STATICTEXT_LINE2;
        static const long ID_PANEL_PARAMS_BTNS;
        static const long ID_PANEL_PRMS_LST;
        static const long ID_PANEL_LIST;
        static const long ID_STATICTEXT_TH_LOG;
        static const long ID_TEXTCTRL_LOG;
        static const long ID_STATICTEXT_LINE3;
        static const long ID_PANEL_LOG;
        static const long ID_PANEL_BOTTOM;
        static const long ID_PANEL_MAIN_UI;
        static const long ID_MENUITEM_FOPEN;
        static const long ID_MENUITEM_FSAVE;
        static const long idMenuQuit;
        static const long ID_MENUITEM_ADEVICES;
        static const long ID_MENUITEM_OPEN_GUIDE;
        static const long idMenuAbout;
        static const long ID_STATUSBAR_MAIN;
        static const long ID_TOOLBAR1;
        //*)

        //(*Declarations(AVPTesterFrame)
        RimPanel* PanelParamsLst;
        wxToolBar* ToolBar1;
        wxStaticText* StaticTextLine1;
        wxStaticText* StaticTextThLog;
        wxStaticText* StaticTextLine2;
        wxMenuItem* MenuItemFSave;
        wxPanel* PanelParamsBtns;
        wxTextCtrl* TextCtrlLog;
        wxBoxSizer* BoxSizerMainUI;
        wxPanel* PanelCtrlBtns;
        wxPanel* PanelMainUI;
        wxListView* ListViewTests;
        wxStaticText* StaticTextLine3;
        wxStaticText* StaticTextParamsTtl;
        RimPanel* PanelList;
        RimPanel* PanelLog;
        wxMenu* MenuSettings;
        RimPanel* PanelBottom;
        wxListView* ListViewParameters;
        GButton* ButtonStartTests;
        wxStaticBitmap* StaticBitmapFADGI;
        RimPanel* PanelTestList;
        wxStaticText* StaticTextProcTtl;
        wxStatusBar* StatusBarMain;
        wxMenuItem* MenuItemDevices;
        wxMenuItem* MenuItemOpenManual;
        wxMenuItem* MenuItemFOpen;
        //*)

		AudioDevicesDialog* mADevicesDialog;
		ResultsDialog* mResultsDialog;

        DECLARE_EVENT_TABLE()

		int mSelectedTestIdx;
		int mSelectedParamIdx;
		std::vector<TestDescriptor> mTestDescriptors;

		bool mTestOn;
		bool mTestIsRunning;
		wxString mLogMsg;
};

#endif // AVPTESTERMAIN_H
