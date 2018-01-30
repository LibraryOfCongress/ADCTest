////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
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


