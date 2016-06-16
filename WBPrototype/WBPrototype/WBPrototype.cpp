
// WBPrototype.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"

#include "WBDatabase.h"

using namespace WBPrototype;
#include "WBPrototype.h"
#include "MainFrm.h"

#include "WBPrototypeDoc.h"
#include "WBPrototypeView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static bool UpdateUIFlag[] = {false, false};

void SetUpdateFlag(size_t idx)
{
    auto size = sizeof(UpdateUIFlag) / sizeof(UpdateUIFlag[0]);

    for (auto i = unsigned(0); i < size; i++)    
    {
        if (idx == i) UpdateUIFlag[i] = true;
        else  UpdateUIFlag[i] = false;
    }
}


// CWBPrototypeApp
BEGIN_MESSAGE_MAP(CWBPrototypeApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CWBPrototypeApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
    ON_COMMAND(ID_SYBASE_SQL_WELL, &CWBPrototypeApp::OnSybaseSQLWell)
    ON_COMMAND(ID_SYBASE_PROC_WELL, &CWBPrototypeApp::OnSybasePROCWell)

    ON_UPDATE_COMMAND_UI(ID_SYBASE_SQL_WELL, &CWBPrototypeApp::OnUpdateSybaseSQLWell)
    ON_UPDATE_COMMAND_UI(ID_SYBASE_PROC_WELL, &CWBPrototypeApp::OnUpdateSybasePROCWell)

END_MESSAGE_MAP()


// CWBPrototypeApp construction

CWBPrototypeApp::CWBPrototypeApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("WBPrototype.AppID.NoVersion"));

    m_nCurrentDB = m_nCurrentRS = -1;
	// Place all significant initialization in InitInstance
}

// The one and only CWBPrototypeApp object

CWBPrototypeApp theApp;


// CWBPrototypeApp initialization

BOOL CWBPrototypeApp::InitInstance()
{
    std::string s = R"(C:\ProgramData\GeoGraphix\WBTrace.txt)";
    WBOPEN(s);
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWBPrototypeDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CWBPrototypeView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


    

    
    


    try
    {
        // Sybase database;
        //"ODBC;DBF=C:\\ProgramData\\GeoGraphix\\Projects\\Sample\\GXDB.db;DBN=Sample-Projects;DSN=GXDB.ZBIGI2MH2PAERCNXUBCEPQNW34;"
        //const string strSybase = string(R"(Driver={Sybase SQL Anywhere 5.0};DBF=C:\ProgramData\GeoGraphix\Projects\Sample\GXDB.db;DBN=Sample-Projects;Uid=dba;Pwd=sql;DSN="";)");
        
#if 0
        const string strSybase = string(R"(ODBC;DBF=C:\ProgramData\GeoGraphix\Projects\Sample\GXDB.db;DBN=Sample-Projects;DSN=GXDB.WS7ICOZPBYHUPMYRJCKYZU7SU4;)");
        CWBDatabase& dbSybase = CWBDatabaseFactory::Get().CreateDatabase(strSybase.c_str());
        dbSybase.CreateRecordset(R"(SELECT * FROM DBA."ListWELL"())");
        dbSybase.CreateRecordset(R"(SELECT * FROM DBA."WELL")");
#endif



#pragma TODO("Oracle recordset")

        const string strOracle = string(R"(Driver={Oracle in OraClient11g_home1};Dbq=OW811GLX;Uid=System;Pwd=system;DSN=DOW.OW811GLX)");
        CWBDatabase& dbOracle = CWBDatabaseFactory::Get().CreateDatabase(strOracle.c_str());

        //dbOracle.CreateRecordset(R"({call DMM_POW."WELL_MASTER_PROC_TEXT"(?)})");
        //dbOracle.CreateRecordset(R"(SELECT * FROM DMM_POW."WELL_MASTER")");
        dbOracle.CreateRecordset(R"({call DMM_POW."WELL_MASTER_PROC"(?)})");
        //dbOracle.CreateRecordset(R"({call DMM_POW."TEST_NUM"(?)})");

        //dbOracle.CreateRecordset(R"({?=call DMM_POW."WELL_MASTER_PROC_TEXT_FUNC"()})");
        //dbOracle.CreateRecordset(R"(select DMM_POW."WELL_MASTER_PROC_TEXT_FUNC"() from dual)")
    }
    catch (CException* pEx)
    {
        CWBExceptionHandler(pEx, __FILE__, __LINE__);
    }
    catch (...)
    {
        CWBExceptionHandler(GetLastError(), __FILE__, __LINE__);
    }// Sybase database;


    // Dispatch commands specified on the command line.  Will return FALSE if
    // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;


	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CWBPrototypeApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

    WBCLOSE();
	return CWinApp::ExitInstance();
}

CWBRecordset* CWBPrototypeApp::GetWBRecordset() const
{
    CWBRecordset* pWBRecordset = nullptr;
    try
    {
        if (m_nCurrentDB == -1) throw std::out_of_range("out of range database index");
        if (m_nCurrentRS == -1) throw std::out_of_range("out of range recordset index");

        CWBDatabase& database = CWBDatabaseFactory::Get()[m_nCurrentDB];
        pWBRecordset = &(database[m_nCurrentRS]);
    }
    catch (std::out_of_range &ex)
    {
        WBOUT << "Out of Rang exception : ("<<m_nCurrentDB<<":"<<m_nCurrentRS<<")"<<ex.what()<<endl;
    }

    return pWBRecordset;
    
}




// CWBPrototypeApp message handlers
void CWBPrototypeApp::OnSybaseSQLWell()
{
    const long nNewDB = 0;
    const long nNewRS = 0;

    SetUpdateFlag(0);
    //if ((nNewDB != m_nCurrentDB) && (nNewRS != m_nCurrentRS))
    {
        m_nCurrentDB = 0;
        m_nCurrentRS = 0;
        OnFileNew();
    }
        
    
}

void CWBPrototypeApp::OnUpdateSybaseSQLWell(CCmdUI* pCmdUI)
{
    // Get a handle to the main window
    CMainFrame* main = ((CMainFrame*)m_pMainWnd);
    const auto bCheck = UpdateUIFlag[0];
    pCmdUI->SetCheck(bCheck);
}

// CWBPrototypeApp message handlers
void CWBPrototypeApp::OnSybasePROCWell()
{
    const long nNewDB = 0;
    const long nNewRS = 0;

    SetUpdateFlag(1);
    //if ((nNewDB != m_nCurrentDB) && (nNewRS != m_nCurrentRS))
    {
        m_nCurrentDB = 0;
        m_nCurrentRS = 1;
        OnFileNew();
    }

}

void CWBPrototypeApp::OnUpdateSybasePROCWell(CCmdUI* pCmdUI)
{
    // Get a handle to the main window
    CMainFrame* main = ((CMainFrame*)m_pMainWnd);
    const auto bCheck = UpdateUIFlag[1];
    pCmdUI->SetCheck(bCheck);
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CWBPrototypeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



// CWBPrototypeApp message handlers



