#ifndef RTSP_H
#define RTSP_H
#pragma message( "Compiling " __FILE__ ) 
#include <memory.h>

#ifdef RTSP_LIB
# define RTSP_EXPORT __declspec(dllexport)
#else
# define RTSP_EXPORT __declspec(dllimport)
#endif

struct SDP
{
	SDP()
	{
		memset(this, 0, sizeof(SDP));
	}
	int nWidth;
	int nHeight;
	int nRate;
	int nSPSLen;
	unsigned char szSPS[128];
	int nPPSLen;
	unsigned char szPPS[128];
	int nRTPPayloadFormat;
	int nRTPTimestampFrequency;
	char* pProtocol;
	char* pMediaName;
	char* szCodec;
};
struct RTSPFrame
{
	SDP* pSDP;
	unsigned char* pFrame;
	int		nFrameSize;
	RTSPFrame()
	{
		memset(this, 0, sizeof(RTSPFrame));
	}
};
typedef void(*PFSDPCallBack)(long hRTSPSession, void* pSDP, long nUser);
typedef void (*PFRtspDataCallBack)(long lHandle, char* pBuffer, int nParam, int nUser);
typedef void (*PFRtspDisConnect)(long lHandle, int nErrorCode, int dwUser, char* szProc);


extern "C"
{
	RTSP_EXPORT long rtsp_login(char* pUrl,
		int httpPort,
		PFRtspDisConnect cbDisconnectCallBack,
		void* pUser,
		int ConnnectTimeout = 1500);
	RTSP_EXPORT long rtsp_play(const char* pUrl,
		int nNetType,
		int httpPort,
		void* cbDataCallBack,
		void* cbDisconnectCallBack,
		void* pUser,
		int nConnectTimeout = 1500,
		int nMaxFrameInterval = 1500);
	RTSP_EXPORT bool rtsp_GetSDP(SDP* pSDP);
	RTSP_EXPORT long rtsp_setCallback(long lPlayHandle, void* cbDataCallBack, void* cbDisconnectCallBack, void* pUser);

	RTSP_EXPORT int	 rtsp_stop(long lHandle);
};


#endif // RTSP_H
