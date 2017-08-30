////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIODEVICESDIALOG_H
#define AUDIODEVICESDIALOG_H

//(*Headers(AudioDevicesDialog)
#include <wx/sizer.h>
#include "AudioDevicesPanel.h"
#include <wx/dialog.h>
//*)

class AudioDevicesDialog: public wxDialog
{
	public:

		AudioDevicesDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~AudioDevicesDialog();
		
		void StartCalibration();
		void StopCalibration();

		//(*Declarations(AudioDevicesDialog)
		AudioDevicesPanel* mADPanel;
		//*)

	protected:

		//(*Identifiers(AudioDevicesDialog)
		static const long ID_ADEVS_PANEL;
		//*)

	private:

		//(*Handlers(AudioDevicesDialog)
		void OnClose(wxCloseEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
