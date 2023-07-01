
// FileCleaner.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#include "pch.h"
#endif

#include "resource.h"		// main symbols


// CFileCleanerApp:
// See FileCleaner.cpp for the implementation of this class
//

class CFileCleanerApp : public CWinApp
{
public:
	CFileCleanerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFileCleanerApp theApp;
