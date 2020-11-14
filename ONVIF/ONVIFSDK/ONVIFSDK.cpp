#ifdef _LIB
#include "ONVIFLib.h"
#else
#include "onvifsdk.h"
#endif
#include "onvifclientdevice.h"
#include "onvifclientmedia.h"
#include "onvifclientptz.h"
#include "stdafx.h"
#include <mmsystem.h>
#include <memory>
#pragma comment(lib,"winmm.lib")
using namespace std;
using namespace std::tr1;

#define  SafeFree(x) {if (x) {free(x);x = nullptr;}};
#define  SafeDelete(Type,x) {if (x){ delete (Type *)x;x = nullptr;}};

int ProbeHost(LPCSTR szHost, WORD nPort, int nNetTimeout)
{
	bool bConnected = true;
	SOCKET s = INVALID_SOCKET;
	DWORD dwError = 0;
	__try
	{
		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == INVALID_SOCKET)
		{
			bConnected = false;
			dwError = WSAGetLastError();
			__leave;
		}

		// 设置为非阻塞模式
		unsigned long nBlockMode = 1;
		if (ioctlsocket(s, FIONBIO, (unsigned long*)&nBlockMode) == SOCKET_ERROR)
		{
			bConnected = false;
			dwError = WSAGetLastError();
			__leave;
		}

		sockaddr_in ServerAddr;
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(nPort);
		//ServerAddr.sin_addr.S_un.S_addr = inet_addr(szServerIP);  
		InetPtonA(AF_INET, szHost, &ServerAddr.sin_addr);
		if (connect(s, (sockaddr*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR &&
			GetLastError() != WSAEWOULDBLOCK)
		{  //连接失败   
			dwError = WSAGetLastError();
			bConnected = false;
			__leave;
		}
		struct timeval timeout;
		fd_set FDConnect;
		FD_ZERO(&FDConnect);
		FD_SET(s, &FDConnect);
		timeout.tv_sec = nNetTimeout / 1000;
		timeout.tv_usec = (nNetTimeout % 1000) * 1000;	//连接超时200 ms,tv_usec的时间单位是微秒
		DWORD dwTickCount = timeGetTime();
		int nRes = select(0, 0, &FDConnect, 0, &timeout);
		if (SOCKET_ERROR == nRes)
		{
			dwError = WSAGetLastError();
			bConnected = false;
			__leave;
		}
		else if (0 == nRes)
		{
			dwError = WSAENETUNREACH;
			DWORD dwTimpeSpan = timeGetTime() - dwTickCount;
			bConnected = false;
			__leave;
		}

	}
	__finally
	{
		shutdown(s, SD_BOTH);
		closesocket(s);
	}
	if (!bConnected)
		return dwError;
	else
		return 0;
}

CONVIFClient::CONVIFClient(const char* strIP, const char* strAccount, const char* strPassword, unsigned short nHttpPort, int nTimeout)
{
	if (!CheckIPAddress(strIP))
		throw std::exception("Invliad device IP address.");

	int nResult = ProbeHost(strIP, nHttpPort, nTimeout);
	if (nResult)
	{
		char szException[1024] = { 0 };
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, nResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szException, 1024, nullptr);
		throw exception(szException);
	}
	m_strCameraIP = _strdup(strIP);
	m_strAccount = _strdup(strAccount);
	m_strPassword = _strdup(strPassword);
	m_nHttpPort = nHttpPort;
	CreateOnvifClient();
}

CONVIFClient::~CONVIFClient()
{
	SafeFree(m_strCameraIP);
	SafeFree(m_strAccount);
	SafeFree(m_strPassword)
		SafeFree(m_strPTZUrl);
	/*_trt__GetProfilesResponse*/
	SafeDelete(_trt__GetProfilesResponse, m_Profiles);
	/*_tptz__GetPresetsResponse*/
	SafeDelete(_tptz__GetPresetsResponse, m_Presets);
	/*shared_ptr<OnvifClientDevice>*/
	SafeDelete(OnvifClientDevice, m_pOnvifDevice);
	/*shared_ptr<OnvifClientMedia>*/
	SafeDelete(OnvifClientMedia, m_pMedia);
	/*shared_ptr<OnvifClientPTZ>*/
	SafeDelete(OnvifClientPTZ, m_pPTZClient);
	SafeDelete(OnvifClientImage, m_pOnvifImage);
}

