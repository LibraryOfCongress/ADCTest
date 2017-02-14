#include "AudioDevicesPanel.h"
#include "Devices\ADevicesManager.h"
#include "System\Prefs.h"
#include "AudioIO\AudioEngine.h"
#include "AudioIO\ProcessParams.h"

//(*InternalHeaders(AudioDevicesPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(AudioDevicesPanel)
const long AudioDevicesPanel::ID_STATICTEXT_LINE_UP = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_GEN_SET_TTL = wxNewId();
const long AudioDevicesPanel::ID_BUTTON_SCAN_AUDIO_SYS = wxNewId();
const long AudioDevicesPanel::ID_PANEL_SYS_GEN_SET = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_IN_HOSTS = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_HOST = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_SYS_SRATE = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_SYS_SRATE = wxNewId();
const long AudioDevicesPanel::ID_PANEL_HOSTS = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_IN_DEVS = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_INPUT_DEVICE = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_IN_CHANS = wxNewId();
const long AudioDevicesPanel::ID_TEXTCTRL_IN_CHANS = wxNewId();
const long AudioDevicesPanel::ID_PANEL_INPUT_DEVS = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_OUT_DEVS = wxNewId();
const long AudioDevicesPanel::ID_CHOICE_OUTPUT_DEVICE = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_OUT_CHANS = wxNewId();
const long AudioDevicesPanel::ID_TEXTCTRL_OUT_CHANS = wxNewId();
const long AudioDevicesPanel::ID_PANEL_OUTPUT_DEVS = wxNewId();
const long AudioDevicesPanel::ID_STATICTEXT_DEV_TST_TTL = wxNewId();
const long AudioDevicesPanel::ID_BUTTON_TST_DEV_START = wxNewId();
const long AudioDevicesPanel::ID_BUTTON_TST_DEV_STOP = wxNewId();
const long AudioDevicesPanel::ID_PANEL_DEV_TEST = wxNewId();
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
const long AudioDevicesPanel::ID_PANEL_REC_IN_R = wxNewId();
const long AudioDevicesPanel::ID_PANEL_RECORDING = wxNewId();
const long AudioDevicesPanel::ID_PANEL_TEST = wxNewId();
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
	wxBoxSizer* BoxSizerOutputDevice;
	wxBoxSizer* BoxSizerInputDevices;
	wxBoxSizer* BoxSizerOutputDevices;
	wxBoxSizer* BoxSizerSRateInner;
	wxBoxSizer* BoxSizerRecTTL;
	wxBoxSizer* BoxSizerPBInR;
	wxBoxSizer* BoxSizerInputDevice;
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* BoxSizerHosts;
	wxBoxSizer* BoxSizerPBTTL;
	wxBoxSizer* BoxSizerInputChannels;
	wxBoxSizer* BoxSizerOutputChannels;
	wxBoxSizer* BoxSizerRecInR;
	wxBoxSizer* BoxSizerGenSettings;
	wxBoxSizer* BoxSizerDevTest;
	wxBoxSizer* BoxSizerGainBtns;
	wxBoxSizer* BoxSizerBtnsLay;
	wxBoxSizer* BoxSizerRescan;
	wxBoxSizer* BoxSizerGainTtl;
	wxBoxSizer* BoxSizerTestStart;
	wxBoxSizer* BoxSizerHostAPIIn;
	wxBoxSizer* BoxSizerDevicesSetttingsTitle;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetForegroundColour(wxColour(0,0,0));
	SetBackgroundColour(wxColour(220,220,220));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	StaticTextLineUp = new wxStaticText(this, ID_STATICTEXT_LINE_UP, wxEmptyString, wxDefaultPosition, wxSize(-1,2), wxNO_BORDER, _T("ID_STATICTEXT_LINE_UP"));
	StaticTextLineUp->SetBackgroundColour(wxColour(180,180,180));
	BoxSizerMain->Add(StaticTextLineUp, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizerDevicesSetttingsTitle = new wxBoxSizer(wxHORIZONTAL);
	StaticTextGenSettingsTitle = new wxStaticText(this, ID_STATICTEXT_GEN_SET_TTL, _("audio devices settings"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_GEN_SET_TTL"));
	BoxSizerDevicesSetttingsTitle->Add(StaticTextGenSettingsTitle, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 2);
	BoxSizerMain->Add(BoxSizerDevicesSetttingsTitle, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelSysGenSettings = new RimPanel(this, ID_PANEL_SYS_GEN_SET, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_SYS_GEN_SET"));
	BoxSizerGenSettings = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerRescan = new wxBoxSizer(wxVERTICAL);
	ButtonScanAudioSys = new GButton(PanelSysGenSettings, ID_BUTTON_SCAN_AUDIO_SYS, _("Scan system"), wxDefaultPosition, wxSize(200,22), 0, wxDefaultValidator, _T("ID_BUTTON_SCAN_AUDIO_SYS"));
	BoxSizerRescan->Add(ButtonScanAudioSys, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 4);
	BoxSizerGenSettings->Add(BoxSizerRescan, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
	PanelSysGenSettings->SetSizer(BoxSizerGenSettings);
	BoxSizerGenSettings->Fit(PanelSysGenSettings);
	BoxSizerGenSettings->SetSizeHints(PanelSysGenSettings);
	BoxSizerMain->Add(PanelSysGenSettings, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelHosts = new RimPanel(this, ID_PANEL_HOSTS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_HOSTS"));
	BoxSizerHosts = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerHostAPIIn = new wxBoxSizer(wxVERTICAL);
	StaticTextInHosts = new wxStaticText(PanelHosts, ID_STATICTEXT_IN_HOSTS, _("host API"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_IN_HOSTS"));
	BoxSizerHostAPIIn->Add(StaticTextInHosts, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 4);
	ChoiceHost = new wxChoice(PanelHosts, ID_CHOICE_HOST, wxDefaultPosition, wxSize(200,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_HOST"));
	BoxSizerHostAPIIn->Add(ChoiceHost, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerHosts->Add(BoxSizerHostAPIIn, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 1);
	BoxSizerSRateInner = new wxBoxSizer(wxVERTICAL);
	StaticTextSysSampleRate = new wxStaticText(PanelHosts, ID_STATICTEXT_SYS_SRATE, _("sample rate"), wxDefaultPosition, wxSize(200,-1), 0, _T("ID_STATICTEXT_SYS_SRATE"));
	BoxSizerSRateInner->Add(StaticTextSysSampleRate, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 4);
	ChoiceSystemSampleRate = new wxChoice(PanelHosts, ID_CHOICE_SYS_SRATE, wxDefaultPosition, wxSize(200,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_SYS_SRATE"));
	BoxSizerSRateInner->Add(ChoiceSystemSampleRate, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerHosts->Add(BoxSizerSRateInner, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	PanelHosts->SetSizer(BoxSizerHosts);
	BoxSizerHosts->Fit(PanelHosts);
	BoxSizerHosts->SetSizeHints(PanelHosts);
	BoxSizerMain->Add(PanelHosts, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelInputDevices = new RimPanel(this, ID_PANEL_INPUT_DEVS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_INPUT_DEVS"));
	BoxSizerInputDevices = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerInputDevice = new wxBoxSizer(wxVERTICAL);
	StaticTextInDevices = new wxStaticText(PanelInputDevices, ID_STATICTEXT_IN_DEVS, _("input devices"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_IN_DEVS"));
	BoxSizerInputDevice->Add(StaticTextInDevices, 0, wxLEFT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 4);
	ChoiceInputDevice = new wxChoice(PanelInputDevices, ID_CHOICE_INPUT_DEVICE, wxDefaultPosition, wxSize(300,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_INPUT_DEVICE"));
	BoxSizerInputDevice->Add(ChoiceInputDevice, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerInputDevices->Add(BoxSizerInputDevice, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 1);
	BoxSizerInputChannels = new wxBoxSizer(wxVERTICAL);
	StaticTextInChannels = new wxStaticText(PanelInputDevices, ID_STATICTEXT_IN_CHANS, _("audio channels"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_IN_CHANS"));
	BoxSizerInputChannels->Add(StaticTextInChannels, 0, wxLEFT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 4);
	TextCtrlInChannels = new wxTextCtrl(PanelInputDevices, ID_TEXTCTRL_IN_CHANS, _("0"), wxDefaultPosition, wxSize(75,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL_IN_CHANS"));
	TextCtrlInChannels->SetBackgroundColour(wxColour(220,220,220));
	BoxSizerInputChannels->Add(TextCtrlInChannels, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerInputDevices->Add(BoxSizerInputChannels, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
	BoxSizerInputDevices->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	PanelInputDevices->SetSizer(BoxSizerInputDevices);
	BoxSizerInputDevices->Fit(PanelInputDevices);
	BoxSizerInputDevices->SetSizeHints(PanelInputDevices);
	BoxSizerMain->Add(PanelInputDevices, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelOutputDevices = new RimPanel(this, ID_PANEL_OUTPUT_DEVS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_OUTPUT_DEVS"));
	BoxSizerOutputDevices = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerOutputDevice = new wxBoxSizer(wxVERTICAL);
	StaticTextOutDevices = new wxStaticText(PanelOutputDevices, ID_STATICTEXT_OUT_DEVS, _("output devices"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_OUT_DEVS"));
	BoxSizerOutputDevice->Add(StaticTextOutDevices, 0, wxLEFT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 4);
	ChoiceOutputDevice = new wxChoice(PanelOutputDevices, ID_CHOICE_OUTPUT_DEVICE, wxDefaultPosition, wxSize(300,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_OUTPUT_DEVICE"));
	BoxSizerOutputDevice->Add(ChoiceOutputDevice, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerOutputDevices->Add(BoxSizerOutputDevice, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 1);
	BoxSizerOutputChannels = new wxBoxSizer(wxVERTICAL);
	StaticTextOutChannels = new wxStaticText(PanelOutputDevices, ID_STATICTEXT_OUT_CHANS, _("audio channels"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_OUT_CHANS"));
	BoxSizerOutputChannels->Add(StaticTextOutChannels, 0, wxLEFT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 4);
	TextCtrlOutChannels = new wxTextCtrl(PanelOutputDevices, ID_TEXTCTRL_OUT_CHANS, _("0"), wxDefaultPosition, wxSize(75,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL_OUT_CHANS"));
	TextCtrlOutChannels->SetBackgroundColour(wxColour(220,220,220));
	BoxSizerOutputChannels->Add(TextCtrlOutChannels, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerOutputDevices->Add(BoxSizerOutputChannels, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 1);
	BoxSizerOutputDevices->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	PanelOutputDevices->SetSizer(BoxSizerOutputDevices);
	BoxSizerOutputDevices->Fit(PanelOutputDevices);
	BoxSizerOutputDevices->SetSizeHints(PanelOutputDevices);
	BoxSizerMain->Add(PanelOutputDevices, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelTest = new RimPanel(this, ID_PANEL_TEST, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_TEST"));
	BoxSizerTest = new wxBoxSizer(wxVERTICAL);
	StaticTextDevTestTitle = new wxStaticText(PanelTest, ID_STATICTEXT_DEV_TST_TTL, _("Audio devices test"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_DEV_TST_TTL"));
	BoxSizerTest->Add(StaticTextDevTestTitle, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerTestStart = new wxBoxSizer(wxHORIZONTAL);
	PanelDevTest = new RimPanel(PanelTest, ID_PANEL_DEV_TEST, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_FULL_REPAINT_ON_RESIZE, _T("ID_PANEL_DEV_TEST"));
	BoxSizerDevTest = new wxBoxSizer(wxHORIZONTAL);
	ButtonDevTestStart = new GButton(PanelDevTest, ID_BUTTON_TST_DEV_START, _("Start test"), wxDefaultPosition, wxSize(100,22), 0, wxDefaultValidator, _T("ID_BUTTON_TST_DEV_START"));
	BoxSizerDevTest->Add(ButtonDevTestStart, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	ButtonDevTestStop = new GButton(PanelDevTest, ID_BUTTON_TST_DEV_STOP, _("Stop test"), wxDefaultPosition, wxSize(100,22), 0, wxDefaultValidator, _T("ID_BUTTON_TST_DEV_STOP"));
	BoxSizerDevTest->Add(ButtonDevTestStop, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerDevTest->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelDevTest->SetSizer(BoxSizerDevTest);
	BoxSizerDevTest->Fit(PanelDevTest);
	BoxSizerDevTest->SetSizeHints(PanelDevTest);
	BoxSizerTestStart->Add(PanelDevTest, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerTest->Add(BoxSizerTestStart, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerMonitor = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerMonitor->Add(1,200,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	PanelPlayback = new wxPanel(PanelTest, ID_PANEL_PLAYBACK, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL|wxNO_FULL_REPAINT_ON_RESIZE, _T("ID_PANEL_PLAYBACK"));
	BoxSizerPlayback = new wxBoxSizer(wxVERTICAL);
	StaticTextPBStreamTTL = new wxStaticText(PanelPlayback, ID_STATICTEXT_PB_STREAM_TTL, _("output stream"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_PB_STREAM_TTL"));
	BoxSizerPlayback->Add(StaticTextPBStreamTTL, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerPBTTL = new wxBoxSizer(wxHORIZONTAL);
	PanelPBInL = new RimPanel(PanelPlayback, ID_PANEL_PB_IN_L, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_FULL_REPAINT_ON_RESIZE, _T("ID_PANEL_PB_IN_L"));
	BoxSizerPBInL = new wxBoxSizer(wxHORIZONTAL);
	PanelPBInL->SetSizer(BoxSizerPBInL);
	BoxSizerPBInL->Fit(PanelPBInL);
	BoxSizerPBInL->SetSizeHints(PanelPBInL);
	BoxSizerPBTTL->Add(PanelPBInL, 1, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 3);
	PanelPBInR = new RimPanel(PanelPlayback, ID_PANEL_PB_IN_R, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PB_IN_R"));
	BoxSizerPBInR = new wxBoxSizer(wxVERTICAL);
	BoxSizerGainTtl = new wxBoxSizer(wxHORIZONTAL);
	StaticTextGainTtl = new wxStaticText(PanelPBInR, ID_STATICTEXT_GAIN_TTL, _("gain"), wxDefaultPosition, wxSize(50,18), wxALIGN_CENTRE, _T("ID_STATICTEXT_GAIN_TTL"));
	BoxSizerGainTtl->Add(StaticTextGainTtl, 1, wxBOTTOM|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerPBInR->Add(BoxSizerGainTtl, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerBtnsLay = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerBtnsLay->Add(1,-1,1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerGainBtns = new wxBoxSizer(wxVERTICAL);
	StaticTextOutGain = new wxStaticText(PanelPBInR, ID_STATICTEXT_GAIN, _("0 dB"), wxDefaultPosition, wxSize(50,18), wxALIGN_CENTRE, _T("ID_STATICTEXT_GAIN"));
	BoxSizerGainBtns->Add(StaticTextOutGain, 1, wxTOP|wxBOTTOM|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	ButtonOutGainUp = new GButton(PanelPBInR, ID_BUTTON_OUT_GAIN_UP, _("+"), wxDefaultPosition, wxSize(50,40), 0, wxDefaultValidator, _T("ID_BUTTON_OUT_GAIN_UP"));
	BoxSizerGainBtns->Add(ButtonOutGainUp, 0, wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP, 5);
	ButtonOutGainDwn = new GButton(PanelPBInR, ID_BUTTON_OUT_GAIN_DWN, _("-"), wxDefaultPosition, wxSize(50,40), 0, wxDefaultValidator, _T("ID_BUTTON_OUT_GAIN_DWN"));
	BoxSizerGainBtns->Add(ButtonOutGainDwn, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerBtnsLay->Add(BoxSizerGainBtns, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizerBtnsLay->Add(1,-1,1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizerPBInR->Add(BoxSizerBtnsLay, 0, wxBOTTOM|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelPBInR->SetSizer(BoxSizerPBInR);
	BoxSizerPBInR->Fit(PanelPBInR);
	BoxSizerPBInR->SetSizeHints(PanelPBInR);
	BoxSizerPBTTL->Add(PanelPBInR, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 3);
	BoxSizerPlayback->Add(BoxSizerPBTTL, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	PanelPlayback->SetSizer(BoxSizerPlayback);
	BoxSizerPlayback->Fit(PanelPlayback);
	BoxSizerPlayback->SetSizeHints(PanelPlayback);
	BoxSizerMonitor->Add(PanelPlayback, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelRecording = new wxPanel(PanelTest, ID_PANEL_RECORDING, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL|wxNO_FULL_REPAINT_ON_RESIZE, _T("ID_PANEL_RECORDING"));
	BoxSizerRecording = new wxBoxSizer(wxVERTICAL);
	StaticTextRecStreamTTL = new wxStaticText(PanelRecording, ID_STATICTEXT_REC_STREAM_TTL, _("input stream"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_REC_STREAM_TTL"));
	BoxSizerRecording->Add(StaticTextRecStreamTTL, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerRecTTL = new wxBoxSizer(wxHORIZONTAL);
	PanelRecInL = new RimPanel(PanelRecording, ID_PANEL_REC_IN_L, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_FULL_REPAINT_ON_RESIZE|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL_REC_IN_L"));
	BoxSizerRecInL = new wxBoxSizer(wxHORIZONTAL);
	PanelRecInL->SetSizer(BoxSizerRecInL);
	BoxSizerRecInL->Fit(PanelRecInL);
	BoxSizerRecInL->SetSizeHints(PanelRecInL);
	BoxSizerRecTTL->Add(PanelRecInL, 1, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 3);
	Panel1RecInR = new RimPanel(PanelRecording, ID_PANEL_REC_IN_R, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_REC_IN_R"));
	BoxSizerRecInR = new wxBoxSizer(wxHORIZONTAL);
	Panel1RecInR->SetSizer(BoxSizerRecInR);
	BoxSizerRecInR->Fit(Panel1RecInR);
	BoxSizerRecInR->SetSizeHints(Panel1RecInR);
	BoxSizerRecTTL->Add(Panel1RecInR, 5, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 3);
	BoxSizerRecording->Add(BoxSizerRecTTL, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	PanelRecording->SetSizer(BoxSizerRecording);
	BoxSizerRecording->Fit(PanelRecording);
	BoxSizerRecording->SetSizeHints(PanelRecording);
	BoxSizerMonitor->Add(PanelRecording, 3, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizerTest->Add(BoxSizerMonitor, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	PanelTest->SetSizer(BoxSizerTest);
	BoxSizerTest->Fit(PanelTest);
	BoxSizerTest->SetSizeHints(PanelTest);
	BoxSizerMain->Add(PanelTest, 2, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SetSizer(BoxSizerMain);
	TimerAudioMonitor.SetOwner(this, ID_TIMER_AUDIO_MONITOR);
	TimerAudioMonitor.Start(50, false);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);

	Connect(ID_BUTTON_SCAN_AUDIO_SYS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonScanAudioSysClick);
	Connect(ID_CHOICE_HOST,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceHostSelect);
	Connect(ID_CHOICE_SYS_SRATE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceSystemSampleRateSelect);
	Connect(ID_CHOICE_INPUT_DEVICE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceInputDeviceSelect);
	Connect(ID_CHOICE_OUTPUT_DEVICE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&AudioDevicesPanel::OnChoiceOutputDeviceSelect);
	Connect(ID_BUTTON_TST_DEV_START,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonDevTestStartClick);
	Connect(ID_BUTTON_TST_DEV_STOP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonDevTestStopClick);
	Connect(ID_BUTTON_OUT_GAIN_UP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonOutGainUpClick);
	Connect(ID_BUTTON_OUT_GAIN_DWN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AudioDevicesPanel::OnButtonOutGainDwnClick);
	Connect(ID_TIMER_AUDIO_MONITOR,wxEVT_TIMER,(wxObjectEventFunction)&AudioDevicesPanel::OnTimerAudioMonitorTrigger);
	//*)

	TimerAudioMonitor.Stop();

	Connect(ID_BUTTON_SCAN_AUDIO_SYS, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonScanAudioSysClick);
    Connect(ID_BUTTON_TST_DEV_START, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonDevTestStartClick);
	Connect(ID_BUTTON_TST_DEV_STOP, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonDevTestStopClick);
	Connect(ID_BUTTON_OUT_GAIN_UP, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonOutGainUpClick);
	Connect(ID_BUTTON_OUT_GAIN_DWN, wxEVT_COMMAND_GBUTTON, (wxObjectEventFunction)&AudioDevicesPanel::OnButtonOutGainDwnClick);

	mNumInputChannels = -1;
	mPrevNumInputChannels = -1;
	mVuMeterIn = NULL;
	mNumOutputChannels = -1;
	mPrevNumOutputChannels = -1;
	mVuMeterOut = NULL;

	ButtonDevTestStop->enable(false);
	ButtonDevTestStart->enable(true);

	PopulateAll();

	//load output gain
	gPrefs->Read(wxT("/Parameters/OutputStreamGain"), &mOutputStreamGain);
	UpdateOutputGain();
}

AudioDevicesPanel::~AudioDevicesPanel()
{
	//(*Destroy(AudioDevicesPanel)
	//*)
}

void AudioDevicesPanel::PopulateAll()
{
	PopulateHostsChoices();

	PopulateInDevicesChoices();

	PopulateOutDevicesChoices();

	ShowInDevChannels();

	ShowOutDevChannels();

}

void AudioDevicesPanel::PopulateHostsChoices()
{
	const std::vector<ADeviceMap> &inMaps = ADevicesManager::Instance()->GetInputDeviceMaps();

	//fill hosts combo
	wxArrayString hostsIn;
	ChoiceHost->Clear();

	// go over our lists add the host to the list if it isn't there yet
	for (size_t j = 0; j < inMaps.size(); j++)
		if (hostsIn.Index(inMaps[j].hostString) == wxNOT_FOUND)
			hostsIn.Add(inMaps[j].hostString);

	ChoiceHost->Clear();
	ChoiceHost->Append(hostsIn);

	if (hostsIn.GetCount() == 0)
	{
		ChoiceHost->Enable(false);
		return;
	}


	//see if the preferred host api is in the list
	wxString prefInHost = gPrefs->Read(wxT("/AudioIO/InputHostName"), wxT(""));
	int itemIdx = ChoiceHost->FindString(prefInHost);

	if (itemIdx != wxNOT_FOUND)
		ChoiceHost->SetSelection(itemIdx);
	else
	{
		ChoiceHost->SetSelection(-1);
	}
}

void AudioDevicesPanel::PopulateInDevicesChoices()
{
	const std::vector<ADeviceMap> &inMaps  = ADevicesManager::Instance()->GetInputDeviceMaps();

	//read what is the current selected host
	int hostSelectionIndex = ChoiceHost->GetCurrentSelection();

	if (hostSelectionIndex < 0)
		return;

	wxString hostIn = ChoiceHost->GetString(hostSelectionIndex);

	// Make sure in dev list is clear in case no host was found
	ChoiceInputDevice->Clear();

	// Repopulate the Input device list available to the user
	for (size_t i = 0; i < inMaps.size(); i++)
	{
		if (hostIn == inMaps[i].hostString)
		{
			ChoiceInputDevice->Append(inMaps[i].deviceString);
		}
	}

	ChoiceInputDevice->Enable(ChoiceInputDevice->GetCount() ? true : false);

	//see if the preferred device is in the list
	wxString prefInDev = gPrefs->Read(wxT("/AudioIO/InputDevName"), wxT(""));
	int itemIdx = ChoiceInputDevice->FindString(prefInDev);

	if (itemIdx != wxNOT_FOUND)
		ChoiceInputDevice->SetSelection(itemIdx);
	else
	{
		ChoiceInputDevice->SetSelection(0);
		gPrefs->Write(wxT("/AudioIO/InputDevName"), ChoiceInputDevice->GetString(0) );
	}


	UpdateSRateChoice();
}

void AudioDevicesPanel::PopulateOutDevicesChoices()
{
	const std::vector<ADeviceMap> &outMaps = ADevicesManager::Instance()->GetOutputDeviceMaps();

	//read what is the current selected host
	int hostSelectionIndex = ChoiceHost->GetCurrentSelection();

	if (hostSelectionIndex < 0)
		return;

	wxString host = ChoiceHost->GetString(hostSelectionIndex);

	// Make sure in dev list is clear in case no host was found
	ChoiceOutputDevice->Clear();

	// Repopulate the Input device list available to the user
	for (size_t i = 0; i < outMaps.size(); i++)
	{
		if (host == outMaps[i].hostString)
		{
			ChoiceOutputDevice->Append(outMaps[i].deviceString);
		}
	}

	ChoiceOutputDevice->Enable(ChoiceOutputDevice->GetCount() ? true : false);

	//see if the preferred device is in the list
	wxString prefOutDev = gPrefs->Read(wxT("/AudioIO/OutputDevName"), wxT(""));
	int itemIdx = ChoiceOutputDevice->FindString(prefOutDev);

	if (itemIdx != wxNOT_FOUND)
		ChoiceOutputDevice->SetSelection(itemIdx);
	else
	{
		ChoiceOutputDevice->SetSelection(0);
		gPrefs->Write(wxT("/AudioIO/OutputDevName"), ChoiceOutputDevice->GetString(0));
	}

	UpdateSRateChoice();
}

void AudioDevicesPanel::OnChoiceInputDeviceSelect(wxCommandEvent& event)
{
	int devSelectionIndex;
	devSelectionIndex = ChoiceInputDevice->GetSelection();

	wxString newDevice = ChoiceInputDevice->GetString(devSelectionIndex);

	//change the host and switch to correct devices.
	gPrefs->Write(wxT("/AudioIO/InputDevName"), newDevice);
	gPrefs->Flush();

	ShowInDevChannels();
}

void AudioDevicesPanel::OnChoiceOutputDeviceSelect(wxCommandEvent& event)
{
	int devSelectionIndex;
	devSelectionIndex = ChoiceOutputDevice->GetSelection();

	wxString newDevice = ChoiceOutputDevice->GetString(devSelectionIndex);

	//change the host and switch to correct devices.
	gPrefs->Write(wxT("/AudioIO/OutputDevName"), newDevice);
	gPrefs->Flush();

	ShowOutDevChannels();
}

void AudioDevicesPanel::ShowInDevChannels()
{
	const std::vector<ADeviceMap> &inMaps = ADevicesManager::Instance()->GetInputDeviceMaps();

	//read what is the current selected host
	int devSelectionIndex = ChoiceInputDevice->GetCurrentSelection();

	if (devSelectionIndex < 0)
		return;

	wxString devIn = ChoiceInputDevice->GetString(devSelectionIndex);

	// Repopulate the Input device list available to the user
	int nCh = -1;
	for (size_t i = 0; i < inMaps.size(); i++)
	{
		if (devIn == inMaps[i].deviceString)
		{
			nCh = inMaps[i].numChannels;
		}
	}

	wxString nChTxt;
	nChTxt.Printf(wxT("%d"), nCh);
	TextCtrlInChannels->SetValue(nChTxt);
	gPrefs->Write(wxT("/AudioIO/InputDevChans"), nCh);
	gPrefs->Flush();

	mNumInputChannels = nCh;
	BuildTestUI();
}

void AudioDevicesPanel::ShowOutDevChannels()
{
	const std::vector<ADeviceMap> &outMaps = ADevicesManager::Instance()->GetOutputDeviceMaps();

	//read what is the current selected host
	int devSelectionIndex = ChoiceOutputDevice->GetCurrentSelection();

	if (devSelectionIndex < 0)
		return;

	wxString devOut = ChoiceOutputDevice->GetString(devSelectionIndex);

	// Repopulate the Input device list available to the user
	int nCh = -1;
	for (size_t i = 0; i < outMaps.size(); i++)
	{
		if (devOut == outMaps[i].deviceString)
		{
			nCh = outMaps[i].numChannels;
		}
	}

	wxString nChTxt;
	nChTxt.Printf(wxT("%d"), nCh);
	TextCtrlOutChannels->SetValue(nChTxt);
	gPrefs->Write(wxT("/AudioIO/OutputDevChans"), nCh);
	gPrefs->Flush();

	mNumOutputChannels = nCh;
	BuildTestUI();
}

void AudioDevicesPanel::OnButtonScanAudioSysClick(wxCommandEvent& event)
{
	ADevicesManager::Instance()->Rescan();
	PopulateAll();
}

void AudioDevicesPanel::UpdateSRateChoice()
{
	int hostSelectionIndex;
	int devSelectionIndex;

	const std::vector<ADeviceMap> &inMaps = ADevicesManager::Instance()->GetInputDeviceMaps();
	hostSelectionIndex = ChoiceHost->GetCurrentSelection();
	wxString hostIn = ChoiceHost->GetString(hostSelectionIndex);
	devSelectionIndex = ChoiceInputDevice->GetCurrentSelection();
	wxString devIn = ChoiceInputDevice->GetString(devSelectionIndex);
	ADeviceMap iMap;
	for (size_t i = 0; i < inMaps.size(); i++)
	{
		if ( (hostIn == inMaps[i].hostString) && (devIn == inMaps[i].deviceString) )
		{
			iMap = inMaps[i];
			break;
		}
	}
	std::vector<double> inputSRates = iMap.supportedRates;

	/////////////////////////////////////////////////////////////
	const std::vector<ADeviceMap> &outMaps = ADevicesManager::Instance()->GetOutputDeviceMaps();
	devSelectionIndex = ChoiceOutputDevice->GetCurrentSelection();
	wxString devOut = ChoiceOutputDevice->GetString(devSelectionIndex);
	ADeviceMap oMap;
	for (size_t i = 0; i < outMaps.size(); i++)
	{
		if (devOut == outMaps[i].deviceString)
		{
			oMap = outMaps[i];
			break;
		}
	}
	std::vector<double> outputSRates = oMap.supportedRates;

	/////////////////////////////////////////////////////////////////////////////////////////
	//populate the choice widget.

	wxArrayString SRArray;

	for (size_t i = 0; i < inputSRates.size(); i++)
	{
		wxString usr;
		usr.Printf(wxT("%g"), inputSRates[i]);
		SRArray.Add(usr);
	}

	ChoiceSystemSampleRate->Clear();
	ChoiceSystemSampleRate->Append(SRArray);

	//see if the preferred sample rate  is in the list
	wxString prefSRate = gPrefs->Read(wxT("/AudioIO/InputDevSRate"), wxT(""));
	int itemIdx = ChoiceSystemSampleRate->FindString(prefSRate);

	if (itemIdx != wxNOT_FOUND)
		ChoiceSystemSampleRate->SetSelection(itemIdx);
	else
	{
		ChoiceSystemSampleRate->SetSelection(0);
		gPrefs->Write(wxT("/AudioIO/InputDevSRate"), ChoiceSystemSampleRate->GetString(0));
	}
}

void AudioDevicesPanel::OnChoiceSystemSampleRateSelect(wxCommandEvent& event)
{
	int SRateSelectionIndex = ChoiceSystemSampleRate->GetSelection();

	wxString newSRate = ChoiceSystemSampleRate->GetString(SRateSelectionIndex);

	//change the host and switch to correct devices.
	gPrefs->Write(wxT("/AudioIO/InputDevSRate"), newSRate);
	gPrefs->Flush();

	UpdateSRateChoice();
}

void AudioDevicesPanel::OnButtonDevTestStartClick(wxCommandEvent& event)
{
    mAudioTestStarted = true;
    ButtonDevTestStop->enable(true);
    ButtonDevTestStart->enable(false);
	gAudioIO->StartDevicesTest();
	EnableSelectionTools(false);
	TimerAudioMonitor.Start(50, false);
}

void AudioDevicesPanel::OnButtonDevTestStopClick(wxCommandEvent& event)
{
	TimerAudioMonitor.Stop();
	mVuMeterIn->Reset();
	mVuMeterOut->Reset();
    mAudioTestStarted = false;
    ButtonDevTestStop->enable(false);
    ButtonDevTestStart->enable(true);
	gAudioIO->StopDevicesTest();
	EnableSelectionTools(true);
}

void AudioDevicesPanel::OnChoiceHostSelect(wxCommandEvent& event)
{
	int hostSelectionIndex;
	hostSelectionIndex = ChoiceHost->GetSelection();
	wxString newHost = ChoiceHost->GetString(hostSelectionIndex);

	//change the host and switch to correct devices.
	gPrefs->Write(wxT("/AudioIO/InputHostName"), newHost);
	gPrefs->Write(wxT("/AudioIO/OutputHostName"), newHost);
	gPrefs->Flush();

	// populate the devices
	PopulateInDevicesChoices();
	PopulateOutDevicesChoices();
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
			mVuMeterOut->SetBackColour(wxColour( 215, 215 , 215));
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
	BoxSizerTest->Layout();
}

void AudioDevicesPanel::EnableSelectionTools(bool enabled)
{
	ChoiceHost->Enable(enabled);
	ChoiceInputDevice->Enable(enabled);
	ChoiceOutputDevice->Enable(enabled);
	ChoiceSystemSampleRate->Enable(enabled);
	TextCtrlInChannels->Enable(enabled);
	TextCtrlOutChannels->Enable(enabled);
	ButtonScanAudioSys->enable(enabled);
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
}

void AudioDevicesPanel::OnButtonOutGainUpClick(wxCommandEvent& event)
{
	mCtrlDwn = wxGetKeyState(WXK_CONTROL);
	
	if (mCtrlDwn)
		mOutputStreamGain += 1;
	else
		mOutputStreamGain += 0.1;

	UpdateOutputGain( true );
}

void AudioDevicesPanel::OnButtonOutGainDwnClick(wxCommandEvent& event)
{
	mCtrlDwn = wxGetKeyState(WXK_CONTROL);

	if (mCtrlDwn)
		mOutputStreamGain -= 1;
	else
		mOutputStreamGain -= 0.1;

	UpdateOutputGain( true );
}

void AudioDevicesPanel::UpdateOutputGain(bool write)
{
	wxString gnStr;
	gnStr.Printf(wxT("%g dB"), mOutputStreamGain);
	StaticTextOutGain->SetLabel(gnStr);
	
	//send to audio engine
	AudioParam msg;
	msg.paramIdx = kOutputGain;
	msg.value = mOutputStreamGain;
	gAudioIO->SetParameter(msg, false);

	if (write)
	{
		gPrefs->Write(wxT("/Parameters/OutputStreamGain"), mOutputStreamGain);
		gPrefs->Flush();
	}
}