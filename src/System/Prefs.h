/*
    This file copyright 2011 Christian Landone.
    All rights reserved.
*/

#ifndef __EXTRACTOR_PREFS__
#define __EXTRACTOR_PREFS__

#include "wx/config.h"
#include "wx/fileconf.h"

#define RELEASE_VERSION 170

void InitPreferences();
void FinishPreferences();

extern wxFileConfig *gPrefs;

#endif


