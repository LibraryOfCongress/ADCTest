#ifndef AUDIODEVICESPANEL_H
#define AUDIODEVICESPANEL_H

//(*Headers(AudioDevicesPanel)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/timer.h>
//*)

#include "Widgets/RimPanel.h"
#include "Widgets/GButton.h"
#include "Widgets/VuMeter.h"

class AudioDevicesPanel: public wxPanel
{
	public:

		AudioDevicesPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~AudioDevicesPanel();

		void PopulateAll();
		void PopulateHostsChoices();
		void PopulateInDevicesChoices();
		void PopulateOutDevicesChoices();
		void ShowInDevChannels();
		void ShowOutDevChannels();
		void UpdateSRateChoice();

		void EnableSelectionTools(bool enabled);

		void BuildTestUI();

		//(*Declarations(AudioDevicesPanel)
		wxPanel* PanelRecording;
		wxPanel* PanelPlayback;
		wxStaticText* StaticTextDevTestTitle;
		GButton* ButtonDevTestStop;
		wxStaticText* StaticTextGenSettingsTitle;
		GButton* ButtonDevTestStart;
		wxStaticText* StaticTextInHosts;
		wxStaticText* StaticTextLineUp;
		RimPanel* PanelDevTest;
		wxTextCtrl* TextCtrlOutChannels;
		wxStaticText* StaticTextOutDevices;
		wxStaticText* StaticTextInDevices;
		wxTimer TimerAudioMonitor;
		wxStaticText* StaticTextSysSampleRate;
		wxStaticText* StaticTextInChannels;
		RimPanel* PanelHosts;
		RimPanel* PanelSysGenSettings;
		wxChoice* ChoiceSystemSampleRate;
		wxBoxSizer* BoxSizerTest;
		wxBoxSizer* BoxSizerRecording;
		wxStaticText* StaticTextOutChannels;
		wxChoice* ChoiceInputDevice;
		wxBoxSizer* BoxSizerPlayback;
		RimPanel* PanelOutputDevices;
		RimPanel* PanelInputDevices;
		GButton* ButtonScanAudioSys;
		wxBoxSizer* BoxSizerMonitor;
		wxChoice* ChoiceOutputDevice;
		wxTextCtrl* TextCtrlInChannels;
		RimPanel* PanelTest;
		wxChoice* ChoiceHost;
		//*)

		VuMeter* mVuMeterIn;
		VuMeter* mVuMeterOut;

	protected:

		//(*Identifiers(AudioDevicesPanel)
		static const long ID_STATICTEXT_LINE_UP;
		static const long ID_STATICTEXT_GEN_SET_TTL;
		static const long ID_BUTTON_SCAN_AUDIO_SYS;
		static const long ID_PANEL_SYS_GEN_SET;
		static const long ID_STATICTEXT_IN_HOSTS;
		static const long ID_CHOICE_HOST;
		static const long ID_STATICTEXT_SYS_SRATE;
		static const long ID_CHOICE_SYS_SRATE;
		static const long ID_PANEL_HOSTS;
		static const long ID_STATICTEXT_IN_DEVS;
		static const long ID_CHOICE_INPUT_DEVICE;
		static const long ID_STATICTEXT_IN_CHANS;
		static const long ID_TEXTCTRL_IN_CHANS;
		static const long ID_PANEL_INPUT_DEVS;
		static const long ID_STATICTEXT_OUT_DEVS;
		static const long ID_CHOICE_OUTPUT_DEVICE;
		static const long ID_STATICTEXT_OUT_CHANS;
		static const long ID_TEXTCTRL_OUT_CHANS;
		static const long ID_PANEL_OUTPUT_DEVS;
		static const long ID_STATICTEXT_DEV_TST_TTL;
		static const long ID_BUTTON_TST_DEV_START;
		static const long ID_BUTTON_TST_DEV_STOP;
		static const long ID_PANEL_DEV_TEST;
		static const long ID_PANEL_PLAYBACK;
		static const long ID_PANEL_RECORDING;
		static const long ID_PANEL_TEST;
		static const long ID_TIMER_AUDIO_MONITOR;
		//*)
		static const long ID_VUMETER_IN;
		static const long ID_VUMETER_OUT;

	private:

		//(*Handlers(AudioDevicesPanel)
		void OnChoiceInputDeviceSelect(wxCommandEvent& event);
		void OnChoiceOutputDeviceSelect(wxCommandEvent& event);
		void OnChoiceSystemSampleRateSelect(wxCommandEvent& event);
		void OnButtonScanAudioSysClick(wxCommandEvent& event);
		void OnButtonDevTestStartClick(wxCommandEvent& event);
		void OnButtonDevTestStopClick(wxCommandEvent& event);
		void OnChoiceHostSelect(wxCommandEvent& event);
		void OnTimerAudioMonitorTrigger(wxTimerEvent& event);
		//*)

		DECLARE_EVENT_TABLE()

		bool mAudioTestStarted;
		int mNumInputChannels;
		int mPrevNumInputChannels;
		int mNumOutputChannels;
		int mPrevNumOutputChannels;
};

#endif
