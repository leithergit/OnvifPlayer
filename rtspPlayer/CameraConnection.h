#pragma once
#pragma message( "Compiling " __FILE__ ) 
#include <memory>
#include <string>

#include "IPCPlaySDK.h"
#include "Stat.h"
#include "../rtsp/rtsp.h"
#include "TimeUtility.h"
#include "DialogChart.h"

#pragma component(browser, off, references)
#include "../Public/onvifsdk/ONVIFLib.h"
#pragma component(browser, on, references)

using namespace  std;
using namespace  std::tr1;
#pragma pack(push)
#pragma pack(1)
struct _IPCConnection
{
	IPC_PLAYHANDLE hPlayhandle;
	CONVIFClient* pOnvif = nullptr;

	int		nWndNo;
	CHAR	szIP[64];
	long	nOSDFont;
	COLORREF nFontColor;
	long	nOSDText[16];
	int		nOSDCount;
	IPC_MEDIAINFO* pMediaHeader;
	bool	bHWAccel;
	HWND	m_hWnd;
	byte* m_pFrameBuffer;
	byte* m_pIFrame;
	FILE* m_pRecordFile;
	UINT	nFrameID;
	CRITICAL_SECTION csPlayHandle;
	bool	bPreStream;		// 是否预取流模式
	int		nIndex;
	int		nConnectType;
	bool	bDecode;
	int		nPort;
	INT		nRow;
	char	szURL[1024];
	int		m_nBufferSize;
	int		m_nFrameLength;
	int		m_nIFrameLength;
	void* pOnStream;
	void* pSDPNotify;
	void* pOnDisconnect;
	CStat* pStat;
	// m_hRtspSession 之前的字段在重连时不会被重置
	long	m_hRtspSession;
	long	m_hRtspLoginSession;
	int		m_nWidth;
	int		m_nHeight;
	double	dfConnectTime;
	DWORD	nLastRecvTime;
	bool	m_bFillHeader;
	bool	m_bInputHeader;
	bool	m_bInputIFrame;
	bool	bRecvFirstFrame;
	bool	bSwitcher;
	HWND	hYUVWnd;
	byte	m_nFPS;
	bool	bWriteMediaHeader;
	RECT	rtZoomBorder;		// 缩放边界
	bool	bPercent;			// 缩放方式，为True时，使用则按百分缩放，否则按象素缩放

	DialogChart* pDialogChart;
	_IPCConnection(int nIndex, int nWndNo)
	{
		ZeroMemory(this, sizeof(_IPCConnection));
		this->nIndex = nIndex;
		this->nWndNo = nWndNo;
		m_nBufferSize = 128 * 1024;
		m_pFrameBuffer = new byte[m_nBufferSize];
		m_pIFrame = new byte[m_nBufferSize];
		InitializeCriticalSection(&csPlayHandle);
		pMediaHeader = new IPC_MEDIAINFO;
	}

	bool MakeIPCHeader(byte* pFrameHeader, int& nHeaderSize, int nFrameID, int nFrameType, int nFrameLength)
	{
		if (!pFrameHeader || !nHeaderSize)
			return false;
		IPCFrameHeaderEx FrameHeader;
		UINT64 tNow = GetExactTime() * 1000 * 1000;
		FrameHeader.nTimestamp = tNow;
		FrameHeader.nLength = nFrameLength;
		FrameHeader.nFrameTag = IPC_TAG;		///< IPC_TAG
		FrameHeader.nFrameUTCTime = time(NULL);
		FrameHeader.nFrameID = nFrameID;
		memcpy(pFrameHeader, &FrameHeader, sizeof(IPCFrameHeaderEx));
		return true;
	}
	~_IPCConnection()
	{
		if (m_hRtspSession)
			rtsp_stop(m_hRtspSession);
		if (hPlayhandle)
			ipcplay_Close(hPlayhandle);
		if (m_pFrameBuffer)
		{
			delete[]m_pFrameBuffer;
		}
		if (m_pIFrame)
		{
			delete[]m_pIFrame;;
		}
		if (m_pRecordFile)
		{
			fclose(m_pRecordFile);
			m_pRecordFile = nullptr;
		}
		if (pStat)
			delete pStat;
		delete pMediaHeader;
		DeleteCriticalSection(&csPlayHandle);
	}
	void Reset()
	{
		rtsp_stop(m_hRtspSession);
		if (hPlayhandle)
			ipcplay_Refresh(hPlayhandle);
		int nSize = sizeof(_IPCConnection) - offsetof(_IPCConnection, m_hRtspSession);
		ZeroMemory(&m_hRtspSession, sizeof(_IPCConnection) - offsetof(_IPCConnection, m_hRtspSession));
	}

	void Reconnect()
	{
		Reset();
		TraceMsgA("%s %d Try to connect Device %s.\n", __FUNCTIONW__, (int)time(0), szURL);
		m_hRtspSession = rtsp_play(szURL, nConnectType, nPort, pOnStream, pOnDisconnect, this, 2000);
		dfConnectTime = GetExactTime();
		nLastRecvTime = 0;
		bRecvFirstFrame = false;
	}

