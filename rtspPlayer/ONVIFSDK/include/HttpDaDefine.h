#pragma once
#pragma message( "Compiling " __FILE__ ) 
#include <windows.h>

void Soap_Trace(LPCSTR pFormat, ...);
bool ParseHttpURL(char const* szUrl, char* szHost, WORD& nPort, char* szUser, char* szPassword, const char ** urlSuffix);

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