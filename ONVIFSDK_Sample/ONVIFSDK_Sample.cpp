// ONVIFLIB_Sample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#ifdef _ONVIF_STATIC
#include "../ONVIFSDK/onvifLIB.h"
#ifdef _DEBUG
#pragma comment(lib,"../3rdparty/openssl-1.0.1g/lib/libeay32MTd.lib")
#pragma comment(lib,"../3rdparty/openssl-1.0.1g/lib/ssleay32MTd.lib")
#pragma comment(lib,"../ONVIFSDK/ONVIFLIBD.lib")
#else
#pragma comment(lib,"../3rdparty/openssl-1.0.1g/lib/libeay32MT.lib")
#pragma comment(lib,"../3rdparty/openssl-1.0.1g/lib/ssleay32MT.lib")
#pragma comment(lib,"../ONVIFSDK/ONVIFLIB.lib")
#endif
#else
#include "../ONVIFSDK/ONVIFSDK.h"
#ifdef _DEBUG
#pragma comment(lib,"../ONVIFSDK/ONVIFSDKD.lib")
#else
#pragma comment(lib,"../ONVIFSDK/ONVIFSDK.lib")
#endif
#endif

#include <string>
#include <iostream>
#include <memory>
using namespace std;
//using namespace std::tr1;

#define _UTF8(UnicodeText)				(UTF8StringW_(UnicodeText).get())
#define _AnsiString(UnicodeText,CP)		(W2AString_(UnicodeText,CP).get())
#define _UnicodeString(AnsiText,CP)		(A2WString_(AnsiText,CP).get())

#define WSA_VERSION MAKEWORD(2,2)

shared_ptr<char> UTF8StringW_(IN LPCWSTR pTextW)
{
	int nLenW = wcslen(pTextW);
	int nNeedBuffSize = ::WideCharToMultiByte(CP_UTF8, NULL, pTextW, nLenW, NULL, 0, NULL, NULL);
	char* pUTF8 = new char[nNeedBuffSize + 1];
	ZeroMemory(pUTF8, nNeedBuffSize + 1);

	WideCharToMultiByte(CP_UTF8, NULL, pTextW, nLenW, pUTF8, nNeedBuffSize + 1, NULL, NULL);
	return shared_ptr<char>(pUTF8);
}

shared_ptr<char> W2AString_(IN LPCWSTR str, UINT nCodePage)
{
	int nLenW = wcslen(str);
	int nNeedBuffSize = ::WideCharToMultiByte(nCodePage, NULL, str, nLenW, NULL, 0, NULL, NULL);
	char* pAnsi = new char[nNeedBuffSize + 1];
	ZeroMemory(pAnsi, nNeedBuffSize + 1);
	WideCharToMultiByte(nCodePage, NULL, str, nLenW, pAnsi, nNeedBuffSize + 1, NULL, NULL);
	return shared_ptr<char>(pAnsi);
}

shared_ptr<WCHAR> A2WString_(IN LPCSTR str, UINT nCodePage)
{
	int nLenA = strlen(str);
	int nNeedBuffSize = ::MultiByteToWideChar(nCodePage, NULL, str, nLenA, NULL, 0);

	WCHAR* pUnicode = new WCHAR[nNeedBuffSize + 1];
	ZeroMemory(pUnicode, (nNeedBuffSize + 1) * sizeof(WCHAR));

	MultiByteToWideChar(nCodePage, 0, str, nLenA, pUnicode, nNeedBuffSize + 1);
	return shared_ptr<WCHAR>(pUnicode);
}