bool CONVIFClient::CheckIPAddress(const char* szAddress)
{
	if (szAddress == NULL)
		return false;
	int nDot = 0;
	int nLen = strlen(szAddress);
	if (nLen < 7 || nLen > 15)
		return false;
	for (int i = 0; i < nLen; i++)
	{
		if (szAddress[i] == '.')
			nDot++;
	}

	if (nDot < 3)
		return false;
	in_addr addr;
	return (inet_pton(AF_INET, szAddress, &addr) == 1);
}

void CONVIFClient::CreateOnvifClient()
{
	OnvifClientDevice* pOnvifDev = new OnvifClientDevice(m_strCameraIP, m_strAccount, m_strPassword, m_nHttpPort);
	if (!pOnvifDev)
		throw std::exception("Insufficient memory!");

	if (pOnvifDev->GetCapabilities() != SOAP_OK)
	{
		delete pOnvifDev;
		throw std::exception("Get Device Capabilities Failed.");
	}

	OnvifClientMedia* pMedia = new OnvifClientMedia(*pOnvifDev);
	if (!pMedia)
	{
		delete pOnvifDev;
		throw std::exception("Insufficient memory!");
	}
	_trt__GetProfilesResponse* pProfiles = new _trt__GetProfilesResponse;
	if (pMedia->GetProfiles(pProfiles) != SOAP_OK)
	{
		delete pOnvifDev;
		delete pMedia;
		throw std::exception("Get Media Profiles Failed.");
	}
	m_pOnvifDevice = pOnvifDev;
	m_Profiles = pProfiles;
	m_pMedia = pMedia;
}

bool CONVIFClient::CreatePTZClient()
{
	if (!m_pOnvifDevice || !m_pMedia || !m_Profiles)
		return false;
	if (m_pPTZClient)
		return true;

	string strPTZUrl;
	if (!m_pOnvifDevice->GetPTZUrl(strPTZUrl))
		return false;

	m_strPTZUrl = _strdup(strPTZUrl.c_str());
	http_da_info* pHttpInfo = nullptr;
	if (m_pMedia)
		pHttpInfo = m_pMedia->GetHttpDa();

	OnvifClientPTZ* pPTZ = new OnvifClientPTZ(*m_pOnvifDevice, pHttpInfo);

	if (!pPTZ)
		return false;
	m_pPTZClient = pPTZ;

	return true;
}

bool CONVIFClient::CreateImageClient()
{
	if (!m_pOnvifDevice || !m_pMedia || !m_Profiles)
		return false;

	string strPTZUrl;
	if (!m_pOnvifDevice->GetPTZUrl(strPTZUrl))
		return false;

	m_strPTZUrl = _strdup(strPTZUrl.c_str());
	http_da_info* pHttpInfo = nullptr;
	if (m_pMedia)
		pHttpInfo = m_pMedia->GetHttpDa();

	OnvifClientImage* pImage = new OnvifClientImage(*m_pOnvifDevice, pHttpInfo);

	if (!pImage)
		return false;
	m_pOnvifImage = pImage;

	return true;
}
bool  CONVIFClient::GetPresets()
{
	if (!m_pPTZClient)
		return false;

	if (m_nDefaultProfile >= m_Profiles->Profiles.size())
		return false;
	if (m_Presets)
		delete m_Presets;
	else
		m_Presets = new _tptz__GetPresetsResponse;
	return m_pPTZClient->GetPresets(*m_Presets, m_Profiles->Profiles[m_nDefaultProfile]->token) == SOAP_OK;
}

