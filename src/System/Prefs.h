////////////////////////////////////////////////////////////////////////////////////////
// ADCTester license
// This code was created in 2017 for the Library of Congress 
// and the other federal government agencies participating in the 
// Federal Agencies Digitization Guidelines Initiative and it is in the public domain.
////////////////////////////////////////////////////////////////////////////////////////


#ifndef __EXTRACTOR_PREFS__
#define __EXTRACTOR_PREFS__

#include "wx/config.h"
#include "wx/fileconf.h"

#define RELEASE_VERSION 110

void InitPreferences();
void FinishPreferences();

extern wxFileConfig *gPrefs;

#endif