	static void __stdcall SwitchCallBack(HWND hWnd, IPC_PLAYHANDLE hPlayerHandle, void* pUserPtr)
	{
		ipcplay_RemoveWindow(hPlayerHandle, hWnd);
		_IPCConnection* pConnection = (_IPCConnection*)pUserPtr;
		rtsp_stop(pConnection->m_hRtspSession);
		pConnection->m_hRtspSession = 0;

		ipcplay_Close(pConnection->hPlayhandle);
		pConnection->hPlayhandle = nullptr;
		printf("%s IP = %s.\n", __FUNCTION__, pConnection->szURL);
		delete pConnection;
	}

	static void RtspDisConnect(long lHandle, int nErrorCode, int dwUser, char* szProc)
	{

	}
	//static void RtspDataCallBack(long lHandle, char* pRTSPFrameIn, int nUnused, int nUser);
	static void RtspDataCallBack(long lHandle, char* pRTSPFrameIn, int nUnused, int nUser)
	{
		_IPCConnection* pThis = (_IPCConnection*)nUser;
		RTSPFrame* pRtspFrame = (RTSPFrame*)pRTSPFrameIn;
		if (!pThis->bRecvFirstFrame)
		{
			pThis->nFrameID = 0;
			pThis->bRecvFirstFrame = true;
			double dfTimespan = TimeSpanEx(pThis->dfConnectTime);
			printf("第一帧时间:%.3f.\n", dfTimespan);
			pThis->nLastRecvTime = timeGetTime();
			//pThis->pRunlog->Runlog(L"%s Recv First Frame.\n", __FUNCTIONW__);
		}
		else
			// 		if (!pThis->pStat)
			// 		{
			// 			CHAR szTitle[256] = { 0 };
			// 			sprintf_s(szTitle, 256, "Camera_%s Frames Interval", pThis->szIP);
			// 			pThis->pStat = new CStat(szTitle, pThis->nWndNo, 5 * 60 * 25);
			// 			//_tprintf(_T("Start Stat:%s.\n"),pThis->szIP);
			// 			pThis->dfLastRecvTime = GetExactTime();
			// 		}
			// 		else
		{
			pThis->nFrameID++;
			if (pThis->pStat)
				pThis->pStat->Stat(MMTimeSpan(pThis->nLastRecvTime));

			if (pThis->pDialogChart)
				pThis->pDialogChart->InputData(pThis->nFrameID, MMTimeSpan(pThis->nLastRecvTime), pRtspFrame->nFrameSize);
			pThis->nLastRecvTime = timeGetTime();
			if (pThis->pStat &&
				pThis->pStat->IsFull())
			{
				pThis->pStat->OutputCSV();
				pThis->pStat->Reset();
			}
		}

		if (!pThis->hPlayhandle && pThis->bDecode)
		{
			int nCodecID = 0, nWidth = 0, nHeiht = 0, nFPS = 0;
			TraceMsgA("\n%s MediaName = %s.\n", __FUNCTION__, pRtspFrame->pSDP->pMediaName);
			if (!pRtspFrame->pSDP->pMediaName || strcmp(pRtspFrame->pSDP->pMediaName, "video"))
			{
				TraceMsgA("%s Not a video stream .\n", __FUNCTION__);
				return;
			}

			if (!pRtspFrame->pSDP->nHeight || !pRtspFrame->pSDP->nWidth || !pRtspFrame->pSDP->szCodec)
			{
				TraceMsgA("%s Not Unknown codec .\n", __FUNCTION__);
				return;
			}
			pThis->m_nWidth = pRtspFrame->pSDP->nWidth;
			pThis->m_nHeight = pRtspFrame->pSDP->nHeight;
			if (strcmp(pRtspFrame->pSDP->szCodec, "H264") == 0)
				pThis->pMediaHeader->nVideoCodec = CODEC_H264;
			else if (strcmp(pRtspFrame->pSDP->szCodec, "H265") == 0)
				pThis->pMediaHeader->nVideoCodec = CODEC_H265;
			else if (strcmp(pRtspFrame->pSDP->szCodec, "JPEG") == 0)
				pThis->pMediaHeader->nVideoCodec = CODEC_MJPEG;
			pThis->pMediaHeader->nAudioCodec = CODEC_G711A;
			pThis->pMediaHeader->nVideoWidth = pRtspFrame->pSDP->nWidth;
			pThis->pMediaHeader->nVideoHeight = pRtspFrame->pSDP->nHeight;
			if (pRtspFrame->pSDP->nRate != 0)
				pThis->pMediaHeader->nFps = pRtspFrame->pSDP->nRate;
			else
				pThis->pMediaHeader->nFps = 25;
			pThis->hPlayhandle = ipcplay_OpenStream(pThis->m_hWnd, NULL, 0);
			// TraceMsgA("*****************************************************************\r\n");
			// TraceMsgA("\tIPCPlayHander[%s] = 0x%08X	Wnd = 0x%08X*\r\n", pThis->szIP, pThis->hPlayhandle, pThis->m_hWnd);
			// TraceMsgA("*****************************************************************\r\n");

			ipcplay_SetD3dShared(pThis->hPlayhandle, pThis->bHWAccel);
			ipcplay_SetStreamHeader(pThis->hPlayhandle, (byte*)pThis->pMediaHeader, sizeof(IPC_MEDIAINFO));
			ipcplay_SetMaxFrameSize(pThis->hPlayhandle, 1024 * 1024);
			int nScreen = 0;
			if (pThis->bSwitcher)
				ipcplay_SetSwitcherCallBack(pThis->hPlayhandle, MAKEWORD(pThis->nWndNo, nScreen), pThis->m_hWnd, (void*)SwitchCallBack, pThis);
			ipcplay_Start(pThis->hPlayhandle, false, true, pThis->bHWAccel);
			//ipcplay_EnableStreamParser(pThis->hPlayhandle, CODEC_H264);
			pThis->m_bFillHeader = true;
		}
		byte szHeader[64] = { 0 };
		int nHeadersize = 64;
		int nNalType = pRtspFrame->pFrame[0] & 0x1F;
		byte* pBuffer = pRtspFrame->pFrame;
		int nBufferLen = pRtspFrame->nFrameSize;
		switch (nNalType)
		{
		case 1:
		case 5:
			//printf("%s VFrame Input size = %d.\n", __FUNCTION__, nBufferLen);
			//bSPSPPSCanOutput = true;
			break;
		case 6:
		{
			printf("%s SEI Input size = %d.\n", __FUNCTION__, nBufferLen);
			break;
		}
		case 7:
		{
			int nWidth, nHeight, nfps;
			//h264_decode_sps((byte *)pBuffer, nBufferLen, nWidth, nHeight, nfps);
			printf("%s SPS Input size = %d\tWidth = %d\tHeight = %d\tFPS = %d.\n", __FUNCTION__, nBufferLen, pRtspFrame->pSDP->nWidth, pRtspFrame->pSDP->nHeight, pRtspFrame->pSDP->nRate);
			break;
		}
		case 8:
		{
			printf("%s PPS Input size = %d.\n", __FUNCTION__, nBufferLen);
			break;
		}

		default:
			printf("%s Unknown NALU Type:%d size = %d.\n", __FUNCTION__, nNalType, nBufferLen);
			break;
		}

		int nTotalLength = pThis->m_nFrameLength + nBufferLen + 4;

		if (pThis->m_nBufferSize < nTotalLength)
		{
			int nNewBufferSize = pThis->m_nBufferSize;
			while (nNewBufferSize < nTotalLength)
				nNewBufferSize *= 2;
			byte* pTemp = new byte[nNewBufferSize];
			if (pTemp == NULL)
				return;

			memcpy(pTemp, pThis->m_pFrameBuffer, pThis->m_nFrameLength);
			delete[]pThis->m_pFrameBuffer;
			pThis->m_pFrameBuffer = pTemp;
			pThis->m_nBufferSize = nNewBufferSize;

		}
		if (nNalType == 0 ||
			nNalType == 5 || nNalType == 1 ||
			nNalType == 7 || nNalType == 8 ||
			nNalType == 31)
		{
			byte szStart_code[] = { 0, 0, 0, 1 };

			memcpy_s(&pThis->m_pFrameBuffer[pThis->m_nFrameLength], pThis->m_nBufferSize - pThis->m_nFrameLength, szStart_code, 4);
			pThis->m_nFrameLength += 4;
			memcpy_s(&pThis->m_pFrameBuffer[pThis->m_nFrameLength], pThis->m_nBufferSize - pThis->m_nFrameLength, pBuffer, nBufferLen);
			pThis->m_nFrameLength += nBufferLen;
			int nFrameType = IPC_P_FRAME;
			if (nNalType == 5)
				nFrameType = IPC_I_FRAME;
			time_t tNow = (time_t)(GetExactTime() * 10000);
			if (pThis->bDecode)
				ipcplay_InputIPCStream(pThis->hPlayhandle, pThis->m_pFrameBuffer, nFrameType, pThis->m_nFrameLength, 0);
			if (pThis->m_pRecordFile)
			{
				fwrite(pThis->m_pFrameBuffer, pThis->m_nFrameLength, 1, pThis->m_pRecordFile);
			}
			//ipcplay_InputStream2(pThis->hPlayhandle, pThis->m_pFrameBuffer, pThis->m_nFrameLength);
			pThis->m_nFrameLength = 0;
			pThis->m_bInputIFrame = true;
		}
	}

};
#pragma pack(pop)
typedef shared_ptr<_IPCConnection> IPCConnectionPtr;
