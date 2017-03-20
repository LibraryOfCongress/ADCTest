/***************************************************************
 * Name:      AVPTesterMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    clandone (administrator@aworldofhertz.com)
 * Created:   2017-01-11
 * Copyright: clandone (www.aworldofhertz.com)
 * License:
 **************************************************************/

#include "AVPTesterMain.h"
#include <wx/msgdlg.h>
#include <wx/listctrl.h>

#include "System/Prefs.h"
#include "Devices/ADevicesManager.h"
#include "AudioIO/AudioEngine.h"

//(*InternalHeaders(AVPTesterFrame)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(AVPTesterFrame)
const long AVPTesterFrame::ID_STATICBITMAP_FADGI = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_PROC_TTL = wxNewId();
const long AVPTesterFrame::ID_LISTVIEW_TESTS = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_LINE1 = wxNewId();
const long AVPTesterFrame::ID_BUTTON_START_TESTS = wxNewId();
const long AVPTesterFrame::ID_PANEL_CTRL_BTNS = wxNewId();
const long AVPTesterFrame::ID_PANEL_TST_LST = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_PARAMS_TTL = wxNewId();
const long AVPTesterFrame::ID_LISTVIEW_PARAMETERS = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_LINE2 = wxNewId();
const long AVPTesterFrame::ID_PANEL_PARAMS_BTNS = wxNewId();
const long AVPTesterFrame::ID_PANEL_PRMS_LST = wxNewId();
const long AVPTesterFrame::ID_PANEL_LIST = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_TH_LOG = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_LOG = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_LINE3 = wxNewId();
const long AVPTesterFrame::ID_PANEL_LOG = wxNewId();
const long AVPTesterFrame::ID_PANEL_BOTTOM = wxNewId();
const long AVPTesterFrame::ID_PANEL_MAIN_UI = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_FOPEN = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_FSAVE = wxNewId();
const long AVPTesterFrame::idMenuQuit = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_ADEVICES = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_OPEN_GUIDE = wxNewId();
const long AVPTesterFrame::idMenuAbout = wxNewId();
const long AVPTesterFrame::ID_STATUSBAR_MAIN = wxNewId();
const long AVPTesterFrame::ID_TOOLBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(AVPTesterFrame,wxFrame)
    //(*EventTable(AVPTesterFrame)
    //*)
	EVT_THREAD( AUDIOT_EVENT, AVPTesterFrame::OnAudioThreadEvent)
END_EVENT_TABLE()

