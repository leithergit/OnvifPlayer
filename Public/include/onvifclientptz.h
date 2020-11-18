#ifndef __ONVIF_CLIENT_PTZ__
#define __ONVIF_CLIENT_PTZ__

#include <string>
#include <map>
#include <iostream>
#include <list>
#include "onvifclientdevice.h"
#include "soapStub.h"
#include "soapPTZBindingProxy.h"
#include "wsseapi.h"

using namespace std;

typedef int(__stdcall PTZBindingProxy::* PtzMemberFunc)(void* p1, void* p2);
class  OnvifClientPTZ :public PTZBindingProxy
{
public:
	Declare_ClassName(OnvifClientPTZ);
	OnvifClientPTZ(OnvifClientDevice& device);

	~OnvifClientPTZ();

	bool m_bHasRelativePTMove;
	bool m_bHasRelativeZoomMove;
	bool m_bHasAbsolutePTMove;
	bool m_bHasAbsoluteZoomMove;
	bool m_bHasContinuousPTMove;
	bool m_bHasContinuousZoomMove;
public:
	int  Initialize();

	int  Stop(_tptz__StopResponse& StopResponse, bool PanTilt, bool Zoom, string profileToken);

	int  AbsoluteMove(_tptz__AbsoluteMoveResponse& AbsoluteMoveResponse, tt__PTZSpeed& Speed, tt__PTZVector& position, string profileToken);

	int  RelativeMove(_tptz__RelativeMoveResponse& relMoveResponse, tt__PTZVector& Translation, tt__PTZSpeed& Speed, string profileToken);

	int  GetPresets(_tptz__GetPresetsResponse& tptz__GetPresetsResponse, string profileToken);

	int  GotoPreset(string strPresetToken, _tptz__GotoPresetResponse& tptz__GotoPresetResponse, string profileToken);

	int  SetPreset(string* pstrPrsetName, string* pstrPresetToken, _tptz__SetPresetResponse& tptz__SetPresetResponse, string profileToken);

	int  RemovePreset(string strPresetToken, _tptz__RemovePresetResponse& tptz__SetPresetResponse, string profileToken);

	int  GotoHomePosition(_tptz__GotoHomePositionResponse& HomePositionResponse, string profileToken);

	int  ContinuousMove(_tptz__ContinuousMoveResponse& ContMoveResponse, tt__PTZSpeed& Speed, LONG64& Timeout, string profileToken);

	int  SetConfiguration(_tptz__SetConfigurationResponse& SetConfigurationResponse, tt__PTZConfiguration& PTZConfiguration, bool ForcePersist);

	int  GetServiceCapabilities(_tptz__GetServiceCapabilitiesResponse& ServiceCapResponse);

	int  GetStatus(_tptz__GetStatusResponse& StatusResponse, string profileToken);

	int  GetNode(_tptz__GetNodeResponse& GetNodeResponse, string nodeToken);

	int  GetNodes(_tptz__GetNodesResponse& GetNodesResponse);

	int  GetConfigurations(_tptz__GetConfigurationsResponse& configurationsResp);

	int  GetConfiguration(_tptz__GetConfigurationResponse& configurationResp);

	int  GetConfigurationOptions(_tptz__GetConfigurationOptionsResponse& configOptions, string configToken);

private:
	OnvifClientDevice& m_Device;
	http_da_info httpinfo;
	bool bHttpda;
	const char* httpuserid = nullptr;
	const char* httppasswd = nullptr;
	std::string m_strUrl;
};

#endif 
