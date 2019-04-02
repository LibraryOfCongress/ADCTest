////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef AVPTESTERMAIN_H
#define AVPTESTERMAIN_H

//(*Headers(AVPTesterFrame)
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/toolbar.h>
//*)

#include "AudioDevicesDialog.h"
#include "Widgets\GButton.h"
#include "TestManager.h"
#include "ResultsDialog.h"
#include "ParamEditDialog.h"
#include "WaitDialog.h"
#include "Project\ProjectManager.h"

class AVPTesterFrame;

//threads ID
enum {
	AUDIOT_EVENT = wxID_HIGHEST + 1
};

class ProjectLoadThread : public wxThread {
public:
	ProjectLoadThread(AVPTesterFrame* dlg);
	ExitCode Entry() override;

private:
	AVPTesterFrame * m_dlg;
};


class AVPTesterFrame: public wxFrame
{
    public:

        AVPTesterFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~AVPTesterFrame();

		void BuildUI();

		ProjectLoadThread* mPrjThread;
		void PopulateTestsListThx();


    private:

		bool wxViewPDFFile(const wxString& pdfFile, const wxString& specificErrorMessage = wxEmptyString);
		bool wxSeparateCommandAndArguments(const wxString& command, wxString& cmd, wxString& args);

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
        void OnButtonDoSingleTestClick(wxCommandEvent& event);
        void OnMenuItemFNewSelected(wxCommandEvent& event);
        void OnMenuItemFCloseSelected(wxCommandEvent& event);
        void OnButtonProjEditCtrlsClick(wxCommandEvent& event);
        void OnMenuItemEditPInfoSelected(wxCommandEvent& event);
        void OnTimerLoadProjectTrigger(wxTimerEvent& event);
        void OnListViewParametersBeginDrag(wxListEvent& event);
        void OnChoiceTestModeSelect(wxCommandEvent& event);
        void OnButtonRespFileClick(wxCommandEvent& event);
        void OnChoiceDACChSelect(wxCommandEvent& event);
        void OnChoiceADCChSelect(wxCommandEvent& event);
        void OnMenuItemInfoSelected(wxCommandEvent& event);
        //*)
		void OnTestMenuPopupClick(wxCommandEvent &evt);

		void OnAudioThreadEvent(wxThreadEvent& event);
		void PopulateTestsList();
		void PopulateProjectInfo();
		void PopulateTestParametersList();
		void PopulateAudioIOInfo(bool enabled);
		void PopulateModeInfo(bool enabled);
		void EnableTestUI(bool enabled, bool isLoading=false);

		void SetTestModeUI(wxString mode, wxString fileName, bool enabled, bool setChoice = true);

		void ParseResults();
		wxString GetOutcomeFromFile(wxString resultsPath);

		void WriteLogMsg(wxString msg);

        //(*Identifiers(AVPTesterFrame)
        static const long ID_STATICTEXT_LOGOLINE_TOP;
        static const long ID_STATICBITMAP_FADGI;
        static const long ID_STATICTEXT_PROJECTINFO_LBL;
        static const long ID_STATICTEXT_PTITLE_LBL;
        static const long ID_TEXTCTRL_PTITLE;
        static const long ID_PANEL_PTITLE;
        static const long ID_STATICTEXT_PDATE_LBL;
        static const long ID_TEXTCTRL_PDATE;
        static const long ID_PANEL_PDATE;
        static const long ID_STATICTEXT_POPS_LBL;
        static const long ID_TEXTCTRL_POPS;
        static const long ID_PANEL_POps;
        static const long ID_STATICTEXT_DAC_LBL;
        static const long ID_TEXTCTRL_DAC;
        static const long ID_PANEL_DAC;
        static const long ID_STATICTEXT_ADC_LBL;
        static const long ID_TEXTCTRL_ADC;
        static const long ID_PANEL_ADC;
        static const long ID_STATICTEXT_COMMENTS_LBL;
        static const long ID_TEXTCTRL_PROJ_DESC;
        static const long ID_PANEL_COMMENTS;
        static const long ID_STATICTEXT_TH_LOG;
        static const long ID_TEXTCTRL_LOG;
        static const long ID_STATICTEXT_LINE3;
        static const long ID_PANEL_LOG;
        static const long ID_STATICTEXT2;
        static const long ID_PANEL_PROJECT;
        static const long ID_STATICTEXT_PROC_TTL;
        static const long ID_LISTVIEW_TESTS;
        static const long ID_STATICTEXT_LINE1;
        static const long ID_BUTTON_START_TESTS;
        static const long ID_PANEL_CTRL_BTNS;
        static const long ID_PANEL_TST_LST;
        static const long ID_STATICTEXT_DESC_TTL;
        static const long ID_TEXTCTRL_TEST_DESCRIPTION;
        static const long ID_STATICTEXT_LINE_0;
        static const long ID_PANEL_TEST_DESCRIPTION;
        static const long ID_STATICTEXT_TEST_MODE;
        static const long ID_CHOICE_TEST_MODE;
        static const long ID_BUTTON_RESP_FILE;
        static const long ID_STATICTEXT3;
        static const long ID_PANEL_TEST_MODE;
        static const long ID_STATICTEXT_AUDIO_ROUTING;
        static const long ID_STATICTEXT_DAC_CH;
        static const long ID_CHOICE_DAC_CH;
        static const long ID_TEXTCTRL_DAC_NAME;
        static const long ID_PANEL_AUDIO_DAC;
        static const long ID_STATICTEXT_ADC_CH;
        static const long ID_CHOICE_ADC_CH;
        static const long ID_TEXTCTRL_ADC_NAME;
        static const long ID_PANEL_AUDIO_ADC;
        static const long ID_STATICTEXT4;
        static const long ID_PANEL_TEST_AUDIO;
        static const long ID_STATICTEXT_PARAMS_TTL;
        static const long ID_LISTVIEW_PARAMETERS;
        static const long ID_STATICTEXT_LINE2;
        static const long ID_BUTTON_DO_SINGLETEST;
        static const long ID_PANEL_PARAMS_BTNS;
        static const long ID_PANEL_PRMS_LST;
        static const long ID_PANEL_LIST;
        static const long ID_PANEL_BOTTOM;
        static const long ID_PANEL_MAIN_UI;
        static const long ID_MENUITEM_FOPEN;
        static const long ID_MENUITEM_FNEW;
        static const long ID_MENUITEM_FCLOSE;
        static const long ID_MENUITEM_FSAVE;
        static const long idMenuQuit;
        static const long ID_MENUITEM_ADEVICES;
        static const long ID_MENUITEM_PINFO;
        static const long ID_MENUITEM_OPEN_GUIDE;
        static const long ID_MENUITEM_OPEN_INFO;
        static const long ID_STATUSBAR_MAIN;
        static const long ID_TOOLBAR1;
        static const long ID_TIMER_LOAD_PRJ;
        //*)

