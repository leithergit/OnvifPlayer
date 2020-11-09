#ifndef _MYSQL_CONNECTOR_H_2015_02_05
#define _MYSQL_CONNECTOR_H_2015_02_05

/////////////////////////////////////////////////////////////////////////
/// COPYRIGHT NOTICE  
/// Copyright (c) 2015 leither
/// All rights reserved.  
///  
/// @file  MySQLConnector.h
/// @brief MySQL C API���ܷ�װ,����:
///		   CMySQLConnectorΪ������,����ά��Mysql�����Ӻ�ִ�в�ѯ; 
///		   CMySQLResult����CMySQLConnectorc����Ĳ�ѯ�����,���ṩ��������ʽӿ�
///		   CMysqlValue Ϊ������е��ֶη����ṩ���ʽӿ�
///		   CMySQLExcpetion Ϊ�쳣������

/// @version 1.21 
/// @author  leither908@gmail.com	QQ:4103774
/// @date    2015.02.02
/// @remark
///        ���뻷������Ҫvc2008����
///		   ��������_STL_SMARTPTR�꣬��ʹ��vc2008�ڲ�������ָ��,��δ������ʹ��boost
///        ������ָ�룬��Ҫ����boost���·��
/////////////////////////////////////////////////////////////////////////
#pragma once
#include <WinSock2.h>
#include <vector>
#include <string>
#include <map>
#include <exception>
#include <Math.h>
#include <list>
#include <assert.h>
#include <mysql.h>

using namespace std;
#ifdef _STD_SMARTPTR
#include <memory>
using namespace std::tr1;
#else
#include <boost/shared_ptr.hpp>
using namespace boost;
#endif

#pragma comment(lib,"libmysql.lib")

#define FreeVector(V,T) {vector<T> Temp;V.swap(Temp);}

#define		MYSQL_SUCCEEDED(hr)   (((UINT)(hr)) == 0)
#define		_MYSQL_DEFAULT_PORT		3306		// Ĭ�����Ӷ˿�3306
#define		_MYSQL_CONNECT_TIMEOUT	10			// Ĭ������ʱ��10��
#define		_MYSQL_READ_TIMEOUT		120			// Ĭ�϶�ȡʱ��60��
#define		_MYSQL_WRITE_TIMEOUT	180			// Ĭ��д��ʱ��120��

class CMyValue
{
public:
	CMyValue()
	{
		szData = NULL;
		nDateLength = NULL;
	}

	CMyValue(const char *szSrcData,int nSrcLength ,enum_field_types nFT)
	{
		SetMyValue(szSrcData,nSrcLength,nFT);
	}

	void SetMyValue(const char *szSrcData,int nSrcLength ,enum_field_types nFT)
	{
		szData = (char *)szSrcData;
		nFieldType = nFT;
		if (!nSrcLength)
		{
			if (szSrcData)
				nDateLength = strlen(szSrcData);
			else
				nDateLength = 0;
		}
		else
			nDateLength = nSrcLength;
	}

	~CMyValue()
	{
		int nBreak = 3;
	}
	const CMyValue & operator =(const char *szSrcData)
	{
		szData = (char *)szSrcData;
		if (szSrcData)
			nDateLength = strlen(szSrcData);
		else
			nDateLength = 0;
	}
	operator byte ()const
	{
		if (szData)
			return (byte)LOBYTE(LOWORD(atoi(szData)));
		else
			return 0;
	}
	operator char ()const
	{
		if (szData)
			return (char)LOBYTE(LOWORD(atoi(szData)));
		else
			return 0;
	}
	operator byte* ()const
	{
		if (szData)
			return (byte *)szData;
		else
			return (byte *)"NULL";
	}

	operator char* ()const
	{
		if (szData)
			return szData;
		else
			return (char *)"NULL";
	}
	operator short()const
	{
		if (szData)
			return (short)LOWORD(atoi(szData));
		else
			return 0;
	}
	operator unsigned short()const
	{
		if (szData)
			return LOWORD(atoi(szData));
		else
			return 0;
	}
	operator int() const
	{
		if (szData)
			return atoi(szData);
		else
			return 0;
	}
	operator unsigned int () const
	{
		if (szData)
			return (unsigned int)atoi(szData);
		else
			return 0;
	}

