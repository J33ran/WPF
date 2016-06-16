
// WBPrototypeView.cpp : implementation of the CWBPrototypeView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WBPrototype.h"
#endif


#include "WBPrototypeDoc.h"
#include "WBPrototypeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using WBPrototype::CWBRecordset;

// CWBPrototypeView
CWBRecordset * GetWBRecordset()
{
    return ((CWBPrototypeApp*)AfxGetApp())->GetWBRecordset();
}

IMPLEMENT_DYNCREATE(CWBPrototypeView, CListView)

BEGIN_MESSAGE_MAP(CWBPrototypeView, CListView)
    ON_WM_CLOSE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// CWBPrototypeView construction/destruction

CWBPrototypeView::CWBPrototypeView()
: CListView()
, m_hThread(INVALID_HANDLE_VALUE)
{
	// TODO: add construction code here

}

CWBPrototypeView::~CWBPrototypeView()
{
}

void CWBPrototypeView::DoDataExchange(CDataExchange* pDX)
{
    CListView::DoDataExchange(pDX);
}

BOOL CWBPrototypeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

    cs.style &= ~LVS_TYPEMASK;
    cs.style |= LVS_REPORT;
    
    return CListView::PreCreateWindow(cs);
}

void CWBPrototypeView::OnClose()
{
    
    Clear();
    CWnd::OnClose();
}


void CWBPrototypeView::OnDestroy()
{
    CWnd::OnDestroy();
}


void CWBPrototypeView::CreateColumns(CRecordset * const pRecordset)
{
    WBOUT << "Creating columns..." << endl;

    CListCtrl& listCtrl = GetListCtrl();
    const short nCount = pRecordset->GetODBCFieldCount();

    WBOUT << "Total ODBCFieldCount :" << nCount<< endl;


    CHeaderCtrl* pHeader = listCtrl.GetHeaderCtrl();
    if (NULL != pHeader)
    {
        const int nColCount = pHeader->GetItemCount();
        for (int nCol = 0; nCol < nColCount; nCol++)
        {
            pHeader->DeleteItem(0);
        }
    }


    for (short nCol = 0; nCol < nCount; nCol++)
    {
        CODBCFieldInfo info;

        pRecordset->GetODBCFieldInfo(nCol, info);
        listCtrl.InsertColumn(nCol, info.m_strName, LVCFMT_LEFT, 100, nCol);
    }

    WBOUT << "Columns created successfully..." << endl;
}

void CWBPrototypeView::OnInitialUpdate()
{    
    CListView::OnInitialUpdate();
    GetParentFrame()->RecalcLayout();

    CListCtrl& listCtrl = GetListCtrl();
    listCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


    Clear();
    m_hThread = CreateThread(NULL, 0, &CWBPrototypeView::PopulateList, LPVOID(this), 0, NULL);
}

void CWBPrototypeView::Clear()
{
    if (m_hThread != INVALID_HANDLE_VALUE)
    {
        ::TerminateThread(m_hThread, 0);
        WaitForSingleObject(m_hThread, INFINITE);
        m_hThread = INVALID_HANDLE_VALUE;

        GetWBRecordset()->Close();
    }
    

    CListCtrl& listCtrl = GetListCtrl();
    listCtrl.DeleteAllItems();
    ASSERT(listCtrl.GetItemCount() == 0);
}

// CWBPrototypeView diagnostics

#ifdef _DEBUG
void CWBPrototypeView::AssertValid() const
{
    CListView::AssertValid();
}

void CWBPrototypeView::Dump(CDumpContext& dc) const
{
    CListView::Dump(dc);
}



DWORD CWBPrototypeView::PopulateList(LPVOID lpParam)
{
    CWBPrototypeView *const pThis = static_cast<CWBPrototypeView *const>(lpParam);
    CWBRecordset* pWBRecordset = ((CWBPrototypeApp*)AfxGetApp())->GetWBRecordset();
    CListCtrl& listCtrl = pThis->GetListCtrl();

    if (!pWBRecordset) return -1;
    
    
    try
    {

        pWBRecordset->Open();
        CRecordset* const pRecordset = pWBRecordset->GetRecordset();

        pThis->CreateColumns(pRecordset);


        // iItem: Zero-based index of the item to which this structure refers. 
        // Rowset is one based index.

        // Set the rowset size
        // pRecordset->SetRowsetSize(10);

        pThis->BeginWaitCursor();
        // loop through the recordset by rowsets 
        DWORD nRow = 0;
       // pRecordset->MoveFirst();
        while (!pRecordset->IsEOF())
        {
            LVITEM item;
            item.mask = LVIF_TEXT;
            item.iItem = nRow;
            item.iSubItem = 0;


            const auto nFields = pRecordset->GetODBCFieldCount();
            for (auto iCol = 0; iCol < nFields; iCol++)
            {
                CString sValue;
                pRecordset->GetFieldValue(iCol, sValue);

                if (sValue.IsEmpty())
                {
                    sValue = "(NULL)";
                }

                char* pszText = (LPTSTR)(LPCTSTR)(sValue);

                if (iCol == 0)
                {
                    item.pszText = pszText;
                    listCtrl.InsertItem(&item);
                }
                else
                {
                    listCtrl.SetItemText(nRow, iCol, pszText);
                }

                // WBOUT << "Insert(" << item.iItem << ", " << item.iSubItem<<")"<< item.pszText << endl;
                sValue.ReleaseBuffer();
            }

            ++nRow;
            pRecordset->MoveNext();
        }
    }
    catch (CException *pException)
    {
        CWBExceptionHandler ex(pException, __FILE__, __LINE__);
    }
    pThis->EndWaitCursor();
    pWBRecordset->Close();
    return 0;
}

CWBPrototypeDoc* CWBPrototypeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWBPrototypeDoc)));
	return (CWBPrototypeDoc*)m_pDocument;
}
#endif //_DEBUG


// CWBPrototypeView message handlers
