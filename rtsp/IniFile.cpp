//#include "StdAfx.h"

#include "IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////
void CIniFile::ReadSection(std::vector<std::string>& setArray)
{
	TCHAR szBuf[1024*64];
	memset(szBuf, 0, sizeof(szBuf));
	TCHAR* p = szBuf;
	int nLen = 0;

	if (GetPrivateProfileString(NULL, NULL, "", 
		szBuf, sizeof(szBuf)/sizeof(TCHAR), 
		m_strFile.c_str()) > 0)
	{
		while (*p != '\0')
		{
			setArray.push_back(p);
			nLen = (int)strlen(p) + 1;
			p += nLen;
		}  
	}
}

void CIniFile::ReadSectionEntry(const TCHAR* pSection, std::vector<std::string>& setArray)
{
    TCHAR szBuf[1024*64] = {0};
	TCHAR* p = szBuf;
	int nLen = 0;
	if (GetPrivateProfileString(pSection, NULL, "", 
								szBuf, sizeof(szBuf)/sizeof(TCHAR), 
								m_strFile.c_str()) > 0)
	{
		while (*p != '\0')
		{
			setArray.push_back(p);
			nLen = (int)strlen(p) + 1;
			p += nLen;
		}  
	}
}

//void CIniFile::ReadSectionEntry(const TCHAR* pSection, std::vector<std::string>& setArray)
//{
//    TCHAR szBuf[1024*64] = {0};
//    TCHAR* p = szBuf;
//    int nLen = 0;
//    if (GetPrivateProfileString(pSection, NULL, "", 
//        szBuf, sizeof(szBuf)/sizeof(TCHAR), 
//        m_strFile.c_str()) > 0)
//    {
//        while (*p != '\0')
//        {
//            setArray.push_back(/*CString(p)*/p);
//            nLen = (int)strlen(p) + 1;
//            p += nLen;
//        }  
//    }
//}

void CIniFile::ReadSectionString(const TCHAR* pSection, std::vector<std::string>& setArray)
{
	std::vector<std::string> ayKey;
	std::string strItem;
	ReadSectionEntry(pSection, ayKey);
	for (int i=0; i < ayKey.size(); ++i)
	{
		ReadString(pSection, ayKey[i].c_str(), strItem);
		if (0 != strItem.length())
			setArray.push_back(strItem);
	}
}

bool CIniFile::ReadString(const char* pSection, const char* pEntry, std::string& strItem)
{

	char szReturn[1024*4];
	memset(szReturn, 0, sizeof(szReturn));

	strItem.empty();
	if (GetPrivateProfileString(pSection, pEntry, "", 
								szReturn, _countof(szReturn), 
								m_strFile.c_str()) > 0)
	{
		strItem = szReturn;
		return true;
	}
	else
		return false;
}

bool CIniFile::ReadInt(const TCHAR* pSection, const TCHAR* pEntry, int& nValue)
{
	TCHAR szReturn[32];
	memset(szReturn, 0, sizeof(szReturn));
	if (GetPrivateProfileStringA(pSection, pEntry, "", 
								szReturn, _countof(szReturn), 
								m_strFile.c_str()) > 0)
		nValue = atoi(szReturn);
	else
		return false;
}

bool CIniFile::ReadInt(const char* pSection, const char* pEntry, short& nValue)
{
	int nIntValue = 0;
	if (ReadInt(pSection, pEntry, nIntValue))
		nValue = (short)nIntValue;
	else
		return false;
}

void CIniFile::WriteString(const TCHAR* pSection, const TCHAR* pEntry, const TCHAR* pItem)
{
	WritePrivateProfileString(pSection, pEntry, pItem, m_strFile.c_str());
}

void CIniFile::WriteInt(const TCHAR* pSection, const TCHAR* pEntry, int nValue)
{
	TCHAR szValue[32];
	sprintf_s(szValue, "%d", nValue);
	WriteString(pSection, pEntry, szValue);
}

void CIniFile::EraseSection(const TCHAR* pSection)
{
	WritePrivateProfileStruct(pSection, NULL, NULL, 0, m_strFile.c_str());
}
//////////////////////////////////////////////////////////////////////////////////////////
