#pragma once
#pragma message( "Compiling " __FILE__ ) 
#include <windows.h>
#include <stdio.h>
#include <list>
#include "Runlog.h"
using namespace std;
class CStat
{
	int		nCount;			// 统计总数量
	float	fTotalValue;	// 所有时间和
	float	fAvgValue;		// 平均时间
	float   fMax, fMin;
	int		nObjIndex;
	char    szStatName[128];
	//float	*pArray;
	int		nArraySize;
	list<float>listValue;
	CRunlog *pRunlog;
	
	CStat()
	{
		ZeroMemory(this, offsetof(CStat,listValue));
	}
public:
	CStat(char *szName, int nIndex, int nSize = 200)
	{
		ZeroMemory(this, offsetof(CStat, listValue));
		nObjIndex = nIndex;
		pRunlog = new CRunlog(szName);
		nArraySize = nSize;
		//pArray = new float[nArraySize];
		//ZeroMemory(pArray, sizeof(float)*nArraySize);
	}
	CStat(int nIndex, int nSize = 200)
	{
		ZeroMemory(this, offsetof(CStat, listValue));
		nObjIndex = nIndex;
		nArraySize = nSize;
		strcpy(szStatName, "TimeStat");
		pRunlog = new CRunlog(szStatName);
// 		pArray = new float[nArraySize];
// 		ZeroMemory(pArray, sizeof(float)*nArraySize);
	}
	~CStat()
	{

		if (pRunlog)
			delete pRunlog;
	}
	const float GetAvgValue()
	{
		return fAvgValue;
	}
	void Stat(float fValue)
	{
		listValue.push_back(fValue);
		if (nCount < nArraySize)
		{
			fTotalValue += fValue;
			if (fMax < fValue)
				fMax = fValue;
			if (fMin == 0.0f ||
				fMin > fValue)
				fMin = fValue;
		}
		else
		{
			fTotalValue -= listValue.front();
		}
	}

	bool IsFull()
	{
		return (nCount >= nArraySize);
	}

	void Reset()
	{
		ZeroMemory(this, offsetof(CStat, nObjIndex));
		listValue.clear();
	}

	void OutputStat(double dfOuputcondition = 0.0f)
	{
		fAvgValue = fTotalValue / nCount;
		if (pRunlog)
			pRunlog->Runlog("%s Obj(%d)(%s)\t nQueueSize = %d\tMaxValue = %.3f\tMinValue = %.3f\tAvgValue = %.3f\n\t", __FUNCTION__, nObjIndex, szStatName, nCount, fMax, fMin, fAvgValue);
		//printf("%s Obj(%d)(%s)\t nQueueSize = %d\tMaxValue = %.3f\tMinValue = %.3f\tAvgValue = %.3f\n\t", __FUNCTION__, nObjIndex, szStatName, nCount, fMax, fMin, fAvgValue);
		char szText[8192] = { 0 };
		int i = 0;
		for (auto it = listValue.begin(); it != listValue.begin(); it++, i++)
		{
			if (dfOuputcondition == 0.0f)
			{
				sprintf(&szText[strlen(szText)], "[%03d] = %.03f\t", i, *it);
				if (i % 100 == 0 && i != 0)
					strcat(szText, "\n\t");
			}
			else
			{
				if (*it < dfOuputcondition)
					continue;
				sprintf(&szText[strlen(szText)], "[%03d] = %.03f\t", i,*it);
			}
		}

		strcat(szText, "\r\n");
		//printf(szText);
		if (pRunlog)
			pRunlog->Runlog(szText);
	}
};

