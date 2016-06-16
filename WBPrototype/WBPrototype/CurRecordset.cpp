#include "stdafx.h"
#include "CurRecordset.h"

namespace WBPrototype
{
    // CCurRecordset
    IMPLEMENT_DYNAMIC(CCurRecordset, CRecordset)

        CCurRecordset::CCurRecordset(CDatabase* pdb)
        : CRecordset(pdb)
        , m_strType('n',255)
        , m_strStatus('n', 255)
    {           
        m_nParams = 1;
        m_cursor = 0;
        m_nFields = 0;

        m_nDefaultType = CRecordset::snapshot;
        //{{AFX_FIELD_INIT(CColumns)
        //}}AFX_FIELD_INIT
    }
 
    void CCurRecordset::DoFieldExchange(CFieldExchange* pFX)
    {
        ////{{AFX_FIELD_MAP(CColumns)  
        pFX->SetFieldType(CFieldExchange::outputParam);
        RFX_Int(pFX, _T("pCursorset"), m_cursor);

        if (m_nFields != 0)
        {
            pFX->SetFieldType(CFieldExchange::outputColumn);
            
            POSITION posName = m_listNames.GetHeadPosition();
            POSITION posValue = m_listValues.GetHeadPosition();

            while (posName != NULL)
            {
                CString& name = m_listNames.GetNext(posName);
                CString& value = m_listValues.GetNext(posValue);

                RFX_Text(pFX, name, value);
            }
        }

        int i = 0;
    }

    void CCurRecordset::Move(long nRows, WORD wFetchType)
    {
        CRecordset::Move(nRows, wFetchType);
    }
    
    void CCurRecordset::PreBindFields()
    {
        WBOUT << "Binding Field columns..." << endl;
        const auto nCount = GetODBCFieldCount();

        WBOUT << "Total ODBCFieldCount :" << nCount << endl;

        for (auto nCol = 0; nCol < nCount; nCol++)
        {
            CODBCFieldInfo info;

            GetODBCFieldInfo(nCol, info);
            WBOUT << "Fieldname = " << static_cast<LPCTSTR>(info.m_strName) << endl;
            m_listNames.AddTail(info.m_strName);
            m_listValues.AddTail(CString("n",255));

        }
            
        m_nFields = nCount;
        // CRecordset::AllocStatusArrays();
        
        WBOUT << "Fields bounded successfully..." << endl;
    }

//////////////////////
#if 0
    BOOL CCurRecordset::Open(UINT nOpenType, LPCTSTR lpszSQL, DWORD dwOptions)
    {
        ASSERT(!IsOpen());
        ASSERT_VALID(this);
        ASSERT(lpszSQL == NULL || AfxIsValidString(lpszSQL));
        ASSERT(nOpenType == AFX_DB_USE_DEFAULT_TYPE ||
            nOpenType == dynaset || nOpenType == snapshot ||
            nOpenType == forwardOnly || nOpenType == dynamic);
        ASSERT(!(dwOptions & readOnly && dwOptions & appendOnly));

        // Can only use optimizeBulkAdd with appendOnly recordsets
        ASSERT((dwOptions & optimizeBulkAdd && dwOptions & appendOnly) ||
            !(dwOptions & optimizeBulkAdd));

        // forwardOnly recordsets have limited functionality
        ASSERT(!(nOpenType == forwardOnly && dwOptions & skipDeletedRecords));

        // Cache state info and allocate hstmt
        SetState(nOpenType, lpszSQL, dwOptions);
        if (!AllocHstmt())
            return FALSE;

        // Check if bookmarks upported (CanBookmark depends on open DB)
        ASSERT(dwOptions & useBookmarks ? CanBookmark() : TRUE);

        TRY
        {
            OnSetOptions(m_hstmt);

            // Allocate the field/param status arrays, if necessary
            BOOL bUnbound = FALSE;
            if (m_nFields > 0 || m_nParams > 0)
                AllocStatusArrays();
            else
                bUnbound = TRUE;

            // Build SQL and prep/execute or just execute direct
            m_strSQL = lpszSQL;
            PrepareAndExecute();

            // Cache some field info and prepare the rowset
            AllocAndCacheFieldInfo();
            AllocRowset();

            // If late binding, still need to allocate status arrays
            if (bUnbound && (m_nFields > 0 || m_nParams > 0))
                AllocStatusArrays();

            // Give derived classes a call before binding
            PreBindFields();

            if (m_nResultCols)
                MoveNext();

            // If EOF, then result set empty, so set BOF as well
            m_bBOF = m_bEOF;
        }
        CATCH_ALL(e)
        {
            Close();
            THROW_LAST();
        }
        END_CATCH_ALL

        return TRUE;
    }

