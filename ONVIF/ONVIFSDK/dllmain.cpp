// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <stdio.h>
#include <WinSock2.h>
#define WSA_VERSION MAKEWORD(2,2)
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		WSADATA		WSAData = { 0 };
		if (WSAStartup(WSA_VERSION, &WSAData) == 0)
		{
			if (LOBYTE(WSAData.wVersion) != LOBYTE(WSA_VERSION) ||
				HIBYTE(WSAData.wVersion) != HIBYTE(WSA_VERSION))
			{
				WSACleanup();
				return false;
			}
		}
		break;
	}

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
