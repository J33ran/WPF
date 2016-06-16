#include "stdafx.h"
#include "WBDatabase.h"


namespace WBPrototype
{
    CWBDatabaseFactory::~CWBDatabaseFactory()
    {
    }

    CWBDatabaseFactory::CWBDatabaseFactory()
        : m_databases()
    {
    }

    CWBDatabase& CWBDatabaseFactory::CreateDatabase(const std::string strConnection)
    {
        CWBDatabase database(strConnection);
        m_databases.push_back(std::move(database));
       
        return m_databases.back();
    }

    CWBDatabase& CWBDatabaseFactory::operator[](const size_t index)
    {
        return m_databases[index];
    }

    CWBDatabase::CWBDatabase(const string sConnection)
        : m_pDatabase(new CDatabase())
    {
        BOOL bRes = m_pDatabase->OpenEx(sConnection.c_str(),  CDatabase::openReadOnly | CDatabase::noOdbcDialog);
        if (bRes == 0)  GENERATE_WB_EXCEPTION(bRes, R"(Database opening failure)");
    }

    CWBDatabase& CWBDatabase::Move(CWBDatabase& rhs)
    {
        m_pDatabase = std::move(rhs.m_pDatabase);
        m_recordsets = std::move(rhs.m_recordsets);

        rhs.m_pDatabase = nullptr;
        return (*this);
    }

    CWBDatabase::CWBDatabase(CWBDatabase&& rhs)
    {
        Move(rhs);
    }

    CWBDatabase& CWBDatabase::operator = (CWBDatabase&& rhs)
    {
        Move(rhs);
        return (*this);
    }

    CWBDatabase::~CWBDatabase()
    {
        m_recordsets.erase(m_recordsets.begin(), m_recordsets.end());

        if(m_pDatabase) m_pDatabase->Close();
        delete m_pDatabase;
    }

    CWBRecordset& CWBDatabase::operator[](const size_t index)
    {
        return m_recordsets[index];
    }

    CWBRecordset& CWBDatabase::CreateRecordset(const std::string strQuery)
    {
        CWBRecordset recordset(m_pDatabase, strQuery);
        m_recordsets.push_back(std::move(recordset));
        return m_recordsets.back();
    }
}