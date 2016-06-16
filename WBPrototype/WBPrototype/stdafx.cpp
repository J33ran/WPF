
// stdafx.cpp : source file that includes just the standard includes
// WBPrototype.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


void AFXAPI ThrowWBException(RETCODE nRetCode, const CString str)
{
    CDBException* pException = new CDBException(nRetCode);
    pException->m_nRetCode = nRetCode;
    pException->m_strError = std::move(str);

    THROW(pException);
}