#include "AVPTesterMain.h"
#include <wx/msgdlg.h>
#include <wx/listctrl.h>
#include <wx/mimetype.h>

#include "System/Prefs.h"
#include "Devices/ADevicesManager.h"
#include "AudioIO/FDAudioEngine.h"
#include "AboutADCTDialog.h"
#include "NewProjectDialog.h"
#include "InfoEditDialog.h"
#include "GenParamEditDialog.h"

//(*InternalHeaders(AVPTesterFrame)
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
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
const long AVPTesterFrame::ID_STATICTEXT_LOGOLINE_TOP = wxNewId();
const long AVPTesterFrame::ID_STATICBITMAP_FADGI = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_PROJECTINFO_LBL = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_PTITLE_LBL = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_PTITLE = wxNewId();
const long AVPTesterFrame::ID_PANEL_PTITLE = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_PDATE_LBL = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_PDATE = wxNewId();
const long AVPTesterFrame::ID_PANEL_PDATE = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_POPS_LBL = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_POPS = wxNewId();
const long AVPTesterFrame::ID_PANEL_POps = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_DAC_LBL = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_DAC = wxNewId();
const long AVPTesterFrame::ID_PANEL_DAC = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_ADC_LBL = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_ADC = wxNewId();
const long AVPTesterFrame::ID_PANEL_ADC = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_COMMENTS_LBL = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_PROJ_DESC = wxNewId();
const long AVPTesterFrame::ID_PANEL_COMMENTS = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_TH_LOG = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_LOG = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_LINE3 = wxNewId();
const long AVPTesterFrame::ID_PANEL_LOG = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT2 = wxNewId();
const long AVPTesterFrame::ID_PANEL_PROJECT = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_PROC_TTL = wxNewId();
const long AVPTesterFrame::ID_LISTVIEW_TESTS = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_LINE1 = wxNewId();
const long AVPTesterFrame::ID_BUTTON_START_TESTS = wxNewId();
const long AVPTesterFrame::ID_PANEL_CTRL_BTNS = wxNewId();
const long AVPTesterFrame::ID_PANEL_TST_LST = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_DESC_TTL = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_TEST_DESCRIPTION = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_LINE_0 = wxNewId();
const long AVPTesterFrame::ID_PANEL_TEST_DESCRIPTION = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_TEST_MODE = wxNewId();
const long AVPTesterFrame::ID_CHOICE_TEST_MODE = wxNewId();
const long AVPTesterFrame::ID_BUTTON_RESP_FILE = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT3 = wxNewId();
const long AVPTesterFrame::ID_PANEL_TEST_MODE = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_AUDIO_ROUTING = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_DAC_CH = wxNewId();
const long AVPTesterFrame::ID_CHOICE_DAC_CH = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_DAC_NAME = wxNewId();
const long AVPTesterFrame::ID_PANEL_AUDIO_DAC = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_ADC_CH = wxNewId();
const long AVPTesterFrame::ID_CHOICE_ADC_CH = wxNewId();
const long AVPTesterFrame::ID_TEXTCTRL_ADC_NAME = wxNewId();
const long AVPTesterFrame::ID_PANEL_AUDIO_ADC = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT4 = wxNewId();
const long AVPTesterFrame::ID_PANEL_TEST_AUDIO = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_PARAMS_TTL = wxNewId();
const long AVPTesterFrame::ID_LISTVIEW_PARAMETERS = wxNewId();
const long AVPTesterFrame::ID_STATICTEXT_LINE2 = wxNewId();
const long AVPTesterFrame::ID_BUTTON_DO_SINGLETEST = wxNewId();
const long AVPTesterFrame::ID_PANEL_PARAMS_BTNS = wxNewId();
const long AVPTesterFrame::ID_PANEL_PRMS_LST = wxNewId();
const long AVPTesterFrame::ID_PANEL_LIST = wxNewId();
const long AVPTesterFrame::ID_PANEL_BOTTOM = wxNewId();
const long AVPTesterFrame::ID_PANEL_MAIN_UI = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_FOPEN = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_FNEW = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_FCLOSE = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_FSAVE = wxNewId();
const long AVPTesterFrame::idMenuQuit = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_ADEVICES = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_PINFO = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_OPEN_GUIDE = wxNewId();
const long AVPTesterFrame::ID_MENUITEM_OPEN_INFO = wxNewId();
const long AVPTesterFrame::ID_STATUSBAR_MAIN = wxNewId();
const long AVPTesterFrame::ID_TOOLBAR1 = wxNewId();
const long AVPTesterFrame::ID_TIMER_LOAD_PRJ = wxNewId();
//*)

BEGIN_EVENT_TABLE(AVPTesterFrame,wxFrame)
    //(*EventTable(AVPTesterFrame)
    //*)
	EVT_THREAD( AUDIOT_EVENT, AVPTesterFrame::OnAudioThreadEvent)
END_EVENT_TABLE()

