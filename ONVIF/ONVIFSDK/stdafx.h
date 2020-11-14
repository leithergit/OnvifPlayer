// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include "vld.h"
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

void Soap_Trace(LPCSTR pFormat, ...);
bool ParseHttpURL(char const* szUrl, char* szHost, WORD& nPort, char* szUser, char* szPassword, const char** urlSuffix);

#define  Declare_ClassName(x) string strClassName = #x;

#define  HttpCall(SoapProxy,pFunc,nResult) {\
	int nResult = 0;\
	string strUrl;\
	string strUser;\
	string strPass;\
	if (!m_Device.GetUserPasswd(strUser, strPass) || !m_Device.GetPTZUrl(strUrl))\
		{\
		Soap_Trace("%s %s:%s Error in Getting Url or User/Passsword.\n",m_Device.GetDevIP(),strClassName.c_str(),#pFunc);\
		return SOAP_ERR;\
		}\
	SoapProxy.soap_endpoint = strUrl.c_str();\
	if (httpinfo)\
		http_da_restore(&SoapProxy, httpinfo);\
	nResult = SoapProxy.pFunc;\
	if (nResult != SOAP_OK)\
		Soap_Trace("%s %s:%s  Result = %d\tSoap Status = %d.\n",m_Device.GetDevIP(),strClassName.c_str(),#pFunc,nResult,SoapProxy.status);\
	if (nResult != SOAP_OK &&\
		SoapProxy.status == 401)\
		{\
		if (SoapProxy.userid)\
			SOAP_FREE(nullptr,(void *)SoapProxy.userid);\
		if (SoapProxy.passwd)\
			SOAP_FREE(nullptr,(void *)SoapProxy.passwd);\
		SoapProxy.userid = soap_strdup(nullptr, strUser.c_str());\
		SoapProxy.passwd = soap_strdup(nullptr, strPass.c_str());\
		if (!httpinfo)\
				{\
			soap_register_plugin(&SoapProxy, http_da); \
			httpinfo = new http_da_info;\
			http_da_save(&SoapProxy, httpinfo, SoapProxy.authrealm, SoapProxy.userid, SoapProxy.passwd);\
				}\
				else\
			http_da_restore(&SoapProxy, httpinfo);\
		nResult = SoapProxy.pFunc;\
		if (nResult != SOAP_OK)\
			Soap_Trace("%s %s:%s  Result = %d\tSoap Status = %d.\n",m_Device.GetDevIP(),strClassName.c_str(),#pFunc,nResult,SoapProxy.status);\
		}\
	return nResult;\
}

#define  SuperHttpCall(SuperProxy,pFunc,nResult) {\
	int nResult = 0;\
	string strUrl;\
	string strUser;\
	string strPass;\
	if (!m_Device.GetUserPasswd(strUser, strPass) || !m_Device.GetPTZUrl(strUrl))\
		{\
		Soap_Trace("%s %s:%s Error in Getting Url or User/Passsword.\n",m_Device.GetDevIP(),strClassName.c_str(),#pFunc);\
		return SOAP_ERR;\
		}\
	SuperProxy->soap_endpoint = strUrl.c_str();\
	if (httpinfo)\
		http_da_restore(SuperProxy, httpinfo);\
	nResult = SuperProxy->pFunc;\
	if (nResult != SOAP_OK)\
		Soap_Trace("%s %s:%s  Result = %d\tSoap Status = %d.\n",m_Device.GetDevIP(),strClassName.c_str(),#pFunc,nResult,SuperProxy->status);\
	if (nResult != SOAP_OK &&\
		SuperProxy->status == 401)\
		{\
		SuperProxy->userid = strUser.c_str();\
		SuperProxy->passwd = strPass.c_str();\
		if (!httpinfo)\
		{\
			soap_register_plugin(SuperProxy, http_da); \
			httpinfo = new http_da_info;\
			http_da_save(SuperProxy, httpinfo, SuperProxy->authrealm, SuperProxy->userid, SuperProxy->passwd);\
		}\
		else\
			http_da_restore(SuperProxy, httpinfo);\
		nResult = SuperProxy->pFunc;\
		if (nResult != SOAP_OK)\
			Soap_Trace("%s %s:%s  Result = %d\tSoap Status = %d.\n",m_Device.GetDevIP(),strClassName.c_str(),#pFunc,nResult,SuperProxy->status);\
	}\
	return nResult;\
}


// TODO: reference additional headers your program requires here
