#ifndef AUDIODEVICESDIALOG_H
#define AUDIODEVICESDIALOG_H

//(*Headers(AudioDevicesDialog)
#include "AudioDevicesPanel.h"
#include <wx/dialog.h>
#include <wx/sizer.h>
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
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