struct Camera
{
	std::string strIP;
	std::string strUser;
	std::string strPass;
};
int _tmain(int argc, _TCHAR* argv[])
{

	Camera CameraArray[] = {
		//  HIKVision
		//{ "192.168.1.101", "admin", "admin12345"},
		//{ "192.168.1.102", "admin", "admin12345" },
		//{ "192.168.1.99", "admin", "admin12345" },
		//{ "192.168.10.53", "admin", "Admin12345" },
		/*{ "192.168.10.54", "admin", "Admin12345" },*/
		// Axis
		{ "192.168.10.10", "root", "pass" },
		//{ "192.168.10.15", "root", "pass" },
		//{ "192.168.10.194", "root", "pass" },
		//{ "192.168.2.11", "root", "pass" }
		//{ "192.168.2.17", "root", "pass" },
		//{ "192.168.20.227", "root", "Pass1234" },
		//{ "192.168.10.42", "root", "pass" }
		//{ "192.168.10.44", "root", "pass" }
	};

#ifdef _ONVIF_STATIC
	WSADATA		WSAData = { 0 };
	if (WSAStartup(WSA_VERSION, &WSAData) == 0)
	{
		if (LOBYTE(WSAData.wVersion) != LOBYTE(WSA_VERSION) ||
			HIBYTE(WSAData.wVersion) != HIBYTE(WSA_VERSION))
		{
			WSACleanup();
			return false;
		}
	}
#endif
	CONVIFClient* pClient = nullptr;
	int nCameraIdx = 0;
	do
	{
		const char* IP = CameraArray[nCameraIdx].strIP.c_str();
		const char* user = CameraArray[nCameraIdx].strUser.c_str();
		const char* pass = CameraArray[nCameraIdx].strPass.c_str();

		try
		{
			if (nCameraIdx)
			{
				system("pause");
				system("cls");
			}

			nCameraIdx++;
			// Create a ONVIFClient for the specified IP
			pClient = new CONVIFClient(IP, user, pass);
			if (!pClient)
			{
				cout << "Insufficent memory!" << endl;
				continue;
			}

			// Set the default Profile for the PTZ operation if it's supported
			if (!pClient->SetDefaultProfile(0))
			{
				cout << "Failed in SetDefaultProfile from Camera:" << IP << endl;
				continue;
			}
			// output the codec information
			int nIndex = 0;
			for (auto it = pClient->m_Profiles->Profiles.begin(); it != pClient->m_Profiles->Profiles.end(); it++)
			{
				tt__Profile* pProfile = *it;
				cout << "****************************************************" << endl;
				cout << "Profile[" << nIndex++ << "]:Name = " << pProfile->Name << "token =" << pProfile->token << endl;
				if (pProfile->VideoSourceConfiguration)
					cout << "\tVideoSourceConfiguration:Name = " << pProfile->VideoSourceConfiguration->Name << "\ttoken = " << pProfile->VideoSourceConfiguration->token << endl;
				if (pProfile->AudioSourceConfiguration)
					cout << "\tAudioSourceConfiguration:Name = " << pProfile->AudioSourceConfiguration->Name << "\ttoken = " << pProfile->AudioSourceConfiguration->token << endl;
				if (pProfile->VideoEncoderConfiguration)
				{
					static string strEncodec[] = { "JPEG","MPEG4","H264","H265" };
					tt__VideoEncoderConfiguration* VideoEncoder = pProfile->VideoEncoderConfiguration;
					cout << "\tVideoEncoderConfiguration:Name = " << VideoEncoder->Name << "\ttoken = " << VideoEncoder->token << endl;
					cout << "\t\t" << "Encodeing = " << strEncodec[VideoEncoder->Encoding] << endl;
					if (VideoEncoder->Resolution)
						cout << "\t\t" << "Resolution = " << VideoEncoder->Resolution->Width << "x" << VideoEncoder->Resolution->Height << endl;
				}

				if (pProfile->AudioEncoderConfiguration)
					cout << "\tAudioEncoderConfiguration:Name = " << pProfile->AudioEncoderConfiguration->Name << "\ttoken = " << pProfile->AudioEncoderConfiguration->token << endl;
				if (pProfile->PTZConfiguration)
				{
					tt__PTZConfiguration* PTZ = pProfile->PTZConfiguration;
					cout << "\tPTZConfiguration:NodeToken = " << PTZ->NodeToken << endl;
					if (PTZ->DefaultAbsolutePantTiltPositionSpace)
						cout << "\t\tDefaultAbsolutePantTiltPositionSpace" << *PTZ->DefaultAbsolutePantTiltPositionSpace << endl;
					if (PTZ->DefaultAbsoluteZoomPositionSpace)
						cout << "\t\tDefaultAbsolutePantTiltPositionSpace" << *PTZ->DefaultAbsoluteZoomPositionSpace << endl;
					if (PTZ->DefaultRelativePanTiltTranslationSpace)
						cout << "\t\tDefaultAbsolutePantTiltPositionSpace" << *PTZ->DefaultAbsolutePantTiltPositionSpace << endl;
					if (PTZ->DefaultRelativeZoomTranslationSpace)
						cout << "\t\tDefaultAbsolutePantTiltPositionSpace" << *PTZ->DefaultRelativeZoomTranslationSpace << endl;
					if (PTZ->DefaultContinuousPanTiltVelocitySpace)
						cout << "\t\tDefaultAbsolutePantTiltPositionSpace" << *PTZ->DefaultContinuousPanTiltVelocitySpace << endl;
					if (PTZ->DefaultContinuousZoomVelocitySpace)
						cout << "\t\tDefaultAbsolutePantTiltPositionSpace" << *PTZ->DefaultContinuousZoomVelocitySpace << endl;
					if (PTZ->PanTiltLimits)
					{
						if (PTZ->PanTiltLimits->Range)
						{
							if (PTZ->PanTiltLimits->Range->XRange)
								cout << "\t\t\tPanTiltLimits,XRange = [" << PTZ->PanTiltLimits->Range->XRange->Min << "," << PTZ->PanTiltLimits->Range->XRange->Max << "]" << endl;
							if (PTZ->PanTiltLimits->Range->YRange)
								cout << "\t\t\tPanTiltLimits,YRange = [" << PTZ->PanTiltLimits->Range->YRange->Min << "," << PTZ->PanTiltLimits->Range->YRange->Max << "]" << endl;
						}
					}
					if (PTZ->ZoomLimits && PTZ->ZoomLimits->Range)
						cout << "\t\t\tZoomLimits,Range = [" << PTZ->ZoomLimits->Range->XRange->Min << "," << PTZ->ZoomLimits->Range->XRange->Max << "]" << endl;
				}
			}

			//pClient->m_pMedia->GetVideoSourceConfiguration()
			// get the default Mediastram URL
			const char* szMediaUrl = pClient->GetMediaStreamUrl(0);
			if (!szMediaUrl)
				cout << "Failed in GetMediaStreamUrl from Camera:" << IP;
			else
				cout << "Camera " << IP << " Stream URL[0]  = " << szMediaUrl << endl;

			// Prepare for  PTZ Operation
			if (!pClient->CreatePTZClient())
			{
				cout << "Failed in CreatePTZClient from Camera " << IP << ",it may not support PTZ!" << endl;
				continue;
			}

			// Get all the Presets from camera
			if (!pClient->GetPresets())
			{
				cout << "Failed in GetPresets from " << IP << ",it may not support PTZ!" << endl;
				continue;
			}

			char strToken[256] = { 0 };
			std::string strPreset = "AAAA";
			bool bResult = pClient->SetPreset(strPreset.c_str(), strToken);
			if (bResult)
				cout << "Succeed in adding a Preset AAAA for Camera " << IP << endl;
			else
				cout << "Failed in  adding a Preset AAAA for Camera " << IP << endl;

			bResult = pClient->RemovePreset(strToken);
			if (bResult)
				cout << "Succeed in removing Preset AAAA for Camera " << IP << endl;
			else
				cout << "Failed in  removing Preset AAAA for Camera " << IP << endl;
			if (pClient->m_Presets)
			{
				// output all the presets
				cout << "***********PTZ Preset list**************" << endl;
				nIndex = 0;
				if (pClient->m_Presets->Preset.size() > 0)
					for (auto it = pClient->m_Presets->Preset.begin(); it != pClient->m_Presets->Preset.end(); it++, nIndex++)
					{
						tt__PTZPreset* PtzPrset = *it;
						if (PtzPrset->Name)
							cout << "\tPreset[" << nIndex << "] Name = " << _AnsiString(_UnicodeString(PtzPrset->Name->c_str(), CP_UTF8), CP_ACP);
						if (PtzPrset->PTZPosition)
						{
							if (PtzPrset->PTZPosition->PanTilt)
								cout << "\tPosition = [" << PtzPrset->PTZPosition->PanTilt->x << "," << PtzPrset->PTZPosition->PanTilt->y;
							if (PtzPrset->PTZPosition->Zoom)
								cout << "," << PtzPrset->PTZPosition->Zoom->x;
						}

						cout << "]" << endl;
					}
				else
					cout << "There is no Presets in Camera " << IP << "!" << endl;
			}
			// try to move the camera
			float x = 0.0f, y = 0.0f, z = 0.1f;
			PTZStatus PTstatus = PTZStatus::PTZ_Unknown, Zstatus = PTZ_Unknown;
			// 
			// 			if (pClient->GetStatus(x, y, z, PTstatus, Zstatus))
			// 				cout << "Camera " << IP << " Current Position = [" << x << "," << y << "," << z << "]" << endl;
			// 			else
			// 				cout << "Camera " << IP << " Failed in  GetStatus!" << endl;
			// 
			// 			int nSpeed = 25;
			// 			PTZCommand nCommandArray[] = { TITLT_UP, TITLT_DOWN, PTZ_IN, PTZ_OUT };
			// 
			// 			char* szCommandArray[] = { "TITLT_UP", "TITLT_DOWN", "PTZ_IN", "PTZ_OUT" };
			// 			long long nTimeOut = 3600;
			// 			int nSoapStatus = SOAP_OK;
			// 			for (int i = 0; i < sizeof(nCommandArray) / sizeof(PTZCommand); i++)
			// 			{
			// 				cout << "\tContinue move " << szCommandArray[i] << endl;
			// 				nSoapStatus = pClient->ContinueMove(nCommandArray[i], nSpeed, nTimeOut);
			// 				Sleep(5000);
			// 			}
			// 				 
			// 			pClient->Stop();
			float fZoom[] = { 0, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f };
			for (int nIndex = 0; nIndex < sizeof(fZoom) / sizeof(float); nIndex++)
				if (pClient->AbsoluteMove(x, y, fZoom[nIndex], 50))
				{
					if (pClient->GetStatus(x, y, z, PTstatus, Zstatus))
						cout << "Camera " << IP << " turn to Position = [" << x << "," << y << "," << z << "]" << endl;
					else
						cout << "Camera " << IP << " Failed in  GetStatus!" << endl;
					Sleep(2000);
				}
				else
					cout << "Camera " << IP << " Failed in AbsoluteMove,it May not support the sepecified PTZ Operation!" << endl;
			cout << "***********Now we will test ONVIF Image Setting Test**************" << endl;
			pClient->CreateImageClient();
			OnvifClientImage* pOnvifImage = (OnvifClientImage*)pClient->GetImagePtr();
			_timg__GetServiceCapabilitiesResponse ImageCapabilitiesResponse;
			int nResult = pOnvifImage->GetServiceCapabilities(&ImageCapabilitiesResponse);

			if (nResult == SOAP_OK && ImageCapabilitiesResponse.Capabilities)
			{
				// 是否有图像稳定系统，如防防抖功能
				cout << "ImageStabilization = " << ImageCapabilitiesResponse.Capabilities->ImageStabilization << endl;

				cout << "Image Capabilities Items size = " << ImageCapabilitiesResponse.Capabilities->__any.size() << endl;
				int nIndex = 0;
				for each (auto var in ImageCapabilitiesResponse.Capabilities->__any)
					cout << "Item[" << nIndex << "] = " << var << endl;
			}

			std::string strVideoSourceToken = pClient->m_Profiles->Profiles[0]->VideoSourceConfiguration->SourceToken;
			_timg__GetImagingSettings timg__GetImagingSettings;
			timg__GetImagingSettings.VideoSourceToken = strVideoSourceToken;
			_timg__GetImagingSettingsResponse ImagingSettingsResponse;

			char* szExposureMode[] = { "AUTO","MANUAL" };
			char* szBacklightCompensationMode[] = { "BacklightCompensationMode__OFF", "BacklightCompensationMode__ON" };
			char* szExposurePriority[] = { "Priority LowNoise",  "Priority FrameRate" };
			char* szAutoFocusMode[] = { "AutoFocusMode__AUTO","AutoFocusMode__MANUAL" };
			char* szIrCutFilterMode[] = { "IrCutFilterMode__ON","IrCutFilterMode__OFF", "IrCutFilterMode__AUTO" };
			char* szWideDynamicMode[] = { "WideDynamicMode__OFF", "WideDynamicMode__ON" };
			char* szWhiteBalanceMode[] = { "WhiteBalanceMode__AUTO", "WhiteBalanceMode__MANUAL" };
			char* szImageStabilizationMode[] = { "ImageStabilizationMode__OFF", "ImageStabilizationMode__ON","ImageStabilizationMode__AUTO","ImageStabilizationMode__Extended" };

			nResult = pOnvifImage->GetImagingSettings(&timg__GetImagingSettings, &ImagingSettingsResponse);
			if (nResult == SOAP_OK && ImagingSettingsResponse.ImagingSettings)
			{
				if (ImagingSettingsResponse.ImagingSettings->BacklightCompensation)
				{
					cout << "Backlight Compensation Level = " << ImagingSettingsResponse.ImagingSettings->BacklightCompensation->Level << endl;
					cout << "BacklightCompensation Mode = " << szBacklightCompensationMode[ImagingSettingsResponse.ImagingSettings->BacklightCompensation->Mode];
				}
				else
					cout << "Setting BacklightCompensation is not supported!" << endl;

				if (ImagingSettingsResponse.ImagingSettings->Brightness)
					cout << "Setting Brightness = " << *(ImagingSettingsResponse.ImagingSettings->Brightness) << endl;
				else
					cout << "Setting Brightness it not supported!" << endl;

				if (ImagingSettingsResponse.ImagingSettings->ColorSaturation)
					cout << "ColorSaturation = " << *(ImagingSettingsResponse.ImagingSettings->ColorSaturation) << endl;
				else
					cout << "Setting ColorSaturation is not supported!" << endl;

				if (ImagingSettingsResponse.ImagingSettings->Contrast)
					cout << "Contrast = " << *(ImagingSettingsResponse.ImagingSettings->Contrast) << endl;
				else
					cout << "Setting Contrast is not supported!" << endl;

				if (ImagingSettingsResponse.ImagingSettings->Exposure)
				{
					tt__Exposure20* Exposure = ImagingSettingsResponse.ImagingSettings->Exposure;
					tt__Rectangle* Window = Exposure->Window;

					cout << "ExposureMode = " << szExposureMode[ImagingSettingsResponse.ImagingSettings->Exposure->Mode] << endl;

					if (Exposure->Priority)
					{
						cout << "ExposurePriority=" << szExposurePriority[*(Exposure->Priority)] << endl;
					}
					if (Exposure->Window)
					{
						cout << "Exposure Window :left = " << *Window->left << endl;
						cout << "Exposure Window :top = " << *Window->top << endl;
						cout << "Exposure Window :right = " << *Window->right << endl;
						cout << "Exposure Window :bottom = " << *Window->bottom << endl;
					}

					cout << "ExposureMode=" << szExposureMode[Exposure->Mode] << endl;

					if (Exposure->ExposureTime)
						cout << "ExposureTime = " << *(Exposure->ExposureTime) << endl;

					if (Exposure->MinExposureTime)
						cout << "MinExposureTime = " << *(Exposure->MinExposureTime) << endl;

					if (Exposure->MaxExposureTime)
						cout << "MaxExposureTime = " << *(Exposure->MaxExposureTime) << endl;

					if (Exposure->Gain)
						cout << "ExposureGain = " << *(Exposure->Gain) << endl;

					if (Exposure->MaxGain)
						cout << "ExposureMaxGain = " << *(Exposure->MaxGain) << endl;

					if (Exposure->MinGain)
						cout << "ExposureMinGain = " << *(Exposure->MinGain) << endl;

					if (Exposure->MaxGain)
						cout << "ExposureMaxIris = " << *(Exposure->MaxGain) << endl;

					if (Exposure->Iris)
						cout << "ExposureIris = " << *(Exposure->Iris) << endl;

					if (Exposure->MaxIris)
						cout << "ExposureMaxIris = " << *(Exposure->MaxIris) << endl;

					if (Exposure->MinIris)
						cout << "ExposureMaxIris = " << *(Exposure->MinIris) << endl;
				}
				else
					cout << "Setting Exposure is not supported!" << endl;
			}

			_timg__SetImagingSettings timg__SetImagingSettings;
			timg__SetImagingSettings.VideoSourceToken = strVideoSourceToken;
			timg__SetImagingSettings.ForcePersistence = false;		// 是否永久保存，若为true,则目标设置备重启后也会继续生效，否则重启后失效
			timg__SetImagingSettings.ImagingSettings = ImagingSettingsResponse.ImagingSettings;
			_timg__SetImagingSettingsResponse timg__SetImagingSettingsResponse;
			nResult = pOnvifImage->SetImagingSettings(&timg__SetImagingSettings, &timg__SetImagingSettingsResponse);
			if (nResult == SOAP_OK)
				cout << "Succeed in Setimage Settings!" << endl;
			else
				cout << "Failed in Setimage Settings!" << endl;


			_timg__GetOptions ImageOptions;
			ImageOptions.VideoSourceToken = strVideoSourceToken;
			_timg__GetOptionsResponse ImageOptionsResponse;
			nResult = pOnvifImage->GetOptions(&ImageOptions, &ImageOptionsResponse);
			if (nResult == SOAP_OK)
			{
				if (ImageOptionsResponse.ImagingOptions)
				{
					tt__ImagingOptions20* ImagingOptions = ImageOptionsResponse.ImagingOptions;
					if (ImagingOptions->BacklightCompensation)
					{
						if (ImagingOptions->BacklightCompensation->Level)
						{
							if (ImagingOptions->BacklightCompensation->Level->__item)
								cout << "BacklightCompensation Level Range Item:" << ImagingOptions->BacklightCompensation->Level->__item << endl;

							cout << "ImagingOptions BacklightCompensation Level = [" << ImagingOptions->BacklightCompensation->Level->Min
								<< ","
								<< ImagingOptions->BacklightCompensation->Level->Max << "]" << endl;
							int nIndex = 0;
							for each (auto var in ImagingOptions->BacklightCompensation->Mode)
								cout << "BacklightCompensation Mode[" << nIndex++ << "] = " << szBacklightCompensationMode[var] << endl;
						}

					}
					else
						cout << "Setting BacklightCompensation is not supported!" << endl;

					if (ImagingOptions->Brightness)
						cout << "Brightness  = [" << ImagingOptions->Brightness->Min << "," << ImagingOptions->Brightness->Max << "]" << endl;

					if (ImagingOptions->ColorSaturation)
						cout << "ColorSaturation  = [" << ImagingOptions->ColorSaturation->Min << "," << ImagingOptions->ColorSaturation->Max << "]" << endl;

					if (ImagingOptions->Contrast)
						cout << "Contrast = [" << ImagingOptions->Contrast->Min << "," << ImagingOptions->Contrast->Max << "]" << endl;

					if (ImagingOptions->Exposure)
					{

						tt__ExposureOptions20* Exposure = ImagingOptions->Exposure;
						if (Exposure->ExposureTime)
							cout << "ExposureTime Item " << Exposure->ExposureTime->__item << " = [" << Exposure->ExposureTime->Min << "," << Exposure->ExposureTime->Max << "]" << endl;

						if (Exposure->MinExposureTime)
							cout << "MinExposureTime Item " << Exposure->MinExposureTime->__item << " = [" << Exposure->MinExposureTime->Min << "," << Exposure->MinExposureTime->Max << "]" << endl;

						if (Exposure->MaxExposureTime)
							cout << "MaxExposureTime Item " << Exposure->MaxExposureTime->__item << " = [" << Exposure->MaxExposureTime->Min << "," << Exposure->MaxExposureTime->Max << "]" << endl;

						if (Exposure->Gain)
							cout << "Gain Item " << Exposure->Gain->__item << " = [" << Exposure->Gain->Min << "," << Exposure->Gain->Max << "]" << endl;

						if (Exposure->MinGain)
							cout << "MinGain Item " << Exposure->MinGain->__item << " = [" << Exposure->MinGain->Min << "," << Exposure->MinGain->Max << "]" << endl;

						if (Exposure->MaxGain)
							cout << "MaxGain Item " << Exposure->MaxGain->__item << " = [" << Exposure->MaxGain->Min << "," << Exposure->MaxGain->Max << "]" << endl;

						if (Exposure->Iris)
							cout << "Iris Item " << Exposure->Iris->__item << " = [" << Exposure->Iris->Min << "," << Exposure->Iris->Max << "]" << endl;

						if (Exposure->MinIris)
							cout << "MinIris Item " << Exposure->MinIris->__item << " = [" << Exposure->MinIris->Min << "," << Exposure->MinIris->Max << "]" << endl;

						if (Exposure->MaxIris)
							cout << "MaxIris Item " << Exposure->MaxIris->__item << " = [" << Exposure->MaxIris->Min << "," << Exposure->MaxIris->Max << "]" << endl;
						nIndex = 0;
						for each (auto var in Exposure->Mode)
							cout << "Exposure Mode[" << nIndex++ << "] = " << szExposureMode[var] << endl;

						nIndex = 0;
						for each (auto var in Exposure->Priority)
							cout << "Exposure Priority[" << nIndex++ << "] = " << szExposurePriority[var] << endl;
					}
					else
					{
						cout << "Exposure option is not supported!" << endl;
					}
					if (ImagingOptions->Focus)
					{
						nIndex = 0;
						for each (auto var in ImagingOptions->Focus->AutoFocusModes)
							cout << "ImagingOptions Focus AutoFocusModes[" << nIndex++ << "] = " << szAutoFocusMode[var] << endl;
						if (ImagingOptions->Focus->DefaultSpeed)
							cout << "ImagingOptions Focus DefaultSpeed = [" << ImagingOptions->Focus->DefaultSpeed->Min << "," << ImagingOptions->Focus->DefaultSpeed->Max << "]" << endl;

						if (ImagingOptions->Focus->NearLimit)
							cout << "ImagingOptions Focus NearLimit = [" << ImagingOptions->Focus->NearLimit->Min << "," << ImagingOptions->Focus->NearLimit->Max << "]" << endl;
						if (ImagingOptions->Focus->FarLimit)
							cout << "ImagingOptions Focus FarLimit = [" << ImagingOptions->Focus->FarLimit->Min << "," << ImagingOptions->Focus->FarLimit->Max << "]" << endl;
						nIndex = 0;
						if (ImagingOptions->Focus->Extension)
							for each (auto var in ImagingOptions->Focus->Extension->__any)
								cout << "ImagingOptions->Focus->Extension [" << nIndex << "] = " << var << endl;

					}

					nIndex = 0;
					for each (auto var in ImagingOptions->IrCutFilterModes)
						cout << "IrCutFilterMode[" << nIndex++ << "] = " << szIrCutFilterMode[var] << endl;

					if (ImagingOptions->Sharpness)
						cout << "Sharpness  = [" << ImagingOptions->Sharpness->Min << "," << ImagingOptions->Sharpness->Max << "]" << endl;

					if (ImagingOptions->WideDynamicRange)
					{
						nIndex = 0;
						for each (auto var  in ImagingOptions->WideDynamicRange->Mode)
							cout << "WideDynamicRange [" << nIndex++ << "] = " << szWideDynamicMode[var] << endl;
						if (ImagingOptions->WideDynamicRange->Level)
							cout << "WideDynamicRange Level  = [" << ImagingOptions->WideDynamicRange->Level->Min << "," << ImagingOptions->WideDynamicRange->Level->Max << "]" << endl;
					}

					if (ImagingOptions->WhiteBalance)
					{
						nIndex = 0;
						for each (auto var in ImagingOptions->WhiteBalance->Mode)
							cout << "WhiteBalanceMode[" << nIndex << "] = " << szWhiteBalanceMode[var] << endl;
						if (ImagingOptions->WhiteBalance->YrGain)
							cout << "WideDynamicRange YrGain  = [" << ImagingOptions->WhiteBalance->YrGain->Min << "," << ImagingOptions->WhiteBalance->YrGain->Max << "]" << endl;
						if (ImagingOptions->WhiteBalance->YbGain)
							cout << "WideDynamicRange YbGain  = [" << ImagingOptions->WhiteBalance->YbGain->Min << "," << ImagingOptions->WhiteBalance->YbGain->Max << "]" << endl;
					}

					if (ImagingOptions->Extension)
					{
						if (ImagingOptions->Extension->ImageStabilization)
						{
							nIndex = 0;
							for each (auto var in ImagingOptions->Extension->ImageStabilization->Mode)
								cout << "ImageStabilization Mode[" << nIndex << "]" << szImageStabilizationMode[var] << endl;
							if (ImagingOptions->Extension->ImageStabilization->Level)
								cout << "ImageStabilization Level = [" << ImagingOptions->Extension->ImageStabilization->Level->Min << "," << ImagingOptions->Extension->ImageStabilization->Level->Max << "]" << endl;
						}
					}
				}
				else
					cout << "Setting ImagingOptions is not supported!" << endl;

			}

			_timg__GetMoveOptions MoveOptions;
			MoveOptions.VideoSourceToken = strVideoSourceToken;
			_timg__GetMoveOptionsResponse MoveOptionsResponse;
			nResult = pOnvifImage->GetMoveOptions(&MoveOptions, &MoveOptionsResponse);
			if (nResult == SOAP_OK)
			{
				if (MoveOptionsResponse.MoveOptions)
				{
					tt__MoveOptions20* MoveOptions = MoveOptionsResponse.MoveOptions;

					if (MoveOptions->Absolute)
					{
						if (MoveOptions->Absolute->Position)
							cout << "Absolute Position Range =  " << "[" << MoveOptions->Absolute->Position->Min << "," << MoveOptions->Absolute->Position->Max << "]" << endl;
						else
							cout << "Absolute Position is not supported!" << endl;
						if (MoveOptions->Absolute->Speed)
							cout << "Absolute Speed range = [" << MoveOptions->Absolute->Speed->Min << "," << MoveOptions->Absolute->Speed->Max << "]" << endl;
						else
							cout << "Absolute speed is not supported!" << endl;
					}
					else
						cout << "Absolute is not supported!" << endl;
					if (MoveOptions->Continuous)
					{
						if (MoveOptions->Continuous->Speed)
							cout << "Move Speed = [" << MoveOptions->Continuous->Speed->Min << "," << MoveOptions->Continuous->Speed->Max << "]" << endl;
						else
							cout << "Continuous Speed it not supported!" << endl;
					}
					else
						cout << "MoveOptions Continuous it not supported!" << endl;
					if (MoveOptions->Relative)
					{
						if (MoveOptions->Relative->Speed)
							cout << "MoveOptions Relative Speed Range = [" << MoveOptions->Relative->Speed->Min << "," << MoveOptions->Relative->Speed->Max << "]" << endl;
						else
							cout << "MoveOptions Relative Speed is not supported!" << endl;
					}
					else
						cout << "MoveOptions Relative is not supported!" << endl;
				}
				else
					cout << "Move options is not supported!" << endl;
			}

			_timg__GetStatus timg__GetStatus;
			timg__GetStatus.VideoSourceToken = strVideoSourceToken;
			_timg__GetStatusResponse StatusResponse;
			nResult = pOnvifImage->GetStatus(&timg__GetStatus, &StatusResponse);
			char* szMoveStatus[] = { "IDLE", "MOVING", "UNKNOWN" };

			if (nResult == SOAP_OK)
			{
				if (StatusResponse.Status)
				{
					if (StatusResponse.Status->FocusStatus20)
					{
						cout << "FocusStatus Position = " << StatusResponse.Status->FocusStatus20->Position << endl;
						cout << "FocusStatus MoveStatus = " << szMoveStatus[StatusResponse.Status->FocusStatus20->MoveStatus] << endl;
						if (StatusResponse.Status->FocusStatus20->Extension)
						{
							nIndex = 0;
							for each (auto var in StatusResponse.Status->FocusStatus20->Extension->__any)
								cout << "Status->FocusStatus20->Extension[" << nIndex++ << "] = " << var << endl;
						}
						else
							cout << "FocusStatus Extension is not supported!" << endl;
						if (StatusResponse.Status->FocusStatus20->Error)
							cout << "FocusStatus Error" << StatusResponse.Status->FocusStatus20->Error << endl;
						if (StatusResponse.Status->FocusStatus20->__anyAttribute)
							cout << "FocusStatus0 anyAttribute" << StatusResponse.Status->FocusStatus20->__anyAttribute << endl;
					}
					else
						cout << "Status FocusStatus it not supported!" << endl;
				}
			}

			//_timg__Move MoveReq;
			//MoveReq.VideoSourceToken = strVideoSourceToken;
			//MoveReq.Focus->
			//	_timg__MoveResponse MoveResponse;
			//nResult = pOnvifImage->Move(&MoveReq, &MoveResponse);

			//_timg__Stop timg__Stop;
			//_timg__StopResponse timg__StopResponse;
			//nResult = pOnvifImage->Stop(&timg__Stop, &timg__StopResponse);

		}
		catch (std::exception e)
		{
			cout << "Camera[" << IP << "] Catch a excpetion:" << e.what() << endl;
		}
	} while (nCameraIdx < sizeof(CameraArray) / sizeof(Camera));

	system("pause");
	return 0;
}
