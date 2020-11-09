// onvifclient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "onvifclient.hpp"
#include <mmsystem.h>
#include <memory>
#pragma comment(lib,"winmm.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	/* 192.168.1.1 is the NVT, 192.168.1.234 is the NVC */
// 	string url = "http://192.168.1.97/onvif/device_service";
// 	string url = "http://192.168.1.98/onvif/device_service";
// 	string url = "http://192.168.1.99/onvif/device_service";
//     string url = "http://192.168.2.53/onvif/device_service";
//   	string user = "admin";
//   	string pass = "Admin12345";

	//string url = "http://192.168.10.10/onvif/device_service";	
	string url = "http://192.168.2.11/onvif/device_service";
	/* Below is where to receive the event */
	string user = "root";
	string pass =  "pass";

	void *pONVIFDev = AllocOnvifPTZ("192.168.2.11", "root", "pass");

	
	//OnvifClientDevice onvifDevice(url, user, pass);
	//_tds__GetCapabilitiesResponse capabilitiesResponse;
	//

	//int nResult = SOAP_OK;
	//if (nResult = onvifDevice.GetCapabilities() != SOAP_OK)
	//{
	//	cout << "GetCapabilities Failed." << endl;
	//	return 0;
	//}

	//OnvifClientMedia media(onvifDevice);
	//_trt__GetProfilesResponse profiles;
	//if (media.GetProfiles(profiles) != SOAP_OK)
	//{
	//	cout << "GetProfiles Failed." << endl;
	//	return 0;
	//}
	//_trt__GetStreamUriResponse StreamUriResponse;
	//tt__StreamSetup StreamSetup;
	//StreamSetup.Stream = tt__StreamType::tt__StreamType__RTP_Unicast;
	//StreamSetup.Transport = new class tt__Transport();
	//if (media.GetStreamUri(StreamUriResponse, StreamSetup, profiles.Profiles[0]->token) == SOAP_OK)
	//{
	//	cout << "Media stream url = " << StreamUriResponse.MediaUri->Uri << endl;
	//}

	//string strurl;
	//if (!onvifDevice.GetPTZUrl(strurl))
	//{
	//	cout << "GetPTZUrl Failed." << endl;
	//	return 0;
	//}
	//
	//OnvifClientPTZ PTZCtrl(onvifDevice);
	//
	//_tptz__GetConfigurationsResponse ptzConfigResp;
	//if (PTZCtrl.GetConfigurations(ptzConfigResp) != SOAP_OK)
	//{
	//	cout << "GetConfigurations of PTZ Faild." << endl;
	//	return 0;
	//}
	//_tptz__GetServiceCapabilitiesResponse ServiceCapResponse;
	//if (PTZCtrl.GetServiceCapabilities(ServiceCapResponse) != SOAP_OK)
	//{
	//	cout << "GetServiceCapabilities of PTZ Failed." << endl;
	//	return 0;
	//}
	//
	//_tptz__ContinuousMoveResponse ContMoveResponse;
	//tt__PTZSpeed Speed;
	//Speed.PanTilt = new tt__Vector2D;
	//Speed.PanTilt->x = 1;
	//Speed.PanTilt->y = 0;
	///*Speed.Zoom = new tt__Vector1D;
	//Speed.Zoom->x = 0.01;*/
	//LONG64 Timeout = 5000; 
	//DWORD dwT1 = timeGetTime();
	//if (PTZCtrl.ContinuousMove(ContMoveResponse, Speed, Timeout, profiles.Profiles[0]->token) != SOAP_OK)
	//{
	//	cout << "ContinuousMove of PTZ Failed." << endl;
	//	delete Speed.PanTilt;
	//	delete Speed.Zoom;
	//	return 0;
	//}
	//int nTimeSpan = timeGetTime() - dwT1;
	//cout << "ContinuousMove Timespan = " << nTimeSpan << endl;
	
//	_tptz__StopResponse StopResponse;
//	bool PanTilt;
//	bool Zoom;
//	if (PTZCtrl.Stop(StopResponse, true, true, profiles.Profiles[0]->token) != SOAP_OK)
//	{
//		cout << "PTZ Stop Failed." << endl;
//		return 0;
//	}
//
//// 获取的摄像机的所有预置位
//	_tptz__GetPresetsResponse GetPresetResp;
//	if (PTZCtrl.GetPresets(GetPresetResp, profiles.Profiles[0]->token) != SOAP_OK)
//	{
//		cout << "GetPresets Failed." << endl;
//		return 0;
//	}
//	//连续调用所有预置位，以下代可正确用于安讯干和海康摄像机的预置位调用
//	for (auto it = GetPresetResp.Preset.begin(); it != GetPresetResp.Preset.end(); it++)
//	{
//		_tptz__GotoPresetResponse GotoPresetResp;
//		DWORD dwTime = timeGetTime();
//		if (PTZCtrl.GotoPreset(*((*it)->token), GotoPresetResp, profiles.Profiles[0]->token) != SOAP_OK)
//		{
//			cout << "GotoPreset Failed." << endl;
//			continue;
//		}
//		DWORD dwTimeSpan = timeGetTime() - dwTime;
//		printf("TimeSpan[%s] = %d\n", ((*it)->token)->c_str(), dwTimeSpan);
//		Sleep(2000);
//	}
	

	return 0;
}