	operator __int64 ()const
	{
		if (szData)
			return _atoi64(szData);
		else
			return 0;
	}

	operator unsigned __int64 ()const
	{
		if (szData)
			return (unsigned __int64)_atoi64(szData);
		else
			return 0;
	}
	operator float () const
	{
		if (szData)
		{
			return (float)atof(szData);
		}
		else
			return 0.0f;
	}

	operator double() const
	{
		if (szData)
		{
			return atof(szData);
		}
		else
			return 0.0f;
	}

	bool GetBlob(byte *pBlobBuff,int nBuffSize)
	{
		if (!pBlobBuff || !nBuffSize)
			return false;
		memcpy_s(pBlobBuff,nBuffSize,szData,nDateLength);
		return true;
	}
private:
	char	*szData;
	int		nDateLength;
	_CRT_FLOAT 	fValue;
	_CRT_DOUBLE	dfValue;
	enum_field_types nFieldType;
};

class  CMySQLException : public std::exception
{
public:
	CMySQLException(const CMySQLException& e) throw() 
		: std::exception(e)
		, what_(e.what_)
		, what_w(e.what_w)
	{
	}

	CMySQLException& operator=(const CMySQLException& e) throw()
	{
		what_ = e.what_;
		what_w = e.what_w;
		return *this;
	}

	~CMySQLException() throw() { }

	
	CMySQLException(const char* w = "") throw()
		: what_(w)
	{
		int nLen = strlen(w);
		what_w.resize(nLen,L' ');
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)w,nLen,(LPWSTR)what_w.c_str(),nLen);
	}
	CMySQLException(const std::string& w) throw()
		: what_(w)
	{
		int nLen = (int)w.length();
		what_w.resize(nLen,L' ');
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)w.c_str(),nLen,(LPWSTR)what_w.c_str(),nLen);
	}
	CMySQLException(MYSQL *pDataBase) throw()		
	{
		char	szError[4096] = {0};
		WCHAR	szErrorW[4096] = {0};
		if (mysql_errno(pDataBase))
		{
			strcpy_s(szError,_TRUNCATE,mysql_error(pDataBase));
			MultiByteToWideChar(CP_ACP, 0, szError, -1, szErrorW, 4096);
		}
		else
		{
			strcpy_s(szError,4096,"Error unknown.");
			wcscpy_s(szErrorW,4096,L"Error unknown.");
		}
		what_ = szError;
		what_w = szErrorW;
	}

	virtual const char* what() const throw()
	{
		return what_.c_str();
	}

	virtual const WCHAR* whatW() const throw()
	{
		return what_w.c_str();
	}
	
	std::string what_;
	std::wstring what_w;
};

//////////////////////////////////////////////////////////////////////////
// ע��:
// �κ�ʱ�򶼲�Ҫ�����ڶ��̼߳�ʹ��CMysqlResult,�����û��Ϊ�̰߳�ȫ���κ� 
// ������ʩ,�������Լ����������̰߳�ȫ����
//////////////////////////////////////////////////////////////////////////

