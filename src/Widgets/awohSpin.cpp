#include "awohSpin.h"

//(*InternalHeaders(awohSpin)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(awohSpin)
const long awohSpin::ID_BUTTON_MINUS = wxNewId();
const long awohSpin::ID_STATICTEXT_VALUE = wxNewId();
const long awohSpin::ID_BUTTON_PLUS = wxNewId();
//*)


DEFINE_EVENT_TYPE(wxEVT_COMMAND_AWOHSPIN)

IMPLEMENT_DYNAMIC_CLASS(awohSpinEvent, wxCommandEvent)  // RTTI

///////////////////////////////////////////////////////////
awohSpinEvent::awohSpinEvent(wxEventType commandType, int id)
		: wxCommandEvent(commandType, id)
{
}

BEGIN_EVENT_TABLE(awohSpin,wxPanel)
	//(*EventTable(awohSpin)
	//*)
END_EVENT_TABLE()

awohSpin::awohSpin(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
:mSize(size)
{
	//(*Initialize(awohSpin)
	wxBoxSizer* BoxSizerMainSpin;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizerMainSpin = new wxBoxSizer(wxHORIZONTAL);
	ButtonMinus = new GButton(this, ID_BUTTON_MINUS, _("<"), wxDefaultPosition, wxSize(18,18), wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BUTTON_MINUS"));
	wxFont ButtonMinusFont(12,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Calibri"),wxFONTENCODING_DEFAULT);
	ButtonMinus->SetFont(ButtonMinusFont);
	BoxSizerMainSpin->Add(ButtonMinus, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticTextValue = new wxStaticText(this, ID_STATICTEXT_VALUE, _("---"), wxDefaultPosition, size, wxALIGN_CENTRE|wxNO_BORDER, _T("ID_STATICTEXT_VALUE"));
	wxFont StaticTextValueFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Calibri"),wxFONTENCODING_DEFAULT);
	StaticTextValue->SetFont(StaticTextValueFont);
	BoxSizerMainSpin->Add(StaticTextValue, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	ButtonPlus = new GButton(this, ID_BUTTON_PLUS, _(">"), wxDefaultPosition, wxSize(18,18), wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BUTTON_PLUS"));
	wxFont ButtonPlusFont(12,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Calibri"),wxFONTENCODING_DEFAULT);
	ButtonPlus->SetFont(ButtonPlusFont);
	BoxSizerMainSpin->Add(ButtonPlus, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizerMainSpin);
	BoxSizerMainSpin->Fit(this);
	BoxSizerMainSpin->SetSizeHints(this);
	//*)
    //StaticTextValue = new wxStaticText(this, ID_STATICTEXT_VALUE, _("---"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE|wxNO_BORDER, _T("ID_STATICTEXT_VALUE"));
	Connect(ID_BUTTON_MINUS,wxEVT_COMMAND_GBUTTON,(wxObjectEventFunction)&awohSpin::OnButtonMinusClick);
	Connect(ID_BUTTON_PLUS,wxEVT_COMMAND_GBUTTON,(wxObjectEventFunction)&awohSpin::OnButtonPlusClick);
    ButtonPlus->setAsToggle(false);
    ButtonMinus->setAsToggle(false);

    StaticTextValue->SetSize(mSize);
    Layout();

	mValueIndex = 0;
}

awohSpin::~awohSpin()
{
	//(*Destroy(awohSpin)
	//*)
}

void
awohSpin::clearValues()
{
    mValues.clear();
}

void
awohSpin::appendValues( wxString name, int value )
{
    awohSpinValue el;
    el.name = name;
    el.value = value;
    mValues.push_back(el);
    mNoValues = mValues.size();
}

void
awohSpin::setBGColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
    mBGColour = new wxColour( red, green, blue, alpha );
    StaticTextValue->SetBackgroundColour( *mBGColour );
    mBtnColour = new wxColour( red-10, green-10, blue-10, alpha );
    ButtonPlus->SetBackgroundColour( *mBtnColour );
    ButtonMinus->SetBackgroundColour( *mBtnColour );
}

void
awohSpin::setFGColour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
    mFGColour = new wxColour( red, green, blue, alpha );
    StaticTextValue->SetForegroundColour( *mFGColour );
    ButtonPlus->SetForegroundColour( *mFGColour );
    ButtonMinus->SetForegroundColour( *mFGColour );
}

void
awohSpin::setValue( int value )
{
    mValueIndex = 0;
    for( size_t i = 0; i < mNoValues; i++ )
    {
        awohSpinValue el = mValues[i];
        if( el.value == value )
        {
            StaticTextValue->SetLabel(el.name);
            return;
        }
        mValueIndex++;
    }
}

void
awohSpin::setValue( wxString name )
{
    mValueIndex = 0;
    for( size_t i = 0; i < mNoValues; i++ )
    {
        awohSpinValue el = mValues[i];
        if( el.name == name )
        {
            StaticTextValue->SetLabel(el.name);
            return;
        }
        mValueIndex++;
    }
}

int
awohSpin::getNumericalValue()
{
    awohSpinValue spinVal = mValues[ mValueIndex ];
    return spinVal.value;
}

wxString
awohSpin::getStringValue()
{
    awohSpinValue spinVal = mValues[ mValueIndex ];
    return spinVal.name;
}

void awohSpin::OnButtonMinusClick(wxCommandEvent& event)
{
    mValueIndex -= 1;
    if( mValueIndex < 0 )
        mValueIndex = 0;

    awohSpinValue entry = mValues[mValueIndex];
    StaticTextValue->SetLabel(entry.name);
    SendEvent();
}

void awohSpin::OnButtonPlusClick(wxCommandEvent& event)
{
    mValueIndex += 1;
    if( mValueIndex > (mNoValues-1) )
        mValueIndex = (mNoValues-1);

    awohSpinValue entry = mValues[mValueIndex];
    StaticTextValue->SetLabel(entry.name);
    SendEvent();
}

void awohSpin::SendEvent()
{
    // Create and event and send it
    awohSpinEvent event(wxEVT_COMMAND_AWOHSPIN, GetId());
    event.SetEventObject(this);
    GetEventHandler()->ProcessEvent(event);
}
