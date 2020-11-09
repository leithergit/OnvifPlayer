#pragma once
#pragma message( "Compiling " __FILE__ ) 
#include <TCHAR.H>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
//#include <windows.h>
#include <time.h>
#include <assert.h>
#include <MMSystem.h>
//#include <thread>
//#include <chrono>
#ifdef Release_D
#undef assert
#define assert	((void)0)
#endif

// using namespace std::this_thread;
// using namespace std::chrono;
#ifdef _UNICODE
#define GetDateTime			GetDateTimeW
#define	_DateTime			DateTimeW
#define UTC2DateTimeString	UTC2DateTimeStringW
#else
#define GetDateTime			GetDateTimeA
#define	_DateTime			DateTimeA
#define UTC2DateTimeString	UTC2DateTimeStringA
#endif


int		GetDateTimeA(CHAR *szDateTime,int nSize);
int		GetDateTimeW(WCHAR *szDateTime,int nSize);
LPCSTR	DateTimeA();
LPCWSTR	DateTimeW();
bool	IsLeapYear(UINT nYear);
UINT64	DateTimeString2UTC(TCHAR *szTime,UINT64 &nTime);
void	UTC2DateTimeStringA(UINT64 nTime,CHAR *szTime,int nSize);
void	UTC2DateTimeStringW(UINT64 nTime,WCHAR *szTime,int nSize);
BOOL	SystemTime2UTC(SYSTEMTIME *pSystemTime,UINT64 *pTime);
BOOL	UTC2SystemTime(UINT64 *pTime,SYSTEMTIME *pSystemTime);
void	StringFromSystemTimeA(CHAR* szTimeString,int nSize,bool bMillionSecond = true);
void	StringFromSystemTimeW(WCHAR* szTimeString,int nSize,bool bMillionSecond = true);

// NTPУʱ��
struct   NTP_Packet
{
	int			Control_Word;   
	int			root_delay;   
	int			root_dispersion;   
	int			reference_identifier;   
	__int64		reference_timestamp;   
	__int64		originate_timestamp;   
	__int64		receive_timestamp;   
	int			transmit_timestamp_seconds;   
	int			transmit_timestamp_fractions;   
};

/************************************************************************/
/* ����˵��:�Զ���ʱ�������ͬ������
/* ����˵��:��
/* �� �� ֵ:�ɹ�����TRUE��ʧ�ܷ���FALSE
/************************************************************************/
BOOL NTPTiming(const char* szTimeServer);

#define EPOCHFILETIME   (116444736000000000UL)
#define TimeSpan(t)		(time(NULL) - (time_t)t)
#define TimeSpanEx(t)	(GetExactTime() - t)
#define MMTimeSpan(t)	(timeGetTime() - t)
typedef struct __ExactTimeBase
{
	LONGLONG	dfFreq;
	LONGLONG	dfCounter;
	time_t		nBaseClock;
	double		dfMilliseconds;
	// ��ȡϵͳ�ĵ�ǰʱ�䣬��λ΢��(ms)
	static UINT64 GetSysTimeMicros()
	{
		// ��1601��1��1��0:0:0:000��1970��1��1��0:0:0:000��ʱ��(��λ100ns)

		FILETIME ft;
		LARGE_INTEGER li;
		UINT64 Time64 = 0;
		GetSystemTimeAsFileTime(&ft);
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		// ��1970��1��1��0:0:0:000�����ڵ�΢����(UTCʱ��)
		Time64 = (li.QuadPart - EPOCHFILETIME) /10;
		return Time64;
	}
	__ExactTimeBase()
	{
		ZeroMemory(this,sizeof(ETB));
		SYSTEMTIME systime1;
		SYSTEMTIME systime2;
		ZeroMemory(&systime1, sizeof(SYSTEMTIME));
		ZeroMemory(&systime2, sizeof(SYSTEMTIME));

		HANDLE hProcess			= GetCurrentProcess();
		HANDLE hThread			= GetCurrentThread();

		DWORD dwPriorityClass	= GetPriorityClass(hProcess);		
		DWORD dwThreadPriority	= GetThreadPriority(hThread);

		DWORD dwError			= 0;
		// �ѽ������ȼ�������ʵʱ��
		if (!SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS))
		{
			dwError = GetLastError();
			if (!SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS))
			{
				dwError = GetLastError();
				if (!SetPriorityClass(hProcess, ABOVE_NORMAL_PRIORITY_CLASS))
				{
					dwError = GetLastError();
				}
			}
		}
		// ���߳����ȼ�������ʵʱ��
		if (!SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL))
		{
			dwError = GetLastError();
			if (!SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST))
			{
				dwError = GetLastError();
				if (!SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL))
				{
					dwError = GetLastError();
				}
			}
		}
		UINT64 t1 = GetSysTimeMicros();
		UINT64 t2 = 0;
		int nCompares = 0;
		// У׼��׼ʱ��
		while (true)
		{
			t2 = GetSysTimeMicros();
			if (t2 != t1)
				break;
			nCompares ++;
		}
		nBaseClock = t2/(1000*1000);
		dfMilliseconds = (double)(t2 - nBaseClock*1000*1000)/1000;

		// �ָ��̺߳ͽ��̵����ȼ�
		SetThreadPriority(hThread, dwThreadPriority);
		SetPriorityClass(hProcess, dwPriorityClass);

