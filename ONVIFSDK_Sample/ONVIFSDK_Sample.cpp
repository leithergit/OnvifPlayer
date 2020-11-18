// ONVIFLIB_Sample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#ifdef _ONVIF_STATIC
#include "../Public/ONVIFSDK/onvifLIB.h"
#ifdef _DEBUG
#pragma comment(lib,"../Public/3rdparty/openssl-1.0.1g/lib/libeay32MTd.lib")
#pragma comment(lib,"../Public/3rdparty/openssl-1.0.1g/lib/ssleay32MTd.lib")
#pragma comment(lib,"../ONVIFSDK/ONVIFLIBD.lib")
#else
#pragma comment(lib,"../Public/3rdparty/openssl-1.0.1g/lib/libeay32MT.lib")
#pragma comment(lib,"../Public/3rdparty/openssl-1.0.1g/lib/ssleay32MT.lib")
#pragma comment(lib,"../Public/ONVIFSDK/ONVIFLIB.lib")
#endif
#else
#include "../Public/ONVIFSDK/ONVIFSDK.h"
#ifdef _DEBUG
#pragma comment(lib,"../Public/ONVIFSDK/ONVIFSDKD.lib")
#else
#pragma comment(lib,"../Public/ONVIFSDK/ONVIFSDK.lib")
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

void OutputProfiles(CONVIFClient* pClient)
{
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
			static string strEncodec[] = { "JPEG", "MPEG4", "H264", "H265" };
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
}

void OutputMediaUrl(CONVIFClient* pClient, const char* IP)
{
	// get the default Mediastram URL
	const char* szMediaUrl = pClient->GetMediaStreamUrl(0);
	if (!szMediaUrl)
		cout << "Failed in GetMediaStreamUrl from Camera:" << IP;
	else
		cout << "Camera " << IP << " Stream URL[0]  = " << szMediaUrl << endl;
}

