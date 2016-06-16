
// WBPrototypeDoc.cpp : implementation of the CWBPrototypeDoc class
//

#include "stdafx.h"

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WBPrototype.h"
#endif

#include "WBPrototypeDoc.h"


#include <propkey.h>

using namespace WBPrototype;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWBPrototypeDoc

IMPLEMENT_DYNCREATE(CWBPrototypeDoc, CDocument)

BEGIN_MESSAGE_MAP(CWBPrototypeDoc, CDocument)
    //ON_COMMAND(ID_SYBASE_ZONE, &CWBPrototypeDoc::OnSybaseZone)
    //ON_UPDATE_COMMAND_UI(ID_SYBASE_ZONE, &CWBPrototypeDoc::OnUpdateSybaseZone)
END_MESSAGE_MAP()


// CWBPrototypeDoc construction/destruction

CWBPrototypeDoc::CWBPrototypeDoc()
    : CDocument()
    , m_pRecordset(nullptr)
{
}

CWBPrototypeDoc::~CWBPrototypeDoc()
{
}

BOOL CWBPrototypeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CWBPrototypeDoc serialization
void CWBPrototypeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWBPrototypeDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CWBPrototypeDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWBPrototypeDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWBPrototypeDoc diagnostics

#ifdef _DEBUG
void CWBPrototypeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWBPrototypeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

//void CWBPrototypeDoc::OnSybaseZone()
//{
//    int i = 0;
//}
//
//
//void CWBPrototypeDoc::OnUpdateSybaseZone(CCmdUI *pCmdUI)
//{
//    static bool bRes = false;
//    pCmdUI->Enable(true);
//    bRes = !bRes;
//}

// CWBPrototypeDoc commands
