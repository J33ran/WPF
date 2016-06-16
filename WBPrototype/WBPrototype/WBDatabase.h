namespace WBPrototype
{
    class CWBRecordset;
    class CWBDatabase
    {
        DELETE_COMPILER_DEFAULT_EXCEPT_MOVE(CWBDatabase);
        
    public:
        CWBDatabase(const string sConnection);
        
        CWBDatabase(CWBDatabase&& rhs);
        CWBDatabase& operator = (CWBDatabase&& rhs);

        ~CWBDatabase();
        CWBRecordset& operator[] (const size_t index);

        CWBRecordset& CreateRecordset(const std::string strQuery);

    private:
        CWBDatabase& CWBDatabase::Move(CWBDatabase& rhs);
    public:
        CDatabase              *m_pDatabase;
        vector<CWBRecordset>    m_recordsets;
    };

    class CWBDatabaseFactory
    {
        DELETE_COMPILER_DEFAULT(CWBDatabaseFactory);
        CWBDatabaseFactory();
        
    public:
        ~CWBDatabaseFactory();
        CWBDatabase& CreateDatabase(const std::string strConnection);
        CWBDatabase& operator[] (const size_t index);

    private:
        std::vector<CWBDatabase>    m_databases;

    public:
        static CWBDatabaseFactory& Get()
        {
            static CWBDatabaseFactory factory;
            return factory;
        }
    };
}

