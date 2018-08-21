#ifndef AUDIODEVICESPANEL_H
#define AUDIODEVICESPANEL_H

//(*Headers(AudioDevicesPanel)
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)
#include "Widgets/RimPanel.h"
#include "Widgets/awohSpin.h"
#include "Widgets/GButton.h"
#include "Widgets/VuMeter.h"
#include "Widgets/wxMathPlot/mathplot.h"

class AudioDevicesPanel: public wxPanel
{
	public:

		AudioDevicesPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~AudioDevicesPanel();

		void RefreshUI();
		void ConfigureChannelSpin();

		void PopulateAll();
		void PopulateHostsChoices();
		void PopulateDevicesChoices(bool handleChanges = true);

		void UpdateInputSRateChoice();
		void UpdateOutputSRateChoice();

		void PopulateInputFrameLenChoice();
		void PopulateOutputFrameLenChoice();

		void EnableSelectionTools(bool enabled);

		void BuildTestUI();

		void StartCalibration();
		void StopCalibration();

		//(*Declarations(AudioDevicesPanel)
		GButton* ButtonDevTestStart;
		GButton* ButtonDevTestStop;
		GButton* ButtonOutGainDwn;
		GButton* ButtonOutGainUp;
		GButton* ButtonPlotReset;
		GButton* ButtonResetLTA;
		GButton* ButtonScanAudioSys;
		RimPanel* PanelChannelLevels;
		RimPanel* PanelDTestGrp;
		RimPanel* PanelFFTCtrls;
		RimPanel* PanelFFTPlot;
		RimPanel* PanelInputs;
		RimPanel* PanelOutputs;
		RimPanel* PanelPBInL;
		RimPanel* PanelPBInR;
		RimPanel* PanelRecInL;
		RimPanel* PanelSysGenSettings;
		awohSpin* SpinFFTLen;
		awohSpin* SpinRTAvg;
		awohSpin* SpinRefCh;
		mpWindow* mRTAMagPLot;
		wxBoxSizer* BoxSizerMonitor;
		wxBoxSizer* BoxSizerPBInL;
		wxBoxSizer* BoxSizerPlayback;
		wxBoxSizer* BoxSizerRecInL;
		wxBoxSizer* BoxSizerRecording;
		wxBoxSizer* BoxSizerTestGrp;
		wxChoice* ChoiceFrameSizeIn;
		wxChoice* ChoiceFrameSizeOut;
		wxChoice* ChoiceHostInput;
		wxChoice* ChoiceHostOutput;
		wxChoice* ChoiceInputDevice;
		wxChoice* ChoiceOutputDevice;
		wxChoice* ChoiceSystemSampleRateIn;
		wxChoice* ChoiceSystemSampleRateOut;
		wxPanel* Panel1RecInR;
		wxPanel* PanelPlayback;
		wxPanel* PanelRecording;
		wxStaticText* StaticTextCalibInstructions;
		wxStaticText* StaticTextChPkLvl;
		wxStaticText* StaticTextDevTestLbl;
		wxStaticText* StaticTextFrameSizeIn;
		wxStaticText* StaticTextFrameSizeOut;
		wxStaticText* StaticTextGainTtl;
		wxStaticText* StaticTextInChannels;
		wxStaticText* StaticTextInDevices;
		wxStaticText* StaticTextInGrpLbl;
		wxStaticText* StaticTextInHosts;
		wxStaticText* StaticTextLineUp;
		wxStaticText* StaticTextOutChannels;
		wxStaticText* StaticTextOutDevices;
		wxStaticText* StaticTextOutGain;
		wxStaticText* StaticTextOutGrpLbl;
		wxStaticText* StaticTextOutHosts;
		wxStaticText* StaticTextPBStreamTTL;
		wxStaticText* StaticTextRTAAvg;
		wxStaticText* StaticTextRTALength;
		wxStaticText* StaticTextRecStreamTTL;
		wxStaticText* StaticTextRefCh;
		wxStaticText* StaticTextSysSampleRateIn;
		wxStaticText* StaticTextSysSampleRateOut;
		wxTextCtrl* TextCtrlInChannels;
		wxTextCtrl* TextCtrlOutChannels;
		wxTimer TimerAudioMonitor;
		//*)

