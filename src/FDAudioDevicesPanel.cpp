#include "FDAudioDevicesPanel.h"
#include "Devices\FDADevicesManager.h"
#include "System\Prefs.h"
#include "AudioIO\ParametersQueue.h"
#include "AudioIO\ProcessParams.h"
#include "AudioIO\FDAudioEngine.h"

//(*InternalHeaders(AudioDevicesPanel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(AudioDevicesPanel)
const long AudioDevicesPanel::ID_STATICTEXT_LINE_UP = wxNewId();
const long AudioDevicesPanel::ID_BUTTON_SCAN_AUDIO_SYS = wxNewId();
const long AudioDevicesPanel::ID_PANEL_SYS_GEN_SET = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_IN_GRP_LBL = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_IN_HOSTS = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_HOST_IN = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_IN_DEVS = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_INPUT_DEVICE = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_SYS_SRATE_IN = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_SYS_SRATE_IN = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_IN_CHANS = wxNewId();
const long AudioDevicesPanel::ID_TEXTCTRL_IN_CHANS = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_FRAMESIZE_IN = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_FRAME_SIZE_IN = wxNewId();
const long AudioDevicesPanel::ID_PANEL_INPUTS = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_OUT_GRP_LBl = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_OUT_HOSTS = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_HOST_OUT = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_OUT_DEVS = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_OUTPUT_DEVICE = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_SYS_SRATE_OUT = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_SYS_SRATE_OUT = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_OUT_CHANS = wxNewId();
const long AudioDevicesPanel::ID_TEXTCTRL_OUT_CHANS = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_FRAMESIZE_OUT = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_FRAME_SIZE_OUT = wxNewId();
const long AudioDevicesPanel::ID_PANEL_OUTPUTS = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_DEV_TST_TTL = wxNewId();
const long AudioDevicesPanel::ID_BUTTON_TST_DEV_START = wxNewId();
const long AudioDevicesPanel::ID_BUTTON_TST_DEV_STOP = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_CAL_INSTR = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_PB_STREAM_TTL = wxNewId();
const long AudioDevicesPanel::ID_PANEL_PB_IN_L = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_GAIN_TTL = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_GAIN = wxNewId();
const long AudioDevicesPanel::ID_BUTTON_OUT_GAIN_UP = wxNewId();
const long AudioDevicesPanel::ID_BUTTON_OUT_GAIN_DWN = wxNewId();
const long AudioDevicesPanel::ID_PANEL_PB_IN_R = wxNewId();
const long AudioDevicesPanel::ID_PANEL_PLAYBACK = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_REC_STREAM_TTL = wxNewId();
const long AudioDevicesPanel::ID_PANEL_REC_IN_L = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_REF_CH = wxNewId();
const long AudioDevicesPanel::ID_SPIN_REF_CH = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_RTA_LENGTH = wxNewId();
const long AudioDevicesPanel::ID_SPIN_FFT_LENGTH = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_RTA_AVG = wxNewId();
const long AudioDevicesPanel::ID_SPIN_RTA_AVG = wxNewId();
const long AudioDevicesPanel::ID_BUTTON_RESET_LTA = wxNewId();
const long AudioDevicesPanel::ID_BUTTON_PLOT_RESET = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_CH_PK_LVL = wxNewId();
const long AudioDevicesPanel::ID_PANEL_CHANNEL_LVLS = wxNewId();
const long AudioDevicesPanel::ID_PANEL_FFT_CTRLS = wxNewId();
const long AudioDevicesPanel::ID_RTA_FFT_PLOT = wxNewId();
const long AudioDevicesPanel::ID_PANEL_FFT_PLOT = wxNewId();
const long AudioDevicesPanel::ID_PANEL_REC_IN_R = wxNewId();
const long AudioDevicesPanel::ID_PANEL_RECORDING = wxNewId();
const long AudioDevicesPanel::ID_PANEL_DTEST_GRP = wxNewId();
const long AudioDevicesPanel::ID_TIMER_AUDIO_MONITOR = wxNewId();
//*)
const long AudioDevicesPanel::ID_VUMETER_IN = wxNewId();
const long AudioDevicesPanel::ID_VUMETER_OUT = wxNewId();

BEGIN_EVENT_TABLE(AudioDevicesPanel,wxPanel)
	//(*EventTable(AudioDevicesPanel)
	//*)
END_EVENT_TABLE()