// #ifdef _DEBUG
// 		TCHAR szText[64] = {0};
// 		_stprintf_s(szText,_T("BaseClock of ETB = %I64d.\n"),nBaseClock);
// 		OutputDebugString(szText);
// #endif
		LARGE_INTEGER LarInt;		
		QueryPerformanceFrequency(&LarInt);	
		dfFreq = LarInt.QuadPart;
		QueryPerformanceCounter(&LarInt);
		dfCounter = LarInt.QuadPart;
	}

	~__ExactTimeBase()
	{
	}

}ETB;
extern ETB g_etb;
#define	InitPerformanceClock	

/// @brief ȡϵͳ��ȷʱ��,��λ��,����Ϊ25΢������
double  GetExactTime();

/// @brief �߳����߷�ʽ
// struct CThreadSleep
// {
// 	enum SleepWay
// 	{
// 		Sys_Sleep = 0,		///< ֱ�ӵ���ϵͳ����Sleep
// 		Wmm_Sleep = 1,		///< ʹ�ö�ý��ʱ����߾���
// 		Std_Sleep = 2		///< ʹC++11�ṩ���߳����ߺ���
// 	};
// 	CThreadSleep()
// 	{
// 		double dfSumSpan1 = 0.0f, dfSumSpan2 = 0.0f, dfSumSpan3 = 0.0f;
// 		double dfT1 = 0.0f;
// 		for (int i = 0; i < 32; i++)
// 		{
// 			dfT1 = GetExactTime();
// 			Sleep(1);
// 			dfSumSpan1 += TimeSpanEx(dfT1);
// 
// 			dfT1 = GetExactTime();
// 			timeBeginPeriod(1); //���þ���Ϊ1����
// 			::Sleep(1); //��ǰ�̹߳���һ����
// 			timeEndPeriod(1); //������������
// 			dfSumSpan2 += TimeSpanEx(dfT1);
// 
// 			dfT1 = GetExactTime();
// 			//std::this_thread::sleep_for(std::chrono::nanoseconds(1));
// 			Sleep(1);
// 			dfSumSpan3 += TimeSpanEx(dfT1);
// 		}
// 		double dfSpanSum = dfSumSpan1;
// 		
// 		if (dfSumSpan1 <= dfSumSpan2)
// 		{
// 			nSleepWay = Sys_Sleep;
// 		}
// 		else
// 		{
// 			dfSpanSum = dfSumSpan2;
// 			nSleepWay = Wmm_Sleep;
// 		}
// 		if (dfSumSpan3 < dfSpanSum)
// 		{
// 			nSleepWay = Std_Sleep;
// 			dfSpanSum = dfSumSpan3;
// 		}
// 		nSleepPrecision = (DWORD)(round(1000 * dfSpanSum)/32);
// 		if (nSleepPrecision == 0)
// 			nSleepPrecision = 1;
// 	}
// 	void operator ()(DWORD nTimems)
// 	{
// 		switch(nSleepWay)
// 		{
// 		case Sys_Sleep:
// 			::Sleep(nTimems);
// 			break;		
// 		case Wmm_Sleep:
// 		{
// 			timeBeginPeriod(1); //���þ���Ϊ1����
// 			::Sleep(nTimems);	//��ǰ�̹߳���һ����
// 			timeEndPeriod(1);	//������������
// 		}
// 			break;
// 		case Std_Sleep:
// 			//std::this_thread::sleep_for(std::chrono::nanoseconds(nTimems*1000));
// 			Sleep(1);
// 			break;
// 		default:
// 			assert(false);
// 			break;
// 		}
// 	}
// 	inline DWORD GetPrecision()
// 	{
// 		return nSleepPrecision;
// 	}
// 
// private:
// 	SleepWay	nSleepWay;
// 	DWORD		nSleepPrecision;
// };
// 
// extern CThreadSleep ThreadSleep;
#define GetSleepPricision()	ThreadSleep.GetPrecision();
#define  SaveWaitTime()	CWaitTime WaitTime(__FILE__,__LINE__,__FUNCTION__);

class CWaitTime
{
	DWORD dwTimeEnter;
	char szFile[512];
	int nLine;
	char szFunction[256];
public:
	CWaitTime(char *szInFile, int nInLine, char *szInFunction)
	{
		dwTimeEnter = timeGetTime();
		strcpy(szFile, szInFile);
		nLine = nInLine;
		strcpy(szFunction, szInFunction);
	}
	~CWaitTime()
	{
		if ((timeGetTime() - dwTimeEnter) > 200)
		{
			char szText[1024] = { 0 };
			sprintf(szText, "Wait Timeout @File:%s %d(%s) WaitTime = %d(ms).\n", szFile, nLine, szFunction, (timeGetTime() - dwTimeEnter));
			OutputDebugStringA(szText);
		}
	}
};
