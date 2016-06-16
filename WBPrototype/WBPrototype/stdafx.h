
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <iomanip>

#include <afxdisp.h>        // MFC Automation classes
#include <afxdb.h>        // MFC Automation classes

#include <string>
#include <vector>


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include <iomanip>


//CFormView
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "
#define TODO( x )  message( __FILE__LINE__"\n"           \
    " ------------------------------------------------\n" \
    "|  TODO :   " #x "\n" \
    " -------------------------------------------------\n")

#define DELETE_COMPILER_DEFAULT_EXCEPT_MOVE(CLASSNAME_) \
    CLASSNAME_(const CLASSNAME_&) = delete; \
    void operator = (const CLASSNAME_&) = delete

#define DELETE_COMPILER_DEFAULT(CLASSNAME_) \
    DELETE_COMPILER_DEFAULT_EXCEPT_MOVE(CLASSNAME_); \
    CLASSNAME_(const CLASSNAME_&&) = delete; \
    void operator = (const CLASSNAME_&&) = delete 
    

//#define FILE(x) (strrchr(#x, '\\') ? strrchr(#x, '\\') + 1 : #x)
#define FILE(x) (strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x)

#include "WBTrace.h"


#define WBOUT WBPrototype::CWBTrace::GetStream()<< FILE(__FILE__) << "|" <<  __LINE__ <<"|"
#define WBOPEN WBPrototype::CWBTrace::Initialise
#define WBCLOSE WBPrototype::CWBTrace::Cleanup

using std::endl;
using std::string;
using std::vector;

void AFXAPI ThrowWBException(RETCODE nRetCode, const CString str);
#define GENERATE_WB_EXCEPTION(nRetCode,str) do{\
    WBOUT << str << endl; \
    ThrowWBException(nRetCode, str);\
}while (0)

struct CWBExceptionHandler
{
    DELETE_COMPILER_DEFAULT(CWBExceptionHandler);
    explicit CWBExceptionHandler(CException *pException, string filename, long line)
    {
        char msg[MAX_PATH] = { 0 };
        pException->GetErrorMessage(msg, MAX_PATH);
          
        WBPrototype::CWBTrace::GetStream() << FILE(filename.c_str()) << "|" << line << "|" << "Exception : " << msg << endl;
        
        if (pException->IsKindOf(RUNTIME_CLASS(CDBException)))
        {
            CDBException* pDBException = (CDBException *)pException;
            WBPrototype::CWBTrace::GetStream() << FILE(filename.c_str()) << "|" << line << "|" << "RetCode : " << pDBException->m_nRetCode << endl;
        }
        
        
        pException->Delete();
        
    }

    explicit CWBExceptionHandler(DWORD dwError, string filename, long line)
    {
        WBPrototype::CWBTrace::GetStream() << FILE(filename.c_str()) << "|" << line << "|" << "GetLastError : " << dwError << endl;
    }

    inline ~CWBExceptionHandler()
    {}
};

#include "WBRecordset.h"



