////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////

#ifndef AUDIODEVICESPANEL_H
#define AUDIODEVICESPANEL_H

//(*Headers(AudioDevicesPanel)
#include "Widgets\wxMathPlot\mathplot.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include "Widgets/awohSpin.h"
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

		void RefreshUI();
		void ConfigureChannelSpin();

		void PopulateAll();
		void PopulateHostsChoices();
		void PopulateDevicesChoices();

		void UpdateSRateChoice();

		void EnableSelectionTools(bool enabled);

		void BuildTestUI();

		void StartCalibration();
		void StopCalibration();

		//(*Declarations(AudioDevicesPanel)
		RimPanel* PanelPBInR;
		wxPanel* PanelRecording;
		wxStaticText* StaticTextRTALength;
		wxPanel* PanelPlayback;
		wxStaticText* StaticTextDevTestTitle;
		GButton* ButtonOutGainDwn;
		awohSpin* SpinRefCh;
		GButton* ButtonDevTestStop;
		wxPanel* Panel1RecInR;
		wxStaticText* StaticTextRefCh;
		GButton* ButtonDevTestStart;
		awohSpin* SpinFFTLen;
		wxStaticText* StaticTextInHosts;
		wxStaticText* StaticTextCalibInstructions;
		wxStaticText* StaticTextChPkLvl;
		wxStaticText* StaticTextLineUp;
		wxStaticText* StaticTextGainTtl;
		RimPanel* PanelDevTest;
		wxTextCtrl* TextCtrlOutChannels;
		wxStaticText* StaticTextOutDevices;
		wxStaticText* StaticTextInDevices;
		RimPanel* PanelPBInL;
		wxTimer TimerAudioMonitor;
		wxStaticText* StaticTextSysSampleRate;
		wxStaticText* StaticTextOutGain;
		wxStaticText* StaticTextInChannels;
		wxStaticText* StaticTextPBStreamTTL;
		RimPanel* PanelHosts;
		RimPanel* PanelFFTCtrls;
		wxBoxSizer* BoxSizerRecInL;
		RimPanel* PanelSysGenSettings;
		wxChoice* ChoiceSystemSampleRate;
		RimPanel* PanelRecInL;
		wxBoxSizer* BoxSizerTest;
		wxBoxSizer* BoxSizerRecording;
		wxStaticText* StaticTextRecStreamTTL;
		GButton* ButtonPlotReset;
		GButton* ButtonOutGainUp;
		wxStaticText* StaticTextRTAAvg;
		wxStaticText* StaticTextOutChannels;
		RimPanel* PanelFFTPlot;
		mpWindow* mRTAMagPLot;
		wxChoice* ChoiceInputDevice;
		RimPanel* PanelChannelLevels;
		wxBoxSizer* BoxSizerPlayback;
		RimPanel* PanelOutputDevices;
		RimPanel* PanelInputDevices;
		GButton* ButtonScanAudioSys;
		wxBoxSizer* BoxSizerMonitor;
		wxChoice* ChoiceOutputDevice;
		wxTextCtrl* TextCtrlInChannels;
		GButton* ButtonResetLTA;
		awohSpin* SpinRTAvg;
		RimPanel* PanelTest;
		wxChoice* ChoiceHost;
		wxBoxSizer* BoxSizerPBInL;
		//*)

		VuMeter* mVuMeterIn;
		VuMeter* mVuMeterOut;

	protected:

		//(*Identifiers(AudioDevicesPanel)
		static const long ID_STATICTEXT_LINE_UP;
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
		static const long ID_STATICTEXT_CAL_INSTR;
		static const long ID_PANEL_DEV_TEST;
		static const long ID_STATICTEXT_PB_STREAM_TTL;
		static const long ID_PANEL_PB_IN_L;
		static const long ID_STATICTEXT_GAIN_TTL;
		static const long ID_STATICTEXT_GAIN;
		static const long ID_BUTTON_OUT_GAIN_UP;
		static const long ID_BUTTON_OUT_GAIN_DWN;
		static const long ID_PANEL_PB_IN_R;
		static const long ID_PANEL_PLAYBACK;
		static const long ID_STATICTEXT_REC_STREAM_TTL;
		static const long ID_PANEL_REC_IN_L;
		static const long ID_STATICTEXT_REF_CH;
		static const long ID_SPIN_REF_CH;
		static const long ID_STATICTEXT_RTA_LENGTH;
		static const long ID_SPIN_FFT_LENGTH;
		static const long ID_STATICTEXT_RTA_AVG;
		static const long ID_SPIN_RTA_AVG;
		static const long ID_BUTTON_RESET_LTA;
		static const long ID_BUTTON_PLOT_RESET;
		static const long ID_STATICTEXT_CH_PK_LVL;
		static const long ID_PANEL_CHANNEL_LVLS;
		static const long ID_PANEL_FFT_CTRLS;
		static const long ID_RTA_FFT_PLOT;
		static const long ID_PANEL_FFT_PLOT;
		static const long ID_PANEL_REC_IN_R;
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
		void OnButtonOutGainUpClick(wxCommandEvent& event);
		void OnButtonOutGainDwnClick(wxCommandEvent& event);
		void OnButtonResetLTAClick(wxCommandEvent& event);
		void OnButtonPlotResetClick(wxCommandEvent& event);
		//*)
		void OnSpinRTAvg(awohSpinEvent& event);
		void OnSpinFFTLength(awohSpinEvent& event);
		void OnSpinFFTReference(awohSpinEvent& event);

		void OnChoice(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()

		void HandleInputDevSelection(int selIdx);
		void HandleOutputDevSelection(int selIdx);

		void CheckCompatibleSampleRates();

		void UpdateOutputGain( bool write = false);
		void ConfigurePlot(size_t RTALength, double sampleRate);
		void ConfigureXAxis();

		bool mAudioTestStarted;
		std::vector<double> mSupportedInputSRates; 
		int mNumInputChannels;
		int mPrevNumInputChannels;
		std::vector<double> mSupportedOutputSRates; 
		int mNumOutputChannels;
		int mPrevNumOutputChannels;

		double mOutputStreamGain;
		bool mCtrlDwn;

		mpFXYVector* mFFTInMagLayer;
		std::vector<float> mPlotXCoords;
		float mMaxXcord;
		double mRTALength;
		double mRTAPlotSize;
		double mSampleRate;
		int mSelectedChannel;
};

#endif