class CMyResult
{
private:	
	map<string,UINT> mapFieldIndex;
	vector<MYSQL_FIELD> FieldVector;
	MYSQL_RES	*m_pResult;	
	MYSQL_ROW	m_pCurRow;		// ��ǰ��
	bool		m_bSucceed;
	ULONGLONG	m_nAffectedrow;
	ULONGLONG	m_nRows;		// ����
	bool		m_bDroped;		// �Ƿ񱻸���
	int			m_nFields;
	ULONG		*m_pFieldLengths;		// ��ǰ�������ֶ�ֵ�ĳ�������
	shared_ptr<CMyValue> m_pCurValue;
public:
	CMyResult()
		:m_pResult(NULL)
		//,m_pFields(NULL)
 		,m_pCurRow(NULL)
		,m_nFields(0)
		,m_nRows(0)
		,m_pFieldLengths(NULL)
		,m_bSucceed(false)
		,m_nAffectedrow(0)
		,m_bDroped(false)
		,m_pCurValue(new CMyValue)
	{
	}
	// ��ѯʧ��ʱ�����ؿյ�ʧ�ܽ����
	CMyResult(bool bSucceed)
		:m_pResult(NULL)
 		,m_pCurRow(NULL)
		,m_nFields(0)
		,m_nRows(0)
		,m_pFieldLengths(NULL)
		,m_bSucceed(bSucceed)
		,m_nAffectedrow(0)
		,m_bDroped(false)
		,m_pCurValue(new CMyValue)
	{
	}

	CMyResult(CMyResult &Result)
	{		
		m_pResult		 = Result.m_pResult;
		m_bDroped		 = false;
		Result.m_bDroped = true;
		m_nFields		 = Result.m_nFields;
		m_nRows			 = Result.m_nRows;
		m_pFieldLengths	 = Result.m_pFieldLengths;
		m_bSucceed		 = Result.m_bSucceed;
		m_nAffectedrow	 = Result.m_nAffectedrow;
		m_pCurValue		 = Result.m_pCurValue;		
		mapFieldIndex.swap(Result.mapFieldIndex);
		FieldVector.swap(Result.FieldVector);
	}
		
	// �޽����������Ӱ���¼���ǵ���
	CMyResult(MYSQL *pDataBase)
		:m_pResult(NULL)
		//,m_pFields(NULL)
 		,m_pCurRow(NULL)
		,m_nFields(0)
		,m_nRows(0)
		,m_pFieldLengths(NULL)
		,m_bSucceed(true)
		,m_bDroped(false)
		,m_pCurValue(new CMyValue)
	{
		if (pDataBase)
		{
			m_nAffectedrow = mysql_affected_rows(pDataBase);
			m_bSucceed = true;
		}
		else
		{
			m_bSucceed = false;
			//throw CMySQLException("invalid mysql handle.");
		}
	}
	// ���ؽ����ʱ����
	CMyResult(MYSQL_RES *pResult)
		:m_nAffectedrow(0)
		,m_bDroped(false)
		,m_pCurValue(new CMyValue)
	{
		if (pResult)
		{			
			m_bSucceed = true;			
			m_pResult = pResult;			
			m_nFields = mysql_num_fields(pResult);			
			if (m_nFields)
			{
				MYSQL_FIELD *pField = NULL;
				m_pCurRow = mysql_fetch_row(m_pResult);
				m_nRows = mysql_num_rows(m_pResult);
				m_pFieldLengths = mysql_fetch_lengths(m_pResult);
				UINT nIndex = 0;
				while((pField = mysql_fetch_field(m_pResult)))
				{
					mapFieldIndex.insert(pair<string,UINT>(pField->name,nIndex));
					FieldVector.push_back(*pField);
					nIndex ++;
				}
			}
			else		
				throw CMySQLException("Field not found in the Result.");
		}
		else
			throw CMySQLException("Invalid Result.");
	}

