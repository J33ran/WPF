#include "stdafx.h"
#include "CurRecordset.h"

namespace WBPrototype
{
    CWBRecordset::CWBRecordset(CDatabase* pDatabase, const string& sQuery)
    {
     
        m_pRecordset = new CCurRecordset(pDatabase);
        //m_pRecordset = new CRecordset(pDatabase);
        m_sQuery = sQuery;
    } 


    BOOL CWBRecordset::Open()
    {
        const DWORD options = !(CRecordset::useExtendedFetch) | CRecordset::readOnly;
        

        //const BOOL bRes = m_pRecordset->Open(CRecordset::dynaset, m_sQuery.c_str(), options);
        const BOOL bRes = m_pRecordset->Open(CRecordset::forwardOnly, m_sQuery.c_str(), options);
        if (bRes == 0) GENERATE_WB_EXCEPTION(bRes, R"(Recordset open failure.)");

        return bRes;
    }

    void CWBRecordset::Close()
    {
        if (m_pRecordset)
        {
            if (m_pRecordset->IsOpen() != 0)
            {
                m_pRecordset->Close();
            }
        }        
    }

    CWBRecordset::~CWBRecordset()
    {
        try
        {
            Close();
            delete m_pRecordset;
        }
        catch (...)
        {
            CWBExceptionHandler(GetLastError(), __FILE__, __LINE__);
        }
    }

    CWBRecordset& CWBRecordset::Move(CWBRecordset& rhs)
    {
        m_pRecordset = std::move(rhs.m_pRecordset);
        m_sQuery = std::move(rhs.m_sQuery);
        rhs.m_pRecordset = nullptr;
        return (*this);
    }

    CWBRecordset::CWBRecordset(CWBRecordset&& rhs)
    {        
        Move(rhs);
    }

    CWBRecordset& CWBRecordset::operator = (CWBRecordset&& rhs)
    {
        return Move(rhs);
    }
}