void GetVideoSourceConfiguration(CONVIFClient* pClient, const char* IP)
{
	//_trt__GetVideoSourceConfigurationResponse GetVideoSourceConfigResponse;
	//char *szToken = (char *)pClient->m_Profiles->Profiles[0]->VideoSourceConfiguration->token.c_str();
	//if (pClient->GetVideoSourceConfigure(szToken, GetVideoSourceConfigResponse) == SOAP_OK)
	//{
	//	GetVideoSourceConfigResponse.Configuration->Bounds->height;
	//}
	//else
	//{
	//	cout << "Failed in GetVideoSourceConfigure from Camera " << IP <<"." << endl;
	//}
}
void GetAllPresets(CONVIFClient* pClient, const char* IP)
{
	if (!pClient->CreatePTZClient())
	{
		cout << "Failed in CreatePTZClient from Camera " << IP << ",it may not support PTZ!" << endl;
		return;
	}
	// Get all the Presets from camera
	if (!pClient->GetPresets())
	{
		cout << "Failed in GetPresets from " << IP << ",it may not support PTZ!" << endl;
		return;
	}
	if (pClient->m_Presets)
	{
		// output all the presets
		cout << "***********PTZ Preset list**************" << endl;
		int nIndex = 0;
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
}

void Add_RemovePreset(CONVIFClient* pClient, const char* IP)
{
	if (!pClient->CreatePTZClient())
	{
		cout << "Failed in CreatePTZClient from Camera " << IP << ",it may not support PTZ!" << endl;
		return;
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
}

void PTZMoveTest(CONVIFClient* pClient, const char* IP)
{
	if (!pClient->CreatePTZClient())
	{
		cout << "Failed in CreatePTZClient from Camera " << IP << ",it may not support PTZ!" << endl;
		return;
	}
	// try to move the camera
	float x, y, z;
	PTZStatus PTstatus = PTZStatus::PTZ_Unknown, Zstatus = PTZ_Unknown;
	if (pClient->GetStatus(x, y, z, PTstatus, Zstatus))
		cout << "Camera " << IP << " Current Position = [" << x << "," << y << "," << z << "]" << endl;
	else
		cout << "Camera " << IP << " Failed in  GetStatus!" << endl;
	int nSpeed = 25;
	PTZCommand nCommandArray[] = { TITLT_UP, TITLT_DOWN, PTZ_IN, PTZ_OUT };
	cout << "***********Now we will test ONVIF PTZ Moving**************" << endl;
	char* szCommandArray[] = { "TITLT_UP", "TITLT_DOWN", "PTZ_IN", "PTZ_OUT" };
	long long nTimeOut = 3600;
	int nSoapStatus = SOAP_OK;
	for (int i = 0; i < sizeof(nCommandArray) / sizeof(PTZCommand); i++)
	{
		cout << "\tContinue move " << szCommandArray[i] << endl;
		nSoapStatus = pClient->ContinueMove(nCommandArray[i], nSpeed, nTimeOut);
		Sleep(5000);
	}

	pClient->Stop();
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
}

void GetPtzStatus(CONVIFClient* pClient, const char* IP)
{
	if (!pClient->CreatePTZClient())
	{
		cout << "Failed in CreatePTZClient from Camera " << IP << ",it may not support PTZ!" << endl;
		return;
	}
	float x = 0.0f, y = 0.0f, z = 0.1f;
	PTZStatus PTstatus = PTZStatus::PTZ_Unknown, Zstatus = PTZ_Unknown;

	if (pClient->GetStatus(x, y, z, PTstatus, Zstatus))
		cout << "Camera " << IP << " Current Position = [" << x << "," << y << "," << z << "]" << endl;
	else
		cout << "Camera " << IP << " Failed in  GetStatus!" << endl;
}

void GetImageCapabilities(CONVIFClient* pClient, const char* IP)
{
	timg__Capabilities* ic = nullptr;
	int nResult = pClient->GetImageCapabilities(&ic);

	if (nResult == SOAP_OK)
	{
		// 是否有图像稳定系统，如防防抖功能
		cout << "ImageStabilization = " << *ic->ImageStabilization << endl;
		cout << "Image Capabilities Items size = " << ic->__any.size() << endl;
		int nIndex = 0;
		for each (auto var in ic->__any)
			cout << "Item[" << nIndex << "] = " << var << endl;
		FreeSoap(ic->soap);
	}
	else
	{
		cout << "The Camera " << IP << "May not support ImageCapabilities!" << endl;
	}
}

void GetImageSettings(CONVIFClient* pClient, const char* IP)
{
	cout << "***********Now we will test ONVIF Image Setting Test**************" << endl;
	char* szExposureMode[] = { "AUTO", "MANUAL" };
	char* szBacklightCompensationMode[] = { "BacklightCompensationMode__OFF", "BacklightCompensationMode__ON" };
	char* szExposurePriority[] = { "Priority LowNoise", "Priority FrameRate" };
	char* szAutoFocusMode[] = { "AutoFocusMode__AUTO", "AutoFocusMode__MANUAL" };
	char* szIrCutFilterMode[] = { "IrCutFilterMode__ON", "IrCutFilterMode__OFF", "IrCutFilterMode__AUTO" };
	char* szWideDynamicMode[] = { "WideDynamicMode__OFF", "WideDynamicMode__ON" };
	char* szWhiteBalanceMode[] = { "WhiteBalanceMode__AUTO", "WhiteBalanceMode__MANUAL" };
	char* szImageStabilizationMode[] = { "ImageStabilizationMode__OFF", "ImageStabilizationMode__ON", "ImageStabilizationMode__AUTO", "ImageStabilizationMode__Extended" };


	timg__Capabilities* pCapabilities = nullptr;
	int nResult = pClient->GetImageCapabilities(&pCapabilities);

	if (nResult == SOAP_OK && pCapabilities)
	{
		// 是否有图像稳定系统，如防防抖功能
		cout << "ImageStabilization = " << pCapabilities->ImageStabilization << endl;

		cout << "Image Capabilities Items size = " << pCapabilities->__any.size() << endl;
		int nIndex = 0;
		for each (auto var in pCapabilities->__any)
			cout << "Item[" << nIndex << "] = " << var << endl;
		FreeSoap(pCapabilities->soap);
	}

	std::string strVideoSourceToken = pClient->m_Profiles->Profiles[0]->VideoSourceConfiguration->SourceToken;


	tt__ImagingSettings20* pIS = nullptr;

	nResult = pClient->GetImageSettings(strVideoSourceToken.c_str(), &pIS);
	if (nResult == SOAP_OK && pIS)
	{
		if (pIS->BacklightCompensation)
		{
			cout << "Backlight Compensation Level = " << pIS->BacklightCompensation->Level << endl;
			cout << "BacklightCompensation Mode = " << szBacklightCompensationMode[pIS->BacklightCompensation->Mode];
		}
		else
			cout << "Setting BacklightCompensation is not supported!" << endl;

		if (pIS->Brightness)
			cout << "Setting Brightness = " << *(pIS->Brightness) << endl;
		else
			cout << "Setting Brightness it not supported!" << endl;

		if (pIS->ColorSaturation)
			cout << "ColorSaturation = " << *(pIS->ColorSaturation) << endl;
		else
			cout << "Setting ColorSaturation is not supported!" << endl;

		if (pIS->Contrast)
			cout << "Contrast = " << *(pIS->Contrast) << endl;
		else
			cout << "Setting Contrast is not supported!" << endl;

		if (pIS->Exposure)
		{
			tt__Exposure20* Exposure = pIS->Exposure;
			tt__Rectangle* Window = Exposure->Window;

			cout << "ExposureMode = " << szExposureMode[pIS->Exposure->Mode] << endl;

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
			FreeSoap(pIS->soap);
		}
		else
			cout << "Setting Exposure is not supported!" << endl;
	}

	nResult = pClient->SetImageSettings(strVideoSourceToken.c_str(), *pIS, false);
	if (nResult == SOAP_OK)
		cout << "Succeed in Setimage Settings!" << endl;
	else
		cout << "Failed in Setimage Settings!" << endl;


	tt__ImagingOptions20* ImagingOptions;

	nResult = pClient->GetImageOpstions(strVideoSourceToken.c_str(), &ImagingOptions);
	int nIndex = 0;
	if (nResult == SOAP_OK)
	{
		if (ImagingOptions)
		{
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
			FreeSoap(ImagingOptions->soap);
		}
		else
			cout << "Setting ImagingOptions is not supported!" << endl;

	}

	//_timg__GetMoveOptions MoveOptions;
	//MoveOptions.VideoSourceToken = strVideoSourceToken;
	//_timg__GetMoveOptionsResponse MoveOptionsResponse;
	//nResult = pOnvifImage->GetMoveOptions(&MoveOptions, &MoveOptionsResponse);
	//if (nResult == SOAP_OK)
	//{
	//	if (MoveOptionsResponse.MoveOptions)
	//	{
	//		tt__MoveOptions20* MoveOptions = MoveOptionsResponse.MoveOptions;

	//		if (MoveOptions->Absolute)
	//		{
	//			if (MoveOptions->Absolute->Position)
	//				cout << "Absolute Position Range =  " << "[" << MoveOptions->Absolute->Position->Min << "," << MoveOptions->Absolute->Position->Max << "]" << endl;
	//			else
	//				cout << "Absolute Position is not supported!" << endl;
	//			if (MoveOptions->Absolute->Speed)
	//				cout << "Absolute Speed range = [" << MoveOptions->Absolute->Speed->Min << "," << MoveOptions->Absolute->Speed->Max << "]" << endl;
	//			else
	//				cout << "Absolute speed is not supported!" << endl;
	//		}
	//		else
	//			cout << "Absolute is not supported!" << endl;
	//		if (MoveOptions->Continuous)
	//		{
	//			if (MoveOptions->Continuous->Speed)
	//				cout << "Move Speed = [" << MoveOptions->Continuous->Speed->Min << "," << MoveOptions->Continuous->Speed->Max << "]" << endl;
	//			else
	//				cout << "Continuous Speed it not supported!" << endl;
	//		}
	//		else
	//			cout << "MoveOptions Continuous it not supported!" << endl;
	//		if (MoveOptions->Relative)
	//		{
	//			if (MoveOptions->Relative->Speed)
	//				cout << "MoveOptions Relative Speed Range = [" << MoveOptions->Relative->Speed->Min << "," << MoveOptions->Relative->Speed->Max << "]" << endl;
	//			else
	//				cout << "MoveOptions Relative Speed is not supported!" << endl;
	//		}
	//		else
	//			cout << "MoveOptions Relative is not supported!" << endl;
	//	}
	//	else
	//		cout << "Move options is not supported!" << endl;
	//}

	tt__ImagingStatus20* Status = nullptr;
	nResult = pClient->GetImageStatus(strVideoSourceToken.c_str(), &Status);
	char* szMoveStatus[] = { "IDLE", "MOVING", "UNKNOWN" };

	if (nResult == SOAP_OK)
	{
		if (Status)
		{
			if (Status->FocusStatus20)
			{
				cout << "FocusStatus Position = " << Status->FocusStatus20->Position << endl;
				cout << "FocusStatus MoveStatus = " << szMoveStatus[Status->FocusStatus20->MoveStatus] << endl;
				if (Status->FocusStatus20->Extension)
				{
					nIndex = 0;
					for each (auto var in Status->FocusStatus20->Extension->__any)
						cout << "Status->FocusStatus20->Extension[" << nIndex++ << "] = " << var << endl;
				}
				else
					cout << "FocusStatus Extension is not supported!" << endl;
				if (Status->FocusStatus20->Error)
					cout << "FocusStatus Error" << Status->FocusStatus20->Error << endl;
				if (Status->FocusStatus20->__anyAttribute)
					cout << "FocusStatus0 anyAttribute" << Status->FocusStatus20->__anyAttribute << endl;
			}
			else
				cout << "Status FocusStatus it not supported!" << endl;
			FreeSoap(Status->soap);
		}
	}
}

void GetDevServices(CONVIFClient* pClient, const char* IP)
{
	_tds__GetServicesResponse Response;
	int nResult = pClient->GetDevServices(Response, false);
	if (SOAP_OK == nResult)
	{
		for each (auto var in Response.Service)
		{
			cout << "Namespace = " << var->Namespace << "\t";
			cout << "XAddr = " << var->XAddr << "\t";
			if (var->Version)
				cout << "Version:" << var->Version->Major << "." << var->Version->Minor << endl;
			if (var->Capabilities && var->Capabilities->__any)
			{
				cout << "Additional Information:" << var->Capabilities->__any << endl;
			}
		}
	}
	else
	{
		cout << "Failed in Getting Device Serices of " << IP << ", Return Code : " << nResult << endl;
	}
}

#define  OutputBoolean(D,x)  cout << #x <<":"<< (D.x == true ? "True" : "False") << endl;
#define  OutputVariant(D,x)  cout << #x <<":"<< D.x <<endl;

#define  OutputBooleanPtrOfPtr(D,x) if (D->x) cout << #x <<":"<< (*(D->x) == true ? "True" : "False") << endl;
#define  OutputVariantPtrOfPtr(D,x) if (D->x) cout << #x <<":"<< (*(D->x)) <<endl;

#define  OutputBooleanPtr(D,x) if (D.x) cout << #x <<":"<< (*(D.x) == true ? "True" : "False") << endl;
#define  OutputVariantPtr(D,x) if (D.x) cout << #x <<":"<< (*(D.x)) <<endl;

void GetDevCapbilities(CONVIFClient* pClient, const char* IP)
{
	_tds__GetServiceCapabilitiesResponse Response;
	int nResult = pClient->GetDevServiceCapabilities(Response);
	if (SOAP_OK == nResult)
	{
		if (Response.Capabilities)
		{
			tds__DeviceServiceCapabilities* pCab = Response.Capabilities;
			if (pCab->System)
			{
				cout << "**********System capbilities**********" << endl;
				OutputBooleanPtrOfPtr(pCab->System, DiscoveryBye);
				OutputBooleanPtrOfPtr(pCab->System, RemoteDiscovery);
				OutputBooleanPtrOfPtr(pCab->System, SystemBackup);
				OutputBooleanPtrOfPtr(pCab->System, SystemLogging);
				OutputBooleanPtrOfPtr(pCab->System, FirmwareUpgrade);
				OutputBooleanPtrOfPtr(pCab->System, HttpFirmwareUpgrade);
				OutputBooleanPtrOfPtr(pCab->System, HttpSystemBackup);
				OutputBooleanPtrOfPtr(pCab->System, HttpSystemLogging);
				OutputBooleanPtrOfPtr(pCab->System, HttpSupportInformation);
				OutputVariantPtrOfPtr(pCab->System, __anyAttribute);
			}
			if (pCab->Network)
			{
				cout << "**********Network capbilities**********" << endl;
				OutputBooleanPtrOfPtr(pCab->Network, IPFilter);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Network, ZeroConfiguration);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Network, IPVersion6);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Network, DynDNS);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Network, Dot11Configuration);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Network, HostnameFromDHCP);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Network, DHCPv6);	/* optional attribute */

				OutputVariantPtrOfPtr(pCab->Network, Dot1XConfigurations);	/* optional attribute */
				OutputVariantPtrOfPtr(pCab->Network, NTP);	/* optional attribute */
				OutputVariantPtrOfPtr(pCab->Network, __anyAttribute);	/* optional attribute */
			}
			if (pCab->Security)
			{
				cout << "**********Security capbilities**********" << endl;
				OutputBooleanPtrOfPtr(pCab->Security, TLS1_x002e0);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, TLS1_x002e1);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, TLS1_x002e2);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, OnboardKeyGeneration);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, AccessPolicyConfig);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, DefaultAccessPolicy);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, Dot1X);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, RemoteUserHandling);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, X_x002e509Token);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, SAMLToken);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, KerberosToken);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, UsernameToken);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, HttpDigest);	/* optional attribute */
				OutputBooleanPtrOfPtr(pCab->Security, RELToken);	/* optional attribute */

				OutputVariantPtrOfPtr(pCab->Security, SupportedEAPMethods);	/* optional attribute */
				OutputVariantPtrOfPtr(pCab->Security, MaxUsers);	/* optional attribute */
				OutputVariantPtrOfPtr(pCab->Security, __anyAttribute);	/* optional attribute */
			}
			if (pCab->Misc)
			{
				cout << "**********Misc capbilities**********" << endl;
				OutputVariantPtrOfPtr(pCab->Misc, AuxiliaryCommands);
				OutputVariantPtrOfPtr(pCab->Misc, __anyAttribute);
			}
		}
	}
	else
		cout << "Failed in Getting Device ServiceCapabilities of " << IP << " , Return Code : " << nResult << endl;
}

