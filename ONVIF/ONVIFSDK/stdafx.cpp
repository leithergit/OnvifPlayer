// stdafx.cpp : source file that includes just the standard includes
// ONVIFLIB.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "vadefs.h"
#include "stdarg.h"
#include "stdio.h"
#include <windows.h>

// and not in this file
#define __countof(array) (sizeof(array)/sizeof(array[0]))
#pragma warning (disable:4996)
void Soap_Trace(LPCSTR pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);
	int nBuff;
	CHAR szBuffer[0x7fff];
	nBuff = _vsnprintf(szBuffer, __countof(szBuffer), pFormat, args);
	OutputDebugStringA(szBuffer);
	va_end(args);
}

bool ParseHttpURL(char const* szUrl, char* szHost, WORD& nPort, char* szUser, char* szPassword, const char** urlSuffix)
{
	char const* prefix = "http://";
	unsigned const prefixLength = 7;
	unsigned const parseBufferSize = 512;
	char parseBuffer[parseBufferSize];
	char const* from = &szUrl[prefixLength];

	nPort = 80;

	do
	{
		if (!szUrl || !szHost || !szUser || !szPassword)
			break;
		// Parse the URL as "http://[<username>[:<password>]@]<server-address-or-name>[:<port>][/<stream-name>]"

		if (_strnicmp(szUrl, prefix, prefixLength) != 0)
			break;

		// Check whether "<username>[:<password>]@" occurs next.
		// We do this by checking whether '@' appears before the end of the URL, or before the first '/'.
		char const* colonPasswordStart = NULL;
		char const* p;
		for (p = from; *p != '\0' && *p != '/'; ++p)
		{
			if (*p == ':' && colonPasswordStart == NULL)
			{
				colonPasswordStart = p;
			}
			else if (*p == '@')
			{
				// We found <username> (and perhaps <password>).  Copy them into newly-allocated result strings:
				if (colonPasswordStart == NULL)
					colonPasswordStart = p;

				char const* usernameStart = from;
				unsigned usernameLen = colonPasswordStart - usernameStart;
				for (unsigned i = 0; i < usernameLen; ++i)
					szUser[i] = usernameStart[i];
				szUser[usernameLen] = '\0';

				char const* passwordStart = colonPasswordStart;
				if (passwordStart < p)
					++passwordStart; // skip over the ':'
				unsigned passwordLen = p - passwordStart;
				for (unsigned j = 0; j < passwordLen; ++j) szPassword[j] = passwordStart[j];
				szPassword[passwordLen] = '\0';

				from = p + 1; // skip over the '@'
				break;
			}
		}

		// Next, parse <server-address-or-name>
		char* to = &parseBuffer[0];
		unsigned i;
		for (i = 0; i < parseBufferSize; ++i)
		{
			if (*from == '\0' || *from == ':' || *from == '/')
			{
				// We've completed parsing the address
				*to = '\0';
				break;
			}
			*to++ = *from++;
		}
		if (i == parseBufferSize)
			break;
		strcpy(szHost, parseBuffer);

		char nextChar = *from;
		if (nextChar == ':')
		{
			int portNumInt;
			if (sscanf(++from, "%d", &portNumInt) != 1)
			{
				break;
			}
			if (portNumInt < 1 || portNumInt > 65535)
			{
				break;
			}
			nPort = portNumInt;
			while (*from >= '0' && *from <= '9') ++from; // skip over port number
		}

		// The remainder of the URL is the suffix:
		if (urlSuffix != NULL)
			*urlSuffix = from;

		return true;
	} while (0);

	return false;
}
