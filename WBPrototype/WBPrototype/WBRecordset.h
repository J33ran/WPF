#pragma once

namespace WBPrototype
{
    class CWBRecordset
    {
        DELETE_COMPILER_DEFAULT_EXCEPT_MOVE(CWBRecordset);
    public:
        BOOL Open();
        void Close();
        CWBRecordset(CDatabase* pDatabase, const string& sQuery);
        ~CWBRecordset();

        CWBRecordset(CWBRecordset&& rhs);
        CWBRecordset& operator = (CWBRecordset&& rhs);

        CRecordset* GetRecordset()  { return m_pRecordset; }
    private:
        CWBRecordset& Move(CWBRecordset& rhs);
        
        CRecordset  *m_pRecordset;
        string       m_sQuery;
    };
}