void GetDevInformation(CONVIFClient* pClient, const char* IP)
{
	_tds__GetDeviceInformationResponse Response;
	int nResult = pClient->GetDeviceInformation(Response);
	if (SOAP_OK == nResult)
	{
		OutputVariant(Response, Manufacturer);
		OutputVariant(Response, Model);	/* required element of type xsd:string */
		OutputVariant(Response, FirmwareVersion);	/* required element of type xsd:string */
		OutputVariant(Response, SerialNumber);	/* required element of type xsd:string */
		OutputVariant(Response, HardwareId);	/* required element of type xsd:string */
	}
	else
		cout << "Faled in Getting Device information of" << IP << ",Return Code:" << nResult << endl;
}

void GetSystemSupportInformation(CONVIFClient* pClient, const char* IP)
{
	_tds__GetSystemSupportInformationResponse Response;
	int nResult = pClient->GetSystemSupportInformation(Response);
	if (SOAP_OK == nResult)
	{
		if (Response.SupportInformation)
		{
			cout << "System Support informatiion:" << endl;
			if (Response.SupportInformation->Binary)
			{
				OutputVariantPtrOfPtr(Response.SupportInformation, String);
				OutputVariantPtrOfPtr(Response.SupportInformation->Binary, xmime__contentType);

				OutputVariantPtr(Response.SupportInformation->Binary->xop__Include, __ptr);
				OutputVariant(Response.SupportInformation->Binary->xop__Include, __size);
				OutputVariantPtr(Response.SupportInformation->Binary->xop__Include, id);
				OutputVariantPtr(Response.SupportInformation->Binary->xop__Include, type);
				OutputVariantPtr(Response.SupportInformation->Binary->xop__Include, options);
			}
		}
	}
	else
		cout << "Failed in Getting System Support Information,Return Code:" << nResult << endl;
}