AudioDevicesPanel::AudioDevicesPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(AudioDevicesPanel)
	wxBoxSizer* BoxSizerAvgOn;
	wxBoxSizer* BoxSizerAxesCtrls;
	wxBoxSizer* BoxSizerBtnCntr;
	wxBoxSizer* BoxSizerBtnsLay;
	wxBoxSizer* BoxSizerChannelLevels;
	wxBoxSizer* BoxSizerDTestGrp;
	wxBoxSizer* BoxSizerDevTest;
	wxBoxSizer* BoxSizerFFTCtrls;
	wxBoxSizer* BoxSizerFFTPlot;
	wxBoxSizer* BoxSizerFFTSize;
	wxBoxSizer* BoxSizerGainBtns;
	wxBoxSizer* BoxSizerGenSettings;
	wxBoxSizer* BoxSizerHostAPIIn;
	wxBoxSizer* BoxSizerHostAPIOut;
	wxBoxSizer* BoxSizerInAvgOn;
	wxBoxSizer* BoxSizerInFFTSize;
	wxBoxSizer* BoxSizerInFrameSize;
	wxBoxSizer* BoxSizerInGrp;
	wxBoxSizer* BoxSizerInRefC;
	wxBoxSizer* BoxSizerInSampleRate;
	wxBoxSizer* BoxSizerInputChannels;
	wxBoxSizer* BoxSizerInputDevice;
	wxBoxSizer* BoxSizerInputDevices;
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerOutFrameSize;
	wxBoxSizer* BoxSizerOutGrp;
	wxBoxSizer* BoxSizerOutSampleRate;
	wxBoxSizer* BoxSizerOutputChannels;
	wxBoxSizer* BoxSizerOutputDevice;
	wxBoxSizer* BoxSizerOutputDevices;
	wxBoxSizer* BoxSizerPBInR;
	wxBoxSizer* BoxSizerPBTTL;
	wxBoxSizer* BoxSizerRTAPlot;
	wxBoxSizer* BoxSizerRecInR;
	wxBoxSizer* BoxSizerRecTTL;
	wxBoxSizer* BoxSizerRefCh;
	wxBoxSizer* BoxSizerRescan;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetForegroundColour(wxColour(128,64,64));
	SetBackgroundColour(wxColour(200,200,200));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	StaticTextLineUp = new wxStaticText(this, ID_STATICTEXT_LINE_UP, wxEmptyString, wxDefaultPosition, wxSize(-1,2), wxNO_BORDER, _T("ID_STATICTEXT_LINE_UP"));
	StaticTextLineUp->SetBackgroundColour(wxColour(180,180,180));
	BoxSizerMain->Add(StaticTextLineUp, 0, wxALL|wxEXPAND, 0);
	PanelSysGenSettings = new RimPanel(this, ID_PANEL_SYS_GEN_SET, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_SYS_GEN_SET"));
	BoxSizerGenSettings = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerRescan = new wxBoxSizer(wxVERTICAL);
	ButtonScanAudioSys = new GButton(PanelSysGenSettings, ID_BUTTON_SCAN_AUDIO_SYS, _("Scan system"), wxDefaultPosition, wxSize(100,22), 0, wxDefaultValidator, _T("ID_BUTTON_SCAN_AUDIO_SYS"));
	BoxSizerRescan->Add(ButtonScanAudioSys, 0, wxALL|wxALIGN_LEFT, 4);
	BoxSizerGenSettings->Add(BoxSizerRescan, 0, wxALL|wxEXPAND, 1);
	PanelSysGenSettings->SetSizer(BoxSizerGenSettings);
	BoxSizerGenSettings->Fit(PanelSysGenSettings);
	BoxSizerGenSettings->SetSizeHints(PanelSysGenSettings);
	BoxSizerMain->Add(PanelSysGenSettings, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	BoxSizerInGrp = new wxBoxSizer(wxVERTICAL);
	StaticTextInGrpLbl = new wxStaticText(this, ID_STATICTEXT_IN_GRP_LBL, _("  Input configuration"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_IN_GRP_LBL"));
	StaticTextInGrpLbl->SetForegroundColour(wxColour(255,255,255));
	StaticTextInGrpLbl->SetBackgroundColour(wxColour(78,183,223));
	wxFont StaticTextInGrpLblFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	StaticTextInGrpLbl->SetFont(StaticTextInGrpLblFont);
	BoxSizerInGrp->Add(StaticTextInGrpLbl, 0, wxALL|wxEXPAND, 1);
	PanelInputs = new RimPanel(this, ID_PANEL_INPUTS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_INPUTS"));
	BoxSizerInputDevices = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerHostAPIIn = new wxBoxSizer(wxVERTICAL);
	StaticTextInHosts = new wxStaticText(PanelInputs, ID_STATICTEXT_IN_HOSTS, _("audio API"), wxDefaultPosition, wxSize(200,-1), 0, _T("ID_STATICTEXT_IN_HOSTS"));
	BoxSizerHostAPIIn->Add(StaticTextInHosts, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	ChoiceHostInput = new wxChoice(PanelInputs, ID_CHOICE_HOST_IN, wxDefaultPosition, wxSize(200,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_HOST_IN"));
	BoxSizerHostAPIIn->Add(ChoiceHostInput, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	BoxSizerInputDevices->Add(BoxSizerHostAPIIn, 0, wxALL|wxALIGN_TOP, 1);
	BoxSizerInputDevice = new wxBoxSizer(wxVERTICAL);
	StaticTextInDevices = new wxStaticText(PanelInputs, ID_STATICTEXT_IN_DEVS, _("audio devices"), wxDefaultPosition, wxSize(250,-1), 0, _T("ID_STATICTEXT_IN_DEVS"));
	BoxSizerInputDevice->Add(StaticTextInDevices, 0, wxLEFT|wxALIGN_LEFT, 4);
	ChoiceInputDevice = new wxChoice(PanelInputs, ID_CHOICE_INPUT_DEVICE, wxDefaultPosition, wxSize(250,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_INPUT_DEVICE"));
	BoxSizerInputDevice->Add(ChoiceInputDevice, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	BoxSizerInputDevices->Add(BoxSizerInputDevice, 0, wxALL|wxALIGN_TOP, 1);
	BoxSizerInSampleRate = new wxBoxSizer(wxVERTICAL);
	StaticTextSysSampleRateIn = new wxStaticText(PanelInputs, ID_STATICTEXT_SYS_SRATE_IN, _("sample rate"), wxDefaultPosition, wxSize(120,-1), 0, _T("ID_STATICTEXT_SYS_SRATE_IN"));
	BoxSizerInSampleRate->Add(StaticTextSysSampleRateIn, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	ChoiceSystemSampleRateIn = new wxChoice(PanelInputs, ID_CHOICE_SYS_SRATE_IN, wxDefaultPosition, wxSize(120,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_SYS_SRATE_IN"));
	BoxSizerInSampleRate->Add(ChoiceSystemSampleRateIn, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	BoxSizerInputDevices->Add(BoxSizerInSampleRate, 0, wxALL, 1);
	BoxSizerInputChannels = new wxBoxSizer(wxVERTICAL);
	StaticTextInChannels = new wxStaticText(PanelInputs, ID_STATICTEXT_IN_CHANS, _("channels"), wxDefaultPosition, wxSize(60,-1), 0, _T("ID_STATICTEXT_IN_CHANS"));
	BoxSizerInputChannels->Add(StaticTextInChannels, 0, wxLEFT|wxEXPAND, 4);
	TextCtrlInChannels = new wxTextCtrl(PanelInputs, ID_TEXTCTRL_IN_CHANS, _("0"), wxDefaultPosition, wxSize(60,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL_IN_CHANS"));
	TextCtrlInChannels->SetBackgroundColour(wxColour(220,220,220));
	BoxSizerInputChannels->Add(TextCtrlInChannels, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT, 5);
	BoxSizerInputDevices->Add(BoxSizerInputChannels, 0, wxALL|wxEXPAND, 1);
	BoxSizerInFrameSize = new wxBoxSizer(wxVERTICAL);
	StaticTextFrameSizeIn = new wxStaticText(PanelInputs, ID_STATICTEXT_FRAMESIZE_IN, _("frame size"), wxDefaultPosition, wxSize(100,-1), 0, _T("ID_STATICTEXT_FRAMESIZE_IN"));
	BoxSizerInFrameSize->Add(StaticTextFrameSizeIn, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	ChoiceFrameSizeIn = new wxChoice(PanelInputs, ID_CHOICE_FRAME_SIZE_IN, wxDefaultPosition, wxSize(100,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_FRAME_SIZE_IN"));
	ChoiceFrameSizeIn->Append(_("64"));
	ChoiceFrameSizeIn->Append(_("128"));
	ChoiceFrameSizeIn->Append(_("256"));
	ChoiceFrameSizeIn->Append(_("512"));
	ChoiceFrameSizeIn->Append(_("1024"));
	ChoiceFrameSizeIn->Append(_("2048"));
	ChoiceFrameSizeIn->Append(_("4096"));
	ChoiceFrameSizeIn->Append(_("8192"));
	ChoiceFrameSizeIn->Append(_("16384"));
	BoxSizerInFrameSize->Add(ChoiceFrameSizeIn, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	BoxSizerInputDevices->Add(BoxSizerInFrameSize, 0, wxALL|wxALIGN_TOP, 1);
	BoxSizerInputDevices->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	PanelInputs->SetSizer(BoxSizerInputDevices);
	BoxSizerInputDevices->Fit(PanelInputs);
	BoxSizerInputDevices->SetSizeHints(PanelInputs);
	BoxSizerInGrp->Add(PanelInputs, 0, wxBOTTOM|wxRIGHT|wxEXPAND, 5);
	BoxSizerMain->Add(BoxSizerInGrp, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	BoxSizerOutGrp = new wxBoxSizer(wxVERTICAL);
	StaticTextOutGrpLbl = new wxStaticText(this, ID_STATICTEXT_OUT_GRP_LBl, _("  Output configuration"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_OUT_GRP_LBl"));
	StaticTextOutGrpLbl->SetForegroundColour(wxColour(255,255,255));
	StaticTextOutGrpLbl->SetBackgroundColour(wxColour(78,183,223));
	wxFont StaticTextOutGrpLblFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	StaticTextOutGrpLbl->SetFont(StaticTextOutGrpLblFont);
	BoxSizerOutGrp->Add(StaticTextOutGrpLbl, 0, wxALL|wxEXPAND, 1);
	PanelOutputs = new RimPanel(this, ID_PANEL_OUTPUTS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_OUTPUTS"));
	BoxSizerOutputDevices = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerHostAPIOut = new wxBoxSizer(wxVERTICAL);
	StaticTextOutHosts = new wxStaticText(PanelOutputs, ID_STATICTEXT_OUT_HOSTS, _("audio API"), wxDefaultPosition, wxSize(200,-1), 0, _T("ID_STATICTEXT_OUT_HOSTS"));
	BoxSizerHostAPIOut->Add(StaticTextOutHosts, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	ChoiceHostOutput = new wxChoice(PanelOutputs, ID_CHOICE_HOST_OUT, wxDefaultPosition, wxSize(200,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_HOST_OUT"));
	BoxSizerHostAPIOut->Add(ChoiceHostOutput, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	BoxSizerOutputDevices->Add(BoxSizerHostAPIOut, 0, wxALL|wxALIGN_TOP, 1);
	BoxSizerOutputDevice = new wxBoxSizer(wxVERTICAL);
	StaticTextOutDevices = new wxStaticText(PanelOutputs, ID_STATICTEXT_OUT_DEVS, _("audio devices"), wxDefaultPosition, wxSize(250,-1), 0, _T("ID_STATICTEXT_OUT_DEVS"));
	BoxSizerOutputDevice->Add(StaticTextOutDevices, 0, wxLEFT|wxALIGN_LEFT, 4);
	ChoiceOutputDevice = new wxChoice(PanelOutputs, ID_CHOICE_OUTPUT_DEVICE, wxDefaultPosition, wxSize(250,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_OUTPUT_DEVICE"));
	BoxSizerOutputDevice->Add(ChoiceOutputDevice, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	BoxSizerOutputDevices->Add(BoxSizerOutputDevice, 0, wxALL|wxALIGN_TOP, 1);
	BoxSizerOutSampleRate = new wxBoxSizer(wxVERTICAL);
	StaticTextSysSampleRateOut = new wxStaticText(PanelOutputs, ID_STATICTEXT_SYS_SRATE_OUT, _("sample rate"), wxDefaultPosition, wxSize(120,-1), 0, _T("ID_STATICTEXT_SYS_SRATE_OUT"));
	BoxSizerOutSampleRate->Add(StaticTextSysSampleRateOut, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	ChoiceSystemSampleRateOut = new wxChoice(PanelOutputs, ID_CHOICE_SYS_SRATE_OUT, wxDefaultPosition, wxSize(120,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_SYS_SRATE_OUT"));
	BoxSizerOutSampleRate->Add(ChoiceSystemSampleRateOut, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	BoxSizerOutputDevices->Add(BoxSizerOutSampleRate, 0, wxALL|wxALIGN_TOP, 1);
	BoxSizerOutputChannels = new wxBoxSizer(wxVERTICAL);
	StaticTextOutChannels = new wxStaticText(PanelOutputs, ID_STATICTEXT_OUT_CHANS, _("channels"), wxDefaultPosition, wxSize(60,-1), 0, _T("ID_STATICTEXT_OUT_CHANS"));
	BoxSizerOutputChannels->Add(StaticTextOutChannels, 0, wxLEFT|wxEXPAND, 4);
	TextCtrlOutChannels = new wxTextCtrl(PanelOutputs, ID_TEXTCTRL_OUT_CHANS, _("0"), wxDefaultPosition, wxSize(60,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL_OUT_CHANS"));
	TextCtrlOutChannels->SetBackgroundColour(wxColour(220,220,220));
	BoxSizerOutputChannels->Add(TextCtrlOutChannels, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	BoxSizerOutputDevices->Add(BoxSizerOutputChannels, 0, wxALL|wxALIGN_TOP, 1);
	BoxSizerOutFrameSize = new wxBoxSizer(wxVERTICAL);
	StaticTextFrameSizeOut = new wxStaticText(PanelOutputs, ID_STATICTEXT_FRAMESIZE_OUT, _("frame size"), wxDefaultPosition, wxSize(100,-1), 0, _T("ID_STATICTEXT_FRAMESIZE_OUT"));
	BoxSizerOutFrameSize->Add(StaticTextFrameSizeOut, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	ChoiceFrameSizeOut = new wxChoice(PanelOutputs, ID_CHOICE_FRAME_SIZE_OUT, wxDefaultPosition, wxSize(100,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_FRAME_SIZE_OUT"));
	ChoiceFrameSizeOut->Append(_("64"));
	ChoiceFrameSizeOut->Append(_("128"));
	ChoiceFrameSizeOut->Append(_("256"));
	ChoiceFrameSizeOut->Append(_("512"));
	ChoiceFrameSizeOut->Append(_("1024"));
	ChoiceFrameSizeOut->Append(_("2048"));
	ChoiceFrameSizeOut->Append(_("4096"));
	ChoiceFrameSizeOut->Append(_("8192"));
	ChoiceFrameSizeOut->Append(_("16384"));
	BoxSizerOutFrameSize->Add(ChoiceFrameSizeOut, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT, 4);
	BoxSizerOutputDevices->Add(BoxSizerOutFrameSize, 0, wxALL, 1);
	BoxSizerOutputDevices->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	PanelOutputs->SetSizer(BoxSizerOutputDevices);
	BoxSizerOutputDevices->Fit(PanelOutputs);
	BoxSizerOutputDevices->SetSizeHints(PanelOutputs);
	BoxSizerOutGrp->Add(PanelOutputs, 0, wxBOTTOM|wxRIGHT|wxEXPAND, 5);
	BoxSizerMain->Add(BoxSizerOutGrp, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	BoxSizerTestGrp = new wxBoxSizer(wxVERTICAL);
	StaticTextDevTestLbl = new wxStaticText(this, ID_STATICTEXT_DEV_TST_TTL, _("  Audio devices calibration"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_DEV_TST_TTL"));
	StaticTextDevTestLbl->SetForegroundColour(wxColour(255,255,255));
	StaticTextDevTestLbl->SetBackgroundColour(wxColour(78,183,223));
	wxFont StaticTextDevTestLblFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	StaticTextDevTestLbl->SetFont(StaticTextDevTestLblFont);
	BoxSizerTestGrp->Add(StaticTextDevTestLbl, 0, wxALL|wxEXPAND, 1);
	PanelDTestGrp = new RimPanel(this, ID_PANEL_DTEST_GRP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_DTEST_GRP"));
	BoxSizerDTestGrp = new wxBoxSizer(wxVERTICAL);
	BoxSizerDevTest = new wxBoxSizer(wxHORIZONTAL);
	ButtonDevTestStart = new GButton(PanelDTestGrp, ID_BUTTON_TST_DEV_START, _("Start"), wxDefaultPosition, wxSize(100,22), 0, wxDefaultValidator, _T("ID_BUTTON_TST_DEV_START"));
	BoxSizerDevTest->Add(ButtonDevTestStart, 0, wxALL|wxALIGN_TOP, 5);
	ButtonDevTestStop = new GButton(PanelDTestGrp, ID_BUTTON_TST_DEV_STOP, _("Stop"), wxDefaultPosition, wxSize(100,22), 0, wxDefaultValidator, _T("ID_BUTTON_TST_DEV_STOP"));
	BoxSizerDevTest->Add(ButtonDevTestStop, 0, wxALL|wxALIGN_TOP, 5);
	StaticTextCalibInstructions = new wxStaticText(PanelDTestGrp, ID_STATICTEXT_CAL_INSTR, _("Adjust volume until the peak value shows -3dB"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_CAL_INSTR"));
	BoxSizerDevTest->Add(StaticTextCalibInstructions, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizerDevTest->Add(-1,-1,1, wxALL|wxALIGN_TOP, 5);
	BoxSizerDTestGrp->Add(BoxSizerDevTest, 0, wxALL|wxEXPAND, 1);
	BoxSizerMonitor = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerMonitor->Add(1,200,0, wxALL|wxALIGN_TOP, 0);
	PanelPlayback = new wxPanel(PanelDTestGrp, ID_PANEL_PLAYBACK, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL|wxNO_FULL_REPAINT_ON_RESIZE, _T("ID_PANEL_PLAYBACK"));
	BoxSizerPlayback = new wxBoxSizer(wxVERTICAL);
	StaticTextPBStreamTTL = new wxStaticText(PanelPlayback, ID_STATICTEXT_PB_STREAM_TTL, _("output stream"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_PB_STREAM_TTL"));
	BoxSizerPlayback->Add(StaticTextPBStreamTTL, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT, 5);
	BoxSizerPBTTL = new wxBoxSizer(wxHORIZONTAL);
	PanelPBInL = new RimPanel(PanelPlayback, ID_PANEL_PB_IN_L, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_FULL_REPAINT_ON_RESIZE, _T("ID_PANEL_PB_IN_L"));
	BoxSizerPBInL = new wxBoxSizer(wxHORIZONTAL);
	PanelPBInL->SetSizer(BoxSizerPBInL);
	BoxSizerPBInL->Fit(PanelPBInL);
	BoxSizerPBInL->SetSizeHints(PanelPBInL);
	BoxSizerPBTTL->Add(PanelPBInL, 1, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 3);
	PanelPBInR = new RimPanel(PanelPlayback, ID_PANEL_PB_IN_R, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PB_IN_R"));
	BoxSizerPBInR = new wxBoxSizer(wxVERTICAL);
	BoxSizerBtnsLay = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerBtnsLay->Add(1,-1,1, wxEXPAND, 5);
	BoxSizerGainBtns = new wxBoxSizer(wxVERTICAL);
	StaticTextGainTtl = new wxStaticText(PanelPBInR, ID_STATICTEXT_GAIN_TTL, _("gain"), wxDefaultPosition, wxSize(40,18), wxALIGN_CENTRE, _T("ID_STATICTEXT_GAIN_TTL"));
	BoxSizerGainBtns->Add(StaticTextGainTtl, 0, wxEXPAND, 5);
	StaticTextOutGain = new wxStaticText(PanelPBInR, ID_STATICTEXT_GAIN, _("0 dB"), wxDefaultPosition, wxSize(40,18), wxALIGN_CENTRE, _T("ID_STATICTEXT_GAIN"));
	BoxSizerGainBtns->Add(StaticTextOutGain, 0, wxBOTTOM|wxEXPAND, 0);
	ButtonOutGainUp = new GButton(PanelPBInR, ID_BUTTON_OUT_GAIN_UP, _("+"), wxDefaultPosition, wxSize(40,30), 0, wxDefaultValidator, _T("ID_BUTTON_OUT_GAIN_UP"));
	BoxSizerGainBtns->Add(ButtonOutGainUp, 0, wxTOP|wxBOTTOM|wxALIGN_LEFT, 5);
	ButtonOutGainDwn = new GButton(PanelPBInR, ID_BUTTON_OUT_GAIN_DWN, _("-"), wxDefaultPosition, wxSize(40,30), 0, wxDefaultValidator, _T("ID_BUTTON_OUT_GAIN_DWN"));
	BoxSizerGainBtns->Add(ButtonOutGainDwn, 0, wxBOTTOM|wxALIGN_LEFT, 5);
	BoxSizerBtnsLay->Add(BoxSizerGainBtns, 0, wxTOP|wxALIGN_TOP, 5);
	BoxSizerBtnsLay->Add(1,-1,1, wxALL|wxEXPAND, 0);
	BoxSizerPBInR->Add(BoxSizerBtnsLay, 0, wxBOTTOM|wxEXPAND, 5);
	PanelPBInR->SetSizer(BoxSizerPBInR);
	BoxSizerPBInR->Fit(PanelPBInR);
	BoxSizerPBInR->SetSizeHints(PanelPBInR);
	BoxSizerPBTTL->Add(PanelPBInR, 1, wxALL|wxEXPAND, 3);
	BoxSizerPlayback->Add(BoxSizerPBTTL, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 0);
	PanelPlayback->SetSizer(BoxSizerPlayback);
	BoxSizerPlayback->Fit(PanelPlayback);
	BoxSizerPlayback->SetSizeHints(PanelPlayback);
	BoxSizerMonitor->Add(PanelPlayback, 1, wxALL|wxEXPAND, 5);
	PanelRecording = new wxPanel(PanelDTestGrp, ID_PANEL_RECORDING, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL|wxNO_FULL_REPAINT_ON_RESIZE, _T("ID_PANEL_RECORDING"));
	BoxSizerRecording = new wxBoxSizer(wxVERTICAL);
	StaticTextRecStreamTTL = new wxStaticText(PanelRecording, ID_STATICTEXT_REC_STREAM_TTL, _("input stream"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_REC_STREAM_TTL"));
	BoxSizerRecording->Add(StaticTextRecStreamTTL, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT, 5);
	BoxSizerRecTTL = new wxBoxSizer(wxHORIZONTAL);
	PanelRecInL = new RimPanel(PanelRecording, ID_PANEL_REC_IN_L, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_FULL_REPAINT_ON_RESIZE|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_REC_IN_L"));
	BoxSizerRecInL = new wxBoxSizer(wxHORIZONTAL);
	PanelRecInL->SetSizer(BoxSizerRecInL);
	BoxSizerRecInL->Fit(PanelRecInL);
	BoxSizerRecInL->SetSizeHints(PanelRecInL);
	BoxSizerRecTTL->Add(PanelRecInL, 1, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 3);
	Panel1RecInR = new wxPanel(PanelRecording, ID_PANEL_REC_IN_R, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_REC_IN_R"));
	BoxSizerRecInR = new wxBoxSizer(wxVERTICAL);
	BoxSizerFFTPlot = new wxBoxSizer(wxHORIZONTAL);
	PanelFFTCtrls = new RimPanel(Panel1RecInR, ID_PANEL_FFT_CTRLS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_FFT_CTRLS"));
	BoxSizerFFTCtrls = new wxBoxSizer(wxVERTICAL);
	BoxSizerRefCh = new wxBoxSizer(wxVERTICAL);
	StaticTextRefCh = new wxStaticText(PanelFFTCtrls, ID_STATICTEXT_REF_CH, _(" channel "), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT_REF_CH"));
	StaticTextRefCh->SetBackgroundColour(wxColour(215,215,215));
	BoxSizerRefCh->Add(StaticTextRefCh, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
	BoxSizerInRefC = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerInRefC->Add(-1,-1,1, wxALL, 0);
	SpinRefCh = new awohSpin(PanelFFTCtrls,ID_SPIN_REF_CH,wxDefaultPosition,wxSize(40,-1));
	BoxSizerInRefC->Add(SpinRefCh, 0, wxLEFT|wxRIGHT, 4);
	BoxSizerInRefC->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	BoxSizerRefCh->Add(BoxSizerInRefC, 0, wxALL|wxEXPAND, 0);
	BoxSizerFFTCtrls->Add(BoxSizerRefCh, 0, wxALL|wxEXPAND, 1);
	BoxSizerFFTCtrls->Add(-1,10,0, wxALL|wxEXPAND, 0);
	BoxSizerFFTSize = new wxBoxSizer(wxVERTICAL);
	StaticTextRTALength = new wxStaticText(PanelFFTCtrls, ID_STATICTEXT_RTA_LENGTH, _(" resolution "), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT_RTA_LENGTH"));
	StaticTextRTALength->SetBackgroundColour(wxColour(215,215,215));
	BoxSizerFFTSize->Add(StaticTextRTALength, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
	BoxSizerInFFTSize = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerInFFTSize->Add(-1,-1,1, wxALL, 0);
	SpinFFTLen = new awohSpin(PanelFFTCtrls,ID_SPIN_FFT_LENGTH,wxDefaultPosition,wxSize(40,-1));
	BoxSizerInFFTSize->Add(SpinFFTLen, 0, wxLEFT|wxRIGHT, 4);
	BoxSizerInFFTSize->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	BoxSizerFFTSize->Add(BoxSizerInFFTSize, 0, wxALL|wxEXPAND, 0);
	BoxSizerFFTCtrls->Add(BoxSizerFFTSize, 0, wxALL|wxEXPAND, 1);
	BoxSizerFFTCtrls->Add(-1,10,0, wxALL|wxEXPAND, 0);
	BoxSizerAvgOn = new wxBoxSizer(wxVERTICAL);
	StaticTextRTAAvg = new wxStaticText(PanelFFTCtrls, ID_STATICTEXT_RTA_AVG, _(" average "), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT_RTA_AVG"));
	StaticTextRTAAvg->SetBackgroundColour(wxColour(215,215,215));
	BoxSizerAvgOn->Add(StaticTextRTAAvg, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 1);
	BoxSizerInAvgOn = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerInAvgOn->Add(-1,-1,1, wxALL, 0);
	SpinRTAvg = new awohSpin(PanelFFTCtrls,ID_SPIN_RTA_AVG,wxDefaultPosition,wxSize(40,-1));
	BoxSizerInAvgOn->Add(SpinRTAvg, 0, wxLEFT|wxRIGHT, 4);
	BoxSizerInAvgOn->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	BoxSizerAvgOn->Add(BoxSizerInAvgOn, 0, wxALL|wxEXPAND, 0);
	BoxSizerFFTCtrls->Add(BoxSizerAvgOn, 0, wxALL|wxEXPAND, 1);
	BoxSizerFFTCtrls->Add(-1,10,1, wxALL|wxEXPAND, 0);
	BoxSizerBtnCntr = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerBtnCntr->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	BoxSizerAxesCtrls = new wxBoxSizer(wxVERTICAL);
	ButtonResetLTA = new GButton(PanelFFTCtrls, ID_BUTTON_RESET_LTA, _("reset avg"), wxDefaultPosition, wxSize(82,18), wxNO_BORDER, wxDefaultValidator, _T("ID_BUTTON_RESET_LTA"));
	BoxSizerAxesCtrls->Add(ButtonResetLTA, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizerAxesCtrls->Add(-1,10,1, wxALL|wxEXPAND, 0);
	ButtonPlotReset = new GButton(PanelFFTCtrls, ID_BUTTON_PLOT_RESET, _("reset plot"), wxDefaultPosition, wxSize(82,18), wxNO_BORDER, wxDefaultValidator, _T("ID_BUTTON_PLOT_RESET"));
	BoxSizerAxesCtrls->Add(ButtonPlotReset, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizerBtnCntr->Add(BoxSizerAxesCtrls, 0, wxALL|wxALIGN_TOP, 0);
	BoxSizerBtnCntr->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	BoxSizerFFTCtrls->Add(BoxSizerBtnCntr, 0, wxALL|wxEXPAND, 0);
	BoxSizerFFTCtrls->Add(-1,30,1, wxALL|wxEXPAND, 0);
	PanelChannelLevels = new RimPanel(PanelFFTCtrls, ID_PANEL_CHANNEL_LVLS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_CHANNEL_LVLS"));
	BoxSizerChannelLevels = new wxBoxSizer(wxVERTICAL);
	StaticTextChPkLvl = new wxStaticText(PanelChannelLevels, ID_STATICTEXT_CH_PK_LVL, _("peak:\nrms:"), wxDefaultPosition, wxSize(90,-1), 0, _T("ID_STATICTEXT_CH_PK_LVL"));
	StaticTextChPkLvl->SetForegroundColour(wxColour(128,64,64));
	StaticTextChPkLvl->SetBackgroundColour(wxColour(210,210,210));
	BoxSizerChannelLevels->Add(StaticTextChPkLvl, 0, wxALL|wxEXPAND, 4);
	PanelChannelLevels->SetSizer(BoxSizerChannelLevels);
	BoxSizerChannelLevels->Fit(PanelChannelLevels);
	BoxSizerChannelLevels->SetSizeHints(PanelChannelLevels);
	BoxSizerFFTCtrls->Add(PanelChannelLevels, 0, wxALL|wxEXPAND, 4);
	PanelFFTCtrls->SetSizer(BoxSizerFFTCtrls);
	BoxSizerFFTCtrls->Fit(PanelFFTCtrls);
	BoxSizerFFTCtrls->SetSizeHints(PanelFFTCtrls);
	BoxSizerFFTPlot->Add(PanelFFTCtrls, 0, wxLEFT|wxRIGHT|wxEXPAND, 1);
	PanelFFTPlot = new RimPanel(Panel1RecInR, ID_PANEL_FFT_PLOT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_FFT_PLOT"));
	BoxSizerRTAPlot = new wxBoxSizer(wxVERTICAL);
	BoxSizerRTAPlot->Add(500,1,0, wxALL|wxALIGN_LEFT, 0);
	mRTAMagPLot = new mpWindow(PanelFFTPlot,ID_RTA_FFT_PLOT,wxDefaultPosition,wxDefaultSize);
	BoxSizerRTAPlot->Add(mRTAMagPLot, 1, wxALL|wxEXPAND, 4);
	PanelFFTPlot->SetSizer(BoxSizerRTAPlot);
	BoxSizerRTAPlot->Fit(PanelFFTPlot);
	BoxSizerRTAPlot->SetSizeHints(PanelFFTPlot);
	BoxSizerFFTPlot->Add(PanelFFTPlot, 1, wxLEFT|wxRIGHT|wxEXPAND, 1);
	BoxSizerRecInR->Add(BoxSizerFFTPlot, 1, wxLEFT|wxRIGHT|wxEXPAND, 1);
	Panel1RecInR->SetSizer(BoxSizerRecInR);
	BoxSizerRecInR->Fit(Panel1RecInR);
	BoxSizerRecInR->SetSizeHints(Panel1RecInR);
	BoxSizerRecTTL->Add(Panel1RecInR, 7, wxALL|wxEXPAND, 3);
	BoxSizerRecording->Add(BoxSizerRecTTL, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 0);
	PanelRecording->SetSizer(BoxSizerRecording);
	BoxSizerRecording->Fit(PanelRecording);
	BoxSizerRecording->SetSizeHints(PanelRecording);
	BoxSizerMonitor->Add(PanelRecording, 4, wxALL|wxEXPAND, 5);
	BoxSizerDTestGrp->Add(BoxSizerMonitor, 0, wxALL|wxEXPAND, 1);
	PanelDTestGrp->SetSizer(BoxSizerDTestGrp);
	BoxSizerDTestGrp->Fit(PanelDTestGrp);
	BoxSizerDTestGrp->SetSizeHints(PanelDTestGrp);
	BoxSizerTestGrp->Add(PanelDTestGrp, 0, wxBOTTOM|wxRIGHT|wxEXPAND, 5);
	BoxSizerMain->Add(BoxSizerTestGrp, 0, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizerMain);
	TimerAudioMonitor.SetOwner(this, ID_TIMER_AUDIO_MONITOR);
	TimerAudioMonitor.Start(50, false);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);

	Connect(ID_BUTTON_SCAN_AUDIO_SYS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonScanAudioSysClick);
	Connect(ID_CHOICE_HOST_IN,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceHostInputSelect);
	Connect(ID_CHOICE_INPUT_DEVICE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceInputDeviceSelect);
	Connect(ID_CHOICE_SYS_SRATE_IN,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceSystemSampleRateInSelect);
	Connect(ID_CHOICE_FRAME_SIZE_IN,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceFrameSizeInSelect);
	Connect(ID_CHOICE_HOST_OUT,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceHostOutputSelect);
	Connect(ID_CHOICE_OUTPUT_DEVICE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceOutputDeviceSelect);
	Connect(ID_CHOICE_SYS_SRATE_OUT,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceSystemSampleRateOutSelect);
	Connect(ID_CHOICE_FRAME_SIZE_OUT,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceFrameSizeOutSelect);
	Connect(ID_BUTTON_TST_DEV_START,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonDevTestStartClick);
	Connect(ID_BUTTON_TST_DEV_STOP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonDevTestStopClick);
	Connect(ID_BUTTON_OUT_GAIN_UP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonOutGainUpClick);
	Connect(ID_BUTTON_OUT_GAIN_DWN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonOutGainDwnClick);
	Connect(ID_BUTTON_RESET_LTA,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonResetLTAClick);
	Connect(ID_BUTTON_PLOT_RESET,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonPlotResetClick);
	Connect(ID_TIMER_AUDIO_MONITOR,wxEVT_TIMER,(wxObjectEventFunction)&AudioDevicesPanel::OnTimerAudioMonitorTrigger);
	//*)

	TimerAudioMonitor.Stop();

	Connect(ID_BUTTON_SCAN_AUDIO_SYS, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonScanAudioSysClick);
	Connect(ID_BUTTON_TST_DEV_START, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonDevTestStartClick);
	Connect(ID_BUTTON_TST_DEV_STOP, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonDevTestStopClick);
	Connect(ID_BUTTON_OUT_GAIN_UP, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonOutGainUpClick);
	Connect(ID_BUTTON_OUT_GAIN_DWN, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonOutGainDwnClick);
	Connect(ID_BUTTON_RESET_LTA, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonResetLTAClick);
	Connect(ID_BUTTON_PLOT_RESET, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonPlotResetClick);

	PanelFFTCtrls->EnableGradient(false);
	PanelFFTPlot->EnableGradient(false);
	PanelChannelLevels->setBGColours(210, 210, 210, 210, 210, 210);

	SpinRefCh->Connect(wxEVT_COMMAND_AWOHSPIN, (wxObjectEventFunction)&AudioDevicesPanel::OnSpinFFTReference, 0, this);
	SpinRefCh->setBGColour(215, 215, 215, wxALPHA_OPAQUE);
	SpinRefCh->setFGColour(128, 64, 64, wxALPHA_OPAQUE);
	mSelectedChannel = 0;

	SpinFFTLen->Connect(wxEVT_COMMAND_AWOHSPIN, (wxObjectEventFunction)&AudioDevicesPanel::OnSpinFFTLength, 0, this);
	SpinFFTLen->appendValues(wxT("512"), 512);
	SpinFFTLen->appendValues(wxT("1024"), 1024);
	SpinFFTLen->appendValues(wxT("2048"), 2048);
	SpinFFTLen->appendValues(wxT("4096"), 4096);
	SpinFFTLen->appendValues(wxT("8192"), 8192);
	SpinFFTLen->appendValues(wxT("16384"), 16384);
	SpinFFTLen->appendValues(wxT("32768"), 32768);
	SpinFFTLen->setBGColour(215, 215, 215, wxALPHA_OPAQUE);;
	SpinFFTLen->setFGColour(128, 64, 64, wxALPHA_OPAQUE);

	SpinRTAvg->Connect(wxEVT_COMMAND_AWOHSPIN, (wxObjectEventFunction)&AudioDevicesPanel::OnSpinRTAvg, 0, this);
	SpinRTAvg->appendValues(wxT("vfast"), 100);
	SpinRTAvg->appendValues(wxT("fast"), 50);
	SpinRTAvg->appendValues(wxT("med "), 10);
	SpinRTAvg->appendValues(wxT("slow"), 1);
	SpinRTAvg->appendValues(wxT("stop"), 0);
	SpinRTAvg->setBGColour(215, 215, 215, wxALPHA_OPAQUE);
	SpinRTAvg->setFGColour(128, 64, 64, wxALPHA_OPAQUE);

	mNumInputChannels = -1;
	mPrevNumInputChannels = -1;
	mVuMeterIn = NULL;
	mNumOutputChannels = -1;
	mPrevNumOutputChannels = -1;
	mVuMeterOut = NULL;

	mRTAMagPLot->EnableDoubleBuffer(true);
	mRTAMagPLot->SetMPScrollbars(false);
	mRTAMagPLot->SetColourTheme(wxColor(220, 220, 220), wxColor(128, 64, 64), wxColor(200, 2000, 2000));
	mRTAMagPLot->SetMargins(20, 20, 20, 40);

	ButtonDevTestStop->enable(false);
	ButtonDevTestStart->enable(true);

	PopulateAll();

	RefreshUI();
}

AudioDevicesPanel::~AudioDevicesPanel()
{
	//(*Destroy(AudioDevicesPanel)
	//*)
	StopCalibration();
}

void
AudioDevicesPanel::RefreshUI()
{//get saved calibration parameters and populate widgets
	std::vector<AudioParam> params = gAudioIO->getCalibrationParameters();
	size_t noParams = params.size();

	for (size_t i = 0; i < noParams; i++)
	{
		AudioParam msg = params[i];

		switch (msg.paramIdx)
		{
		case kOutputGain:
		{
			mOutputStreamGain = msg.value;
			UpdateOutputGain(false);
		}
		break;

		case kFFTLength:
		{
			mRTALength = (size_t)msg.value;
			mRTAPlotSize = 1 + (mRTALength / 2);
			SpinFFTLen->setValue(mRTALength);
		}
		break;

		case kFFTAverage:
		{
			int LTASlope = (int)(msg.value);
			SpinRTAvg->setValue(LTASlope);
		}
		break;

		default:
			break;
		}
	}
}

void
AudioDevicesPanel::ConfigureChannelSpin()
{
	SpinRefCh->clearValues();

	int chIdx = 1;
	for (int i = 0; i < mNumInputChannels; i++)
	{
		wxString vStr;
		vStr.Printf(wxT("%d"), chIdx);
		SpinRefCh->appendValues(vStr, i);
		chIdx++;
	}
	SpinRefCh->setValue(0);
}

void
AudioDevicesPanel::PopulateAll()
{
	PopulateHostsChoices();
	PopulateDevicesChoices(false);
	PopulateInputFrameLenChoice();
	PopulateOutputFrameLenChoice();
}

void AudioDevicesPanel::PopulateHostsChoices()
{
	size_t i;

	///////////////////////////////////////////////////////////////////////////////////////////////
	wxArrayString InputHosts;
	const std::vector<ADeviceMap> &inMaps = ADevicesManager::Instance()->GetInputDeviceMaps();
	for (i = 0; i < inMaps.size(); i++)
	{
		if (InputHosts.Index(inMaps[i].hostString) == wxNOT_FOUND)
			InputHosts.Add(inMaps[i].hostString);
	}
	ChoiceHostInput->Clear();
	ChoiceHostInput->Append(InputHosts);

	if (InputHosts.GetCount() == 0)
		ChoiceHostInput->Enable(false);

	ChoiceHostInput->InvalidateBestSize();
	ChoiceHostInput->SetMaxSize(ChoiceHostInput->GetBestSize());

	//see if the preferred host api is in the list
	wxString prefInHost = gPrefs->Read(wxT("/AudioIO/InputAudioHostName"), wxT(""));
	int itemInIdx = ChoiceHostInput->FindString(prefInHost);

	if (itemInIdx != wxNOT_FOUND)
		ChoiceHostInput->SetSelection(itemInIdx);
	else
	{
		ChoiceHostInput->SetSelection(-1);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	wxArrayString OutputHosts;
	const std::vector<ADeviceMap> &outMaps = ADevicesManager::Instance()->GetOutputDeviceMaps();
	for (i = 0; i < outMaps.size(); i++)
	{
		if (OutputHosts.Index(outMaps[i].hostString) == wxNOT_FOUND)
			OutputHosts.Add(outMaps[i].hostString);
	}

	ChoiceHostOutput->Clear();
	ChoiceHostOutput->Append(OutputHosts);

	if (OutputHosts.GetCount() == 0)
		ChoiceHostOutput->Enable(false);

	ChoiceHostOutput->InvalidateBestSize();
	ChoiceHostOutput->SetMaxSize(ChoiceHostOutput->GetBestSize());

	//see if the preferred host api is in the list
	wxString prefOutHost = gPrefs->Read(wxT("/AudioIO/OutputAudioHostName"), wxT(""));
	int itemOutIdx = ChoiceHostOutput->FindString(prefOutHost);

	if (itemOutIdx != wxNOT_FOUND)
		ChoiceHostOutput->SetSelection(itemOutIdx);
	else
	{
		ChoiceHostOutput->SetSelection(-1);
	}
}

void AudioDevicesPanel::PopulateDevicesChoices(bool handleChanges)
{
	//read what is the current selected input host
	int inHostIndex = ChoiceHostInput->GetCurrentSelection();

	if (inHostIndex < 0)
		return;

	wxString inputHost = ChoiceHostInput->GetString(inHostIndex);

	//////////////////////////////////////////////////////////////////////////////
	// input devices
	const std::vector<ADeviceMap> &inMaps = ADevicesManager::Instance()->GetInputDeviceMaps();
	ChoiceInputDevice->Clear();

	// Repopulate the Input device list available to the user
	for (size_t i = 0; i < inMaps.size(); i++)
	{
		if (inputHost == inMaps[i].hostString)
		{
			ChoiceInputDevice->Append(inMaps[i].deviceString);
		}
	}

	ChoiceInputDevice->Enable(ChoiceInputDevice->GetCount() ? true : false);
	//see if the preferred device is in the list
	wxString prefInDev = gPrefs->Read(wxT("/AudioIO/InputDevName"), wxT(""));
	wxString prefInDevCh = gPrefs->Read(wxT("/AudioIO/InputDevChans"), wxT(""));
	int itemIdxIn = ChoiceInputDevice->FindString(prefInDev);

	if (itemIdxIn != wxNOT_FOUND)
	{
		ChoiceInputDevice->SetSelection(itemIdxIn);
		HandleInputDevSelection(itemIdxIn, handleChanges);
	}
	else
	{
		ChoiceInputDevice->SetSelection(0);
		HandleInputDevSelection(0, handleChanges);
	}

	///////////////////////////////////////////////////////////////////////
	//read what is the current selected input host
	int outHostIndex = ChoiceHostOutput->GetCurrentSelection();

	if (outHostIndex < 0)
		return;

	wxString outputHost = ChoiceHostOutput->GetString(outHostIndex);

	//////////////////////////////////////////////////////////////////////
	// output devices
	const std::vector<ADeviceMap> &outMaps = ADevicesManager::Instance()->GetOutputDeviceMaps();

	ChoiceOutputDevice->Clear();

	// Repopulate the Input device list available to the user
	for (size_t i = 0; i < outMaps.size(); i++)
	{
		if (outputHost == outMaps[i].hostString)
		{
			ChoiceOutputDevice->Append(outMaps[i].deviceString);
		}
	}

	ChoiceOutputDevice->Enable(ChoiceOutputDevice->GetCount() ? true : false);

	//see if the preferred device is in the list
	wxString prefOutDev = gPrefs->Read(wxT("/AudioIO/OutputDevName"), wxT(""));
	wxString prefOutDevCh = gPrefs->Read(wxT("/AudioIO/OutputDevChans"), wxT(""));
	int itemIdxOut = ChoiceOutputDevice->FindString(prefOutDev);

	if (itemIdxOut != wxNOT_FOUND)
	{
		ChoiceOutputDevice->SetSelection(itemIdxOut);
		HandleOutputDevSelection(itemIdxOut, handleChanges);
	}
	else
	{
		ChoiceOutputDevice->SetSelection(0);
		HandleOutputDevSelection(0, handleChanges);
	}
}

void
AudioDevicesPanel::HandleInputDevSelection(int selIdx, bool checkSR)
{
	if (selIdx < 0)
		selIdx = ChoiceInputDevice->GetSelection();

	wxString devIn = ChoiceInputDevice->GetString(selIdx);

	int hostIdx = ChoiceHostInput->GetCurrentSelection();
	wxString host = ChoiceHostInput->GetString(hostIdx);

	//find max supported channels for this device and get sompatible sample rates
	const std::vector<ADeviceMap> &inMaps = ADevicesManager::Instance()->GetInputDeviceMaps();
	int nCh = -1;
	mSupportedInputSRates.clear();
	for (size_t i = 0; i < inMaps.size(); i++)
	{
		if ((host == inMaps[i].hostString) && (devIn == inMaps[i].deviceString))
		{
			nCh = inMaps[i].numChannels;
			mSupportedInputSRates = inMaps[i].supportedRates;
		}
	}

	//write the new configuration to the preferences file.
	//this will be used by the Audio Engine to open the stream
	gPrefs->Write(wxT("/AudioIO/InputDevName"), devIn);
	gPrefs->Write(wxT("/AudioIO/InputDevChans"), nCh);
	gPrefs->Flush();

	mNumInputChannels = nCh;
	wxString nChTxt;
	nChTxt.Printf(wxT("%d"), nCh);
	TextCtrlInChannels->SetValue(nChTxt);

	//Rebuild the metering UI to handle max channels
	BuildTestUI();

	//check for compatible rates for both devices
	//if (checkSR)
		//CheckCompatibleSampleRates();

	UpdateInputSRateChoice();

	//update the channel spin control on the FFT panel
	ConfigureChannelSpin();
}

void
AudioDevicesPanel::PopulateInputFrameLenChoice()
{
	wxString fLen = gPrefs->Read(wxT("/AudioIO/InputFrameSize"), wxT(""));
	int itemIdx = ChoiceFrameSizeIn->FindString(fLen);

	if (itemIdx != wxNOT_FOUND)
		ChoiceFrameSizeIn->SetSelection(itemIdx);
	else
	{
		ChoiceFrameSizeIn->SetSelection(3);
		gPrefs->Write(wxT("/AudioIO/InputFrameSize"), ChoiceFrameSizeIn->GetString(3));
	}
}

void
AudioDevicesPanel::PopulateOutputFrameLenChoice()
{
	wxString fLen = gPrefs->Read(wxT("/AudioIO/OutputFrameSize"), wxT(""));
	int itemIdx = ChoiceFrameSizeOut->FindString(fLen);

	if (itemIdx != wxNOT_FOUND)
		ChoiceFrameSizeOut->SetSelection(itemIdx);
	else
	{
		ChoiceFrameSizeOut->SetSelection(3);
		gPrefs->Write(wxT("/AudioIO/OutputFrameSize"), ChoiceFrameSizeIn->GetString(3));
	}
}

void
AudioDevicesPanel::UpdateInputSRateChoice()
{
	wxArrayString SRArray;

	size_t noSupportedInRates = mSupportedInputSRates.size();
	wxString cRt;

	for (size_t i = 0; i < noSupportedInRates; i++)
	{
		double inRate = mSupportedInputSRates[i];
		cRt.Printf(wxT("%.0f"), inRate);
		SRArray.Add(cRt);
	}

	ChoiceSystemSampleRateIn->Clear();
	ChoiceSystemSampleRateIn->Append(SRArray);

	//see if the preferred sample rate is in the list
	wxString prefSRate = gPrefs->Read(wxT("/AudioIO/InputAudioSRate"), wxT(""));
	int itemIdx = ChoiceSystemSampleRateIn->FindString(prefSRate);

	if (itemIdx != wxNOT_FOUND)
		ChoiceSystemSampleRateIn->SetSelection(itemIdx);
	else
	{
		ChoiceSystemSampleRateIn->SetSelection(0);
		gPrefs->Write(wxT("/AudioIO/InputAudioSRate"), ChoiceSystemSampleRateIn->GetString(0));
	}
}


void
AudioDevicesPanel::HandleOutputDevSelection(int selIdx, bool checkSR)
{
	if (selIdx < 0)
		selIdx = ChoiceOutputDevice->GetSelection();

	wxString devOut = ChoiceOutputDevice->GetString(selIdx);

	int hostIdx = ChoiceHostOutput->GetCurrentSelection();
	wxString host = ChoiceHostOutput->GetString(hostIdx);

	//find max supported channels for this device and get sompatible sample rates
	const std::vector<ADeviceMap> &outMaps = ADevicesManager::Instance()->GetOutputDeviceMaps();
	int nCh = -1;
	mSupportedOutputSRates.clear();
	for (size_t i = 0; i < outMaps.size(); i++)
	{
		if ((host == outMaps[i].hostString) && (devOut == outMaps[i].deviceString))
		{
			nCh = outMaps[i].numChannels;
			mSupportedOutputSRates = outMaps[i].supportedRates;
		}
	}

	//write the new configuration to the preferences file.
	//this will be used by the Audio Engine to open the stream
	gPrefs->Write(wxT("/AudioIO/OutputDevName"), devOut);
	gPrefs->Write(wxT("/AudioIO/OutputDevChans"), nCh);
	gPrefs->Flush();

	mNumOutputChannels = nCh;
	wxString nChTxt;
	nChTxt.Printf(wxT("%d"), nCh);
	TextCtrlOutChannels->SetValue(nChTxt);

	//Rebuild the metering UI to handle max channels
	BuildTestUI();

	//if (checkSR)
		//CheckCompatibleSampleRates();

	UpdateOutputSRateChoice();
}

void
AudioDevicesPanel::UpdateOutputSRateChoice()
{
	wxArrayString SRArray;

	size_t noSupportedOutRates = mSupportedOutputSRates.size();
	wxString cRt;

	for (size_t i = 0; i < noSupportedOutRates; i++)
	{
		double outRate = mSupportedOutputSRates[i];
		cRt.Printf(wxT("%.0f"), outRate);
		SRArray.Add(cRt);
	}

	ChoiceSystemSampleRateOut->Clear();
	ChoiceSystemSampleRateOut->Append(SRArray);

	//see if the preferred sample rate is in the list
	wxString prefSRate = gPrefs->Read(wxT("/AudioIO/OutputAudioSRate"), wxT(""));
	int itemIdx = ChoiceSystemSampleRateOut->FindString(prefSRate);

	if (itemIdx != wxNOT_FOUND)
		ChoiceSystemSampleRateOut->SetSelection(itemIdx);
	else
	{
		ChoiceSystemSampleRateOut->SetSelection(0);
		gPrefs->Write(wxT("/AudioIO/OutputAudioSRate"), ChoiceSystemSampleRateOut->GetString(0));
	}
}

void AudioDevicesPanel::OnChoiceHostInputSelect(wxCommandEvent& event)
{
	int hostSelectionIndex;
	hostSelectionIndex = ChoiceHostInput->GetSelection();
	wxString newHost = ChoiceHostInput->GetString(hostSelectionIndex);

	//change the host and switch to correct devices.
	gPrefs->Write(wxT("/AudioIO/InputAudioHostName"), newHost);
	gPrefs->Flush();

	// populate the devices
	PopulateDevicesChoices();

	CheckIfASIO_IO();
}

void AudioDevicesPanel::OnChoiceHostOutputSelect(wxCommandEvent& event)
{
	int hostSelectionIndex;
	hostSelectionIndex = ChoiceHostOutput->GetSelection();
	wxString newHost = ChoiceHostOutput->GetString(hostSelectionIndex);

	//change the host and switch to correct devices.
	gPrefs->Write(wxT("/AudioIO/OutputAudioHostName"), newHost);
	gPrefs->Flush();

	// populate the devices
	PopulateDevicesChoices();

	CheckIfASIO_IO();
}

void AudioDevicesPanel::CheckIfASIO_IO()
{
	wxString outHostAPI = ChoiceHostOutput->GetString(ChoiceHostOutput->GetSelection());
	wxString inHostAPI  = ChoiceHostInput->GetString(ChoiceHostInput->GetSelection());

	outHostAPI.MakeLower(); 
	inHostAPI.MakeLower(); 
	bool isIOASIO = outHostAPI.Contains(wxT("asio")) && inHostAPI.Contains(wxT("asio"));

	if (isIOASIO)
	{
		wxString inFL = ChoiceFrameSizeIn->GetString(ChoiceFrameSizeIn->GetSelection());
		int itemIdxFL = ChoiceFrameSizeOut->FindString(inFL);
		ChoiceFrameSizeOut->SetSelection(itemIdxFL);
		gPrefs->Write(wxT("/AudioIO/OutputFrameSize"), ChoiceFrameSizeOut->GetString(itemIdxFL));
		ChoiceFrameSizeOut->Enable(false);

		wxString inSR = ChoiceSystemSampleRateIn->GetString(ChoiceSystemSampleRateIn->GetSelection());
		int itemIdxSR = ChoiceSystemSampleRateOut->FindString(inSR);

		if (itemIdxSR != wxNOT_FOUND) {
			ChoiceSystemSampleRateOut->SetSelection(itemIdxSR);
			gPrefs->Write(wxT("/AudioIO/OutputAudioSRate"), ChoiceSystemSampleRateOut->GetString(itemIdxSR));
			ChoiceSystemSampleRateOut->Enable(false);
			gPrefs->Flush();
		}
		else
		{
			wxMessageBox( wxT("Cannot use unequal sampling rates on ASIO driver"), wxT("Warning"));
		}
	}
	else
	{
		ChoiceSystemSampleRateOut->Enable(true);
		ChoiceFrameSizeOut->Enable(true);
	}
}

void AudioDevicesPanel::OnChoiceInputDeviceSelect(wxCommandEvent& event)
{
	int devSelectionIndex;
	devSelectionIndex = ChoiceInputDevice->GetSelection();
	HandleInputDevSelection(devSelectionIndex);

	CheckIfASIO_IO();
}

void AudioDevicesPanel::OnChoiceOutputDeviceSelect(wxCommandEvent& event)
{
	int devSelectionIndex;
	devSelectionIndex = ChoiceOutputDevice->GetSelection();
	HandleOutputDevSelection(devSelectionIndex);

	CheckIfASIO_IO();
}

void AudioDevicesPanel::OnChoiceSystemSampleRateInSelect(wxCommandEvent& event)
{
	int SRateSelectionIndex = ChoiceSystemSampleRateIn->GetSelection();
	wxString newSRate = ChoiceSystemSampleRateIn->GetString(SRateSelectionIndex);

	//change the host and switch to correct devices.
	gPrefs->Write(wxT("/AudioIO/InputAudioSRate"), newSRate);
	gPrefs->Flush();

	CheckIfASIO_IO();
}

void AudioDevicesPanel::OnChoiceSystemSampleRateOutSelect(wxCommandEvent& event)
{
	int SRateSelectionIndex = ChoiceSystemSampleRateOut->GetSelection();
	wxString newSRate = ChoiceSystemSampleRateOut->GetString(SRateSelectionIndex);

	//change the host and switch to correct devices.
	gPrefs->Write(wxT("/AudioIO/OutputAudioSRate"), newSRate);
	gPrefs->Flush();

	CheckIfASIO_IO();
}

void AudioDevicesPanel::OnChoiceFrameSizeInSelect(wxCommandEvent& event)
{
	int FSizeSelectionIndex = ChoiceFrameSizeIn->GetSelection();
	wxString newFSize = ChoiceFrameSizeIn->GetString(FSizeSelectionIndex);

	//change the host and switch to correct devices.
	gPrefs->Write(wxT("/AudioIO/InputFrameSize"), newFSize);
	gPrefs->Flush();

	CheckIfASIO_IO();
}

void AudioDevicesPanel::OnChoiceFrameSizeOutSelect(wxCommandEvent& event)
{
	int FSizeSelectionIndex = ChoiceFrameSizeOut->GetSelection();
	wxString newFSize = ChoiceFrameSizeOut->GetString(FSizeSelectionIndex);

	//change the host and switch to correct devices.
	gPrefs->Write(wxT("/AudioIO/OutputFrameSize"), newFSize);
	gPrefs->Flush();

	CheckIfASIO_IO();
}

void AudioDevicesPanel::OnButtonScanAudioSysClick(wxCommandEvent& event)
{
	EnableSelectionTools(false);
	ADevicesManager::Instance()->Rescan();
	PopulateAll();
	EnableSelectionTools(true);
}

void AudioDevicesPanel::OnButtonDevTestStartClick(wxCommandEvent& event)
{
	StartCalibration();
}

void AudioDevicesPanel::OnButtonDevTestStopClick(wxCommandEvent& event)
{
	StopCalibration();
}

void AudioDevicesPanel::OnButtonOutGainUpClick(wxCommandEvent& event)
{
	mCtrlDwn = wxGetKeyState(WXK_CONTROL);

	if (mCtrlDwn)
		mOutputStreamGain += 1;
	else
		mOutputStreamGain += 0.1;

	if (mOutputStreamGain > 0)
		mOutputStreamGain = 0;

	UpdateOutputGain(true);
}

void AudioDevicesPanel::OnButtonOutGainDwnClick(wxCommandEvent& event)
{
	mCtrlDwn = wxGetKeyState(WXK_CONTROL);

	if (mCtrlDwn)
		mOutputStreamGain -= 1;
	else
		mOutputStreamGain -= 0.1;

	UpdateOutputGain(true);
}

void AudioDevicesPanel::UpdateOutputGain(bool write)
{
	wxString gnStr;
	gnStr.Printf(wxT("%g dB"), mOutputStreamGain);
	StaticTextOutGain->SetLabel(gnStr);

	if (write)
	{
		//send to audio engine
		AudioParam msg;
		msg.paramIdx = kOutputGain;
		msg.value = mOutputStreamGain;
		gAudioIO->SetParameter(msg, false);
	}
}

void AudioDevicesPanel::OnButtonResetLTAClick(wxCommandEvent& event)
{
	AudioParam msg;
	msg.paramIdx = kFFTAverageReset;
	msg.value = 1;
	gAudioIO->SetParameter(msg, false);
}

void AudioDevicesPanel::OnButtonPlotResetClick(wxCommandEvent& event)
{
	mRTAMagPLot->Fit(0, mMaxXcord, -145, 6);
}

void
AudioDevicesPanel::OnSpinFFTReference(awohSpinEvent& event)
{
	mSelectedChannel = SpinRefCh->getNumericalValue();
	//send to audio engine
	AudioParam msg;
	msg.paramIdx = kFFTRefChannel;
	msg.value = (float)mSelectedChannel;
	gAudioIO->SetParameter(msg, false);
}

void
AudioDevicesPanel::OnSpinRTAvg(awohSpinEvent& event)
{
	float pVal = (float)SpinRTAvg->getNumericalValue();
	//send to audio engine
	AudioParam msg;
	msg.paramIdx = kFFTAverage;
	msg.value = pVal;
	gAudioIO->SetParameter(msg, false);
}

void
AudioDevicesPanel::OnSpinFFTLength(awohSpinEvent& event)
{
	float pVal = (float)SpinFFTLen->getNumericalValue();
	//send to audio engine
	AudioParam msg;
	msg.paramIdx = kFFTLength;
	msg.value = pVal;
	gAudioIO->SetParameter(msg, false);
}

void
AudioDevicesPanel::StartCalibration()
{
	mAudioTestStarted = true;
	ButtonDevTestStop->enable(true);
	ButtonDevTestStart->enable(false);
	EnableSelectionTools(false);

	gAudioIO->StartCalibration();
	TimerAudioMonitor.Start(50, false);
}

void
AudioDevicesPanel::StopCalibration()
{
	TimerAudioMonitor.Stop();

	if (mVuMeterIn)
		mVuMeterIn->Reset();

	if (mVuMeterOut)
		mVuMeterOut->Reset();

	mAudioTestStarted = false;
	ButtonDevTestStop->enable(false);
	ButtonDevTestStart->enable(true);
	
	gAudioIO->StopDevices();
	EnableSelectionTools(true);
}

void AudioDevicesPanel::BuildTestUI()
{
	// the number of channels has changed
	// rebuild meters
	if (mNumInputChannels != mPrevNumInputChannels)
	{
		mPrevNumInputChannels = mNumInputChannels;

		if (mVuMeterIn)
		{
			delete mVuMeterIn;
			mVuMeterIn = NULL;
		}

		if (mPrevNumInputChannels > 0)
		{
			mVuMeterIn = new VuMeter(PanelRecInL, ID_VUMETER_OUT, wxT(""), wxT("dB"), wxDefaultPosition, wxSize(30 * mNumInputChannels, 120));
			//mVuMeterIn = new VuMeter(PanelRecording, ID_VUMETER_OUT, wxT(""), wxT("dB"), wxDefaultPosition, wxSize(30 * mNumInputChannels, 120));
			// configure vu meters
			mVuMeterIn->setRange(mNumInputChannels, -120, 5, -6, -24);
			mVuMeterIn->SetBackColour(wxColour(215, 215, 215));
			mVuMeterIn->SetNeedleColour(wxColour(255, 255, 255));
			mVuMeterIn->EnableTicks(true, false);
			LevelMetrics li;
			li.numChannels = mNumInputChannels;
			for (int nCh = 0; nCh < mNumInputChannels; nCh++)
			{
				li.peak[nCh] = -115;
				li.rms[nCh] = -115;
			}
			mVuMeterIn->SetValue(li);
			BoxSizerRecInL->Add(mVuMeterIn, 0, wxEXPAND | wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
			BoxSizerRecInL->Fit(PanelRecInL);
			//BoxSizerRecording->Add(mVuMeterIn, 0, wxEXPAND | wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
			//BoxSizerRecording->Fit(PanelRecording);
		}
	}

	if (mNumOutputChannels != mPrevNumOutputChannels)
	{
		mPrevNumOutputChannels = mNumOutputChannels;

		if (mVuMeterOut)
		{
			delete mVuMeterOut;
			mVuMeterOut = NULL;
		}

		if (mPrevNumOutputChannels > 0)
		{
			//mVuMeterOut = new VuMeter(PanelPlayback, ID_VUMETER_OUT, wxT(""), wxT("dB"), wxDefaultPosition, wxSize(30 * mNumOutputChannels, 120));
			mVuMeterOut = new VuMeter(PanelPBInL, ID_VUMETER_OUT, wxT(""), wxT("dB"), wxDefaultPosition, wxSize(30 * mNumOutputChannels, 120));
			mVuMeterOut->setRange(mNumOutputChannels, -120, 5, -6, -24);
			mVuMeterOut->SetBackColour(wxColour(215, 215, 215));
			mVuMeterOut->SetNeedleColour(wxColour(255, 255, 255));
			mVuMeterOut->EnableTicks(true, false);
			LevelMetrics lo;
			lo.numChannels = mNumOutputChannels;
			for (int nCh = 0; nCh < mNumOutputChannels; nCh++)
			{
				lo.peak[nCh] = -115;
				lo.rms[nCh] = -115;
			}
			mVuMeterOut->SetValue(lo);
			//BoxSizerPlayback->Add(mVuMeterOut, 0, wxEXPAND | wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
			//BoxSizerPlayback->Fit(PanelPlayback);
			BoxSizerPBInL->Add(mVuMeterOut, 0, wxEXPAND | wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
			BoxSizerPBInL->Layout();
			BoxSizerPBInL->Fit(PanelPBInL);
		}
	}
	BoxSizerRecording->Layout();
	BoxSizerPlayback->Layout();
}

void AudioDevicesPanel::EnableSelectionTools(bool enabled)
{
	ChoiceHostInput->Enable(enabled);
	ChoiceHostOutput->Enable(enabled);
	ChoiceInputDevice->Enable(enabled);
	ChoiceOutputDevice->Enable(enabled);
	ChoiceSystemSampleRateIn->Enable(enabled);
	ChoiceSystemSampleRateOut->Enable(enabled);
	TextCtrlInChannels->Enable(enabled);
	TextCtrlOutChannels->Enable(enabled);
	ButtonScanAudioSys->enable(enabled);
	ChoiceFrameSizeIn->Enable(enabled);
	ChoiceFrameSizeOut->Enable(enabled);
}

void
AudioDevicesPanel::ConfigurePlot(size_t RTALength, double sampleRate)
{
	////////////////////////////////////////////////////////////
	//FFT analysis of EQ response    //FFT analysis of EQ response
	mRTALength = RTALength;
	mRTAPlotSize = 1 + mRTALength / 2;
	mSampleRate = sampleRate;

	wxColour mInPlotClr(128, 64, 64);
	wxColour mPlotTextColour(128, 128, 128);

	//////////////////////////////////////////////////////////////////
	double fstep = (double)mSampleRate / (double)mRTALength;
	mRTAMagPLot->DelAllLayers(true, true);

	// Create a mpFXYVector layer
	mFFTInMagLayer = new mpFXYVector();
	wxPen inVectorpen(mInPlotClr, 1, wxSOLID);
	mFFTInMagLayer->SetPen(inVectorpen);
	mFFTInMagLayer->SetDrawOutsideMargins(false);
	mFFTInMagLayer->SetContinuity(true);

	wxPen textPen(mPlotTextColour, 1, wxSOLID);
	wxFont graphFont(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	mpScaleXSpectrum* xaxis = new mpScaleXSpectrum(wxT("Frequency"), mpALIGN_BOTTOM, true, mpX_LOGF, fstep);
	mpScaleY* yaxis = new mpScaleY(wxT("dB"), mpALIGN_LEFT, true);
	xaxis->SetFont(graphFont);
	xaxis->SetPen(textPen);
	xaxis->SetDrawOutsideMargins(false);

	yaxis->SetFont(graphFont);
	yaxis->SetPen(textPen);
	yaxis->SetDrawOutsideMargins(false);

	mRTAMagPLot->AddLayer(xaxis);
	mRTAMagPLot->AddLayer(yaxis);
	mRTAMagPLot->AddLayer(mFFTInMagLayer);

	mpInfoCoords *nfo;
	mRTAMagPLot->AddLayer(nfo = new mpInfoCoords(wxRect(300, 0, 160, 20), wxTRANSPARENT_BRUSH, fstep));

	ConfigureXAxis();

	((mpScaleXSpectrum*)(mRTAMagPLot->GetLayer(0)))->SetTicks(false);
	((mpScaleY*)(mRTAMagPLot->GetLayer(1)))->SetTicks(false);

	mRTAMagPLot->Refresh();
	mRTAMagPLot->Fit(0, mMaxXcord, -145, 6);

	mRTAMagPLot->EnableMousePanZoom(true);
	mRTAMagPLot->EnableDrawingMode(false);
	mRTAMagPLot->EnableXSelectMode(false);
}

void
AudioDevicesPanel::ConfigureXAxis()
{
	double xcord;
	mPlotXCoords.clear();
	mPlotXCoords.push_back(0);

	for (size_t i = 1; i < mRTAPlotSize; i++)
	{
		xcord = log10((double)i);
		mPlotXCoords.push_back((xcord));
	}

	mMaxXcord = xcord;

	mRTAMagPLot->SetLogarithmic(true);
	((mpScaleXSpectrum*)(mRTAMagPLot->GetLayer(0)))->SetLogarithmic(true);
	((mpInfoCoords*)(mRTAMagPLot->GetLayer(3)))->SetLogarithmic(true);
}

void AudioDevicesPanel::OnTimerAudioMonitorTrigger(wxTimerEvent& event)
{
	// input channels
	LevelAnalyser* lvIn = gAudioIO->GetInputsLevelAnalyser();
	if (lvIn)
	{
		bool nLInData = false;
		LevelMetrics lIn = lvIn->getMetrics(&nLInData);
		if (nLInData)
		{
			mVuMeterIn->SetValue(lIn);
			wxString textLevels;
			float peak = lIn.peak[mSelectedChannel];
			float rms = lIn.rms[mSelectedChannel];
			textLevels.Printf(wxT(" peak: %0.1f dB  \n rms:  %0.1fdB  "), peak, rms);
			StaticTextChPkLvl->SetLabel(textLevels);

			if ((peak >= -3.1) && (peak <= -2.9))
			{
				StaticTextChPkLvl->SetBackgroundColour(wxColour(200, 255, 200, 255));
				StaticTextCalibInstructions->SetBackgroundColour(wxColour(200, 255, 200, 255));
			}
			else if ((peak > -2.9))
			{
				StaticTextChPkLvl->SetBackgroundColour(wxColour(255, 200, 200, 255));
				StaticTextCalibInstructions->SetBackgroundColour(wxColour(255, 200, 200, 255));
			}
			else
			{
				StaticTextChPkLvl->SetBackgroundColour(wxColour(210, 210, 210, 255));
				StaticTextCalibInstructions->SetBackgroundColour(wxColour(210, 210, 210, 255));
			}

		}
	}

	// output channels
	LevelAnalyser* lvOut = gAudioIO->GetOutputsLevelAnalyser();
	if (lvOut)
	{
		bool nLOutData = false;
		LevelMetrics lOut = lvOut->getMetrics(&nLOutData);
		if (nLOutData)
		{
			mVuMeterOut->SetValue(lOut);
		}
	}

	//RTA
	bool newdata = false;
	FFTPlotData vData = gAudioIO->GetFFTPlotData(&newdata);
	if (newdata)
	{
		size_t vlen = vData.MagData.size();
		if ((vData.sampleRate != mSampleRate) || (vlen != mRTAPlotSize))
		{
			size_t RTALength = 2 * (vlen - 1);
			ConfigurePlot(RTALength, vData.sampleRate);
		}

		mFFTInMagLayer->SetData(mPlotXCoords, vData.MagData);
		mRTAMagPLot->Refresh();
	}
}