        //(*Declarations(AVPTesterFrame)
        GButton* ButtonDoSingleTest;
        GButton* ButtonRespFile;
        GButton* ButtonStartTests;
        RimPanel* PanelADC;
        RimPanel* PanelAudioADC;
        RimPanel* PanelAudioDAC;
        RimPanel* PanelBottom;
        RimPanel* PanelComments;
        RimPanel* PanelDAC;
        RimPanel* PanelLog;
        RimPanel* PanelPDate;
        RimPanel* PanelPOps;
        RimPanel* PanelPTitle;
        RimPanel* PanelParamsLst;
        RimPanel* PanelProject;
        RimPanel* PanelTestAudio;
        RimPanel* PanelTestDescription;
        RimPanel* PanelTestList;
        RimPanel* PanelTestMode;
        wxBoxSizer* BoxSizerMainUI;
        wxChoice* ChoiceADCCh;
        wxChoice* ChoiceDACCh;
        wxChoice* ChoiceTestMode;
        wxListView* ListViewParameters;
        wxListView* ListViewTests;
        wxMenu* MenuSettings;
        wxMenuItem* MenuItemDevices;
        wxMenuItem* MenuItemEditPInfo;
        wxMenuItem* MenuItemFClose;
        wxMenuItem* MenuItemFNew;
        wxMenuItem* MenuItemFOpen;
        wxMenuItem* MenuItemFSave;
        wxMenuItem* MenuItemInfo;
        wxMenuItem* MenuItemOpenManual;
        wxPanel* PanelCtrlBtns;
        wxPanel* PanelList;
        wxPanel* PanelMainUI;
        wxPanel* PanelParamsBtns;
        wxStaticBitmap* StaticBitmapFADGI;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticTextADCCh;
        wxStaticText* StaticTextADCLbl;
        wxStaticText* StaticTextAudioRouting;
        wxStaticText* StaticTextCommentsLbl;
        wxStaticText* StaticTextDACCh;
        wxStaticText* StaticTextDACLbl;
        wxStaticText* StaticTextLine0;
        wxStaticText* StaticTextLine1;
        wxStaticText* StaticTextLine2;
        wxStaticText* StaticTextLine3;
        wxStaticText* StaticTextLogoLineTop;
        wxStaticText* StaticTextPDateLbl;
        wxStaticText* StaticTextPOpsLbl;
        wxStaticText* StaticTextPTitleLbl;
        wxStaticText* StaticTextParamsTtl;
        wxStaticText* StaticTextProcTtl;
        wxStaticText* StaticTextProjectInfoLabel;
        wxStaticText* StaticTextTestDescTTl;
        wxStaticText* StaticTextTestMode;
        wxStaticText* StaticTextThLog;
        wxStatusBar* StatusBarMain;
        wxTextCtrl* TextCtrlADC;
        wxTextCtrl* TextCtrlADCName;
        wxTextCtrl* TextCtrlDAC;
        wxTextCtrl* TextCtrlDACName;
        wxTextCtrl* TextCtrlLog;
        wxTextCtrl* TextCtrlPDate;
        wxTextCtrl* TextCtrlPOps;
        wxTextCtrl* TextCtrlPTitle;
        wxTextCtrl* TextCtrlProjDescription;
        wxTextCtrl* TextCtrlTestDescription;
        wxTimer TimerLoadProject;
        wxToolBar* ToolBar1;
        //*)

		AudioDevicesDialog* mADevicesDialog;
		ResultsDialog* mResultsDialog;
		ParamEditDialog* mParamDialog;

        DECLARE_EVENT_TABLE()

		int mSelectedTestIdx;
		std::vector<TestDescriptor> mTestDescriptors;
		int mSelectedParamIdx;
		std::vector<TestParameter> mParametersDescriptors;

		ProjectManager* mPManager;
		bool mProjectLoading;
		WaitDialog* mWaitDialog;
		int mWaitDialogID;

		bool mTestOn;
		bool mTestIsRunning;
		wxString mLogMsg;
};

#endif // AVPTESTERMAIN_H