		VuMeter* mVuMeterIn;
		VuMeter* mVuMeterOut;

	protected:

		//(*Identifiers(AudioDevicesPanel)
		static const long ID_STATICTEXT_LINE_UP;
		static const long ID_BUTTON_SCAN_AUDIO_SYS;
		static const long ID_PANEL_SYS_GEN_SET;
		static const long ID_STATICTEXT_IN_GRP_LBL;
		static const long ID_STATICTEXT_IN_HOSTS;
		static const long ID_CHOICE_HOST_IN;
		static const long ID_STATICTEXT_IN_DEVS;
		static const long ID_CHOICE_INPUT_DEVICE;
		static const long ID_STATICTEXT_SYS_SRATE_IN;
		static const long ID_CHOICE_SYS_SRATE_IN;
		static const long ID_STATICTEXT_IN_CHANS;
		static const long ID_TEXTCTRL_IN_CHANS;
		static const long ID_STATICTEXT_FRAMESIZE_IN;
		static const long ID_CHOICE_FRAME_SIZE_IN;
		static const long ID_PANEL_INPUTS;
		static const long ID_STATICTEXT_OUT_GRP_LBl;
		static const long ID_STATICTEXT_OUT_HOSTS;
		static const long ID_CHOICE_HOST_OUT;
		static const long ID_STATICTEXT_OUT_DEVS;
		static const long ID_CHOICE_OUTPUT_DEVICE;
		static const long ID_STATICTEXT_SYS_SRATE_OUT;
		static const long ID_CHOICE_SYS_SRATE_OUT;
		static const long ID_STATICTEXT_OUT_CHANS;
		static const long ID_TEXTCTRL_OUT_CHANS;
		static const long ID_STATICTEXT_FRAMESIZE_OUT;
		static const long ID_CHOICE_FRAME_SIZE_OUT;
		static const long ID_PANEL_OUTPUTS;
		static const long ID_STATICTEXT_DEV_TST_TTL;
		static const long ID_BUTTON_TST_DEV_START;
		static const long ID_BUTTON_TST_DEV_STOP;
		static const long ID_STATICTEXT_CAL_INSTR;
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
		static const long ID_PANEL_DTEST_GRP;
		static const long ID_TIMER_AUDIO_MONITOR;
		//*)

		static const long ID_VUMETER_IN;
		static const long ID_VUMETER_OUT;

	private:
		//(*Handlers(AudioDevicesPanel)
		void OnChoiceHostInputSelect(wxCommandEvent& event);
		void OnChoiceHostOutputSelect(wxCommandEvent& event);
		void OnChoiceInputDeviceSelect(wxCommandEvent& event);
		void OnChoiceOutputDeviceSelect(wxCommandEvent& event);
		void OnChoiceSystemSampleRateInSelect(wxCommandEvent& event);
		void OnChoiceSystemSampleRateOutSelect(wxCommandEvent& event);
		void OnChoiceFrameSizeInSelect(wxCommandEvent& event);
		void OnChoiceFrameSizeOutSelect(wxCommandEvent& event);
		void OnButtonScanAudioSysClick(wxCommandEvent& event);
		void OnButtonDevTestStartClick(wxCommandEvent& event);
		void OnButtonDevTestStopClick(wxCommandEvent& event);
		void OnButtonOutGainUpClick(wxCommandEvent& event);
		void OnButtonOutGainDwnClick(wxCommandEvent& event);
		void OnButtonResetLTAClick(wxCommandEvent& event);
		void OnButtonPlotResetClick(wxCommandEvent& event);
		void OnTimerAudioMonitorTrigger(wxTimerEvent& event);
		//*)

		void OnSpinRTAvg(awohSpinEvent& event);
		void OnSpinFFTLength(awohSpinEvent& event);
		void OnSpinFFTReference(awohSpinEvent& event);

		DECLARE_EVENT_TABLE()

		void HandleInputDevSelection(int selIdx, bool checkSR = true);
		void HandleOutputDevSelection(int selIdx, bool checkSR = true);

		void CheckIfASIO_IO();

		void UpdateOutputGain(bool write = false);
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