void GetSystemDateTime(CONVIFClient* pClient, const char* IP)
{
	_tds__GetSystemDateAndTimeResponse Response;
	int nResult = pClient->GetSystemDateAndTime(Response);
	if (SOAP_OK == nResult)
	{
		if (Response.SystemDateAndTime)
		{
			char* szDateTimeType[] = { "Manual","NTP" };
			cout << "System date time:" << endl;
			cout << "Datetime Type" << szDateTimeType[Response.SystemDateAndTime->DateTimeType] << endl;
			cout << "DaylightSavings" << Response.SystemDateAndTime->DaylightSavings << endl;
			if (Response.SystemDateAndTime->TimeZone)
				cout << "Time Zone:" << Response.SystemDateAndTime->TimeZone->TZ << endl;
			if (Response.SystemDateAndTime->UTCDateTime)
			{
				if (Response.SystemDateAndTime->UTCDateTime->Date)
				{
					cout << "UTC Date:" << Response.SystemDateAndTime->UTCDateTime->Date->Year << "-"
						<< Response.SystemDateAndTime->UTCDateTime->Date->Month << "-"
						<< Response.SystemDateAndTime->UTCDateTime->Date->Day << "-"
						<< endl;
				}
				if (Response.SystemDateAndTime->UTCDateTime->Time)
				{
					cout << "UTC Time:" << Response.SystemDateAndTime->UTCDateTime->Time->Hour << ":"
						<< Response.SystemDateAndTime->UTCDateTime->Time->Minute << ":"
						<< Response.SystemDateAndTime->UTCDateTime->Time->Second << ":"
						<< endl;
				}
			}
			if (Response.SystemDateAndTime->LocalDateTime)
			{
				if (Response.SystemDateAndTime->LocalDateTime->Date)
				{
					cout << "UTC Date:" << Response.SystemDateAndTime->LocalDateTime->Date->Year << "-"
						<< Response.SystemDateAndTime->LocalDateTime->Date->Month << "-"
						<< Response.SystemDateAndTime->LocalDateTime->Date->Day << "-"
						<< endl;
				}
				if (Response.SystemDateAndTime->LocalDateTime->Time)
				{
					cout << "UTC Time:" << Response.SystemDateAndTime->LocalDateTime->Time->Hour << ":"
						<< Response.SystemDateAndTime->LocalDateTime->Time->Minute << ":"
						<< Response.SystemDateAndTime->LocalDateTime->Time->Second << ":"
						<< endl;
				}
			}
			if (Response.SystemDateAndTime->Extension)
			{
				cout << "System Datetime Extension:" << endl;
				for each (auto var in Response.SystemDateAndTime->Extension->__any)
					cout << var << endl;
			}
		}
	}
	else
		cout << "Failed in Getting System date and time of " << IP << ",Return code:" << nResult << endl;
}