const char* CONVIFClient::GetMediaStreamUrl(char* szProfileToken)
{
	if (!m_pMedia || !m_Profiles)
		return nullptr;

	if (!m_Profiles->Profiles.size())
		return nullptr;

	_trt__GetStreamUriResponse StreamUriResponse;
	tt__StreamSetup StreamSetup;
	StreamSetup.Stream = tt__StreamType::tt__StreamType__RTP_Unicast;
	StreamSetup.Transport = new class tt__Transport();
	if (m_pMedia->GetStreamUri(StreamUriResponse, StreamSetup, szProfileToken) != SOAP_OK)
		return nullptr;
	else
		return StreamUriResponse.MediaUri->Uri.c_str();
}

int CONVIFClient::GetVideoSourceConfigure(char *szVideoConfigSourceToken, _trt__GetVideoSourceConfigurationResponse &GetVideoSourceConfigResponse)
{
	if (m_pMedia)
	{
		return m_pMedia->GetVideoSourceConfiguration(GetVideoSourceConfigResponse, szVideoConfigSourceToken);
	}
	else
		return -1;
}

int CONVIFClient::GetImageCapabilities(Image_Capabilities& Capabilities)
{
	if (!m_pOnvifImage)
	{
		if (!CreateImageClient())
			return -1;
	}
	_timg__GetServiceCapabilitiesResponse ImageCapabilitiesResponse;
	int nResult = m_pOnvifImage->GetServiceCapabilities(&ImageCapabilitiesResponse);

	if (nResult == SOAP_OK && ImageCapabilitiesResponse.Capabilities)
	{
		Capabilities.pBoolImageStabilization = ImageCapabilitiesResponse.Capabilities->ImageStabilization;
		if (ImageCapabilitiesResponse.Capabilities->__any.size())
			Capabilities.vecAny = ImageCapabilitiesResponse.Capabilities->__any;
		if (ImageCapabilitiesResponse.Capabilities->__anyAttribute)
			Capabilities.szAnyAttribute = ImageCapabilitiesResponse.Capabilities->__anyAttribute;
		else
			Capabilities.szAnyAttribute = nullptr;
		return nResult;
	}
	else
		return nResult;
}

int CONVIFClient::SetImageSetting(const char* szVideoSourceToken, ImagingSettings& ImagingSettings, bool bForcePersistence)
{
	if (!m_pOnvifImage)
	{
		if (!CreateImageClient())
			return -1;
	}
	_timg__SetImagingSettings timg__SetImagingSettings;
	timg__SetImagingSettings.VideoSourceToken = szVideoSourceToken;
	timg__SetImagingSettings.ForcePersistence = &bForcePersistence;		// 是否永久保存，若为true,则目标设置备重启后也会继续生效，否则重启后失效
	timg__SetImagingSettings.ImagingSettings = (tt__ImagingSettings20*)&ImagingSettings;
	_timg__SetImagingSettingsResponse timg__SetImagingSettingsResponse;
	return  m_pOnvifImage->SetImagingSettings(&timg__SetImagingSettings, &timg__SetImagingSettingsResponse);
}

int CONVIFClient::GetImageSetting(const char* szVideoSourceToken, ImagingSettings** ppImagingSettings)
{
	if (!m_pOnvifImage)
	{
		if (!CreateImageClient())
			return -1;
	}

	_timg__GetImagingSettings timg__GetImagingSettings;
	timg__GetImagingSettings.VideoSourceToken = szVideoSourceToken;
	_timg__GetImagingSettingsResponse ImagingSettingsResponse;

	int nResult = m_pOnvifImage->GetImagingSettings(&timg__GetImagingSettings, &ImagingSettingsResponse);
	if (nResult == SOAP_OK && ImagingSettingsResponse.ImagingSettings)
	{
		*ppImagingSettings = (ImagingSettings*)ImagingSettingsResponse.ImagingSettings;

		//soap_destroy(ImagingSettingsResponse.ImagingSettings->soap);
		//delete ImagingSettingsResponse.ImagingSettings;
		return nResult;
	}
	else
		return nResult;
}

