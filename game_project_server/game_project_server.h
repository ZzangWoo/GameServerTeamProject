
// game_project_server.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cgame_project_serverApp:
// See game_project_server.cpp for the implementation of this class
//

class Cgame_project_serverApp : public CWinApp
{
public:
	Cgame_project_serverApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cgame_project_serverApp theApp;