void SetSystemDateTime(CONVIFClient* pClient, const char* IP)
{
	tt__SetDateTimeType DateTimeType = tt__SetDateTimeType::tt__SetDateTimeType__Manual;
	bool DayLightSavings = false;
	tt__TimeZone Timezone;
	Timezone.TZ = "1";
	tt__DateTime UTCDateTime;
	_tds__GetSystemDateAndTimeResponse Response1;
	int nResult = pClient->GetSystemDateAndTime(Response1);
	if (SOAP_OK == nResult)
	{
		_tds__SetSystemDateAndTimeResponse Response2;
		nResult = pClient->SetSystemDateAndTime(Response1.SystemDateAndTime->DateTimeType,
			Response1.SystemDateAndTime->DaylightSavings,
			*(Response1.SystemDateAndTime->TimeZone),
			*(Response1.SystemDateAndTime->UTCDateTime),
			Response2);
		if (SOAP_OK == nResult)
			cout << "Succeed in Setting System date time for IP " << IP << endl;
		else
			cout << "Failed in Setting System date and time of " << IP << ",Return code:" << nResult << endl;
	}
	else
		cout << "Failed in Getting System date and time of " << IP << ",Return code:" << nResult << endl;
}

void SetSysemFactoryDefault(CONVIFClient* pClient, const char* IP)
{
	enum tt__FactoryDefaultType nFactoryDefaultType = tt__FactoryDefaultType::tt__FactoryDefaultType__Soft;
	_tds__SetSystemFactoryDefaultResponse Response;
	int nResult = pClient->SetSystemFactoryDefault(nFactoryDefaultType, Response);
	if (SOAP_OK == nResult)
		cout << "Succeed in Setting SystemFactoryDefault for " << IP << endl;
	else
		cout << "Failed in Setting SystemFactoryDefault for " << IP << endl;
}