const char* CONVIFClient::GetMediaStreamUrl(int nProfile)
{
	if (!m_pMedia || !m_Profiles)
		return nullptr;

	if (!m_Profiles->Profiles.size())
		return nullptr;
	if (nProfile >= m_Profiles->Profiles.size() || nProfile < 0)
		return nullptr;
	_trt__GetStreamUriResponse StreamUriResponse;
	tt__StreamSetup StreamSetup;
	StreamSetup.Stream = tt__StreamType::tt__StreamType__RTP_Unicast;
	StreamSetup.Transport = new class tt__Transport();
	if (m_pMedia->GetStreamUri(StreamUriResponse, StreamSetup, m_Profiles->Profiles[nProfile]->token) != SOAP_OK)
		return nullptr;
	else
		return StreamUriResponse.MediaUri->Uri.c_str();
}
// tt__MoveStatus__IDLE = 0, tt__MoveStatus__MOVING = 1, tt__MoveStatus__UNKNOWN
bool CONVIFClient::GetStatus(float& x, float& y, float& z, PTZStatus& nPTStatus, PTZStatus& nZoomStatus)
{
	if (!m_pPTZClient)
		return false;
	_tptz__GetStatusResponse StatusResponse;
	nPTStatus = PTZ_Unknown;
	nZoomStatus = PTZ_Unknown;
	bool bResult = ((OnvifClientPTZ*)m_pPTZClient)->GetStatus(StatusResponse, ((_trt__GetProfilesResponse*)m_Profiles)->Profiles[0]->token) == SOAP_OK;
	if (!bResult)
	{
		//TraceMsgA("%s PTZ Operation Failed.\n", __FUNCTION__);
		return false;
	}

	if (!StatusResponse.PTZStatus)
		return false;

	if (!StatusResponse.PTZStatus->Position)
		return false;

	if (StatusResponse.PTZStatus->Position->PanTilt)
	{
		x = StatusResponse.PTZStatus->Position->PanTilt->x;
		y = StatusResponse.PTZStatus->Position->PanTilt->y;
	}
	if (StatusResponse.PTZStatus->Position->Zoom)
		z = StatusResponse.PTZStatus->Position->Zoom->x;

	if (!StatusResponse.PTZStatus->MoveStatus)
		return false;

	if (StatusResponse.PTZStatus->MoveStatus->PanTilt)
		nPTStatus = (PTZStatus) * (StatusResponse.PTZStatus->MoveStatus->PanTilt);
	if (StatusResponse.PTZStatus->MoveStatus->Zoom)
		nZoomStatus = (PTZStatus) * (StatusResponse.PTZStatus->MoveStatus->Zoom);

	return true;
}

