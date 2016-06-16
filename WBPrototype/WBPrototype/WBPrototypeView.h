
// WBPrototypeView.h : interface of the CWBPrototypeView class
//

#pragma once

#include "resource.h"

class CWBPrototypeView : public CListView
{
protected: // create from serialization only
	CWBPrototypeView();
	DECLARE_DYNCREATE(CWBPrototypeView)

public:
	enum{ IDD = IDD_WBPROTOTYPE_FORM };

// Attributes
public:
	CWBPrototypeDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void Clear();
    //virtual UINT FillList(Recordset& rs);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CWBPrototypeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    void CreateColumns(CRecordset * const pRecordset);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnClose();
    afx_msg void OnDestroy();

private:
    HANDLE  m_hThread;
    static DWORD WINAPI PopulateList(LPVOID lpParam);
};

#ifndef _DEBUG  // debug version in WBPrototypeView.cpp
inline CWBPrototypeDoc* CWBPrototypeView::GetDocument() const
   { return reinterpret_cast<CWBPrototypeDoc*>(m_pDocument); }
#endif

