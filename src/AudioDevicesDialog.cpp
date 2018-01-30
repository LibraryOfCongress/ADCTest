#include "AudioDevicesDialog.h"

//(*InternalHeaders(AudioDevicesDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(AudioDevicesDialog)
const long AudioDevicesDialog::ID_ADEVS_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(AudioDevicesDialog,wxDialog)
	//(*EventTable(AudioDevicesDialog)
	//*)
END_EVENT_TABLE()

AudioDevicesDialog::AudioDevicesDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(AudioDevicesDialog)
	wxBoxSizer* BoxSizerMain;

	Create(parent, id, _("audio devices setup"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizerMain = new wxBoxSizer(wxHORIZONTAL);
	mADPanel = new AudioDevicesPanel(this,ID_ADEVS_PANEL,wxDefaultPosition,wxDefaultSize);
	BoxSizerMain->Add(mADPanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	Center();
	//*)
}

AudioDevicesDialog::~AudioDevicesDialog()
{
	//(*Destroy(AudioDevicesDialog)
	//*)
}


void
AudioDevicesDialog::StartCalibration()
{
	mADPanel->StartCalibration();
}

void
AudioDevicesDialog::StopCalibration()
{
	mADPanel->StopCalibration();
}

void AudioDevicesDialog::OnClose(wxCloseEvent& event)
{
}
