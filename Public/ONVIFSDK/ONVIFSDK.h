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
	std::vector<char* >__any;
	bool* ImageStabilization;		/* optional attribute */
	char* __anyAttribute;			/* optional attribute */
};

enum BacklightCompensationMode
{
	BacklightCompensationMode__OFF = 0,
	BacklightCompensationMode__ON = 1
};
struct BacklightCompensation
{
	BacklightCompensationMode nMode;
	float* fLevel;
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
	float* fBottom;	/* optional attribute */
	float* fTop;		/* optional attribute */
	float* fRight;	/* optional attribute */
	float* fLeft;	/* optional attribute */
};
struct Exposure
{
	ExposureMode		Mode;	/* required element of type tt:ExposureMode */
	ExposurePriority* Priority;	/* optional element of type tt:ExposurePriority */
	RangeRectangle* Window;	/* optional element of type tt:Rectangle */
	float* fMinExposureTime;	/* optional element of type xsd:float */
	float* fMaxExposureTime;	/* optional element of type xsd:float */
	float* fMinGain;	/* optional element of type xsd:float */
	float* fMaxGain;	/* optional element of type xsd:float */
	float* fMinIris;	/* optional element of type xsd:float */
	float* fMaxIris;	/* optional element of type xsd:float */
	float* fExposureTime;	/* optional element of type xsd:float */
	float* fGain;	/* optional element of type xsd:float */
	float* fIris;	/* optional element of type xsd:float */
};

enum AutoFocusMode
{
	AutoFocusMode__AUTO = 0,
	AutoFocusMode__MANUAL = 1
};
struct OnvifExtension
{
	std::vector<char* >__any;
};
struct FocusConfiguration
{
	AutoFocusMode AutoFocusMode;	/* required element of type tt:AutoFocusMode */
	float* fDefaultSpeed;	/* optional element of type xsd:float */
	float* fNearLimit;	/* optional element of type xsd:float */
	float* fFarLimit;	/* optional element of type xsd:float */
	class  FocusConfiguration20Extension* Extension;	/* optional element of type tt:FocusConfiguration20Extension */
	char* __anyAttribute;	/* optional attribute */
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
	WideDynamicMode Mode;	/* required element of type tt:WideDynamicMode */
	float* fLevel;	/* optional element of type xsd:float */
};

enum WhiteBalanceMode
{
	WhiteBalanceMode__AUTO = 0,
	WhiteBalanceMode__MANUAL = 1
};

struct WhiteBalance
{
	WhiteBalanceMode Mode;			/* required element of type tt:WhiteBalanceMode */
	float* fCrGain;					/* optional element of type xsd:float */
	float* fCbGain;					/* optional element of type xsd:float */
	WhiteBalanceMode* Extension;	/* optional element of type tt:WhiteBalance20Extension */
	char* __anyAttribute;			/* optional attribute */
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
	ImageStabilizationMode Mode;	/* required element of type tt:ImageStabilizationMode */
	float* fLevel;	/* optional element of type xsd:float */
	OnvifExtension* Extension;	/* optional element of type tt:ImageStabilizationExtension */
	char* __anyAttribute;	/* optional attribute */
};

struct IrCutFilterAutoAdjustment
{
	std::string BoundaryType;	/* required element of type xsd:string */
	float* BoundaryOffset;	/* optional element of type xsd:float */
	LONG64* ResponseTime;	/* optional element of type xsd:duration */
	OnvifExtension* Extension;	/* optional element of type tt:IrCutFilterAutoAdjustmentExtension */
	char* __anyAttribute;	/* optional attribute */
};

struct ImagingSettingsExtensionEx
{
public:
	std::vector<IrCutFilterAutoAdjustment* >IrCutFilterAutoAdjustment;	/* optional element of type tt:IrCutFilterAutoAdjustment */
	OnvifExtension* Extension;											/* optional element of type tt:ImagingSettingsExtension203 */
};

struct ImagingSettingsExtension
{
	std::vector<char* >__any;
	ImageStabilization* ImageStabilization;	/* optional element of type tt:ImageStabilization */
	ImagingSettingsExtensionEx* Extension;	/* optional element of type tt:ImagingSettingsExtension202 */
};

struct ImagingSettings
{
	BacklightCompensation* pBacklightCompensation;	/* optional element of type tt:BacklightCompensation20 */
	float* Brightness;								/* optional element of type xsd:float */
	float* ColorSaturation;							/* optional element of type xsd:float */
	float* Contrast;								/* optional element of type xsd:float */
	Exposure* pExposure;								/* optional element of type tt:Exposure20 */
	FocusConfiguration* pFocus;						/* optional element of type tt:FocusConfiguration20 */
	enum IrCutFilterMode* IrCutFilter;				/* optional element of type tt:IrCutFilterMode */
	float* Sharpness;								/* optional element of type xsd:float */
	WideDynamicRange* pWideDynamicRange;				/* optional element of type tt:WideDynamicRange20 */
	WhiteBalance* pWhiteBalance;						/* optional element of type tt:WhiteBalance20 */
	ImagingSettingsExtension* pExtension;			/* optional element of type tt:ImagingSettingsExtension20 */
	char* __anyAttribute;							/* optional attribute */
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