    void CCurRecordset::AllocAndCacheFieldInfo()
    {
        m_nResultCols = -1;
        ASSERT(GetODBCFieldCount() < 0);

        if (m_rgODBCFieldInfos != NULL)
        {
            delete[] m_rgODBCFieldInfos;
            m_rgODBCFieldInfos = NULL;
        }

        RETCODE nRetCode;
        SWORD nActualLen;	   // Cache the number of result columns.
        AFX_ODBC_CALL(::SQLNumResultCols(m_hstmt, &m_nResultCols));
        if (!Check(nRetCode))
        {
            TRACE(_T("Error: Can't get field info.\n"));
            AfxThrowDBException(nRetCode, m_pDatabase, m_hstmt);
        }

        // If there are no fields, quit now
        if (m_nResultCols == 0)
            return;

        // Allocate buffer and get the ODBC meta data.
        m_rgODBCFieldInfos = new CODBCFieldInfo[m_nResultCols];

        LPSTR lpszFieldName;

#ifdef _UNICODE
        // Need proxy to temporarily store non-UNICODE name.
        lpszFieldName = new char[MAX_FNAME_LEN + 1];
#endif

        // Get the field info for each field.
        for (WORD n = 1; n <= GetODBCFieldCount(); n++)
        {
#ifndef _UNICODE 	// Reset the buffer to point to next element.
            lpszFieldName = m_rgODBCFieldInfos[n - 1].m_strName.GetBuffer(MAX_FNAME_LEN + 1);
#endif

            AFX_ODBC_CALL(::SQLDescribeCol(m_hstmt, n,
                (UCHAR*)lpszFieldName, MAX_FNAME_LEN, &nActualLen,
                &m_rgODBCFieldInfos[n - 1].m_nSQLType,
                &m_rgODBCFieldInfos[n - 1].m_nPrecision,
                &m_rgODBCFieldInfos[n - 1].m_nScale,
                &m_rgODBCFieldInfos[n - 1].m_nNullability));

#ifndef _UNICODE
            m_rgODBCFieldInfos[n - 1].m_strName.ReleaseBuffer(nActualLen);
#else		 // Copy the proxy data to correct element.
            m_rgODBCFieldInfos[n - 1].m_strName = lpszFieldName;
#endif

            if (!Check(nRetCode))
            {
                TRACE1("Error: ODBC failure getting field #%d info.\n", n);
                AfxThrowDBException(nRetCode, m_pDatabase, m_hstmt);
            }
        }

#ifdef _UNICODE
        delete[] lpszFieldName;
#endif
    }
    
   void CCurRecordset::GetFieldValue(short nIndex, CString& strValue)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	// No data or no column info fetched yet
	if (GetODBCFieldCount() <= 0)
	{
		ASSERT(FALSE);
		return;
	}

	// Convert index to 1-based and check range
	nIndex++;
	if (nIndex < 1 || nIndex > GetODBCFieldCount())
	{
		ThrowDBException(AFX_SQL_ERROR_FIELD_NOT_FOUND);
	}

	int nLen = GetTextLen(m_rgODBCFieldInfos[nIndex - 1].m_nSQLType,
			m_rgODBCFieldInfos[nIndex - 1].m_nPrecision);
#ifndef _UNICODE
		CString& strData = strValue;
#else
		CString strProxy;
		CString& strData = strProxy;
#endif
	void* pvData = strData.GetBufferSetLength(nLen);

	// Now can actually get the data
	long nActualSize = GetData(m_pDatabase, m_hstmt, nIndex,
		SQL_C_CHAR, pvData, nLen,
		m_rgODBCFieldInfos[nIndex - 1].m_nSQLType);

	// Handle NULL data separately
	if (nActualSize == SQL_NULL_DATA)
	{
		// Clear value
		strValue.Empty();
	}
	else
	{
		// May need to cleanup and call SQLGetData again if necessary
		GetLongCharDataAndCleanup(m_pDatabase, m_hstmt, nIndex,
			nActualSize, &pvData, nLen, strData,
			m_rgODBCFieldInfos[nIndex - 1].m_nSQLType);

#ifdef _UNICODE
	// Now must convert string to UNICODE
	strValue = (LPCSTR)strData.GetBuffer(0);
#endif // _UNIOCDE
	}

	if(m_rgODBCFieldInfos[nIndex - 1].m_nSQLType == SQL_BINARY ||
		m_rgODBCFieldInfos[nIndex - 1].m_nSQLType == SQL_LONGVARBINARY ||
		m_rgODBCFieldInfos[nIndex - 1].m_nSQLType == SQL_VARBINARY)
		if(!strValue.IsEmpty())
			strValue = _T("0x") + strValue;
}
#endif
}