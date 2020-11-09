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
	//int					_nConnectTimeout;		// 连接摄像机最大超时值，超过这个值，认为连接失败，建议不超过500ms
	int					_nMaxFrameInterval;		// 允许的最大帧间隔，超过这个时间，将认为是掉线，建议不要超过2000ms
};

#define RTSP_CLIENT_VERBOSITY_LEVEL 1 