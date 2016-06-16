#pragma once

namespace WBPrototype
{
    class CCurRecordset  : public CRecordset
    {
        DELETE_COMPILER_DEFAULT_EXCEPT_MOVE(CCurRecordset);
    public:
    
        int 			m_cursor;
        //{{AFX_FIELD(CColumns, CRecordset)
        CString         m_strType, m_strStatus = "";
        //}}AFX_FIELD
        CStringList     m_listNames;
        CStringList     m_listValues;
        

        CCurRecordset(CDatabase* pDatabase = NULL);
        DECLARE_DYNAMIC(CCurRecordset)

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCurRecordset)

        // virtual CString GetDefaultConnect(); // Default connection string
        // virtual CString GetDefaultSQL(); // Default SQL for Recordset
        virtual void DoFieldExchange(CFieldExchange* pFX); // RFX support
        virtual void Move(long nrows, WORD wfetchtype);
        virtual void PreBindFields();
#if 0
        ////////////////////////////////////////////
        virtual BOOL Open(UINT nOpenType, LPCTSTR lpszSQL, DWORD dwOptions);
        virtual void AllocAndCacheFieldInfo(); 
        virtual void GetFieldValue(short nIndex, CString& strValue);
#endif
        //}}AFX_VIRTUAL
        // Implementation
#ifdef _DEBUG
        // virtual void AssertValid() const;
        //virtual void Dump(CDumpContext& dc) const;
#endif
    };
}