	CMyResult& operator = (MYSQL_RES *pResult)
	{
		if (pResult)
		{			
			m_pResult = pResult;
			m_nFields = mysql_num_fields(pResult);
			if (m_nFields)
			{
				MYSQL_FIELD *pField = NULL;
				m_pCurRow = mysql_fetch_row(m_pResult);
				m_pFieldLengths = mysql_fetch_lengths(m_pResult);
				UINT nIndex = 0;
				while((pField = mysql_fetch_field(m_pResult)))
				{
					mapFieldIndex.insert(pair<string,UINT>(pField->name,nIndex));
					FieldVector.push_back(*pField);
					nIndex ++;
				}
			}
			else
				throw CMySQLException("Field not found in the Result.");
			return *this;
		}
		else
			throw CMySQLException("Invalid Result.");
	}
	CMyResult& operator = (CMyResult &Result)
	{
		if (!m_bDroped && m_pResult)
			mysql_free_result(m_pResult);
		m_pResult		 = Result.m_pResult;
		m_bDroped		 = false;
		Result.m_bDroped = true;
		m_nFields		 = Result.m_nFields;
		m_nRows			 = Result.m_nRows;
		m_pCurRow		 = Result.m_pCurRow;
		m_pFieldLengths	 = Result.m_pFieldLengths;
		//m_pCurRowObj	 = Result.m_pCurRowObj;
		m_bSucceed		 = Result.m_bSucceed;
		m_nAffectedrow	 = Result.m_nAffectedrow;
		mapFieldIndex.swap(Result.mapFieldIndex);
		FieldVector.swap(Result.FieldVector);
		return *this;
	}

	static CMyResult FromResult(MYSQL_RES *pResult)
	{
		if (pResult)
			return CMyResult(pResult);
		else
			throw CMySQLException("Invalid Result.");
	}

	static CMyResult FromResult(bool bResult)
	{
		return CMyResult(bResult);		
	}
	void Reset()
	{
		if (!m_bDroped && m_pResult)
		{
			m_pCurRow		= NULL;			
			m_nFields		= NULL;
			m_nRows			= 0;
			m_pFieldLengths	= NULL;
			m_bSucceed		= false;
			m_nAffectedrow	= 0;
			m_bDroped		= false;
			mapFieldIndex.clear();
			FreeVector(FieldVector,MYSQL_FIELD);
			mysql_free_result(m_pResult);
			m_pResult = NULL;
		}
	}
	
	~CMyResult()
	{
		if (!m_bDroped && m_pResult)
			mysql_free_result(m_pResult);
	}
	const MYSQL_FIELD &GetField(const int nColIndex)
	{
		if (m_pResult && !m_bDroped)
		{
			if (nColIndex >=0 && nColIndex < m_nFields)
				return FieldVector[nColIndex];
				//return mysql_fetch_field_direct(m_pResult,nColIndex);
			else
				throw CMySQLException("The column out of bound.");
		}
		else
			throw CMySQLException("Invalid result.");
	}

	const char *GetValue(const char *szColName)
	{
		if (m_pResult && !m_bDroped)
		{
			if (!szColName || !strlen(szColName))
				throw CMySQLException("Invalid field name");
			map<string,UINT>::iterator it = mapFieldIndex.find(szColName);
			if (it != mapFieldIndex.end())
			{
				return m_pCurRow[it->second];
			}
			else
			{
				CHAR szException[255] = {0};
				sprintf_s(szException,255,"The field '%s' not found.",szColName);
				throw CMySQLException(szException);
			}
		}
		else
			throw CMySQLException("Invalid result.");
	}

	const char *GetValue(const int nColIndex)
	{
		if (m_pResult && !m_bDroped)
		{
			if (nColIndex >=0 && nColIndex < m_nFields)
				return m_pCurRow[nColIndex];
			else
				throw CMySQLException("The column out of bound.");
		}
		else
			throw CMySQLException("Invalid result.");
	}

	const CMyValue operator[](const char *szColName)
	{
		if (m_pResult && !m_bDroped)
		{
			if (!szColName || !strlen(szColName))
				throw CMySQLException("Invalid field name");
			map<string,UINT>::iterator it = mapFieldIndex.find(szColName);
			if (it != mapFieldIndex.end())
			{
				int nIndex = it->second;
				m_pCurValue->SetMyValue(m_pCurRow[nIndex],m_pFieldLengths[nIndex],FieldVector[nIndex].type);
				return *m_pCurValue;
			}
			else
			{
				CHAR szException[255] = {0};
				sprintf_s(szException,255,"The field '%s' not found.",szColName);
				throw CMySQLException(szException);
			}
		}
		else
			throw CMySQLException("Invalid result.");
	}

