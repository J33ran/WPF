
// WBPrototype.h : main header file for the WBPrototype application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CWBPrototypeApp:
// See WBPrototype.cpp for the implementation of this class
//
using WBPrototype::CWBRecordset;

class CWBPrototypeApp : public CWinApp
{
public:
	CWBPrototypeApp();
    WBPrototype::CWBRecordset* GetWBRecordset() const;

private:
    
    long            m_nCurrentDB;
    long            m_nCurrentRS;
   
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
    afx_msg void OnSybaseSQLWell();
    afx_msg void OnSybasePROCWell();

    afx_msg void OnUpdateSybaseSQLWell(CCmdUI* pCmdUI);
    afx_msg void OnUpdateSybasePROCWell(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

extern CWBPrototypeApp theApp;