bool CONVIFClient::AbsoluteMove(/*PTZCommand nCommand,*/ float x, float y, float z, int nSpeed, PTZ_Flag nFlag)
{
	if (!m_pPTZClient)
		return false;

	//tt__PTZConfiguration *pPTZConfig = nullptr;
	//if (m_Profiles->Profiles[m_nDefaultProfile]->PTZConfiguration)
	//	pPTZConfig = m_Profiles->Profiles[m_nDefaultProfile]->PTZConfiguration;

	//if (pPTZConfig )
	//{
	//	if (pPTZConfig->PanTiltLimits && pPTZConfig->PanTiltLimits->Range)
	//	{
	//		if (x < pPTZConfig->PanTiltLimits->Range->XRange->Min ||
	//			x > pPTZConfig->PanTiltLimits->Range->XRange->Max)
	//			return false;
	//		if (y < pPTZConfig->PanTiltLimits->Range->YRange->Min ||
	//			y > pPTZConfig->PanTiltLimits->Range->YRange->Max)
	//			return false;
	//	}
	//	if (pPTZConfig->ZoomLimits && pPTZConfig->ZoomLimits->Range)
	//		if (z < pPTZConfig->ZoomLimits->Range->XRange->Min ||
	//			z > pPTZConfig->ZoomLimits->Range->XRange->Max)
	//			return false;
	//}
	//if (100 < nSpeed || 0 > nSpeed)
	//	return false;
	/*if (1.0f < x || -1.0 > x)
		return false;
	if (1.0f < y || -1.0 > y)
		return false;
	if (1.0f < z || 0.0 > z)
		return false;*/

	bool bExecutable = true;
	bool bResult = true;

	tt__PTZSpeed Speed;
	tt__PTZVector position;

	do
	{
		if (((OnvifClientPTZ*)m_pPTZClient)->m_bHasAbsolutePTMove)
		{
			if ((nFlag & PTZ_Flag_PT) == PTZ_Flag_PT)
			{
				Speed.PanTilt = new tt__Vector2D;
				Speed.PanTilt->x = 0.01 * nSpeed;
				Speed.PanTilt->y = 0.01 * nSpeed;

				position.PanTilt = new tt__Vector2D;
				position.PanTilt->x = x;
				position.PanTilt->y = y;
			}
			else
			{
				Speed.PanTilt = nullptr;
				position.PanTilt = nullptr;
			}
		}

		if (((OnvifClientPTZ*)m_pPTZClient)->m_bHasAbsoluteZoomMove)
		{
			if ((nFlag & PTZ_Flag_ZOOM) == PTZ_Flag_ZOOM)
			{
				Speed.Zoom = new tt__Vector1D;
				position.Zoom = new tt__Vector1D;
				position.Zoom->x = z;
				Speed.Zoom->x = 0.01 * nSpeed;
			}
			else
			{
				position.Zoom = nullptr;
				Speed.Zoom = nullptr;
			}
		}

		/*switch (nCommand)
		{
		case PAN_LEFT:
			if (Speed.PanTilt)
				Speed.PanTilt->x = (-0.01)*nSpeed;
			else
				bExecutable = false;
			break;
		case PAN_RIGHT:
			if (Speed.PanTilt)
				Speed.PanTilt->x = 0.01*nSpeed;
			else
				bExecutable = false;
			break;
		case TITLT_UP:
			if (Speed.PanTilt)
				Speed.PanTilt->y = 0.01*nSpeed;
			else
				bExecutable = false;
			break;
		case TITLT_DOWN:
			if (Speed.PanTilt)
				Speed.PanTilt->y = (-0.01)*nSpeed;
			else
				bExecutable = false;
			break;
		case PT_LEFTUP:
			if (Speed.PanTilt)
			{
				Speed.PanTilt->x = (-0.01)*nSpeed;
				Speed.PanTilt->y = 0.01*nSpeed;
			}
			else
				bExecutable = false;
			break;
		case PT_LEFTDOWN:
			if (Speed.PanTilt)
			{
				Speed.PanTilt->x = (-0.01)*nSpeed;
				Speed.PanTilt->y = (-0.01)*nSpeed;
			}
			else
				bExecutable = false;
			break;
		case PT_RIGHTUP:
			if (Speed.PanTilt)
			{
				Speed.PanTilt->x = 0.01*nSpeed;
				Speed.PanTilt->y = 0.01*nSpeed;
			}
			else
				bExecutable = false;
			break;
		case PT_RIGHTDOWN:
			if (Speed.PanTilt)
			{
				Speed.PanTilt->x = 0.01*nSpeed;
				Speed.PanTilt->y = (-0.01)*nSpeed;
			}
			else
				bExecutable = false;
			break;
		case PTZ_IN:
			if (Speed.Zoom)
				Speed.Zoom->x = 0.01*nSpeed;
			else
				bExecutable = false;
			break;
		case PTZ_OUT:
			if (Speed.Zoom)
				Speed.Zoom->x = (-0.01)*nSpeed;
			else
				bExecutable = false;
			break;
		default:
			bExecutable = false;
			break;
		}*/

		_tptz__AbsoluteMoveResponse AbsMoveResponse;
		bResult = ((OnvifClientPTZ*)m_pPTZClient)->AbsoluteMove(AbsMoveResponse, Speed, position, ((_trt__GetProfilesResponse*)m_Profiles)->Profiles[0]->token) == SOAP_OK;

	} while (0);

	if (Speed.PanTilt)
		delete Speed.PanTilt;
	if (Speed.Zoom)
		delete Speed.Zoom;
	if (position.PanTilt)
		delete position.PanTilt;
	if (position.Zoom)
		delete position.Zoom;
	return bResult;
}

