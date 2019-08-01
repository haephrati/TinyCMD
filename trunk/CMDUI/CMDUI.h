/*
Tiny CMD - by Michael Haephrati
(c)2019, Michael Haephrati and Secured Globe, Inc.
https://www.securedglobe.net

How to run CMD commands from a c++ based Dialog applicationb
Article first published at CodeProject.
https://www.codeproject.com/Articles/5163375/Tiny-CMD

*/

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCMDUIApp:
// See CMDUI.cpp for the implementation of this class
//

class CCMDUIApp : public CWinApp
{
public:
	CCMDUIApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCMDUIApp theApp;
