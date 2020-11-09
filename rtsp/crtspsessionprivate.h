#pragma once

#include "ourRTSPClient.h"
#include "BasicUsageEnvironment.hh"

#include "rtsp.h"
#include <list>
#include "Stat.h"
using namespace std;

class CRTSPSessionPrivate
{
public:
	static void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void sendLivenessCommand(void* clientData);
	static void continueAfterGetParameter(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void continueAfterOPTIONS(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void setupNextSubsession(RTSPClient* rtspClient);
	static void subsessionAfterPlaying(void* clientData);
	static void subsessionByeHandler(void* clientData);
	static void scheduleLivenessCommand(void* clientData);
	static void continueAfterGET_PARAMETER(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void continueAfterLivenessCommand(RTSPClient* rtspClient, int resultCode, Boolean serverSupportsGetParameter);
	static void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);
	static int rtspClientCount;

public:
	bool	m_bWait;
	int		m_nResult;
	HANDLE	m_hEvent;
	bool	m_bHeartbeat;
	double  m_fNextBeat;
	int		m_nHeartBeatInterval;
	double  fLastRecvCheckTime = 0.0f;
#ifdef _DEBUG
	list<double> listRecvTime;
#endif
	int		m_nCheckNumber;
	int		m_nCallCount = 0;

	Boolean m_bUseTCP;

	CRITICAL_SECTION   csCallBack;
	PFSDPCallBack		_rtspSDPNotify;
	PFRtspDataCallBack	_rtspStream;
	PFRtspDisConnect	_rtspDisconnect;
	void*				_user;
	//int					_nConnectTimeout;		// ������������ʱֵ���������ֵ����Ϊ����ʧ�ܣ����鲻����500ms
	int					_nMaxFrameInterval;		// ��������֡������������ʱ�䣬����Ϊ�ǵ��ߣ����鲻Ҫ����2000ms
};

#define RTSP_CLIENT_VERBOSITY_LEVEL 1 