void SystemReboot(CONVIFClient* pClient, const char* IP)
{
	_tds__SystemRebootResponse Response;
	int nResult = pClient->SystemReboot(Response);
	if (SOAP_OK == nResult)
		cout << "Succeed in Setting SystemFactoryDefault for " << IP << ".System Message:" << Response.Message << endl;
	else
		cout << "Failed in Setting SystemFactoryDefault for " << IP << endl;
}

void GetSystemLog(CONVIFClient* pClient, const char* IP)
{
	enum tt__SystemLogType nLogType = tt__SystemLogType::tt__SystemLogType__Access;
	_tds__GetSystemLogResponse Response;
	int nResult = pClient->GetSystemLog(nLogType, Response);
	if (nResult == SOAP_OK)
	{
		if (Response.SystemLog)
		{
			cout << "Access log of " << IP << ":" << endl;
			if (Response.SystemLog->String)
				cout << "Log Message:" << Response.SystemLog->String << endl;
			if (Response.SystemLog->Binary)
			{
				if (Response.SystemLog->Binary->xop__Include.type)
					cout << "Type:" << Response.SystemLog->Binary->xop__Include.type << endl;
				cout << "Size:" << Response.SystemLog->Binary->xop__Include.__size << endl;
				if (Response.SystemLog->Binary->xop__Include.id)
					cout << "id:" << Response.SystemLog->Binary->xop__Include.id << endl;
				if (Response.SystemLog->Binary->xop__Include.options)
					cout << "option:" << Response.SystemLog->Binary->xop__Include.options << endl;
			}
		}
	}
	else
		cout << "Failed in Getting System log of " << IP << endl;

	nLogType = tt__SystemLogType::tt__SystemLogType__System;
	nResult = pClient->GetSystemLog(nLogType, Response);
	if (nResult == SOAP_OK)
	{
		if (Response.SystemLog)
		{
			cout << "System log of " << IP << ":" << endl;
			if (Response.SystemLog->String)
				cout << "Log Message:" << Response.SystemLog->String << endl;
			if (Response.SystemLog->Binary)
			{
				if (Response.SystemLog->Binary->xop__Include.type)
					cout << "Type:" << Response.SystemLog->Binary->xop__Include.type << endl;
				cout << "Size:" << Response.SystemLog->Binary->xop__Include.__size << endl;
				if (Response.SystemLog->Binary->xop__Include.id)
					cout << "id:" << Response.SystemLog->Binary->xop__Include.id << endl;
				if (Response.SystemLog->Binary->xop__Include.options)
					cout << "option:" << Response.SystemLog->Binary->xop__Include.options << endl;
			}
		}
	}
	else
		cout << "Failed in Getting System log of " << IP << endl;
}

void GetNTP(CONVIFClient* pClient, const char* IP)
{
	_tds__GetNTPResponse Response;
	int nResult = pClient->GetNTP(Response);
	if (SOAP_OK == nResult)
	{
		char* szNetworkType[] = { "IPv4","IPv6","DNS" };
		if (Response.NTPInformation)
		{
			cout << "From DHCP:" << (Response.NTPInformation->FromDHCP ? "True" : "False") << endl;
			if (Response.NTPInformation->FromDHCP)
			{
				if (Response.NTPInformation->NTPFromDHCP.size())
					for each (auto var in Response.NTPInformation->NTPFromDHCP)
					{
						cout << szNetworkType[var->Type] << "=";
						switch (var->Type)
						{
						case tt__NetworkHostType__IPv4:
							if (var->IPv4Address)
								cout << var->IPv4Address << endl;
							break;
						case tt__NetworkHostType__IPv6:
							if (var->IPv6Address)
								cout << var->IPv6Address << endl;
							break;
						case tt__NetworkHostType__DNS:
							if (var->DNSname)
								cout << var->DNSname << endl;
							break;
						default:
							break;
						}
						if (var->Extension &&
							var->Extension->__any.size())
						{
							cout << "Network Extension" << endl;
							for each (auto var1 in var->Extension->__any)
								cout << var1 << endl;
						}
					}
			}
			else
			{
				if (Response.NTPInformation->NTPManual.size())
					for each (auto var in Response.NTPInformation->NTPManual)
					{
						cout << szNetworkType[var->Type] << "=";
						switch (var->Type)
						{
						case tt__NetworkHostType__IPv4:
							if (var->IPv4Address)
								cout << var->IPv4Address << endl;
							break;
						case tt__NetworkHostType__IPv6:
							if (var->IPv6Address)
								cout << var->IPv6Address << endl;
							break;
						case tt__NetworkHostType__DNS:
							if (var->DNSname)
								cout << var->DNSname << endl;
							break;
						default:
							break;
						}
						if (var->Extension &&
							var->Extension->__any.size())
						{
							cout << "Network Extension" << endl;
							for each (auto var1 in var->Extension->__any)
								cout << var1 << endl;
						}
					}
			}
		}
	}
	else
		cout << "Failed in Gettint NTP of " << IP << endl;
}

