#pragma once
#include <windows.h>
#include "Utility.h"
#include "TimeUtility.h"
#include "CriticalSectionAgent.h"
#include "AutoLock.h"
#include <list>
using namespace std;
class CStat
{
	
	DWORD	fTotalValue;	// 所有时间和
	DWORD	fAvgValue;		// 平均时间
	DWORD   fMax, fMin;
	int		nObjIndex;
	char    szStatName[128];
	char	szCurFileName[256];
	int		nMaxSize;
	FILE	*fp;
	CCriticalSectionAgent csList;
	list<DWORD> listData;	
	
	CStat()
	{
		ZeroMemory(this, offsetof(CStat, csList));
	}
public:
	CStat(char *szName, int nIndex, int nMaxSize = 200)
	{
		ZeroMemory(this, offsetof(CStat, csList));
		nObjIndex = nIndex;
		strcpy(szStatName, szName);
		this->nMaxSize = nMaxSize;
		fTotalValue = 0;
		fAvgValue = 0;
		fMax = 0; 
		fMin = 0;
		fp = nullptr;

	}
	CStat(int nIndex, int nMaxSize = 200)
	{
		/*ZeroMemory(&fTotalValue, sizeof(CStat) - sizeof(listData));*/
		nObjIndex = nIndex;
		this->nMaxSize = nMaxSize;
		strcpy(szStatName, "TimeStat");
		fTotalValue = 0;
		fAvgValue = 0;
		fMax = 0;
		fMin = 0;
		fp = NULL;
	}
	~CStat()
	{
		if (listData.size())
			OutputCSV();
		fclose(fp);
	}
	const float GetAvgValue()
	{
		return fAvgValue;
	}
	void Stat(DWORD fValue)
	{
		CAutoLock lock(csList.Get());
		if (listData.size() < nMaxSize)
		{
			listData.push_back(fValue);
			fTotalValue += fValue;
			if (fMax < fValue)
				fMax = fValue;
			if (fMin == 0 ||
				fMin > fValue)
				fMin = fValue;
		}
	}

	bool IsFull()
	{
		CAutoLock lock(csList.Get());
		return (listData.size() >= nMaxSize);
	}

	void Reset()
	{
		fTotalValue = 0;
		fAvgValue = 0;
		fMax = 0;
		fMin = 0;
		CAutoLock lock(csList.Get());
		listData.clear();
	}

	void OutputStat(double dfOuputcondition = 0.0f)
	{
		//fAvgValue = fTotalValue / listData.size();
		//if (!GetStdHandle(STD_OUTPUT_HANDLE))
		//	return;
		//CAutoLock lock(csList.Get());
		//int nListSize = listData.size();
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		//printf("%s Obj(%d)(%s)\t nQueueSize = %d", __FUNCTION__, nObjIndex, szStatName, nListSize);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		//printf("\tMaxValue = %.3f\tMinValue=%.3f", fMax, fMin);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
		//printf("\tAvgValue = %.3f", fAvgValue);
		//
		//char szText[1024*16] = { 0 };
		//long dfDiffSum = 0;
		//int i = 0;
		//for (auto it = listData.begin() ;it != listData.end(); i++,it ++)
		//if (dfOuputcondition == 0.0f)
		//{
		//	dfDiffSum += abs((long)*it - (long)fAvgValue);
		//	sprintf(&szText[strlen(szText)], "[%03d] = %.03f\t", i, *it);
		//	if ((i+1) % 10 == 0 )
		//		strcat(szText, "\n\t");
		//}
		//else
		//{
		//	if (*it < dfOuputcondition)
		//		continue;
		//	sprintf(&szText[strlen(szText)], "[%03d] = %.03f\t", i, *it);
		//}
		//
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
		//printf("\tAvg Time diff = %.3f.\n\t", dfDiffSum / nListSize);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		//
		//strcat(szText, "\r\n");
		//printf(szText);
	}

	void OutputCSV(DWORD dfOuputcondition = 0)
	{
		TraceFunction();
		char szFileName[256] = { 0 };		
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);		
		sprintf_s(szFileName, 256, "%s_%04d%02d%02d.csv", szStatName, sysTime.wYear, sysTime.wMonth, sysTime.wDay);
		if (strcmp(szCurFileName, szFileName) != 0)
		{
			TraceMsgA("%s Create New File.\n", __FUNCTION__);
			if (fp)
				fclose(fp);
			fp = fopen(szFileName, "ab+");
			strcpy(szCurFileName, szFileName);
		}
				
		CAutoLock lock(csList.Get());
		int nListSize = listData.size();
		fAvgValue = fTotalValue / nListSize;
		int nBufferSize = 64 * 1024;
		char szText[1024 * 64] = { 0 };
		char szHeader[1024] = { 0 };
		DWORD dfDiffSum = 0;
		int i = 0;
		for (auto it = listData.begin(); it != listData.end(); i++, it++)
			if (dfOuputcondition == 0)
			{
				dfDiffSum += abs((long)*it - (long)fAvgValue);
				sprintf(&szText[strlen(szText)], "%d,", *it);
				if ((i + 1) % 25 == 0)
					strcat(szText, "\n");
			}
			else
			{
				if (*it < dfOuputcondition)
					continue;
				sprintf(&szText[strlen(szText)], "%d,", (int)(*it * 1000));
			}

		sprintf_s(szHeader,1024,"DateTime\t\t\t,nQueueSize\t,MaxValue\t,MinValue\t,AvgValue\t,AvgFloat\n");
		int len = strlen(szHeader);
		char szDateTime[32] = { 0 };
		GetDateTimeA(szDateTime, 32);
		sprintf_s(&szHeader[len], 1024 - len, "%s,\t%d\t\t,%d\t\t,%d\t\t\t,%d\t\t\t,%d\n", szDateTime, nListSize, fMax , fMin, fAvgValue , (dfDiffSum  / nListSize));
		fprintf(fp, szHeader);
		fprintf(fp, "%s\n",szText);
		fflush(fp);
		TraceMsgA("%s Log ouput Finished.\n", __FUNCTION__);
	}
};