	const CMyValue &operator[](const int nColIndex)
	{
		if (m_pResult && !m_bDroped)
		{
			if (nColIndex >=0 && nColIndex < m_nFields)
			{
				m_pCurValue->SetMyValue(m_pCurRow[nColIndex],m_pFieldLengths[nColIndex],FieldVector[nColIndex].type);
				return *m_pCurValue;
			}				
			else
				throw CMySQLException("The column out of bound.");
		}
		else
			throw CMySQLException("Invalid result.");
	}

	const MYSQL_ROW  &operator ++()
	{
		if (m_pResult && !m_bDroped)
		{
			m_pCurRow= mysql_fetch_row(m_pResult);
			if (m_pCurRow)
			{
				m_pFieldLengths = mysql_fetch_lengths(m_pResult);
				if (!m_pFieldLengths)
					throw CMySQLException("The MySQL API mysql_fetch_lengths() return null.");
			}			
			return m_pCurRow;
		}
		else
			throw CMySQLException("Invalid result.");
	}

	// ����true������
	// ������
	// �������ɹ�
	const bool operator !()
	{
		return(m_bDroped || !m_bSucceed);		
	}
// 	operator bool()const
// 	{
// 		return (!m_bDroped && m_bSucceed);
// 	}
	const int &FieldCount()
	{
		return m_nFields;
	}
	const ULONGLONG &Affectedrow()
	{
		return m_nAffectedrow;
	}
	const ULONGLONG &RowCount()
	{
		return m_nRows;
	}
	const bool &IsCopyied()
	{
		return m_bDroped;
	}
	const ULONG & FieldLength(int nColIndex)
	{
		if (m_pFieldLengths)
			return m_pFieldLengths[nColIndex];
		else
			throw CMySQLException("m_pFieldLengths has not be filled.");
	}
};
//////////////////////////////////////////////////////////////////////////
// ע��:
// �κ�ʱ�򶼲�Ҫ�����ڶ��̼߳�ʹ��CMySQLConnector,�����û��Ϊ�̰߳�ȫ���κ� 
// ������ʩ,�������Լ����������̰߳�ȫ����
//////////////////////////////////////////////////////////////////////////

class CMySQLConnector
{

public:
	CMySQLConnector(void)
	{
		mysql_library_init(0,NULL,NULL); 	
		if (!mysql_init(&m_myDataBase))
		{
			throw CMySQLException("insufficent memory.");
		}
		m_nPort = _MYSQL_DEFAULT_PORT;
		m_bConnected = false;
		m_nConnectFlag = CLIENT_MULTI_STATEMENTS|CLIENT_MULTI_RESULTS;
		ZeroMemory(m_szErrorMsg,1024);
		ZeroMemory(m_szServer,64);
		ZeroMemory(m_szAccount,32);
		ZeroMemory(m_szPWD,32);
		ZeroMemory(m_szCharSet,16);
		ZeroMemory(m_szDBName,128);
	}