void GetScopes(CONVIFClient* pClient, const char* IP)
{
	char* szScopeDefinition[] = { "Fixed","Configurable" };
	_tds__GetScopesResponse Response;
	int nResult = pClient->GetScopes(Response);
	if (SOAP_OK == nResult)
	{
		if (Response.Scopes.size())
		{
			for each (auto var in Response.Scopes)
			{
				cout << "Scope Item:" << var->ScopeItem << ":" << szScopeDefinition[var->ScopeDef] << endl;
			}
		}
	}
	else
		cout << "Failed in Gettting Scopes of " << IP << endl;
}
enum MenuItem
{
	Menu_Device = 1,
	Menu_GetProfiles,
	Menu_GetMediaUrl,
	Menu_ListAllPrests,
	Menu_PTZMoveTest,
	Menu_AddandRemovePreset,
	Menu_GetPTZStatus,
	Menu_GetImageCapbilites,
	Menu_GetImggeSetting,
	Menu_Exit
};

enum DevMenu
{
	Get_Device_Services = 1,
	Get_Device_Capabilites,
	Get_Device_information,
	Get_SystemSupport_Information,
	Get_System_DateTime,
	Set_System_DateTime,
	Set_System_Factory_Default,
	System_Reboot,
	Get_System_Log,
	Get_NTP,
	Set_NTP,
	Get_Scopes,
	Return_Mainmenu
};

MenuItem Menu()
{
	int nItem = 1;
	int nSelectedItem = -1;
	cout << "Please Selece a item to excute:" << endl;
	cout << "\t" << nItem++ << ".Device Menu" << endl;
	cout << "\t" << nItem++ << ".Get Profiles" << endl;
	cout << "\t" << nItem++ << ".Get Media Url" << endl;
	cout << "\t" << nItem++ << ".List All Presets" << endl;
	cout << "\t" << nItem++ << ".PTZ Move Test" << endl;
	cout << "\t" << nItem++ << ".Add a Preset and then Remove it" << endl;
	cout << "\t" << nItem++ << ".Get Current PTZ Status" << endl;
	cout << "\t" << nItem++ << ".Get Image Capabilities" << endl;
	cout << "\t" << nItem++ << ".Get Image Settings" << endl;
	//cout << "\t" << nItem++ << ".Get Snapshot Url" << endl;
	cout << "\t" << nItem++ << ".Exit" << endl;
	cout << "\t" << "Please input a Item:";
	cin >> nSelectedItem;
	return (MenuItem)nSelectedItem;
}

DevMenu MenuDev()
{
	int nItem = 1;
	int nSelectedItem = -1;
	cout << "Please Selece a item to excute:" << endl;
	cout << "\t" << nItem++ << ".Get Device Services" << endl;
	cout << "\t" << nItem++ << ".Get Device Capabilites" << endl;
	cout << "\t" << nItem++ << ".Get Device information" << endl;
	cout << "\t" << nItem++ << ".Get SystemSupport Information" << endl;
	cout << "\t" << nItem++ << ".Get System Date And Time" << endl;
	cout << "\t" << nItem++ << ".Set System Date And Time" << endl;
	cout << "\t" << nItem++ << ".Set System Factory Default" << endl;
	cout << "\t" << nItem++ << ".System Reboot" << endl;
	cout << "\t" << nItem++ << ".Get System Log" << endl;
	cout << "\t" << nItem++ << ".Get NTP" << endl;
	cout << "\t" << nItem++ << ".Set NTP" << endl;
	cout << "\t" << nItem++ << ".Get Scopes" << endl;
	//cout << "\t" << nItem++ << ".Get Snapshot Url" << endl;
	cout << "\t" << nItem++ << ".Return to Main Menu" << endl;
	cout << "\t" << "Please input a Item:";
	cin >> nSelectedItem;
	return (DevMenu)nSelectedItem;
}