AVPTesterFrame::AVPTesterFrame(wxWindow* parent,wxWindowID id)
:mADevicesDialog(NULL)
{
    //(*Initialize(AVPTesterFrame)
    wxBoxSizer* BoxSizerParamsBtns;
    wxBoxSizer* BoxSizerLists;
    wxBoxSizer* BoxSizerLog;
    wxBoxSizer* BoxSizerFrame;
    wxMenuItem* MenuItemInfo;
    wxMenuItem* MenuItemQuit;
    wxMenu* MenuFile;
    wxBoxSizer* BoxSizerTestParams;
    wxBoxSizer* BoxSizerLogo;
    wxBoxSizer* BoxSizerTestList;
    wxMenuBar* MenuBarMain;
    wxMenu* Menu2;
    wxBoxSizer* BoxSizerCtrlBtns;
    wxBoxSizer* BoxSizerBottom;

    Create(parent, wxID_ANY, _("ADC Performance Test Tool"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxCLOSE_BOX, _T("wxID_ANY"));
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("UI\\favicon.ico"))));
    	SetIcon(FrameIcon);
    }
    BoxSizerFrame = new wxBoxSizer(wxHORIZONTAL);
    PanelMainUI = new wxPanel(this, ID_PANEL_MAIN_UI, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_MAIN_UI"));
    PanelMainUI->SetForegroundColour(wxColour(0,0,0));
    PanelMainUI->SetBackgroundColour(wxColour(210,210,210));
    BoxSizerMainUI = new wxBoxSizer(wxVERTICAL);
    BoxSizerLogo = new wxBoxSizer(wxHORIZONTAL);
    StaticBitmapFADGI = new wxStaticBitmap(PanelMainUI, ID_STATICBITMAP_FADGI, wxBitmap(wxImage(_T("UI\\fadgi.bmp"))), wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("ID_STATICBITMAP_FADGI"));
    BoxSizerLogo->Add(StaticBitmapFADGI, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP, 5);
    BoxSizerMainUI->Add(BoxSizerLogo, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
    PanelList = new RimPanel(PanelMainUI, ID_PANEL_LIST, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_LIST"));
    BoxSizerLists = new wxBoxSizer(wxHORIZONTAL);
    PanelTestList = new RimPanel(PanelList, ID_PANEL_TST_LST, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TST_LST"));
    BoxSizerTestList = new wxBoxSizer(wxVERTICAL);
    StaticTextProcTtl = new wxStaticText(PanelTestList, ID_STATICTEXT_PROC_TTL, _("  Tests"), wxDefaultPosition, wxSize(-1,21), 0, _T("ID_STATICTEXT_PROC_TTL"));
    StaticTextProcTtl->SetForegroundColour(wxColour(255,255,255));
    StaticTextProcTtl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextProcTtlFont(11,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextProcTtl->SetFont(StaticTextProcTtlFont);
    BoxSizerTestList->Add(StaticTextProcTtl, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
    ListViewTests = new wxListView(PanelTestList, ID_LISTVIEW_TESTS, wxDefaultPosition, wxSize(300,200), wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL|wxLC_NO_SORT_HEADER|wxNO_BORDER, wxDefaultValidator, _T("ID_LISTVIEW_TESTS"));
    ListViewTests->SetBackgroundColour(wxColour(200,200,200));
    wxFont ListViewTestsFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    ListViewTests->SetFont(ListViewTestsFont);
    BoxSizerTestList->Add(ListViewTests, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
    StaticTextLine1 = new wxStaticText(PanelTestList, ID_STATICTEXT_LINE1, wxEmptyString, wxDefaultPosition, wxSize(-1,2), 0, _T("ID_STATICTEXT_LINE1"));
    StaticTextLine1->SetForegroundColour(wxColour(255,255,255));
    StaticTextLine1->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextLine1Font(11,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextLine1->SetFont(StaticTextLine1Font);
    BoxSizerTestList->Add(StaticTextLine1, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
    PanelCtrlBtns = new wxPanel(PanelTestList, ID_PANEL_CTRL_BTNS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_CTRL_BTNS"));
    PanelCtrlBtns->SetBackgroundColour(wxColour(190,190,190));
    BoxSizerCtrlBtns = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerCtrlBtns->Add(1,-1,0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
    BoxSizerCtrlBtns->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
    ButtonStartTests = new GButton(PanelCtrlBtns, ID_BUTTON_START_TESTS, _("Start tests"), wxDefaultPosition, wxSize(300,20), 0, wxDefaultValidator, _T("ID_BUTTON_START_TESTS"));
    ButtonStartTests->SetForegroundColour(wxColour(255,255,255));
    BoxSizerCtrlBtns->Add(ButtonStartTests, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_LEFT|wxALIGN_TOP, 2);
    BoxSizerCtrlBtns->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
    BoxSizerCtrlBtns->Add(1,-1,0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
    PanelCtrlBtns->SetSizer(BoxSizerCtrlBtns);
    BoxSizerCtrlBtns->Fit(PanelCtrlBtns);
    BoxSizerCtrlBtns->SetSizeHints(PanelCtrlBtns);
    BoxSizerTestList->Add(PanelCtrlBtns, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
    PanelTestList->SetSizer(BoxSizerTestList);
    BoxSizerTestList->Fit(PanelTestList);
    BoxSizerTestList->SetSizeHints(PanelTestList);
    BoxSizerLists->Add(PanelTestList, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 4);
    PanelParamsLst = new RimPanel(PanelList, ID_PANEL_PRMS_LST, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PRMS_LST"));
    BoxSizerTestParams = new wxBoxSizer(wxVERTICAL);
    StaticTextParamsTtl = new wxStaticText(PanelParamsLst, ID_STATICTEXT_PARAMS_TTL, _("  Parameters"), wxDefaultPosition, wxSize(-1,21), 0, _T("ID_STATICTEXT_PARAMS_TTL"));
    StaticTextParamsTtl->SetForegroundColour(wxColour(255,255,255));
    StaticTextParamsTtl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextParamsTtlFont(11,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextParamsTtl->SetFont(StaticTextParamsTtlFont);
    BoxSizerTestParams->Add(StaticTextParamsTtl, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
    ListViewParameters = new wxListView(PanelParamsLst, ID_LISTVIEW_PARAMETERS, wxDefaultPosition, wxSize(275,200), wxLC_REPORT|wxLC_NO_HEADER|wxNO_BORDER, wxDefaultValidator, _T("ID_LISTVIEW_PARAMETERS"));
    ListViewParameters->SetBackgroundColour(wxColour(200,200,200));
    wxFont ListViewParametersFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    ListViewParameters->SetFont(ListViewParametersFont);
    BoxSizerTestParams->Add(ListViewParameters, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
    StaticTextLine2 = new wxStaticText(PanelParamsLst, ID_STATICTEXT_LINE2, wxEmptyString, wxDefaultPosition, wxSize(-1,2), 0, _T("ID_STATICTEXT_LINE2"));
    StaticTextLine2->SetForegroundColour(wxColour(255,255,255));
    StaticTextLine2->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextLine2Font(11,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextLine2->SetFont(StaticTextLine2Font);
    BoxSizerTestParams->Add(StaticTextLine2, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
    PanelParamsBtns = new wxPanel(PanelParamsLst, ID_PANEL_PARAMS_BTNS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PARAMS_BTNS"));
    PanelParamsBtns->SetBackgroundColour(wxColour(190,190,190));
    BoxSizerParamsBtns = new wxBoxSizer(wxHORIZONTAL);
    PanelParamsBtns->SetSizer(BoxSizerParamsBtns);
    BoxSizerParamsBtns->Fit(PanelParamsBtns);
    BoxSizerParamsBtns->SetSizeHints(PanelParamsBtns);
    BoxSizerTestParams->Add(PanelParamsBtns, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
    PanelParamsLst->SetSizer(BoxSizerTestParams);
    BoxSizerTestParams->Fit(PanelParamsLst);
    BoxSizerTestParams->SetSizeHints(PanelParamsLst);
    BoxSizerLists->Add(PanelParamsLst, 0, wxTOP|wxBOTTOM|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 4);
    PanelList->SetSizer(BoxSizerLists);
    BoxSizerLists->Fit(PanelList);
    BoxSizerLists->SetSizeHints(PanelList);
    BoxSizerMainUI->Add(PanelList, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
    BoxSizerMainUI->Add(-1,5,0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
    PanelBottom = new RimPanel(PanelMainUI, ID_PANEL_BOTTOM, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_BOTTOM"));
    PanelBottom->SetBackgroundColour(wxColour(210,210,210));
    BoxSizerBottom = new wxBoxSizer(wxVERTICAL);
    PanelLog = new RimPanel(PanelBottom, ID_PANEL_LOG, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_LOG"));
    PanelLog->SetBackgroundColour(wxColour(210,210,210));
    BoxSizerLog = new wxBoxSizer(wxVERTICAL);
    StaticTextThLog = new wxStaticText(PanelLog, ID_STATICTEXT_TH_LOG, _("  Process log"), wxDefaultPosition, wxSize(-1,21), 0, _T("ID_STATICTEXT_TH_LOG"));
    StaticTextThLog->SetForegroundColour(wxColour(255,255,255));
    StaticTextThLog->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextThLogFont(11,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextThLog->SetFont(StaticTextThLogFont);
    BoxSizerLog->Add(StaticTextThLog, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
    TextCtrlLog = new wxTextCtrl(PanelLog, ID_TEXTCTRL_LOG, wxEmptyString, wxDefaultPosition, wxSize(-1,100), wxTE_MULTILINE|wxTE_READONLY|wxNO_BORDER, wxDefaultValidator, _T("ID_TEXTCTRL_LOG"));
    TextCtrlLog->SetBackgroundColour(wxColour(200,200,200));
    wxFont TextCtrlLogFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    TextCtrlLog->SetFont(TextCtrlLogFont);
    BoxSizerLog->Add(TextCtrlLog, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
    StaticTextLine3 = new wxStaticText(PanelLog, ID_STATICTEXT_LINE3, wxEmptyString, wxDefaultPosition, wxSize(-1,2), 0, _T("ID_STATICTEXT_LINE3"));
    StaticTextLine3->SetForegroundColour(wxColour(255,255,255));
    StaticTextLine3->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextLine3Font(11,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextLine3->SetFont(StaticTextLine3Font);
    BoxSizerLog->Add(StaticTextLine3, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
    PanelLog->SetSizer(BoxSizerLog);
    BoxSizerLog->Fit(PanelLog);
    BoxSizerLog->SetSizeHints(PanelLog);
    BoxSizerBottom->Add(PanelLog, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 4);
    PanelBottom->SetSizer(BoxSizerBottom);
    BoxSizerBottom->Fit(PanelBottom);
    BoxSizerBottom->SetSizeHints(PanelBottom);
    BoxSizerMainUI->Add(PanelBottom, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
    PanelMainUI->SetSizer(BoxSizerMainUI);
    BoxSizerMainUI->Fit(PanelMainUI);
    BoxSizerMainUI->SetSizeHints(PanelMainUI);
    BoxSizerFrame->Add(PanelMainUI, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
    SetSizer(BoxSizerFrame);
    MenuBarMain = new wxMenuBar();
    MenuFile = new wxMenu();
    MenuItemFOpen = new wxMenuItem(MenuFile, ID_MENUITEM_FOPEN, _("&Open File\tCtrl-o"), wxEmptyString, wxITEM_NORMAL);
    MenuFile->Append(MenuItemFOpen);
    MenuItemFSave = new wxMenuItem(MenuFile, ID_MENUITEM_FSAVE, _("&Save File\tCtrl-s"), wxEmptyString, wxITEM_NORMAL);
    MenuFile->Append(MenuItemFSave);
    MenuItemQuit = new wxMenuItem(MenuFile, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    MenuFile->Append(MenuItemQuit);
    MenuBarMain->Append(MenuFile, _("&File"));
    MenuSettings = new wxMenu();
    MenuItemDevices = new wxMenuItem(MenuSettings, ID_MENUITEM_ADEVICES, _("&Devices\tCtrl-d"), wxEmptyString, wxITEM_NORMAL);
    MenuSettings->Append(MenuItemDevices);
    MenuBarMain->Append(MenuSettings, _("&Settings"));
    Menu2 = new wxMenu();
    MenuItemOpenManual = new wxMenuItem(Menu2, ID_MENUITEM_OPEN_GUIDE, _("Manual"), wxEmptyString, wxITEM_NORMAL);
    Menu2->Append(MenuItemOpenManual);
    MenuItemInfo = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItemInfo);
    MenuBarMain->Append(Menu2, _("Help"));
    SetMenuBar(MenuBarMain);
    StatusBarMain = new wxStatusBar(this, ID_STATUSBAR_MAIN, 0, _T("ID_STATUSBAR_MAIN"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBarMain->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBarMain->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBarMain);
    BoxSizerFrame->Fit(this);
    BoxSizerFrame->SetSizeHints(this);
    Center();

    Connect(ID_LISTVIEW_TESTS,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnListViewTestsItemSelect);
    Connect(ID_LISTVIEW_TESTS,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&AVPTesterFrame::OnListViewTestsItemActivated);
    Connect(ID_LISTVIEW_TESTS,wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,(wxObjectEventFunction)&AVPTesterFrame::OnListViewTestsItemRClick);
    Connect(ID_BUTTON_START_TESTS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AVPTesterFrame::OnButtonStartTestsClick);
    Connect(ID_LISTVIEW_PARAMETERS,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnListViewParametersItemSelect);
    Connect(ID_LISTVIEW_PARAMETERS,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&AVPTesterFrame::OnListViewParametersItemActivated);
    Connect(ID_MENUITEM_FOPEN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemFOpenSelected);
    Connect(ID_MENUITEM_FSAVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemFSaveSelected);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnQuit);
    Connect(ID_MENUITEM_ADEVICES,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemDevicesSelected);
    Connect(ID_MENUITEM_OPEN_GUIDE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemOpenManualSelected);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnAbout);
    //*)

	Connect(ID_BUTTON_START_TESTS, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AVPTesterFrame::OnButtonStartTestsClick);

	InitPreferences();

	InitAudioIO();

	BuildUI();

	if (gAudioIO)
		gAudioIO->SetParent(this);

    //SetIcon(wxIcon(_T("UI/favicon.ico"), wxBITMAP_TYPE_ICO) );
	if (gAudioIO)
	{
		TestManager* tp = gAudioIO->GetTestManager();
		tp->OpenProject(wxT("default.xml"));
		PopulateTestsList();
	}

	mTestIsRunning = false;
	mTestOn = false;
}

AVPTesterFrame::~AVPTesterFrame()
{
    //(*Destroy(AVPTesterFrame)
    //*)

	if (mADevicesDialog) {
		delete mADevicesDialog;
		mADevicesDialog = NULL;
	}

    DeinitAudioIO();
	FinishPreferences();
}

void AVPTesterFrame::BuildUI()
{
	mADevicesDialog = new AudioDevicesDialog(this);

	//set up test list

	wxListItem itemColInputs;
	itemColInputs.SetAlign(wxLIST_FORMAT_LEFT);
	itemColInputs.SetText(_T("test name"));
	ListViewTests->InsertColumn(0, itemColInputs);
	itemColInputs.SetText(_T("enabled"));
	ListViewTests->InsertColumn(1, itemColInputs);

	itemColInputs.SetAlign(wxLIST_FORMAT_LEFT);
	itemColInputs.SetText(_T("parameter"));
	ListViewParameters->InsertColumn(0, itemColInputs);
	itemColInputs.SetText(_T("value"));
	ListViewParameters->InsertColumn(1, itemColInputs);
}

void AVPTesterFrame::OnMenuItemFOpenSelected(wxCommandEvent& event)
{
	wxFileDialog openFileDlg(this, wxT("Open project"), wxT(""), wxT(""), wxT("Test files (*.xml)|*.xml"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if( openFileDlg.ShowModal() == wxID_OK )
    {
        wxString filePath = openFileDlg.GetPath();
        TestManager* tm = gAudioIO->GetTestManager();
        tm->OpenProject(filePath);
		PopulateTestsList();
    }
}

void AVPTesterFrame::OnMenuItemFSaveSelected(wxCommandEvent& event)
{
	TestManager* tm = gAudioIO->GetTestManager();
	tm->SaveProject();
}

void AVPTesterFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void AVPTesterFrame::OnAbout(wxCommandEvent& event)
{
	wxString msg = wxT("AVPreserve Low-cost ADC Test Tool Prototype\nCopyright 2017 Audiovisual Preservation Solutions, Inc.");// wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void AVPTesterFrame::OnMenuItemDevicesSelected(wxCommandEvent& event)
{
	if (mTestIsRunning)
	{
		wxMessageBox(wxT("The test procedure is running\nSettings modifications are not permitted"), wxT("Attention!"));
	}
	else
	{
		mADevicesDialog->ShowModal();
		mADevicesDialog->StopCalibration();
	}
}


void
AVPTesterFrame::OnMenuItemOpenManualSelected(wxCommandEvent& event)
{

}

void
AVPTesterFrame::EnableTestUI(bool enable)
{
	ListViewTests->Enable(enable);
	ListViewParameters->Enable(enable);
	
	if( enable){ 
		ButtonStartTests->SetLbText(wxT("Start tests")); 
		mTestOn = false;
	}
	else {
		ButtonStartTests->SetLbText(wxT("Stop tests"));
		mTestOn = true;
	}
}

void
AVPTesterFrame::OnAudioThreadEvent(wxThreadEvent& event)
{
	AudioThreadEvent ePl = event.GetPayload<AudioThreadEvent>();

	switch (ePl.processID)
	{
		//calibration process
		case 1:
		{
			WriteLogMsg( wxT("Calibration ") + ePl.eventMessage + wxT("\n") + ePl.debugInfo);
			
			if (ePl.threadFinished)
			{
				mADevicesDialog->StopCalibration();
				wxMessageBox(ePl.eventMessage, wxT("Calibration finished"));
			}
			
		}
		break;

		//test process
		case 2:
		{
			WriteLogMsg(wxT("Test Event: ") + ePl.eventMessage);

			switch (ePl.eventID)
			{
				case AVP_PROCESS_START:
				// test procedure has started
				{
					mTestIsRunning = true;
					EnableTestUI(false);
				}
				break;

				case AVP_PROCESS_STAGE:
				// test procedure in progress
				{
					mTestIsRunning = true;
					EnableTestUI(false);
					int count = ePl.eventCounter;
					int range = ePl.eventRange;
					wxString cntMsg; cntMsg.Printf(wxT(" %d of %d"), count + 1, range);
					WriteLogMsg(ePl.eventMessage + cntMsg);
					ListViewTests->Select(count);
				}
				break;
			}

			if (ePl.threadFinished)
			{
				gAudioIO->StopTestProcedure();
				if( ePl.eventID < 0 )
					wxMessageBox(wxT("Procedure terminated with errors"), wxT("Tests finished"));
				else
					wxMessageBox(wxT("Procedure terminated successfully"), wxT("Tests finished"));
	
				mTestIsRunning = false;
				EnableTestUI(true);
			}

		}
		break;
	}
}

void AVPTesterFrame::WriteLogMsg( wxString msg )
{
	mLogMsg += wxT("\n");
	mLogMsg += msg;
	TextCtrlLog->AppendText(wxT("\n")+msg);
}

void AVPTesterFrame::OnButtonStartTestsClick(wxCommandEvent& event)
{
	if (mTestOn) {
		WriteLogMsg(wxT("button stop"));
		gAudioIO->StopTestProcedure();
	}
	else {
		mLogMsg.Clear();
		TextCtrlLog->Clear();
		WriteLogMsg(wxT("button start"));
		gAudioIO->StartTestProcedure();
	}
	mTestOn = !mTestOn;
}

void
AVPTesterFrame::PopulateTestsList()
{
	mSelectedTestIdx = -1;
	mSelectedParamIdx = -1;

	ListViewTests->DeleteAllItems();

	TestManager* tm = gAudioIO->GetTestManager();
	mTestDescriptors = tm->GetTestDescriptors();

	for (size_t i = 0; i < mTestDescriptors.size(); i++)
	{
		TestDescriptor desc = mTestDescriptors[i];

		ListViewTests->InsertItem(i, desc.name, 0);

		if(desc.enabled == wxT("true"))
			ListViewTests->SetItem(i, 1, wxT("enabled"));
		else
			ListViewTests->SetItem(i, 1, wxT("disabled"));
	}
	ListViewTests->SetColumnWidth(0, 150);
	ListViewTests->SetColumnWidth(1, 150);

	ListViewParameters->DeleteAllItems();
}

void
AVPTesterFrame::PopulateTestParametersList()
{
	mSelectedParamIdx = -1;
	ListViewParameters->DeleteAllItems();

	TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];
	TestManager* tm = gAudioIO->GetTestManager();
	std::vector<TestParameter> params = tm->GetTestParameters(dsc.ID);
	for (size_t i = 0; i < params.size(); i++)
	{
		TestParameter pr = params[i];
		ListViewParameters->InsertItem(i, pr.name, 0);
		ListViewParameters->SetItem(i, 1, pr.value);
	}
	ListViewParameters->SetColumnWidth(0, 125);
	ListViewParameters->SetColumnWidth(1, 150);
}

void AVPTesterFrame::OnListViewTestsItemSelect(wxListEvent& event)
{
	mSelectedTestIdx = event.m_itemIndex;
	PopulateTestParametersList();
}

void AVPTesterFrame::OnListViewTestsItemActivated(wxListEvent& event)
{
}

void AVPTesterFrame::OnListViewTestsItemRClick(wxListEvent& event)
{
	mSelectedTestIdx = event.m_itemIndex;

	wxMenu menu(wxT("test procedure"));
	menu.Append(0, wxT("&enable"));
	menu.Append(1, wxT("&disable"));
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AVPTesterFrame::OnTestMenuPopupClick), NULL, this);
	PopupMenu(&menu, event.GetPoint());
}

void AVPTesterFrame::OnTestMenuPopupClick(wxCommandEvent &evt)
{
	TestDescriptor dsc= mTestDescriptors[mSelectedTestIdx];
	TestManager* tm = gAudioIO->GetTestManager();

	void *data = static_cast<wxMenu *>(evt.GetEventObject())->GetClientData();
	switch (evt.GetId())
	{
		case 0:
		{
			tm->EnableTest(dsc.ID, true);
		}
		break;

		case 1:
		{
			tm->EnableTest(dsc.ID, false);
		}
		break;
	}
	PopulateTestsList();
}
void AVPTesterFrame::OnListViewParametersItemSelect(wxListEvent& event)
{
}

void AVPTesterFrame::OnListViewParametersItemActivated(wxListEvent& event)
{
}