	CMySQLConnector(CHAR *szServer,CHAR *szAccount,CHAR *szPWD,CHAR *szDbName = NULL,WORD nPort = 3306,
		ULONG nConnectFlag = CLIENT_MULTI_STATEMENTS|CLIENT_MULTI_RESULTS,CHAR *szCharSet = "UTF8") 
	{
		mysql_library_init(0,NULL,NULL); 
		if (!mysql_init(&m_myDataBase))
		{
			throw CMySQLException("insufficent memory.");
		}
		ResetData();
// 		ZeroMemory(m_szErrorMsg,1024);
// 		ZeroMemory(m_szServer,64);
// 		ZeroMemory(m_szAccount,32);
// 		ZeroMemory(m_szPWD,32);
// 		ZeroMemory(m_szCharSet,16);
// 		ZeroMemory(m_szDBName,128);
		if (!Connect(szServer,szAccount,szPWD,szDbName,nPort,nConnectFlag,szCharSet))
		{
			if (mysql_errno(&m_myDataBase))
				sprintf_s(m_szErrorMsg,1024,"Connecting Server %s@%s:%d#%s failed,Error:%s.\n",szAccount,szServer,nPort,szDbName,mysql_error(&m_myDataBase));
			else
				sprintf_s(m_szErrorMsg,1024,"Connecting Server %d@%s:%d#%s failed,Error:Unknown.\n",szAccount,szServer,nPort,szDbName);
			throw CMySQLException(m_szErrorMsg);
		}
		m_bConnected = true;
	}
	~CMySQLConnector()
	{		
		Disconnect();
		ResetData();	
	}
	bool SetMysqlOptions(mysql_option option,void *pArgs)
	{
		if (!mysql_options(&m_myDataBase,option,pArgs))
		{
			sprintf_s(m_szErrorMsg,1024,"Set mysql options failed,Error:%s.\n",mysql_error(&m_myDataBase));
			throw CMySQLException(m_szErrorMsg);			
		}
		else
			return true;
	}
	bool Connect(LPCSTR szServer,LPCSTR szAccount,LPCSTR szPWD,LPCSTR szDbName = NULL,WORD nPort = 3306,
		ULONG nConnectFlag = CLIENT_MULTI_STATEMENTS|CLIENT_MULTI_RESULTS,CHAR *szCharSet = "UTF8")
	{
		if (!szServer || 
			!szAccount ||
			!szPWD)
			return false;
		m_bConnected = false;
		__try
		{
			ZeroMemory(m_szErrorMsg,1024);
			
			int nConnectTimeout =  _MYSQL_CONNECT_TIMEOUT;
			if (mysql_options(&m_myDataBase,MYSQL_OPT_CONNECT_TIMEOUT,&nConnectTimeout))
				__leave;
			int nReadTimeout = _MYSQL_READ_TIMEOUT;			
			if (mysql_options(&m_myDataBase,MYSQL_OPT_READ_TIMEOUT,&nReadTimeout))
				__leave;
			int nWriteTimeout = _MYSQL_READ_TIMEOUT;			
			if (mysql_options(&m_myDataBase,MYSQL_OPT_WRITE_TIMEOUT,&nReadTimeout))
				__leave;
			
			bool bReconnect = true;
			if (mysql_options(&m_myDataBase,MYSQL_OPT_RECONNECT,&bReconnect))
				__leave;			

			if (mysql_options(&m_myDataBase,MYSQL_SET_CHARSET_NAME,szCharSet))
				__leave;
			
			if (!mysql_real_connect(&m_myDataBase,szServer,szAccount,szPWD,szDbName,nPort,NULL,nConnectFlag))
				__leave;

			
			strcpy_s(m_szServer,64,szServer);
			strcpy_s(m_szAccount,32,szAccount);
			strcpy_s(m_szPWD,32,szPWD);
			strcpy_s(m_szDBName,128,szDbName);
			m_bConnected = true;
		}
		__finally
		{
			if (!m_bConnected)
			{
				const char *szError = mysql_error(&m_myDataBase);
				if (mysql_errno(&m_myDataBase))
					sprintf_s(m_szErrorMsg,1024,"Connecting server %d@%s:%d#%s failed,Error:%s.\n",szAccount,szServer,nPort,szDbName,mysql_error(&m_myDataBase));
				else
					sprintf_s(m_szErrorMsg,1024,"Connecting Server %d@%s:%d#%s failed,Error:Unknown.\n",szAccount,szServer,nPort,szDbName);
			}
		}
		return m_bConnected;		
	}
	bool IsConnected()
	{
		return m_bConnected;
	}
	void Disconnect()
	{
		if (!m_bConnected)
			return;
		if (&m_myDataBase)
		{
			mysql_close(&m_myDataBase);	
			m_bConnected = false;
		}
	}
	// ִ�ж�����sql���,�����޽������sql��ѯ
	// ����ִ���޽�������ص�sql���,���ִ�в�ѯ���,��ֻ���ؽ����������
	ULONGLONG ExecuteSQL(LPCSTR szSql,ULONG nMysqlLength)
	{
		if (!m_bConnected)
			throw CMySQLException("please connect to the mysql server at first.");

		if (!szSql ||
			nMysqlLength == 0)
			throw CMySQLException("Invalid SQL statement.");

		ResetResult();				// ���ý����,��Ȼ���ܻ����ͬ������
		MYSQL_RES *pResult = NULL;
		if (mysql_real_query(&m_myDataBase,szSql,nMysqlLength))
		{
			if (mysql_errno(&m_myDataBase))
				sprintf_s(m_szErrorMsg,1024,"Query failure,SQL = %s,Error = %s.\n",szSql,mysql_error(&m_myDataBase));
			else
				sprintf_s(m_szErrorMsg,1024,"Query failure,SQL = %s.\n",szSql);
			throw CMySQLException(m_szErrorMsg);
		}
		pResult = mysql_store_result(&m_myDataBase);
		shared_ptr<MYSQL_RES> Deallocator(pResult,mysql_free_result);		
		if (pResult)									// �н��������
		{	
			return mysql_num_fields(pResult);			// ���ص�����
		}
		else											// �޽����,�������ԭ��
		{
			int nErrorNo = mysql_errno(&m_myDataBase);
			if (nErrorNo == 0)							// ����ִ��
			{
				return  mysql_affected_rows(&m_myDataBase);				
			}
			else										// �д�����
			{
				throw CMySQLException(&m_myDataBase);
			}
		}
	}