bool CONVIFClient::ContinueMove(PTZCommand nCommand, int nSpeed, LONG64 nTimeout)
{
	if (!m_pPTZClient)
		return false;
	bool bExecutable = true;
	bool bResult = true;
	tt__PTZSpeed Speed;
	do
	{
		if (((OnvifClientPTZ*)m_pPTZClient)->m_bHasContinuousPTMove)
		{
			Speed.PanTilt = new tt__Vector2D;
			Speed.PanTilt->x = 0;
			Speed.PanTilt->y = 0;
		}

		if (((OnvifClientPTZ*)m_pPTZClient)->m_bHasContinuousZoomMove)
			Speed.Zoom = new tt__Vector1D;

		switch (nCommand)
		{
		case PAN_LEFT:
			if (Speed.PanTilt)
				Speed.PanTilt->x = (-0.01) * nSpeed;
			else
				bExecutable = false;
			break;
		case PAN_RIGHT:
			if (Speed.PanTilt)
				Speed.PanTilt->x = 0.01 * nSpeed;
			else
				bExecutable = false;
			break;
		case TITLT_UP:
			if (Speed.PanTilt)
				Speed.PanTilt->y = 0.01 * nSpeed;
			else
				bExecutable = false;
			break;
		case TITLT_DOWN:
			if (Speed.PanTilt)
				Speed.PanTilt->y = (-0.01) * nSpeed;
			else
				bExecutable = false;
			break;
		case PT_LEFTUP:
			if (Speed.PanTilt)
			{
				Speed.PanTilt->x = (-0.01) * nSpeed;
				Speed.PanTilt->y = 0.01 * nSpeed;
			}
			else
				bExecutable = false;
			break;
		case PT_LEFTDOWN:
			if (Speed.PanTilt)
			{
				Speed.PanTilt->x = (-0.01) * nSpeed;
				Speed.PanTilt->y = (-0.01) * nSpeed;
			}
			else
				bExecutable = false;
			break;
		case PT_RIGHTUP:
			if (Speed.PanTilt)
			{
				Speed.PanTilt->x = 0.01 * nSpeed;
				Speed.PanTilt->y = 0.01 * nSpeed;
			}
			else
				bExecutable = false;
			break;
		case PT_RIGHTDOWN:
			if (Speed.PanTilt)
			{
				Speed.PanTilt->x = 0.01 * nSpeed;
				Speed.PanTilt->y = (-0.01) * nSpeed;
			}
			else
				bExecutable = false;
			break;
		case PTZ_IN:
			if (Speed.Zoom)
				Speed.Zoom->x = 0.01 * nSpeed;
			else
				bExecutable = false;
			break;
		case PTZ_OUT:
			if (Speed.Zoom)
				Speed.Zoom->x = (-0.01) * nSpeed;
			else
				bExecutable = false;
			break;
		default:
			bExecutable = false;
			break;
		}

		if (!bExecutable)
			break;
		_tptz__ContinuousMoveResponse ContMoveResponse;
		bResult = ((OnvifClientPTZ*)m_pPTZClient)->ContinuousMove(ContMoveResponse, Speed, nTimeout, ((_trt__GetProfilesResponse*)m_Profiles)->Profiles[0]->token) == SOAP_OK;

	} while (0);

	if (Speed.PanTilt)
		delete Speed.PanTilt;
	if (Speed.Zoom)
		delete Speed.Zoom;
	return bResult;
}

