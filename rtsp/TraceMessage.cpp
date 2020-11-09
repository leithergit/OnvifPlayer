#include "TraceMessage.h"

void TraceMsgA(LPCSTR pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);
	int nBuff;
	CHAR szBuffer[0x7fff];
	nBuff = _vsnprintf(szBuffer, __countof(szBuffer), pFormat, args);
	OutputDebugStringA(szBuffer);
	va_end(args);	
}