	ULONGLONG ExecuteSQL(LPCSTR pFormat,...)
	{
		if (!m_bConnected)
			throw CMySQLException("please connect to the mysql server at first.");

		ResetResult();				// ���ý����,��Ȼ���ܻ����ͬ������
		shared_ptr <char> szSQL(new char [0xFFFF]);
		va_list args;
		va_start(args, pFormat);
		int nSQLLen = _vsnprintf_s(szSQL.get(), 0xFFFF,_TRUNCATE,pFormat, args);
		if (nSQLLen == -1)
			throw CMySQLException("The size of the input arguments for SQL is more then 64K.");		
		va_end(args);
		return ExecuteSQL(szSQL.get(),nSQLLen);
	}
	// ִ��sql��ѯ�������ؽ����	
	CMyResult QueryBinary(LPCSTR szSql,ULONG nMysqlLength)
	{
		if (!m_bConnected)
			throw CMySQLException("please connect to the mysql server at first.");
		
		if (!szSql ||
			nMysqlLength == 0)
			throw CMySQLException("Invalid SQL statement.");
		ResetResult();				// ���ý����,��Ȼ���ܻ����ͬ������
		MYSQL_RES *pResult = NULL;
		if (mysql_real_query(&m_myDataBase,szSql,nMysqlLength))
		{
			if (mysql_errno(&m_myDataBase))
			{
				if (strlen(szSql) < 900)
					sprintf_s(m_szErrorMsg,1024,"Query failure,SQL = %s,Error = %s.\n",szSql,mysql_error(&m_myDataBase));
				else
					sprintf_s(m_szErrorMsg,1024,"Query failure,Error = %s.\n",mysql_error(&m_myDataBase));
			}
			else
				sprintf_s(m_szErrorMsg,1024,"Query failure,SQL = %s.\n",szSql);
			throw CMySQLException(m_szErrorMsg);
		}
		pResult = mysql_store_result(&m_myDataBase);
		if (pResult)
		{// �н��������
#if _DEBUG
			int nNumFields = mysql_num_fields(pResult);			// ���ص�����
#endif			
			return CMyResult::FromResult(pResult);
		}
		else
		{// �޽����,�������ԭ��
			int nErrorNo = mysql_errno(&m_myDataBase);
			if (nErrorNo == 0)
			{// ����ִ��
				//nResult = mysql_affected_rows(&m_myDataBase);
				return CMyResult(&m_myDataBase);
			}
			else	// �д�����
			{
				throw CMySQLException(&m_myDataBase);
			}
		}	
	}
	
