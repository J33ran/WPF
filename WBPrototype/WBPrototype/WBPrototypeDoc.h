
// WBPrototypeDoc.h : interface of the CWBPrototypeDoc class
//


#pragma once


class CWBPrototypeDoc : public CDocument
{
protected: // create from serialization only
	CWBPrototypeDoc();
	DECLARE_DYNCREATE(CWBPrototypeDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CWBPrototypeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    CRecordset   *m_pRecordset;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnSybaseZone();
    afx_msg void OnUpdateSybaseZone(CCmdUI *pCmdUI);

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