void OnDevMenu(CONVIFClient* pClient, const char* IP)
{
	try
	{
		system("cls");
		bool bLoopDev = true;
		while (bLoopDev)
		{
			DevMenu nDevItem = MenuDev();
			switch (nDevItem)
			{
			case Get_Device_Services:
			{
				GetDevServices(pClient, IP);
				system("pause");
				system("cls");
				break;
			}
			case Get_Device_Capabilites:
			{
				GetDevCapbilities(pClient, IP);
				system("pause");
				system("cls");
				break;
			}
			case Get_Device_information:
			{
				GetDevInformation(pClient, IP);
				system("pause");
				system("cls");
				break;
			}
			case Get_SystemSupport_Information:
			{
				GetSystemSupportInformation(pClient, IP);
				system("pause");
				system("cls");
				break;
			}
			case Get_System_DateTime:
			{
				GetSystemDateTime(pClient, IP);
				system("pause");
				system("cls");
				break;
			}
			case Set_System_DateTime:
			{
				SetSystemDateTime(pClient, IP);
				system("pause");
				system("cls");
				break;
			};
			case Set_System_Factory_Default:
			{
				SetSysemFactoryDefault(pClient, IP);
				system("pause");
				system("cls");
				break;
			};
			case System_Reboot:
			{
				SystemReboot(pClient, IP);
				system("pause");
				system("cls");
				break;
			};
			case Get_System_Log:
			{
				GetSystemLog(pClient, IP);
				system("pause");
				system("cls");
				break;
			};
			case Get_NTP:
			{
				GetNTP(pClient, IP);
				system("pause");
				system("cls");
				break;
			};
			case Set_NTP:
			{
				system("pause");
				system("cls");
				break;
			};
			case Get_Scopes:
			{
				GetScopes(pClient, IP);
				system("pause");
				system("cls");
				break;
			};
			case Return_Mainmenu:
				bLoopDev = false;
				system("cls");
				break;
			default:
				break;
			}
		}
	}
	catch (std::exception e)
	{
		cout << "Camera[" << IP << "] Catch a excpetion:" << e.what() << endl;
	}
}
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
		//{ "192.168.10.10", "root", "pass" },
		//{ "192.168.10.15", "root", "pass" },
		//{ "192.168.10.194", "root", "pass" },
		//{ "192.168.2.11", "root", "pass" }
		//{ "192.168.2.17", "root", "pass" },
		{ "192.168.20.227", "root", "Pass1234" },
		//{ "192.168.20.228", "root", "Pass1234" },
		//{ "192.168.10.42", "root", "pass" },
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

	const char* IP = CameraArray[nCameraIdx].strIP.c_str();
	const char* user = CameraArray[nCameraIdx].strUser.c_str();
	const char* pass = CameraArray[nCameraIdx].strPass.c_str();

	do
	{
		try
		{
			// Create a ONVIFClient for the specified IP
			pClient = new CONVIFClient(IP, user, pass);
			if (!pClient)
			{
				cout << "Insufficent memory!" << endl;
				break;
			}

			// Set the default Profile for the PTZ operation if it's supported
			if (!pClient->SetDefaultProfile(0))
			{
				cout << "Failed in SetDefaultProfile from Camera:" << IP << endl;
				break;;
			}

			bool bLoop = true;
			while (bLoop)
			{
				MenuItem nItem = Menu();
				switch (nItem)
				{
				case Menu_Device:
				{
					OnDevMenu(pClient, IP);
					system("cls");
				}
				break;
				case Menu_GetProfiles:
					OutputProfiles(pClient);
					system("pause");
					system("cls");
					break;
				case Menu_GetMediaUrl:
					OutputMediaUrl(pClient, IP);
					system("pause");
					system("cls");
					break;
				case Menu_ListAllPrests:
					GetAllPresets(pClient, IP);
					system("pause");
					system("cls");
					break;
				case Menu_PTZMoveTest:
					PTZMoveTest(pClient, IP);
					system("pause");
					system("cls");
					break;
				case Menu_AddandRemovePreset:
					Add_RemovePreset(pClient, IP);
					system("pause");
					system("cls");
					break;
				case Menu_GetPTZStatus:
					GetPtzStatus(pClient, IP);
					system("pause");
					system("cls");
					break;
					break;
				case Menu_GetImageCapbilites:
					GetImageCapabilities(pClient, IP);
					system("pause");
					system("cls");
					break;
				case Menu_GetImggeSetting:
					GetImageSettings(pClient, IP);
					break;
				case Menu_Exit:
					bLoop = false;
					break;
				default:
					cout << "Please Input a valid Item:" << endl;
					break;
				}
			}
			if (pClient)
				delete pClient;
		}
		catch (std::exception e)
		{
			cout << "Camera[" << IP << "] Catch a excpetion:" << e.what() << endl;
		}
	} while (0);

	system("pause");
	return 0;
}