	// ִ�ж�����SQL��䣬�����ؽ����
	CMyResult Query(LPCSTR szSql,int nSQLLength)
	{
		if (!m_bConnected)
			throw CMySQLException("please connect to the mysql server at first.");
		
		if (!szSql ||
			nSQLLength == 0)
			throw CMySQLException("Invalid SQL statement.");
		return QueryBinary(szSql,nSQLLength);	
	}
	// ʹ�ø�ʽ���ı�,���뵽�ı��ܳ��Ȳ��˳���64K
	CMyResult Query(LPCSTR pFormat,...)
	{
		shared_ptr <char> szSQL(new char [0xFFFF]);
		va_list args;
		va_start(args, pFormat);
		int nSQLLen = _vsnprintf_s(szSQL.get(), 0xFFFF,_TRUNCATE,pFormat, args);
		if (nSQLLen == -1)
			throw CMySQLException("The size of the input arguments for SQL is more then 64K.");		
		va_end(args);
		return QueryBinary(szSQL.get(),nSQLLen);
	}
	// ȡ��һ�����
	bool GetNextResult(CMyResult &Result)
	{
		Result.Reset();
		int nNextResult = mysql_next_result(&m_myDataBase);
		if (nNextResult == 0)		// �ж�������
		{			
			MYSQL_RES *pResult = NULL;
			pResult = mysql_store_result(&m_myDataBase);
			if (pResult)
			{// �н��������
				int nNumFields = mysql_num_fields(pResult);			// ���ص�����
				Result = CMyResult::FromResult(pResult);
				return true;
			}
			else
			{// �޽����,�������ԭ��
				int nErrorNo = mysql_errno(&m_myDataBase);
				if (nErrorNo == 0)			// ִ�гɹ�
				{
					Result = CMyResult(&m_myDataBase);
					return false;
				}
				else						// �д�����
					throw CMySQLException(&m_myDataBase);
			}
		}
		else if (nNextResult == -1)
		{// û�и���Ľ����
			return false;
		}
		else
			throw CMySQLException("There is no more result.");
		return false;
	}
	// ���ý����
	void ResetResult()
	{
		MYSQL_RES *res = NULL;
		do  
		{  
			res = mysql_store_result(&m_myDataBase);  
			mysql_free_result(res);  
		}
		while( !mysql_next_result(&m_myDataBase )); 
	}

	size_t escape_string(char* to, const char* from, size_t length)
	{		
		return mysql_real_escape_string(&m_myDataBase, to, from, static_cast<unsigned long>(length));
	}
	const char* GetErrorMsg()
	{
		return (const char *)m_szErrorMsg;
	}
	CMyResult Attach(MYSQL &myDatabase,CHAR *szSQL = NULL)
	{
		ResetData();
		m_myDataBase = myDatabase;
		if (!szSQL)
		{
			return Query(szSQL);
		}
		else
			return CMyResult(false);
	}
	
	void Detach()
	{
		Disconnect();
		ResetData();
	}


	void ResetData()
	{
		int nOffset = offsetof(CMySQLConnector,m_bConnected);
		int nSize = sizeof(CMySQLConnector);
		ZeroMemory(&m_bConnected,sizeof(CMySQLConnector) - offsetof(CMySQLConnector,m_bConnected));
// 		ZeroMemory(m_szServer,64);
// 		ZeroMemory(m_szAccount,32);
// 		ZeroMemory(m_szPWD,32);
// 		ZeroMemory(m_szCharSet,16);
// 		ZeroMemory(m_szDBName,128);
	}
private:
		
	 MYSQL	m_myDataBase;
	 CMyResult m_Result;
	 bool	m_bConnected;	
	 WORD	m_nPort;
	 CHAR	m_szErrorMsg[1024];
	 CHAR	m_szServer[64];	 
	 CHAR	m_szAccount[32];
	 char	m_szPWD[32];
	 char	m_szCharSet[16];
	 char	m_szDBName[128];
	 ULONG	m_nConnectFlag;
	
};


#endif	//_MYSQL_CONNECTOR_H_2015_02_05
