#ifndef __ONVIF_CLIENT__
#define __ONVIF_CLIENT__

#pragma once
//////////////////////////////////////////////////////////////////////////
// ******************************Attention*******************************
// ONVIFClient is build with VS2013 SP5 ,if you want to use it with other 
// verison Visual stdio you may need to rebuild it with your vs other with 
// it may cause your applicatoin crash
//////////////////////////////////////////////////////////////////////////

#include "onvifclientdevice.h"
#include "onvifclientmedia.h"
#include "onvifclientptz.h"
#include "OnvifClientImage.h"

enum PTZCommand
{
	PAN_LEFT,
	PAN_RIGHT,
	TITLT_UP,
	TITLT_DOWN,
	PT_LEFTUP,
	PT_LEFTDOWN,
	PT_RIGHTUP,
	PT_RIGHTDOWN,
	PTZ_IN,
	PTZ_OUT
};

enum PTZStatus
{
	PTZ_Idle,
	PTZ_Moving,
	PTZ_Unknown
};

#define DEV_CONTINUEMOVE                  (1000*60*15)
enum PTZ_Flag
{
	PTZ_Flag_PT = 1,
	PTZ_Flag_ZOOM = 2
};

class  CONVIFClient
{
public:
	static bool CheckIPAddress(const char* szAddress);

	CONVIFClient(const char* strIP, const char* strAccount, const char* strPassword, unsigned short nPort = 80, int nTimeout = 1500);

	~CONVIFClient();
	// tt__MoveStatus__IDLE = 0, tt__MoveStatus__MOVING = 1, tt__MoveStatus__UNKNOWN
	bool GetStatus(float& x, float& y, float& z, PTZStatus& nPTStatus, PTZStatus& nZoomStatus);

	bool AbsoluteMove(float x, float y, float z, int nSpeed = 50, PTZ_Flag nFlag = (PTZ_Flag)(PTZ_Flag_PT | PTZ_Flag_ZOOM));

	bool ContinueMove(PTZCommand nCommand, int nSpeed, long long nTimeout);

	bool GotoHomePosition();

	bool GotoPreset(const char* strPresetToken);

	// if you want to Create a new preset the parameter strPresetToken must be a buffer receiver the new Preset token
	// if the parameter strPresetToken it not nullptr and not a empty string ,it will change the preset specified by parameter strPresetToken
	bool SetPreset(const char* strPrsetName, char* strPresetToken);

	bool RemovePreset(const char* strPresetToken);

	void Stop();

	// Set the defuault Profile for PTZ Operations
	bool SetDefaultProfile(int nIndex = 0);

	const char* GetMediaStreamUrl(char* szProfileToken);

	const char* GetMediaStreamUrl(int nProfile);

	int GetVideoSourceConfigure(char *szVideoSourceToken, _trt__GetVideoSourceConfigurationResponse &GetVideoSourceConfigResponse);

	bool CreatePTZClient();

	bool CreateImageClient();

	bool GetPresets();

	const OnvifClientDevice* GetDevicePtr();
	const OnvifClientMedia* GetMediaPtr();
	const OnvifClientPTZ* GetPtzPtr();
	const OnvifClientImage* GetImagePtr();
public:
	_trt__GetProfilesResponse* m_Profiles = nullptr;
	/*_tptz__GetPresetsResponse*/
	_tptz__GetPresetsResponse* m_Presets = nullptr;

private:
	void CreateOnvifClient();
	char* m_strCameraIP = nullptr;
	char* m_strAccount = nullptr;
	char* m_strPassword = nullptr;
	char* m_strPTZUrl = nullptr;
	unsigned short m_nHttpPort = 80;
	OnvifClientDevice* m_pOnvifDevice = nullptr;
	OnvifClientMedia* m_pMedia = nullptr;
	OnvifClientPTZ* m_pPTZClient = nullptr;
	OnvifClientImage* m_pOnvifImage = nullptr;

	int m_nDefaultProfile = 0;
};

//void* AllocOnvifClient(const char* strIP, const char* strAccount, const char* strPassword)
//{
//	CONVIFClient* pONVIFClient = new CONVIFClient(strIP, strAccount, strPassword);
//	if (pONVIFClient)
//		return pONVIFClient;
//	else
//		return nullptr;
//}
//
//void FreeOnvifPTZ(void* p)
//{
//	if (!p)
//		return;
//	CONVIFClient* pONVIFClient = (CONVIFClient*)p;
//	delete pONVIFClient;
//}


#endif /* __ONVIF_CLIENT__ */