AVPTesterFrame::AVPTesterFrame(wxWindow* parent,wxWindowID id)
:mADevicesDialog(NULL)
,mResultsDialog(NULL)
,mParamDialog(NULL)
,mPManager(NULL)
,mProjectLoading(false)
,mPrjThread(NULL)
,mWaitDialog(NULL)
{
    //(*Initialize(AVPTesterFrame)
    wxBoxSizer* BoxSizerADC;
    wxBoxSizer* BoxSizerAudioADC;
    wxBoxSizer* BoxSizerAudioDAC;
    wxBoxSizer* BoxSizerBottom;
    wxBoxSizer* BoxSizerComments;
    wxBoxSizer* BoxSizerCtrlBtns;
    wxBoxSizer* BoxSizerDAC;
    wxBoxSizer* BoxSizerFrame;
    wxBoxSizer* BoxSizerLeftPanels;
    wxBoxSizer* BoxSizerLists;
    wxBoxSizer* BoxSizerLog;
    wxBoxSizer* BoxSizerLogo;
    wxBoxSizer* BoxSizerLogoInner;
    wxBoxSizer* BoxSizerModeOpts;
    wxBoxSizer* BoxSizerPDate;
    wxBoxSizer* BoxSizerPOps;
    wxBoxSizer* BoxSizerPTAudio;
    wxBoxSizer* BoxSizerPTestNode;
    wxBoxSizer* BoxSizerPTitle;
    wxBoxSizer* BoxSizerPanelProject;
    wxBoxSizer* BoxSizerParamsBtns;
    wxBoxSizer* BoxSizerRightPanels;
    wxBoxSizer* BoxSizerTestDescription;
    wxBoxSizer* BoxSizerTestList;
    wxBoxSizer* BoxSizerTestParams;
    wxMenu* Menu2;
    wxMenu* MenuFile;
    wxMenuBar* MenuBarMain;
    wxMenuItem* MenuItemQuit;

    Create(parent, wxID_ANY, _("ADCTest"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxCLOSE_BOX, _T("wxID_ANY"));
    SetBackgroundColour(wxColour(200,200,200));
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
    BoxSizerLogo = new wxBoxSizer(wxVERTICAL);
    StaticTextLogoLineTop = new wxStaticText(PanelMainUI, ID_STATICTEXT_LOGOLINE_TOP, wxEmptyString, wxDefaultPosition, wxSize(-1,4), 0, _T("ID_STATICTEXT_LOGOLINE_TOP"));
    StaticTextLogoLineTop->SetBackgroundColour(wxColour(78,183,223));
    BoxSizerLogo->Add(StaticTextLogoLineTop, 0, wxALL|wxEXPAND, 0);
    BoxSizerLogoInner = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerLogoInner->Add(10,-1,0, wxALL|wxEXPAND, 0);
    StaticBitmapFADGI = new wxStaticBitmap(PanelMainUI, ID_STATICBITMAP_FADGI, wxBitmap(wxImage(_T("UI\\fadgi.bmp"))), wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("ID_STATICBITMAP_FADGI"));
    BoxSizerLogoInner->Add(StaticBitmapFADGI, 0, wxTOP|wxBOTTOM, 4);
    BoxSizerLogo->Add(BoxSizerLogoInner, 1, wxALL|wxEXPAND, 0);
    BoxSizerMainUI->Add(BoxSizerLogo, 0, wxALL|wxEXPAND, 0);
    PanelList = new wxPanel(PanelMainUI, ID_PANEL_LIST, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_LIST"));
    BoxSizerLists = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerLists->Add(5,-1,0, wxALL|wxEXPAND, 0);
    BoxSizerLeftPanels = new wxBoxSizer(wxHORIZONTAL);
    PanelProject = new RimPanel(PanelList, ID_PANEL_PROJECT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PROJECT"));
    BoxSizerPanelProject = new wxBoxSizer(wxVERTICAL);
    StaticTextProjectInfoLabel = new wxStaticText(PanelProject, ID_STATICTEXT_PROJECTINFO_LBL, _("  Project info"), wxDefaultPosition, wxSize(-1,21), 0, _T("ID_STATICTEXT_PROJECTINFO_LBL"));
    StaticTextProjectInfoLabel->SetForegroundColour(wxColour(255,255,255));
    StaticTextProjectInfoLabel->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextProjectInfoLabelFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextProjectInfoLabel->SetFont(StaticTextProjectInfoLabelFont);
    BoxSizerPanelProject->Add(StaticTextProjectInfoLabel, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
    PanelPTitle = new RimPanel(PanelProject, ID_PANEL_PTITLE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PTITLE"));
    BoxSizerPTitle = new wxBoxSizer(wxHORIZONTAL);
    StaticTextPTitleLbl = new wxStaticText(PanelPTitle, ID_STATICTEXT_PTITLE_LBL, _(" Title:"), wxDefaultPosition, wxSize(75,21), 0, _T("ID_STATICTEXT_PTITLE_LBL"));
    StaticTextPTitleLbl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextPTitleLblFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextPTitleLbl->SetFont(StaticTextPTitleLblFont);
    BoxSizerPTitle->Add(StaticTextPTitleLbl, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 1);
    TextCtrlPTitle = new wxTextCtrl(PanelPTitle, ID_TEXTCTRL_PTITLE, wxEmptyString, wxDefaultPosition, wxSize(200,22), wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_PTITLE"));
    TextCtrlPTitle->SetBackgroundColour(wxColour(200,200,200));
    wxFont TextCtrlPTitleFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
    TextCtrlPTitle->SetFont(TextCtrlPTitleFont);
    BoxSizerPTitle->Add(TextCtrlPTitle, 0, wxALL|wxEXPAND, 1);
    PanelPTitle->SetSizer(BoxSizerPTitle);
    BoxSizerPTitle->Fit(PanelPTitle);
    BoxSizerPTitle->SetSizeHints(PanelPTitle);
    BoxSizerPanelProject->Add(PanelPTitle, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 2);
    PanelPDate = new RimPanel(PanelProject, ID_PANEL_PDATE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PDATE"));
    BoxSizerPDate = new wxBoxSizer(wxHORIZONTAL);
    StaticTextPDateLbl = new wxStaticText(PanelPDate, ID_STATICTEXT_PDATE_LBL, _(" Date:"), wxDefaultPosition, wxSize(75,21), 0, _T("ID_STATICTEXT_PDATE_LBL"));
    StaticTextPDateLbl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextPDateLblFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextPDateLbl->SetFont(StaticTextPDateLblFont);
    BoxSizerPDate->Add(StaticTextPDateLbl, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 1);
    TextCtrlPDate = new wxTextCtrl(PanelPDate, ID_TEXTCTRL_PDATE, wxEmptyString, wxDefaultPosition, wxSize(200,22), wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_PDATE"));
    TextCtrlPDate->SetBackgroundColour(wxColour(200,200,200));
    wxFont TextCtrlPDateFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
    TextCtrlPDate->SetFont(TextCtrlPDateFont);
    BoxSizerPDate->Add(TextCtrlPDate, 0, wxALL|wxEXPAND, 1);
    PanelPDate->SetSizer(BoxSizerPDate);
    BoxSizerPDate->Fit(PanelPDate);
    BoxSizerPDate->SetSizeHints(PanelPDate);
    BoxSizerPanelProject->Add(PanelPDate, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 2);
    PanelPOps = new RimPanel(PanelProject, ID_PANEL_POps, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_POps"));
    BoxSizerPOps = new wxBoxSizer(wxHORIZONTAL);
    StaticTextPOpsLbl = new wxStaticText(PanelPOps, ID_STATICTEXT_POPS_LBL, _(" Operator:"), wxDefaultPosition, wxSize(75,21), 0, _T("ID_STATICTEXT_POPS_LBL"));
    StaticTextPOpsLbl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextPOpsLblFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextPOpsLbl->SetFont(StaticTextPOpsLblFont);
    BoxSizerPOps->Add(StaticTextPOpsLbl, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 1);
    TextCtrlPOps = new wxTextCtrl(PanelPOps, ID_TEXTCTRL_POPS, wxEmptyString, wxDefaultPosition, wxSize(200,22), wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_POPS"));
    TextCtrlPOps->SetBackgroundColour(wxColour(200,200,200));
    wxFont TextCtrlPOpsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
    TextCtrlPOps->SetFont(TextCtrlPOpsFont);
    BoxSizerPOps->Add(TextCtrlPOps, 0, wxALL|wxEXPAND, 1);
    PanelPOps->SetSizer(BoxSizerPOps);
    BoxSizerPOps->Fit(PanelPOps);
    BoxSizerPOps->SetSizeHints(PanelPOps);
    BoxSizerPanelProject->Add(PanelPOps, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 2);
    PanelDAC = new RimPanel(PanelProject, ID_PANEL_DAC, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_DAC"));
    BoxSizerDAC = new wxBoxSizer(wxHORIZONTAL);
    StaticTextDACLbl = new wxStaticText(PanelDAC, ID_STATICTEXT_DAC_LBL, _(" Audio DAC:"), wxDefaultPosition, wxSize(75,21), 0, _T("ID_STATICTEXT_DAC_LBL"));
    StaticTextDACLbl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextDACLblFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextDACLbl->SetFont(StaticTextDACLblFont);
    BoxSizerDAC->Add(StaticTextDACLbl, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 1);
    TextCtrlDAC = new wxTextCtrl(PanelDAC, ID_TEXTCTRL_DAC, wxEmptyString, wxDefaultPosition, wxSize(200,40), wxTE_NO_VSCROLL|wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_DAC"));
    TextCtrlDAC->SetBackgroundColour(wxColour(200,200,200));
    wxFont TextCtrlDACFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
    TextCtrlDAC->SetFont(TextCtrlDACFont);
    BoxSizerDAC->Add(TextCtrlDAC, 0, wxALL|wxEXPAND, 2);
    PanelDAC->SetSizer(BoxSizerDAC);
    BoxSizerDAC->Fit(PanelDAC);
    BoxSizerDAC->SetSizeHints(PanelDAC);
    BoxSizerPanelProject->Add(PanelDAC, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 2);
    PanelADC = new RimPanel(PanelProject, ID_PANEL_ADC, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_ADC"));
    BoxSizerADC = new wxBoxSizer(wxHORIZONTAL);
    StaticTextADCLbl = new wxStaticText(PanelADC, ID_STATICTEXT_ADC_LBL, _(" Audio ADC:"), wxDefaultPosition, wxSize(75,21), 0, _T("ID_STATICTEXT_ADC_LBL"));
    StaticTextADCLbl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextADCLblFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextADCLbl->SetFont(StaticTextADCLblFont);
    BoxSizerADC->Add(StaticTextADCLbl, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 1);
    TextCtrlADC = new wxTextCtrl(PanelADC, ID_TEXTCTRL_ADC, wxEmptyString, wxDefaultPosition, wxSize(200,40), wxTE_NO_VSCROLL|wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_ADC"));
    TextCtrlADC->SetBackgroundColour(wxColour(200,200,200));
    wxFont TextCtrlADCFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
    TextCtrlADC->SetFont(TextCtrlADCFont);
    BoxSizerADC->Add(TextCtrlADC, 0, wxALL|wxEXPAND, 2);
    PanelADC->SetSizer(BoxSizerADC);
    BoxSizerADC->Fit(PanelADC);
    BoxSizerADC->SetSizeHints(PanelADC);
    BoxSizerPanelProject->Add(PanelADC, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 2);
    BoxSizerPanelProject->Add(-1,10,0, wxALL|wxEXPAND, 0);
    PanelComments = new RimPanel(PanelProject, ID_PANEL_COMMENTS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_COMMENTS"));
    BoxSizerComments = new wxBoxSizer(wxVERTICAL);
    StaticTextCommentsLbl = new wxStaticText(PanelComments, ID_STATICTEXT_COMMENTS_LBL, _(" Remarks:"), wxDefaultPosition, wxSize(-1,21), wxALIGN_LEFT, _T("ID_STATICTEXT_COMMENTS_LBL"));
    StaticTextCommentsLbl->SetForegroundColour(wxColour(255,255,255));
    StaticTextCommentsLbl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextCommentsLblFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextCommentsLbl->SetFont(StaticTextCommentsLblFont);
    BoxSizerComments->Add(StaticTextCommentsLbl, 0, wxALL|wxEXPAND, 1);
    TextCtrlProjDescription = new wxTextCtrl(PanelComments, ID_TEXTCTRL_PROJ_DESC, wxEmptyString, wxDefaultPosition, wxSize(200,50), wxTE_NO_VSCROLL|wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL_PROJ_DESC"));
    TextCtrlProjDescription->SetBackgroundColour(wxColour(200,200,200));
    wxFont TextCtrlProjDescriptionFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    TextCtrlProjDescription->SetFont(TextCtrlProjDescriptionFont);
    BoxSizerComments->Add(TextCtrlProjDescription, 1, wxALL|wxEXPAND, 2);
    PanelComments->SetSizer(BoxSizerComments);
    BoxSizerComments->Fit(PanelComments);
    BoxSizerComments->SetSizeHints(PanelComments);
    BoxSizerPanelProject->Add(PanelComments, 1, wxALL|wxEXPAND, 2);
    BoxSizerPanelProject->Add(-1,10,0, wxALL|wxEXPAND, 0);
    PanelLog = new RimPanel(PanelProject, ID_PANEL_LOG, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_LOG"));
    PanelLog->SetBackgroundColour(wxColour(210,210,210));
    BoxSizerLog = new wxBoxSizer(wxVERTICAL);
    StaticTextThLog = new wxStaticText(PanelLog, ID_STATICTEXT_TH_LOG, _("  Process log"), wxDefaultPosition, wxSize(-1,21), 0, _T("ID_STATICTEXT_TH_LOG"));
    StaticTextThLog->SetForegroundColour(wxColour(255,255,255));
    StaticTextThLog->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextThLogFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextThLog->SetFont(StaticTextThLogFont);
    BoxSizerLog->Add(StaticTextThLog, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
    TextCtrlLog = new wxTextCtrl(PanelLog, ID_TEXTCTRL_LOG, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_MULTILINE|wxTE_READONLY|wxNO_BORDER, wxDefaultValidator, _T("ID_TEXTCTRL_LOG"));
    TextCtrlLog->SetBackgroundColour(wxColour(200,200,200));
    wxFont TextCtrlLogFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    TextCtrlLog->SetFont(TextCtrlLogFont);
    BoxSizerLog->Add(TextCtrlLog, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 1);
    StaticTextLine3 = new wxStaticText(PanelLog, ID_STATICTEXT_LINE3, wxEmptyString, wxDefaultPosition, wxSize(-1,2), 0, _T("ID_STATICTEXT_LINE3"));
    StaticTextLine3->SetForegroundColour(wxColour(255,255,255));
    StaticTextLine3->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextLine3Font(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextLine3->SetFont(StaticTextLine3Font);
    BoxSizerLog->Add(StaticTextLine3, 0, wxALL|wxEXPAND, 0);
    PanelLog->SetSizer(BoxSizerLog);
    BoxSizerLog->Fit(PanelLog);
    BoxSizerLog->SetSizeHints(PanelLog);
    BoxSizerPanelProject->Add(PanelLog, 1, wxALL|wxEXPAND, 2);
    StaticText2 = new wxStaticText(PanelProject, ID_STATICTEXT2, wxEmptyString, wxDefaultPosition, wxSize(-1,2), 0, _T("ID_STATICTEXT2"));
    StaticText2->Hide();
    StaticText2->SetForegroundColour(wxColour(255,255,255));
    StaticText2->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticText2Font(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticText2->SetFont(StaticText2Font);
    BoxSizerPanelProject->Add(StaticText2, 0, wxBOTTOM|wxEXPAND, 2);
    PanelProject->SetSizer(BoxSizerPanelProject);
    BoxSizerPanelProject->Fit(PanelProject);
    BoxSizerPanelProject->SetSizeHints(PanelProject);
    BoxSizerLeftPanels->Add(PanelProject, 1, wxLEFT|wxEXPAND, 4);
    BoxSizerLists->Add(BoxSizerLeftPanels, 1, wxALL|wxEXPAND, 4);
    BoxSizerLists->Add(10,-1,0, wxALL|wxEXPAND, 0);
    PanelTestList = new RimPanel(PanelList, ID_PANEL_TST_LST, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TST_LST"));
    BoxSizerTestList = new wxBoxSizer(wxVERTICAL);
    StaticTextProcTtl = new wxStaticText(PanelTestList, ID_STATICTEXT_PROC_TTL, _("  Test procedures"), wxDefaultPosition, wxSize(-1,21), 0, _T("ID_STATICTEXT_PROC_TTL"));
    StaticTextProcTtl->SetForegroundColour(wxColour(255,255,255));
    StaticTextProcTtl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextProcTtlFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextProcTtl->SetFont(StaticTextProcTtlFont);
    BoxSizerTestList->Add(StaticTextProcTtl, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
    ListViewTests = new wxListView(PanelTestList, ID_LISTVIEW_TESTS, wxDefaultPosition, wxSize(250,352), wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL|wxLC_NO_SORT_HEADER|wxNO_BORDER, wxDefaultValidator, _T("ID_LISTVIEW_TESTS"));
    ListViewTests->SetBackgroundColour(wxColour(200,200,200));
    wxFont ListViewTestsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    ListViewTests->SetFont(ListViewTestsFont);
    BoxSizerTestList->Add(ListViewTests, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 1);
    StaticTextLine1 = new wxStaticText(PanelTestList, ID_STATICTEXT_LINE1, wxEmptyString, wxDefaultPosition, wxSize(-1,2), 0, _T("ID_STATICTEXT_LINE1"));
    StaticTextLine1->SetForegroundColour(wxColour(255,255,255));
    StaticTextLine1->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextLine1Font(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextLine1->SetFont(StaticTextLine1Font);
    BoxSizerTestList->Add(StaticTextLine1, 0, wxALL|wxEXPAND, 0);
    PanelCtrlBtns = new wxPanel(PanelTestList, ID_PANEL_CTRL_BTNS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_CTRL_BTNS"));
    PanelCtrlBtns->SetBackgroundColour(wxColour(190,190,190));
    BoxSizerCtrlBtns = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerCtrlBtns->Add(1,-1,0, wxALL|wxEXPAND, 0);
    BoxSizerCtrlBtns->Add(-1,-1,1, wxALL|wxEXPAND, 0);
    ButtonStartTests = new GButton(PanelCtrlBtns, ID_BUTTON_START_TESTS, _("Start procedure"), wxDefaultPosition, wxSize(320,20), 0, wxDefaultValidator, _T("ID_BUTTON_START_TESTS"));
    ButtonStartTests->SetForegroundColour(wxColour(255,255,255));
    BoxSizerCtrlBtns->Add(ButtonStartTests, 0, wxALL|wxALIGN_TOP, 2);
    BoxSizerCtrlBtns->Add(-1,-1,1, wxALL|wxEXPAND, 0);
    BoxSizerCtrlBtns->Add(1,-1,0, wxALL|wxEXPAND, 0);
    PanelCtrlBtns->SetSizer(BoxSizerCtrlBtns);
    BoxSizerCtrlBtns->Fit(PanelCtrlBtns);
    BoxSizerCtrlBtns->SetSizeHints(PanelCtrlBtns);
    BoxSizerTestList->Add(PanelCtrlBtns, 0, wxALL|wxEXPAND, 1);
    PanelTestList->SetSizer(BoxSizerTestList);
    BoxSizerTestList->Fit(PanelTestList);
    BoxSizerTestList->SetSizeHints(PanelTestList);
    BoxSizerLists->Add(PanelTestList, 0, wxALL|wxEXPAND, 4);
    BoxSizerLists->Add(10,-1,0, wxALL|wxEXPAND, 0);
    BoxSizerRightPanels = new wxBoxSizer(wxVERTICAL);
    PanelTestDescription = new RimPanel(PanelList, ID_PANEL_TEST_DESCRIPTION, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TEST_DESCRIPTION"));
    BoxSizerTestDescription = new wxBoxSizer(wxVERTICAL);
    StaticTextTestDescTTl = new wxStaticText(PanelTestDescription, ID_STATICTEXT_DESC_TTL, _("  Test description"), wxDefaultPosition, wxSize(-1,21), 0, _T("ID_STATICTEXT_DESC_TTL"));
    StaticTextTestDescTTl->SetForegroundColour(wxColour(255,255,255));
    StaticTextTestDescTTl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextTestDescTTlFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextTestDescTTl->SetFont(StaticTextTestDescTTlFont);
    BoxSizerTestDescription->Add(StaticTextTestDescTTl, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
    TextCtrlTestDescription = new wxTextCtrl(PanelTestDescription, ID_TEXTCTRL_TEST_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxSize(-1,50), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_TEST_DESCRIPTION"));
    TextCtrlTestDescription->SetBackgroundColour(wxColour(200,200,200));
    wxFont TextCtrlTestDescriptionFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    TextCtrlTestDescription->SetFont(TextCtrlTestDescriptionFont);
    BoxSizerTestDescription->Add(TextCtrlTestDescription, 0, wxALL|wxEXPAND, 2);
    StaticTextLine0 = new wxStaticText(PanelTestDescription, ID_STATICTEXT_LINE_0, wxEmptyString, wxDefaultPosition, wxSize(-1,2), 0, _T("ID_STATICTEXT_LINE_0"));
    StaticTextLine0->SetForegroundColour(wxColour(255,255,255));
    StaticTextLine0->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextLine0Font(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextLine0->SetFont(StaticTextLine0Font);
    BoxSizerTestDescription->Add(StaticTextLine0, 0, wxBOTTOM|wxEXPAND, 2);
    PanelTestDescription->SetSizer(BoxSizerTestDescription);
    BoxSizerTestDescription->Fit(PanelTestDescription);
    BoxSizerTestDescription->SetSizeHints(PanelTestDescription);
    BoxSizerRightPanels->Add(PanelTestDescription, 0, wxALL|wxEXPAND, 2);
    BoxSizerRightPanels->Add(-1,10,0, wxALL|wxEXPAND, 0);
    PanelTestMode = new RimPanel(PanelList, ID_PANEL_TEST_MODE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TEST_MODE"));
    BoxSizerPTestNode = new wxBoxSizer(wxVERTICAL);
    StaticTextTestMode = new wxStaticText(PanelTestMode, ID_STATICTEXT_TEST_MODE, _("  Test mode"), wxDefaultPosition, wxSize(-1,21), 0, _T("ID_STATICTEXT_TEST_MODE"));
    StaticTextTestMode->SetForegroundColour(wxColour(255,255,255));
    StaticTextTestMode->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextTestModeFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextTestMode->SetFont(StaticTextTestModeFont);
    BoxSizerPTestNode->Add(StaticTextTestMode, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
    BoxSizerModeOpts = new wxBoxSizer(wxHORIZONTAL);
    ChoiceTestMode = new wxChoice(PanelTestMode, ID_CHOICE_TEST_MODE, wxDefaultPosition, wxSize(75,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_TEST_MODE"));
    BoxSizerModeOpts->Add(ChoiceTestMode, 0, wxALL|wxEXPAND, 2);
    BoxSizerModeOpts->Add(10,0,1, wxALL|wxEXPAND, 0);
    ButtonRespFile = new GButton(PanelTestMode, ID_BUTTON_RESP_FILE, wxEmptyString, wxDefaultPosition, wxSize(225,22), 0, wxDefaultValidator, _T("ID_BUTTON_RESP_FILE"));
    BoxSizerModeOpts->Add(ButtonRespFile, 0, wxALL, 2);
    BoxSizerModeOpts->Add(5,-1,0, wxALL|wxEXPAND, 0);
    BoxSizerPTestNode->Add(BoxSizerModeOpts, 0, wxALL|wxEXPAND, 2);
    StaticText3 = new wxStaticText(PanelTestMode, ID_STATICTEXT3, wxEmptyString, wxDefaultPosition, wxSize(-1,2), 0, _T("ID_STATICTEXT3"));
    StaticText3->SetForegroundColour(wxColour(255,255,255));
    StaticText3->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticText3Font(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticText3->SetFont(StaticText3Font);
    BoxSizerPTestNode->Add(StaticText3, 0, wxBOTTOM|wxEXPAND, 2);
    PanelTestMode->SetSizer(BoxSizerPTestNode);
    BoxSizerPTestNode->Fit(PanelTestMode);
    BoxSizerPTestNode->SetSizeHints(PanelTestMode);
    BoxSizerRightPanels->Add(PanelTestMode, 0, wxALL|wxEXPAND, 2);
    BoxSizerRightPanels->Add(-1,10,0, wxALL|wxEXPAND, 0);
    PanelTestAudio = new RimPanel(PanelList, ID_PANEL_TEST_AUDIO, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_TEST_AUDIO"));
    BoxSizerPTAudio = new wxBoxSizer(wxVERTICAL);
    StaticTextAudioRouting = new wxStaticText(PanelTestAudio, ID_STATICTEXT_AUDIO_ROUTING, _("  Audio routing"), wxDefaultPosition, wxSize(-1,21), 0, _T("ID_STATICTEXT_AUDIO_ROUTING"));
    StaticTextAudioRouting->SetForegroundColour(wxColour(255,255,255));
    StaticTextAudioRouting->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextAudioRoutingFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextAudioRouting->SetFont(StaticTextAudioRoutingFont);
    BoxSizerPTAudio->Add(StaticTextAudioRouting, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
    PanelAudioDAC = new RimPanel(PanelTestAudio, ID_PANEL_AUDIO_DAC, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_AUDIO_DAC"));
    BoxSizerAudioDAC = new wxBoxSizer(wxHORIZONTAL);
    StaticTextDACCh = new wxStaticText(PanelAudioDAC, ID_STATICTEXT_DAC_CH, _(" DAC chan:  "), wxDefaultPosition, wxSize(75,16), 0, _T("ID_STATICTEXT_DAC_CH"));
    BoxSizerAudioDAC->Add(StaticTextDACCh, 0, wxALL|wxEXPAND, 2);
    BoxSizerAudioDAC->Add(5,-1,0, wxALL|wxEXPAND, 0);
    ChoiceDACCh = new wxChoice(PanelAudioDAC, ID_CHOICE_DAC_CH, wxDefaultPosition, wxSize(40,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_DAC_CH"));
    ChoiceDACCh->Disable();
    BoxSizerAudioDAC->Add(ChoiceDACCh, 0, wxALL|wxEXPAND, 2);
    BoxSizerAudioDAC->Add(5,0,0, wxALL|wxEXPAND, 0);
    TextCtrlDACName = new wxTextCtrl(PanelAudioDAC, ID_TEXTCTRL_DAC_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_DAC_NAME"));
    TextCtrlDACName->Disable();
    TextCtrlDACName->SetBackgroundColour(wxColour(200,200,200));
    BoxSizerAudioDAC->Add(TextCtrlDACName, 1, wxALL|wxEXPAND, 2);
    BoxSizerAudioDAC->Add(5,-1,0, wxALL|wxEXPAND, 0);
    PanelAudioDAC->SetSizer(BoxSizerAudioDAC);
    BoxSizerAudioDAC->Fit(PanelAudioDAC);
    BoxSizerAudioDAC->SetSizeHints(PanelAudioDAC);
    BoxSizerPTAudio->Add(PanelAudioDAC, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 2);
    PanelAudioADC = new RimPanel(PanelTestAudio, ID_PANEL_AUDIO_ADC, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_AUDIO_ADC"));
    BoxSizerAudioADC = new wxBoxSizer(wxHORIZONTAL);
    StaticTextADCCh = new wxStaticText(PanelAudioADC, ID_STATICTEXT_ADC_CH, _(" ADC chan:  "), wxDefaultPosition, wxSize(75,16), 0, _T("ID_STATICTEXT_ADC_CH"));
    BoxSizerAudioADC->Add(StaticTextADCCh, 0, wxALL|wxEXPAND, 2);
    BoxSizerAudioADC->Add(5,-1,0, wxALL|wxEXPAND, 0);
    ChoiceADCCh = new wxChoice(PanelAudioADC, ID_CHOICE_ADC_CH, wxDefaultPosition, wxSize(40,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_ADC_CH"));
    ChoiceADCCh->Disable();
    BoxSizerAudioADC->Add(ChoiceADCCh, 0, wxALL|wxEXPAND, 2);
    BoxSizerAudioADC->Add(5,0,0, wxALL|wxEXPAND, 0);
    TextCtrlADCName = new wxTextCtrl(PanelAudioADC, ID_TEXTCTRL_ADC_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_ADC_NAME"));
    TextCtrlADCName->Disable();
    TextCtrlADCName->SetBackgroundColour(wxColour(200,200,200));
    BoxSizerAudioADC->Add(TextCtrlADCName, 1, wxALL|wxEXPAND, 2);
    BoxSizerAudioADC->Add(5,-1,0, wxALL|wxEXPAND, 0);
    PanelAudioADC->SetSizer(BoxSizerAudioADC);
    BoxSizerAudioADC->Fit(PanelAudioADC);
    BoxSizerAudioADC->SetSizeHints(PanelAudioADC);
    BoxSizerPTAudio->Add(PanelAudioADC, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 2);
    StaticText4 = new wxStaticText(PanelTestAudio, ID_STATICTEXT4, wxEmptyString, wxDefaultPosition, wxSize(-1,2), 0, _T("ID_STATICTEXT4"));
    StaticText4->SetForegroundColour(wxColour(255,255,255));
    StaticText4->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticText4Font(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticText4->SetFont(StaticText4Font);
    BoxSizerPTAudio->Add(StaticText4, 0, wxBOTTOM|wxEXPAND, 2);
    PanelTestAudio->SetSizer(BoxSizerPTAudio);
    BoxSizerPTAudio->Fit(PanelTestAudio);
    BoxSizerPTAudio->SetSizeHints(PanelTestAudio);
    BoxSizerRightPanels->Add(PanelTestAudio, 0, wxALL|wxEXPAND, 2);
    BoxSizerRightPanels->Add(-1,10,0, wxALL|wxEXPAND, 0);
    PanelParamsLst = new RimPanel(PanelList, ID_PANEL_PRMS_LST, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PRMS_LST"));
    BoxSizerTestParams = new wxBoxSizer(wxVERTICAL);
    StaticTextParamsTtl = new wxStaticText(PanelParamsLst, ID_STATICTEXT_PARAMS_TTL, _("  Test parameters"), wxDefaultPosition, wxSize(-1,21), 0, _T("ID_STATICTEXT_PARAMS_TTL"));
    StaticTextParamsTtl->SetForegroundColour(wxColour(255,255,255));
    StaticTextParamsTtl->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextParamsTtlFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextParamsTtl->SetFont(StaticTextParamsTtlFont);
    BoxSizerTestParams->Add(StaticTextParamsTtl, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
    ListViewParameters = new wxListView(PanelParamsLst, ID_LISTVIEW_PARAMETERS, wxDefaultPosition, wxSize(300,150), wxLC_REPORT|wxLC_NO_HEADER|wxNO_BORDER, wxDefaultValidator, _T("ID_LISTVIEW_PARAMETERS"));
    ListViewParameters->SetBackgroundColour(wxColour(200,200,200));
    wxFont ListViewParametersFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    ListViewParameters->SetFont(ListViewParametersFont);
    BoxSizerTestParams->Add(ListViewParameters, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 1);
    StaticTextLine2 = new wxStaticText(PanelParamsLst, ID_STATICTEXT_LINE2, wxEmptyString, wxDefaultPosition, wxSize(-1,2), 0, _T("ID_STATICTEXT_LINE2"));
    StaticTextLine2->SetForegroundColour(wxColour(255,255,255));
    StaticTextLine2->SetBackgroundColour(wxColour(78,183,223));
    wxFont StaticTextLine2Font(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial Narrow"),wxFONTENCODING_DEFAULT);
    StaticTextLine2->SetFont(StaticTextLine2Font);
    BoxSizerTestParams->Add(StaticTextLine2, 0, wxALL|wxEXPAND, 0);
    PanelParamsBtns = new wxPanel(PanelParamsLst, ID_PANEL_PARAMS_BTNS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PARAMS_BTNS"));
    PanelParamsBtns->SetBackgroundColour(wxColour(190,190,190));
    BoxSizerParamsBtns = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerParamsBtns->Add(-1,-1,1, wxALL|wxEXPAND, 0);
    ButtonDoSingleTest = new GButton(PanelParamsBtns, ID_BUTTON_DO_SINGLETEST, _("Perform selected test"), wxDefaultPosition, wxSize(320,20), 0, wxDefaultValidator, _T("ID_BUTTON_DO_SINGLETEST"));
    ButtonDoSingleTest->SetForegroundColour(wxColour(255,255,255));
    BoxSizerParamsBtns->Add(ButtonDoSingleTest, 0, wxALL|wxALIGN_TOP, 2);
    BoxSizerParamsBtns->Add(-1,-1,1, wxALL|wxEXPAND, 0);
    PanelParamsBtns->SetSizer(BoxSizerParamsBtns);
    BoxSizerParamsBtns->Fit(PanelParamsBtns);
    BoxSizerParamsBtns->SetSizeHints(PanelParamsBtns);
    BoxSizerTestParams->Add(PanelParamsBtns, 1, wxALL|wxEXPAND, 1);
    PanelParamsLst->SetSizer(BoxSizerTestParams);
    BoxSizerTestParams->Fit(PanelParamsLst);
    BoxSizerTestParams->SetSizeHints(PanelParamsLst);
    BoxSizerRightPanels->Add(PanelParamsLst, 1, wxTOP|wxLEFT|wxRIGHT, 2);
    BoxSizerLists->Add(BoxSizerRightPanels, 0, wxALL|wxEXPAND, 4);
    BoxSizerLists->Add(5,-1,0, wxALL|wxEXPAND, 0);
    PanelList->SetSizer(BoxSizerLists);
    BoxSizerLists->Fit(PanelList);
    BoxSizerLists->SetSizeHints(PanelList);
    BoxSizerMainUI->Add(PanelList, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizerMainUI->Add(-1,5,0, wxALL|wxEXPAND, 0);
    PanelBottom = new RimPanel(PanelMainUI, ID_PANEL_BOTTOM, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_BOTTOM"));
    PanelBottom->SetBackgroundColour(wxColour(210,210,210));
    BoxSizerBottom = new wxBoxSizer(wxVERTICAL);
    PanelBottom->SetSizer(BoxSizerBottom);
    BoxSizerBottom->Fit(PanelBottom);
    BoxSizerBottom->SetSizeHints(PanelBottom);
    BoxSizerMainUI->Add(PanelBottom, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    PanelMainUI->SetSizer(BoxSizerMainUI);
    BoxSizerMainUI->Fit(PanelMainUI);
    BoxSizerMainUI->SetSizeHints(PanelMainUI);
    BoxSizerFrame->Add(PanelMainUI, 1, wxALL|wxALIGN_TOP, 0);
    SetSizer(BoxSizerFrame);
    MenuBarMain = new wxMenuBar();
    MenuFile = new wxMenu();
    MenuItemFOpen = new wxMenuItem(MenuFile, ID_MENUITEM_FOPEN, _("&Open Project\tCtrl-o"), wxEmptyString, wxITEM_NORMAL);
    MenuFile->Append(MenuItemFOpen);
    MenuItemFNew = new wxMenuItem(MenuFile, ID_MENUITEM_FNEW, _("&New Project\tCtrl-n"), wxEmptyString, wxITEM_NORMAL);
    MenuFile->Append(MenuItemFNew);
    MenuItemFClose = new wxMenuItem(MenuFile, ID_MENUITEM_FCLOSE, _("&Close Project\tCtrl-c"), wxEmptyString, wxITEM_NORMAL);
    MenuFile->Append(MenuItemFClose);
    MenuItemFSave = new wxMenuItem(MenuFile, ID_MENUITEM_FSAVE, _("&Save Project\tCtrl-s"), wxEmptyString, wxITEM_NORMAL);
    MenuFile->Append(MenuItemFSave);
    MenuItemQuit = new wxMenuItem(MenuFile, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    MenuFile->Append(MenuItemQuit);
    MenuBarMain->Append(MenuFile, _("&Project"));
    MenuSettings = new wxMenu();
    MenuItemDevices = new wxMenuItem(MenuSettings, ID_MENUITEM_ADEVICES, _("&Devices\tCtrl-d"), wxEmptyString, wxITEM_NORMAL);
    MenuSettings->Append(MenuItemDevices);
    MenuItemEditPInfo = new wxMenuItem(MenuSettings, ID_MENUITEM_PINFO, _("Edit &Info"), wxEmptyString, wxITEM_NORMAL);
    MenuSettings->Append(MenuItemEditPInfo);
    MenuBarMain->Append(MenuSettings, _("&Settings"));
    Menu2 = new wxMenu();
    MenuItemOpenManual = new wxMenuItem(Menu2, ID_MENUITEM_OPEN_GUIDE, _("Manual"), wxEmptyString, wxITEM_NORMAL);
    Menu2->Append(MenuItemOpenManual);
    MenuItemInfo = new wxMenuItem(Menu2, ID_MENUITEM_OPEN_INFO, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItemInfo);
    MenuBarMain->Append(Menu2, _("Help"));
    SetMenuBar(MenuBarMain);
    StatusBarMain = new wxStatusBar(this, ID_STATUSBAR_MAIN, 0, _T("ID_STATUSBAR_MAIN"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBarMain->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBarMain->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBarMain);
    TimerLoadProject.SetOwner(this, ID_TIMER_LOAD_PRJ);
    TimerLoadProject.Start(100, false);
    BoxSizerFrame->Fit(this);
    BoxSizerFrame->SetSizeHints(this);
    Center();

    Connect(ID_LISTVIEW_TESTS,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnListViewTestsItemSelect);
    Connect(ID_LISTVIEW_TESTS,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&AVPTesterFrame::OnListViewTestsItemActivated);
    Connect(ID_LISTVIEW_TESTS,wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,(wxObjectEventFunction)&AVPTesterFrame::OnListViewTestsItemRClick);
    Connect(ID_BUTTON_START_TESTS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AVPTesterFrame::OnButtonStartTestsClick);
    Connect(ID_CHOICE_TEST_MODE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnChoiceTestModeSelect);
    Connect(ID_BUTTON_RESP_FILE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AVPTesterFrame::OnButtonRespFileClick);
    Connect(ID_CHOICE_DAC_CH,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnChoiceDACChSelect);
    Connect(ID_CHOICE_ADC_CH,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnChoiceADCChSelect);
    Connect(ID_LISTVIEW_PARAMETERS,wxEVT_COMMAND_LIST_BEGIN_DRAG,(wxObjectEventFunction)&AVPTesterFrame::OnListViewParametersBeginDrag);
    Connect(ID_LISTVIEW_PARAMETERS,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnListViewParametersItemSelect);
    Connect(ID_LISTVIEW_PARAMETERS,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&AVPTesterFrame::OnListViewParametersItemActivated);
    Connect(ID_BUTTON_DO_SINGLETEST,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AVPTesterFrame::OnButtonDoSingleTestClick);
    Connect(ID_MENUITEM_FOPEN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemFOpenSelected);
    Connect(ID_MENUITEM_FNEW,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemFNewSelected);
    Connect(ID_MENUITEM_FCLOSE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemFCloseSelected);
    Connect(ID_MENUITEM_FSAVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemFSaveSelected);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnQuit);
    Connect(ID_MENUITEM_ADEVICES,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemDevicesSelected);
    Connect(ID_MENUITEM_PINFO,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemEditPInfoSelected);
    Connect(ID_MENUITEM_OPEN_GUIDE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemOpenManualSelected);
    Connect(ID_MENUITEM_OPEN_INFO,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AVPTesterFrame::OnMenuItemInfoSelected);
    Connect(ID_TIMER_LOAD_PRJ,wxEVT_TIMER,(wxObjectEventFunction)&AVPTesterFrame::OnTimerLoadProjectTrigger);
    //*)

	TimerLoadProject.Stop();
	//mWaitDialog = new WaitDialog(this);

	PanelPTitle->setRimColour(78,183,223);
	PanelPDate->setRimColour(78, 183, 223);
	PanelPOps->setRimColour(78, 183, 223);
	PanelDAC->setRimColour(78, 183, 223);
	PanelADC->setRimColour(78, 183, 223);
	PanelComments->setRimColour(78, 183, 223);

	Connect(ID_BUTTON_START_TESTS, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AVPTesterFrame::OnButtonStartTestsClick);
	Connect(ID_BUTTON_DO_SINGLETEST, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AVPTesterFrame::OnButtonDoSingleTestClick);
	Connect(ID_BUTTON_RESP_FILE, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AVPTesterFrame::OnButtonRespFileClick);

	InitPreferences();

	InitAudioIO();

	BuildUI();

	if (gAudioIO)
		gAudioIO->SetParent(this);

    //SetIcon(wxIcon(_T("UI/favicon.ico"), wxBITMAP_TYPE_ICO) );
	SetIcon(wxIcon(_T("sycon"), wxBITMAP_TYPE_ICO_RESOURCE));

	mTestIsRunning = false;
	mTestOn = false;

	mPManager = new ProjectManager();

	/////////////
	/*
	if (mPManager->OpenProject(wxT("C:\\Users\\pl\\Desktop\\MEGATEST\\MEGATEST.avt")) == MgmtOK)
	{
		PopulateProjectInfo();
		PopulateTestsList();
	}
	*/
}

AVPTesterFrame::~AVPTesterFrame()
{
    //(*Destroy(AVPTesterFrame)
    //*)

	if (mADevicesDialog) {
		delete mADevicesDialog;
		mADevicesDialog = NULL;
	}

	if (mResultsDialog) {
		delete mResultsDialog;
		mResultsDialog = NULL;
	}

	delete mPManager;

    DeinitAudioIO();
	FinishPreferences();
}

void AVPTesterFrame::BuildUI()
{
	mADevicesDialog = new AudioDevicesDialog(this);
	mResultsDialog = new ResultsDialog(this);
	mResultsDialog->Show(false);

	//set up test list
	wxListItem itemColInputs;
	itemColInputs.SetAlign(wxLIST_FORMAT_LEFT);
	itemColInputs.SetText(_T("test name"));
	ListViewTests->InsertColumn(0, itemColInputs);
	itemColInputs.SetText(_T("enabled"));
	ListViewTests->InsertColumn(1, itemColInputs);
	itemColInputs.SetText(_T("result"));
	ListViewTests->InsertColumn(2, itemColInputs);

	itemColInputs.SetAlign(wxLIST_FORMAT_LEFT);
	itemColInputs.SetText(_T("parameter"));
	ListViewParameters->InsertColumn(0, itemColInputs);
	itemColInputs.SetText(_T("value"));
	ListViewParameters->InsertColumn(1, itemColInputs);

	mSelectedTestIdx = -1;
}

void AVPTesterFrame::OnMenuItemFOpenSelected(wxCommandEvent& event)
{
	wxFileDialog openFileDlg(this, wxT("Open project"), wxT(""), wxT(""), wxT("ADCTest files (*.avt)|*.avt"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if( openFileDlg.ShowModal() == wxID_OK )
    {
        wxString filePath = openFileDlg.GetPath();
		if (mPManager->OpenProject(filePath) == MgmtOK)
		{
			PopulateProjectInfo();
			PopulateTestsList();
		}
    }
}

void AVPTesterFrame::OnMenuItemFSaveSelected(wxCommandEvent& event)
{
	mPManager->SetProjectRemarks(TextCtrlProjDescription->GetValue());
	mPManager->SaveProject();
}

void AVPTesterFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void AVPTesterFrame::OnAbout(wxCommandEvent& event)
{
	AboutADCTDialog dlg(this);
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
		PopulateAudioIOInfo(true);
	}
}

void
AVPTesterFrame::OnMenuItemOpenManualSelected(wxCommandEvent& event)
{
	wxString url = wxT("userguide.pdf");
	wxViewPDFFile(url);
}

void
AVPTesterFrame::EnableTestUI(bool enable, bool isLoading)
{
	ListViewTests->Enable(enable);
	ListViewParameters->Enable(enable);
	ButtonDoSingleTest->enable(enable);
	MenuItemDevices->Enable(enable);

	MenuItemFOpen->Enable(enable);
	MenuItemFNew->Enable(enable);
	MenuItemFClose->Enable(enable);
	MenuItemFSave->Enable(enable);
	MenuItemEditPInfo->Enable(enable);

	PopulateAudioIOInfo(enable);
	PopulateModeInfo(enable);

	if (isLoading)
	{
		ButtonStartTests->enable(false);
	}
	else
	{
		ButtonStartTests->enable(true);

		if (enable) {
			ButtonStartTests->SetLbText(wxT("Start procedure"));
			mTestOn = false;
		}
		else {
			ButtonStartTests->SetLbText(wxT("Stop procedure"));
			mTestOn = true;
		}
	}
}

void AVPTesterFrame::ParseResults()
{
}

wxString AVPTesterFrame::GetOutcomeFromFile(wxString resultsPath)
{
	wxString outcome(wxT("unavail."));

	wxXmlNode* resNodes = new wxXmlNode();
	wxXmlDocument* readSchema = new wxXmlDocument();

	if (wxFileName::FileExists(resultsPath))
	{
		if (readSchema->Load(resultsPath))
		{
			resNodes = readSchema->DetachRoot();

			wxXmlNode* otcNode = resNodes->GetChildren();
			while (otcNode) {
				if (otcNode->GetName() == wxT("testoutcome")) {
					outcome = otcNode->GetAttribute(wxT("value"));
					break;
				}
				otcNode = otcNode->GetNext();
			}
		}
	}
	delete readSchema;
	delete resNodes;

	return outcome;
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
				//wxMessageBox(ePl.eventMessage, wxT("Calibration finished"));
			}

		}
		break;

		//test process
		case 2:
		{
			WriteLogMsg(wxT("Test run: ") + ePl.eventMessage + ePl.debugInfo);

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
					//ButtonDoSingleTest->enable(false);
				}
				break;

				case AVP_PROCESS_RESULT:
				// test procedure completed
				{
					mTestIsRunning = true;
					EnableTestUI(false);
					int count = ePl.eventCounter;
					int range = ePl.eventRange;
					wxString result = ePl.eventMessage;
					wxString cntMsg; cntMsg.Printf(wxT(" %d of %d"), count + 1, range);
					WriteLogMsg(ePl.eventMessage + cntMsg);

					ListViewTests->SetItem(count, 2, result);

					if (result.Contains(wxT("error")))
						ListViewTests->SetItemBackgroundColour(count, wxColour(255, 255, 200, 255));
					if( result == wxT("pass"))
						ListViewTests->SetItemBackgroundColour(count, wxColour(200,255,200, 255));
					if (result == wxT("fail"))
						ListViewTests->SetItemBackgroundColour(count, wxColour(255, 200, 200, 255));
					if (result == wxT("retest"))
						ListViewTests->SetItemBackgroundColour(count, wxColour(255, 255, 200, 255));
				}
				break;
			}

			if (ePl.threadFinished)
			{
				gAudioIO->StopTestProcedure();
				if( ePl.eventID < 0 )
					wxMessageBox(wxT("Procedure completed with errors"), wxT("Tests finished"));
				else
					wxMessageBox(wxT("Procedure completed successfully"), wxT("Tests finished"));

				mTestIsRunning = false;
				EnableTestUI(true);

				ListViewTests->Select(ePl.eventCounter, false);
			}

		}
		break;
	}
}

void AVPTesterFrame::PopulateProjectInfo()
{
	TextCtrlPTitle->SetValue(mPManager->GetProjectTitle());
	TextCtrlPDate->SetValue(mPManager->GetProjectDate());
	TextCtrlProjDescription->SetValue(mPManager->GetProjectRemarks());

	std::vector<ProjectInfo> info = mPManager->GetProjectInfo();

	size_t infoSize = info.size();

	if (infoSize == 0)
	{
		TextCtrlPOps->SetValue(wxEmptyString);
		TextCtrlDAC->SetValue(wxEmptyString);
		TextCtrlADC->SetValue(wxEmptyString);
		return;
	}


	for (size_t i = 0; i < infoSize; i++)
	{
		ProjectInfo ie = info[i];

		if (ie.infoName == wxT("opsname"))
		{
			TextCtrlPOps->SetValue(ie.infoValue);
		}
		else if (ie.infoName == wxT("dacname"))
		{
			TextCtrlDAC->SetValue(ie.infoValue);
		}
		else if (ie.infoName == wxT("adcname"))
		{
			TextCtrlADC->SetValue(ie.infoValue);
		}
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
	if (!mPManager->IsProjectLoaded()) {
		wxMessageBox(wxT("Load or create a test project first"), wxT("Error"));
		return;
	}


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

void AVPTesterFrame::OnButtonDoSingleTestClick(wxCommandEvent& event)
{
	if (!mPManager->IsProjectLoaded()) {
		wxMessageBox(wxT("Load or create a test project first"), wxT("Error"));
		return;
	}

	if( mSelectedTestIdx >= 0 )
		gAudioIO->StartTestProcedure(mSelectedTestIdx);
}

void
AVPTesterFrame::PopulateTestsList()
{
	mPrjThread = new ProjectLoadThread(this);

	if (mPrjThread->Create() == wxTHREAD_NO_ERROR)
	{
		if (mPrjThread->Run() == wxTHREAD_NO_ERROR)
		{
			EnableTestUI(false, true);
			TimerLoadProject.Start(250, false);
			//mWaitDialog->ShowModal();
		}
	}
}

void
AVPTesterFrame::PopulateTestsListThx()
{
	mProjectLoading = true;

	mSelectedTestIdx = -1;
	mSelectedParamIdx = -1;

	ListViewTests->DeleteAllItems();
	ListViewTests->SetColumnWidth(0, 150);
	ListViewTests->SetColumnWidth(1, 65);
	ListViewTests->SetColumnWidth(1, 75);

	mTestDescriptors = mPManager->GetTestDescriptors();
	wxString dataPath = mPManager->GetDataFolderPath();

	for (size_t i = 0; i < mTestDescriptors.size(); i++)
	{
		TestDescriptor desc = mTestDescriptors[i];


		ListViewTests->InsertItem(i, desc.name, 0);

		if (desc.enabled == wxT("true"))
			ListViewTests->SetItem(i, 1, wxT("enabled"));
		else
			ListViewTests->SetItem(i, 1, wxT("disabled"));

		ListViewTests->SetItem(i, 2, wxT("LOADING..."));

		//check if results are already available from a previous run
		wxString resultsFilePath = dataPath + wxT('\\') + desc.resultPath;
		wxString outcome = GetOutcomeFromFile(resultsFilePath);

		ListViewTests->SetItem(i, 2, outcome);

		if (outcome == wxT("pass"))
			ListViewTests->SetItemBackgroundColour(i, wxColour(200, 255, 200, 255));
		if (outcome == wxT("fail"))
			ListViewTests->SetItemBackgroundColour(i, wxColour(255, 200, 200, 255));
		if (outcome == wxT("retest"))
			ListViewTests->SetItemBackgroundColour(i, wxColour(255, 255, 200, 255));

	}

	ListViewParameters->DeleteAllItems();
	TextCtrlTestDescription->SetValue(wxEmptyString);
	mProjectLoading = false;
}

void
AVPTesterFrame::PopulateTestParametersList()
{
	mSelectedParamIdx = -1;
	ListViewParameters->DeleteAllItems();

	mTestDescriptors = mPManager->GetTestDescriptors();
	TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];
	mParametersDescriptors = mPManager->GetTestParameters(dsc.ID);

	if (mParametersDescriptors.size() < 2)
	{
		ButtonDoSingleTest->enable(false);
		PopulateAudioIOInfo(false);
		PopulateModeInfo(false);
	}
	else
	{
		for (size_t i = 0; i < mParametersDescriptors.size(); i++)
		{
			TestParameter pr = mParametersDescriptors[i];
			ListViewParameters->InsertItem(i, pr.alias, 0);
			ListViewParameters->SetItem(i, 1, pr.value);
		}

		TestFileIOInfo finfo = mPManager->GetTestFileIOInfo(dsc.ID);

		PopulateAudioIOInfo(true);
		PopulateModeInfo(true);

		ButtonDoSingleTest->enable(!mTestIsRunning);
	}

	ListViewParameters->SetColumnWidth(0, 200);
	ListViewParameters->SetColumnWidth(1, 100);
}

void AVPTesterFrame::OnListViewTestsItemSelect(wxListEvent& event)
{
	mSelectedTestIdx = event.m_itemIndex;

	if (mSelectedTestIdx >= 0)
	{
		TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];
		TextCtrlTestDescription->SetValue(dsc.alias);
	}

	PopulateTestParametersList();
}

void AVPTesterFrame::OnListViewTestsItemActivated(wxListEvent& event)
{
	TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];
	wxString wSeparator = wxT("\\");
	wxString wFolder =mPManager->GetDataFolderPath();
	wxString wFileName = dsc.resultPath;

	wxString filePath = wFolder + wSeparator + wFileName;
	if (wxFileName::FileExists(filePath))
	{
		mResultsDialog->OpenResultsFile(filePath);
		mResultsDialog->Show(true);
		mResultsDialog->EnablePanZoom(true);
	}
	else
		wxMessageBox(wxT("No results are available for this test"), wxT("missing .."));
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
	TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];

	void *data = static_cast<wxMenu *>(evt.GetEventObject())->GetClientData();
	switch (evt.GetId())
	{
		case 0:
		{
			mPManager->EnableTest(dsc.ID, true);
			ListViewTests->SetItem(mSelectedTestIdx, 1, wxT("enabled"));
		}
		break;

		case 1:
		{
			mPManager->EnableTest(dsc.ID, false);
			ListViewTests->SetItem(mSelectedTestIdx, 1, wxT("disabled"));
		}
		break;
	}
}

void AVPTesterFrame::OnListViewParametersItemSelect(wxListEvent& event)
{
	mSelectedParamIdx = event.m_itemIndex;
}

void AVPTesterFrame::OnListViewParametersItemActivated(wxListEvent& event)
{
	if ((mSelectedTestIdx >= 0) && (mSelectedParamIdx >= 0))
	{
		TestParameter pr = mParametersDescriptors[mSelectedParamIdx];

		mParamDialog = new ParamEditDialog(this);
		mParamDialog->EditParameter(mSelectedTestIdx, pr.name);

		if (mParamDialog->ShowModal() == wxID_OK)
		{
			PopulateTestParametersList();
		}

		delete mParamDialog;
	}
}


bool
AVPTesterFrame::wxViewPDFFile(const wxString& command, const wxString& specificErrorMessage)
{
	// First see if there are any arguments
	wxString actualCommand, arguments;
	wxSeparateCommandAndArguments(command, actualCommand, arguments);

	wxString ext;// path, file;
	//wxSplitPath(actualCommand, &volume, &path, &file, &ext, wxPATH_NATIVE);
	wxFileName ff(actualCommand);
	ext = ff.GetExt();

	wxString extLower(ext.Lower());
	if (extLower == wxT("html") ||
		extLower == wxT("htm") ||
		extLower == wxT("sgml"))
	{
		//wxViewHTMLFile(command);
		return true;
	}
	else
	{
		bool isApp = false;
		if (ext.Lower() == wxT("exe") ||
			ext.Lower() == wxT("com") ||
			ext.Lower() == wxT("cmd") ||
			ext.Lower() == wxT("bat"))
			isApp = true;

		if (ext.IsEmpty())
			isApp = true;

		if (isApp)
		{
			wxString command2 = actualCommand;
			if (command2.GetChar(0) != wxT('"'))
				command2 = wxT("\"") + actualCommand + wxT("\"");
			return (wxExecute(command2) != 0);
		}

		wxString msg;
		wxString errMsg = specificErrorMessage;
		if (errMsg.IsEmpty())
		{
			errMsg = _("Please install a suitable application.");
		}

		if (!ext.IsEmpty())
		{
			wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
			if (!ft)
			{
				wxString msg;
				msg.Printf(_("Could not determine the application for extension %s.\n%s"), (const wxChar*)ext, (const wxChar*)errMsg);
				wxMessageBox(msg, wxT("Run command"), wxOK | wxICON_EXCLAMATION);
				return false;
			}

			wxString cmd;
			wxString params = command;
			bool ok = ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(params));

			//wxMessageBox(cmd);

			delete ft;

			if (!ok)
			{
				// TODO: some kind of configuration dialog here.
				wxString msg;
				msg.Printf(wxT("Could not determine the command for running %s.\n%s"), (const wxChar*)command, (const wxChar*)errMsg);

				wxMessageBox(msg, wxT("Run command"), wxOK | wxICON_EXCLAMATION);
				return false;
			}

			ok = (wxExecute(cmd, false) != 0);
			return ok;
		}
		return false;
	}
	return true;
}

// Separate the command-line from the arguments
bool
AVPTesterFrame::wxSeparateCommandAndArguments(const wxString& command, wxString& cmd, wxString& args)
{
	wxString lowerCaseCommand(command.Lower());
	cmd = command;
	args = wxEmptyString;

	wxString command2 = command;
	command2.Replace(wxT("\""), wxT(""));
	wxArrayString toFind;
	toFind.Add(wxT(".exe "));
	toFind.Add(wxT(".com "));
	toFind.Add(wxT(".bat "));
	toFind.Add(wxT(".cmd "));
	size_t i;
	size_t sz = toFind.GetCount();
	for (i = 0; i < sz; i++)
	{
		int pos = command2.Find(toFind[i]);
		if (pos > -1)
		{
			cmd = command2.Mid(0, pos + toFind[i].Length() - 1);
			args = command2.Mid(pos + toFind[i].Length());
			return true;
		}
	}
	return true;
}


void AVPTesterFrame::OnMenuItemFNewSelected(wxCommandEvent& event)
{
	wxString projectPath, projectName;

	NewProjectDialog dlg(this);
	if (dlg.ShowModal() == wxID_OK)
	{
		dlg.GetNewProjectDefs(projectPath, projectName);
		std::vector<ProjectInfo> pinfo = dlg.GetNewProjectInfo();

		int res = mPManager->NewProject(projectPath, projectName, pinfo);

		if (res == MgmtOK)
		{
			PopulateProjectInfo();
			PopulateTestsList();
		}
	}
}

void AVPTesterFrame::OnMenuItemFCloseSelected(wxCommandEvent& event)
{
	mPManager->CloseProject();
	PopulateProjectInfo();
	PopulateTestsList();
}

void AVPTesterFrame::OnButtonProjEditCtrlsClick(wxCommandEvent& event)
{
}

void AVPTesterFrame::OnMenuItemEditPInfoSelected(wxCommandEvent& event)
{
	if (!mPManager->IsProjectLoaded()) {
		wxMessageBox(wxT("Load or create a test project first"), wxT("Error"));
		return;
	}

	InfoEditDialog dlg(this);
	dlg.SetProjectInfo(mPManager->GetProjectInfo());
	if (dlg.ShowModal() == wxID_OK)
	{
		mPManager->SetProjectInfo(dlg.GetProjectInfo());
		PopulateProjectInfo();
	}
}

void AVPTesterFrame::OnTimerLoadProjectTrigger(wxTimerEvent& event)
{
	if (mProjectLoading == false)
	{
		//mWaitDialog->EndModal(mWaitDialogID);
		TimerLoadProject.Stop();
		EnableTestUI(true);
	}
}


/////////////////////////////////////////////////////////////////////
ProjectLoadThread::ProjectLoadThread(AVPTesterFrame* dlg) :wxThread()
{
	m_dlg = dlg;
};

ProjectLoadThread::ExitCode ProjectLoadThread::Entry()
{
	m_dlg->PopulateTestsListThx();

	return 0;
}


void AVPTesterFrame::OnListViewParametersBeginDrag(wxListEvent& event)
{
}

void AVPTesterFrame::OnChoiceTestModeSelect(wxCommandEvent& event)
{
	if (mSelectedTestIdx < 0)
		return;

	TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];

	int TModeSelected = ChoiceTestMode->GetSelection();

	if (TModeSelected == 0)
	{
		mPManager->SetTestType(dsc.ID, wxT("normal"));
	}
	else if (TModeSelected == 1)
	{
		mPManager->SetTestType(dsc.ID, wxT("offline"));
	}
	else
	{
		ButtonRespFile->enable(false);
	}
}

void AVPTesterFrame::OnButtonRespFileClick(wxCommandEvent& event)
{
	if (mSelectedTestIdx < 0)
		return;

	TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];

	if (ChoiceTestMode->GetSelection() == 0)
	{
		TestFileIOInfo finfo = mPManager->GetTestFileIOInfo(dsc.ID);

		GenParamEditDialog dlg(this);
		dlg.SetParamValue(finfo.responseFileName);
		if (dlg.ShowModal() == wxID_OK)
		{
			mPManager->SetTestResponseFileName( dsc.ID, dlg.GetParamValue());
			PopulateTestParametersList();
		}
	}
	else if (ChoiceTestMode->GetSelection() == 1)
	{
		wxFileDialog openFileDlg(this, wxT("Open response file"), wxT(""), wxT(""), wxT("Audio files (*.wav)|*.wav"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if (openFileDlg.ShowModal() == wxID_OK)
		{
			wxString filePath = openFileDlg.GetPath();
			wxString folder = openFileDlg.GetDirectory();
			wxString fileName = openFileDlg.GetFilename();

			if (mSelectedTestIdx >= 0)
			{
				if (folder != mPManager->GetDataFolderPath())
				{
					wxMessageBox(wxT("Offline response files MUST be placed in the project's folder"), wxT("Attention!"));
				}
				else
				{
					mPManager->SetTestResponseFileName(dsc.ID, fileName);
					PopulateTestParametersList();
				}
			}
		}
	}
}

void
AVPTesterFrame::PopulateModeInfo(bool enabled)
{
	ChoiceTestMode->Clear();
	ChoiceTestMode->Enable(enabled);
	ButtonRespFile->enable(enabled);
	ButtonRespFile->SetLbText(wxT(""));

	if (enabled && (mSelectedTestIdx >= 0))
	{
		ChoiceTestMode->Append(wxT("Normal"));
		ChoiceTestMode->Append(wxT("Offline"));

		mTestDescriptors = mPManager->GetTestDescriptors();
		TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];
		TestFileIOInfo finfo = mPManager->GetTestFileIOInfo(dsc.ID);

		ButtonRespFile->SetLbText(finfo.responseFileName);

		if (finfo.testType == wxT("normal")) {
			ChoiceTestMode->SetSelection(0);
		}
		else if (finfo.testType == wxT("offline")) {
			ChoiceTestMode->SetSelection(1);
		}
	}
}


void
AVPTesterFrame::PopulateAudioIOInfo(bool enabled)
{
	TextCtrlDACName->Enable(enabled);
	TextCtrlDACName->SetValue(wxEmptyString);
	ChoiceDACCh->Clear();
	ChoiceDACCh->Enable(enabled);
	TextCtrlADCName->Enable(enabled);
	TextCtrlADCName->SetValue(wxEmptyString);
	ChoiceADCCh->Clear();
	ChoiceADCCh->Enable(enabled);

	if (enabled && (mSelectedTestIdx >= 0))
	{
		//get current audio engine configuration info
		AudioIOInfo engineInfo = gAudioIO->GetAudioIOInfo();

		//populate channel choices
		TextCtrlDACName->SetValue(engineInfo.playbackDeviceName);
		wxString pbChanIndex;
		for (int i = 0; i < engineInfo.pbDevNumChannels; i++) {
			pbChanIndex.Printf(wxT("%d"), i);
			ChoiceDACCh->Append(pbChanIndex);
		}

		TextCtrlADCName->SetValue(engineInfo.recordingDeviceName);
		wxString recChanIndex;
		for (int i = 0; i < engineInfo.recDevNumChannels; i++) {
			recChanIndex.Printf(wxT("%d"), i);
			ChoiceADCCh->Append(recChanIndex);
		}


		mTestDescriptors = mPManager->GetTestDescriptors();
		TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];
		TestAudioIOInfo ainfo = mPManager->GetTestAudioIOInfo(dsc.ID);

		wxCommandEvent dummyEv;

		//check if selected playback channel index exceeds number of available channels
		if ((ainfo.signalChIdxNum + 1) > engineInfo.pbDevNumChannels) {
			ChoiceDACCh->SetSelection(0);
			OnChoiceDACChSelect(dummyEv);
		}
		else {
			ChoiceDACCh->SetSelection(ainfo.signalChIdxNum);
		}

		//check if selected recording channel index exceeds number of available channels
		if ((ainfo.responseChIdxNum + 1) > engineInfo.recDevNumChannels) {
			ChoiceADCCh->SetSelection(0);
			OnChoiceADCChSelect(dummyEv);
		}
		else {
			ChoiceADCCh->SetSelection(ainfo.responseChIdxNum);
		}
	}
}

void AVPTesterFrame::OnChoiceDACChSelect(wxCommandEvent& event)
{
	int sel = ChoiceDACCh->GetSelection();
	wxString selStr; selStr.Printf(wxT("%d"), sel);
	if (mSelectedTestIdx >= 0)
	{
		mTestDescriptors = mPManager->GetTestDescriptors();
		TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];
		mPManager->SetTestSignalChannel(dsc.ID, selStr);
	}
}

void AVPTesterFrame::OnChoiceADCChSelect(wxCommandEvent& event)
{
	int sel = ChoiceADCCh->GetSelection();
	wxString selStr; selStr.Printf(wxT("%d"), sel);
	if (mSelectedTestIdx >= 0)
	{
		mTestDescriptors = mPManager->GetTestDescriptors();
		TestDescriptor dsc = mTestDescriptors[mSelectedTestIdx];
		mPManager->SetTestResponseChannel(dsc.ID, selStr);
	}
}

void AVPTesterFrame::OnMenuItemInfoSelected(wxCommandEvent& event)
{
	AboutADCTDialog dlg(this);
	dlg.ShowModal();
}
