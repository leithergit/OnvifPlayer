#pragma once
#include <string>
#include <map>
#include <iostream>
#include <list>
#include "onvifclientdevice.h"
#include "soapStub.h"
#include "soapImagingBindingProxy.h"
#include "wsseapi.h"

using namespace std;
class OnvifClientImage :public ImagingBindingProxy
{

public:
	Declare_ClassName(OnvifClientImage);

	OnvifClientImage(OnvifClientDevice& device, http_da_info* pHttpInfo = nullptr);
	~OnvifClientImage();
	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_timg__GetServiceCapabilitiesResponse* timg__GetServiceCapabilitiesResponse);

	/// Web service operation 'GetImagingSettings' (returns error code or SOAP_OK)
	virtual	int GetImagingSettings(_timg__GetImagingSettings* timg__GetImagingSettings, _timg__GetImagingSettingsResponse* timg__GetImagingSettingsResponse);

	/// Web service operation 'SetImagingSettings' (returns error code or SOAP_OK)
	virtual	int SetImagingSettings(_timg__SetImagingSettings* timg__SetImagingSettings, _timg__SetImagingSettingsResponse* timg__SetImagingSettingsResponse);

	/// Web service operation 'GetOptions' (returns error code or SOAP_OK)
	virtual	int GetOptions(_timg__GetOptions* timg__GetOptions, _timg__GetOptionsResponse* timg__GetOptionsResponse);

	/// Web service operation 'Move' (returns error code or SOAP_OK)
	virtual	int Move(_timg__Move* timg__Move, _timg__MoveResponse* timg__MoveResponse);

	/// Web service operation 'Stop' (returns error code or SOAP_OK)
	virtual	int Stop(_timg__Stop* timg__Stop, _timg__StopResponse* timg__StopResponse);

	/// Web service operation 'GetStatus' (returns error code or SOAP_OK)
	virtual	int GetStatus(_timg__GetStatus* timg__GetStatus, _timg__GetStatusResponse* timg__GetStatusResponse);

	/// Web service operation 'GetMoveOptions' (returns error code or SOAP_OK)
	virtual	int GetMoveOptions(_timg__GetMoveOptions* timg__GetMoveOptions, _timg__GetMoveOptionsResponse* timg__GetMoveOptionsResponse);

private:
	OnvifClientDevice& m_Device;
	//ImagingBindingProxy ImagingProxy;
	http_da_info* httpinfo = nullptr;
	const char *httpuserid = nullptr;
	const char *httppasswd = nullptr;
};