bool CONVIFClient::GotoHomePosition()
{
	if (!m_pPTZClient)
		return false;

	_tptz__GotoHomePositionResponse GotoHomeResponse;
	string profileToken;

	return (((OnvifClientPTZ*)m_pPTZClient)->GotoHomePosition(GotoHomeResponse, ((_trt__GetProfilesResponse*)m_Profiles)->Profiles[0]->token) == SOAP_OK);
}

bool CONVIFClient::GotoPreset(const char* strPresetToken)
{
	if (!m_pPTZClient)
		return false;

	_tptz__GotoPresetResponse GotoPresetResponse;
	string profileToken;

	return  (((OnvifClientPTZ*)m_pPTZClient)->GotoPreset(strPresetToken, GotoPresetResponse, strPresetToken) == SOAP_OK);
}

bool CONVIFClient::SetPreset(const char* strPrsetName, char* strPresetToken)
{
	if (!m_pPTZClient || !m_Profiles || !m_Profiles->Profiles.size())
		return false;

	if (m_nDefaultProfile >= m_Profiles->Profiles.size())
		return false;

	_tptz__SetPresetResponse SetPresetResponse;
	string profileToken;
	if (strPresetToken && strlen(strPresetToken))
	{// Modify an existing preset
		std::string strName(strPrsetName);
		std::string strToken(strPresetToken);
		return (((OnvifClientPTZ*)m_pPTZClient)->SetPreset(&strName, &strToken, SetPresetResponse, m_Profiles->Profiles[m_nDefaultProfile]->token) == SOAP_OK);
	}
	else
	{// add a new one
		std::string strName(strPrsetName);

		if (((OnvifClientPTZ*)m_pPTZClient)->SetPreset(&strName, nullptr, SetPresetResponse, m_Profiles->Profiles[m_nDefaultProfile]->token) == SOAP_OK)
		{
			strcpy(strPresetToken, SetPresetResponse.PresetToken.c_str());
			return true;
		}
		else
			return false;
	}
}

bool CONVIFClient::RemovePreset(const char* strPresetToken)
{
	if (!m_pPTZClient || !m_Profiles || !m_Profiles->Profiles.size())
		return false;

	if (m_nDefaultProfile >= m_Profiles->Profiles.size())
		return false;

	_tptz__RemovePresetResponse RemovePresetResponse;
	string profileToken;

	return (((OnvifClientPTZ*)m_pPTZClient)->RemovePreset(strPresetToken, RemovePresetResponse, m_Profiles->Profiles[m_nDefaultProfile]->token) == SOAP_OK);
}

void CONVIFClient::Stop()
{
	if (!m_pPTZClient)
		return;

	_tptz__StopResponse StopResponse;
	((OnvifClientPTZ*)m_pPTZClient)->Stop(StopResponse, true, true, ((_trt__GetProfilesResponse*)m_Profiles)->Profiles[m_nDefaultProfile]->token);
}

// Set the defuault Profile for PTZ Operations
bool CONVIFClient::SetDefaultProfile(int nIndex)
{
	if (!m_Profiles || !m_Profiles->Profiles.size())
		return false;
	if (nIndex < m_Profiles->Profiles.size() && nIndex >= 0)
	{
		m_nDefaultProfile = nIndex;
		return true;
	}
	else
		return false;
}

const OnvifClientDevice* CONVIFClient::GetDevicePtr()
{
	return m_pOnvifDevice;
}
const OnvifClientMedia* CONVIFClient::GetMediaPtr()
{
	return m_pMedia;
}
const OnvifClientPTZ* CONVIFClient::GetPtzPtr()
{
	return m_pPTZClient;
}
