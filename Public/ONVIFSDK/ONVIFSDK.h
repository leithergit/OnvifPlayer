#ifndef __ONVIF_CLIENT__
#define __ONVIF_CLIENT__

#pragma once
#ifdef ONVIFLIB_EXPORTS
#define ONVIFLIB_API __declspec(dllexport)
#else
#define ONVIFLIB_API __declspec(dllimport)
#endif
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


struct Image_Capabilities
{
	char szReserve[sizeof(xsd__anyType)];
	std::vector<char* >vecAny;
	bool* pBoolImageStabilization;		/* optional attribute */
	char* szAnyAttribute;			/* optional attribute */
	//~Image_Capabilities()
	//{
	//	if (vecAny.size())
	//		for (auto it = vecAny.begin();it != vecAny.end();)
	//		 {
	//			free(*it);
	//			it = vecAny.erase(it);
	//		 }
	//	if (pBoolImageStabilization)
	//		free(pBoolImageStabilization);
	//	if (szAnyAttribute)
	//		free(szAnyAttribute);

	//}
};

enum BacklightCompensationMode
{
	BacklightCompensationMode__OFF = 0,
	BacklightCompensationMode__ON = 1
};
struct BacklightCompensation
{
	char szReserve[sizeof(xsd__anyType)];
	BacklightCompensationMode enumMode;
	float* pFloatLevel;
};

enum ExposureMode
{
	ExposureMode__AUTO = 0,
	ExposureMode__MANUAL = 1
};

enum ExposurePriority
{
	ExposurePriority__LowNoise = 0,
	ExposurePriority__FrameRate = 1
};
struct RangeRectangle
{
	char szReserve[sizeof(xsd__anyType)];
	float* pFloatBottom;	/* optional attribute */
	float* pFloatTop;		/* optional attribute */
	float* pFloatRight;	/* optional attribute */
	float* pFloatLeft;	/* optional attribute */
};
struct Exposure
{
	char szReserve[sizeof(xsd__anyType)];
	ExposureMode		enumMode;	/* required element of type tt:ExposureMode */
	ExposurePriority* pEnumPriority;	/* optional element of type tt:ExposurePriority */
	RangeRectangle* pRangeWindow;	/* optional element of type tt:Rectangle */
	float* pFloatMinExposureTime;	/* optional element of type xsd:float */
	float* pFloatMaxExposureTime;	/* optional element of type xsd:float */
	float* pFloatMinGain;	/* optional element of type xsd:float */
	float* pFloatMaxGain;	/* optional element of type xsd:float */
	float* pFloatMinIris;	/* optional element of type xsd:float */
	float* pFloatMaxIris;	/* optional element of type xsd:float */
	float* pFloatExposureTime;	/* optional element of type xsd:float */
	float* pFloatGain;	/* optional element of type xsd:float */
	float* pFloatIris;	/* optional element of type xsd:float */
};

enum AutoFocusMode
{
	AutoFocusMode__AUTO = 0,
	AutoFocusMode__MANUAL = 1
};
struct OnvifExtension
{
	char szReserve[sizeof(xsd__anyType)];
	std::vector<char* >vecAny;
};
struct FocusConfiguration
{
	char szReserve[sizeof(xsd__anyType)];
	AutoFocusMode enumAutoFocusMode;	/* required element of type tt:AutoFocusMode */
	float* pFloatDefaultSpeed;	/* optional element of type xsd:float */
	float* pFloatNearLimit;	/* optional element of type xsd:float */
	float* pFloatFarLimit;	/* optional element of type xsd:float */
	OnvifExtension* pExtension;	/* optional element of type tt:FocusConfiguration20Extension */
	char* szAnyAttribute;	/* optional attribute */
};
enum IrCutFilterMode
{
	IrCutFilterMode__ON = 0,
	IrCutFilterMode__OFF = 1,
	IrCutFilterMode__AUTO = 2
};

enum WideDynamicMode
{
	WideDynamicMode__OFF = 0,
	WideDynamicMode__ON = 1
};

struct WideDynamicRange
{
	char szReserve[sizeof(xsd__anyType)];
	WideDynamicMode enumMode;	/* required element of type tt:WideDynamicMode */
	float* pFloatLevel;	/* optional element of type xsd:float */
};

enum WhiteBalanceMode
{
	WhiteBalanceMode__AUTO = 0,
	WhiteBalanceMode__MANUAL = 1
};

struct WhiteBalance
{
	char szReserve[sizeof(xsd__anyType)];
	WhiteBalanceMode enumMode;			/* required element of type tt:WhiteBalanceMode */
	float* pFloatCrGain;					/* optional element of type xsd:float */
	float* pFloatCbGain;					/* optional element of type xsd:float */
	OnvifExtension* pExtension;	/* optional element of type tt:WhiteBalance20Extension */
	char* szAnyAttribute;			/* optional attribute */
};

enum ImageStabilizationMode
{
	ImageStabilizationMode__OFF = 0,
	ImageStabilizationMode__ON = 1,
	ImageStabilizationMode__AUTO = 2,
	ImageStabilizationMode__Extended = 3
};

struct ImageStabilization
{
	char szReserve[sizeof(xsd__anyType)];
	ImageStabilizationMode enumMode;	/* required element of type tt:ImageStabilizationMode */
	float* pFloatLevel;	/* optional element of type xsd:float */
	OnvifExtension* pExtension;	/* optional element of type tt:ImageStabilizationExtension */
	char* szAnyAttribute;	/* optional attribute */
};

struct IrCutFilterAutoAdjustment
{
	char szReserve[sizeof(xsd__anyType)];
	std::string strBoundaryType;	/* required element of type xsd:string */
	float* pFloatBoundaryOffset;	/* optional element of type xsd:float */
	LONG64* pLongLongResponseTime;	/* optional element of type xsd:duration */
	OnvifExtension* Extension;	/* optional element of type tt:IrCutFilterAutoAdjustmentExtension */
	char* szAnyAttribute;	/* optional attribute */
};

struct ImagingSettingsExtensionEx
{
	char szReserve[sizeof(xsd__anyType)];
	std::vector<IrCutFilterAutoAdjustment* >vecIrCutFilterAutoAdjustment;	/* optional element of type tt:IrCutFilterAutoAdjustment */
	OnvifExtension* pExtension;											/* optional element of type tt:ImagingSettingsExtension203 */
};

struct ImagingSettingsExtension
{
	char szReserve[sizeof(xsd__anyType)];
	std::vector<char* >vecAny;
	ImageStabilization* pImageStabilization;	/* optional element of type tt:ImageStabilization */
	ImagingSettingsExtensionEx* pExtension;	/* optional element of type tt:ImagingSettingsExtension202 */
};

struct ImagingSettings
{
	char szReserve[sizeof(xsd__anyType)];
	BacklightCompensation* pBacklightCompensation;	/* optional element of type tt:BacklightCompensation20 */
	float* pFloatBrightness;						/* optional element of type xsd:float */
	float* pFloatColorSaturation;					/* optional element of type xsd:float */
	float* pFloatContrast;							/* optional element of type xsd:float */
	Exposure* pExposure;							/* optional element of type tt:Exposure20 */
	FocusConfiguration* pFocus;						/* optional element of type tt:FocusConfiguration20 */
	IrCutFilterMode* pEnumIrCutFilter;				/* optional element of type tt:IrCutFilterMode */
	float* pFloatSharpness;							/* optional element of type xsd:float */
	WideDynamicRange* pWideDynamicRange;			/* optional element of type tt:WideDynamicRange20 */
	WhiteBalance* pWhiteBalance;					/* optional element of type tt:WhiteBalance20 */
	ImagingSettingsExtension* pImagingSettingsExtension;/* optional element of type tt:ImagingSettingsExtension20 */
	char* szAnyAttribute;							/* optional attribute */
};


class  ONVIFLIB_API CONVIFClient
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

	bool CreatePTZClient();

	bool GetPresets();

	int GetImageCapabilities(Image_Capabilities& Capabilities);

	int SetImageSetting(const char* szVideoSourceToken, ImagingSettings& ImagingSettings, bool bForcePersistence = false);

	int GetImageSetting(const char* szVideoSourceToken, ImagingSettings** ppImagingSettings);

	int GetVideoSourceConfigure(char *szVideoSourceToken, _trt__GetVideoSourceConfigurationResponse &GetVideoSourceConfigResponse);

	const OnvifClientDevice* GetDevicePtr();
	const OnvifClientMedia* GetMediaPtr();
	const OnvifClientPTZ* GetPtzPtr();
public:
	_trt__GetProfilesResponse* m_Profiles = nullptr;
	/*_tptz__GetPresetsResponse*/
	_tptz__GetPresetsResponse* m_Presets = nullptr;

private:
	void CreateOnvifClient();
	bool CreateImageClient();
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


#endif /* __ONVIF_CLIENT__ */
