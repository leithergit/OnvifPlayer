﻿#include "stdafx.h"
#include "OnvifClientImage.h"

OnvifClientImage::OnvifClientImage(OnvifClientDevice& device, http_da_info* pHttpInfo)
	:m_Device(device), ImagingBindingProxy(SOAP_C_UTFSTRING)
{
	string strPTZUrl;
	bool bHasCapabilities = false;
	if (!m_Device.GetPTZUrl(strPTZUrl))
	{
		if (m_Device.GetCapabilities() == SOAP_OK)
			bHasCapabilities = true;
	}
	else
		bHasCapabilities = true;
	/*if (bHasCapabilities)
	{

		if (Initialize() != SOAP_OK)
			throw std::exception("Initialize PTZ failed,the specified device may not support PTZ!");
	}
	else
		throw std::exception("The Failed in Getting Capabilities from device!");*/
}

OnvifClientImage::~OnvifClientImage()
{
	if (httpinfo)
	{
		http_da_release(this, httpinfo);
		delete httpinfo;
	}
}

int OnvifClientImage::GetServiceCapabilities(_timg__GetServiceCapabilitiesResponse* timg__GetServiceCapabilitiesResponse)
{
	_timg__GetServiceCapabilities timg__GetServiceCapabilities;
	SuperHttpCall(this, __super::GetServiceCapabilities(NULL, NULL, &timg__GetServiceCapabilities, timg__GetServiceCapabilitiesResponse), nResult);
}

/// Web service operation 'GetImagingSettings' (returns error code or SOAP_OK)
int OnvifClientImage::GetImagingSettings(_timg__GetImagingSettings* timg__GetImagingSettings, _timg__GetImagingSettingsResponse* timg__GetImagingSettingsResponse)
{
	int nResult = 0;
	string strUrl;
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass) || !m_Device.GetPTZUrl(strUrl))
	{
		Soap_Trace("%s %s:%s Error in Getting Url or User/Passsword.\n", m_Device.GetDevIP(), strClassName.c_str(), "GetImagingSettings");
		return SOAP_ERR;
	}
	this->soap_endpoint = strUrl.c_str();
	if (httpinfo)
		http_da_restore(this, httpinfo);
	nResult = __super::GetImagingSettings(NULL, NULL, timg__GetImagingSettings, timg__GetImagingSettingsResponse);
	if (nResult != SOAP_OK)
		Soap_Trace("%s %s:%s  Result = %d\tSoap Status = %d.\n", m_Device.GetDevIP(), strClassName.c_str(), "GetImagingSettings", nResult, this->status);
	if (nResult != SOAP_OK &&
		this->status == 401)
	{
		if (this->userid)
			SOAP_FREE(nullptr, (void *)this->userid);
		if (this->passwd)
			SOAP_FREE(nullptr, (void *)this->passwd);
		this->userid =  strUser.c_str();
		this->passwd =  strPass.c_str();
		
		if (!httpinfo)
		{
			soap_register_plugin(this, http_da);
			httpinfo = new http_da_info;
			http_da_save(this, httpinfo, this->authrealm, this->userid, this->passwd);
		}
		else
			http_da_restore(this, httpinfo);
		nResult = __super::GetImagingSettings(NULL, NULL, timg__GetImagingSettings, timg__GetImagingSettingsResponse);
		if (nResult != SOAP_OK)
			Soap_Trace("%s %s:%s  Result = %d\tSoap Status = %d.\n", m_Device.GetDevIP(), strClassName.c_str(), "GetImagingSettings", nResult, this->status);
	}
	return nResult;
	//SuperHttpCall(this, __super::GetImagingSettings(NULL, NULL, timg__GetImagingSettings, timg__GetImagingSettingsResponse), nResult);
}


/// Web service operation 'SetImagingSettings' (returns error code or SOAP_OK)
int OnvifClientImage::SetImagingSettings(_timg__SetImagingSettings* timg__SetImagingSettings, _timg__SetImagingSettingsResponse* timg__SetImagingSettingsResponse)
{
	SuperHttpCall(this, __super::SetImagingSettings(NULL, NULL, timg__SetImagingSettings, timg__SetImagingSettingsResponse), nResult);
}


/// Web service operation 'GetOptions' (returns error code or SOAP_OK)
int OnvifClientImage::GetOptions(_timg__GetOptions* timg__GetOptions, _timg__GetOptionsResponse* timg__GetOptionsResponse)
{
	SuperHttpCall(this, __super::GetOptions(NULL, NULL, timg__GetOptions, timg__GetOptionsResponse), nResult);
}

/// Web service operation 'Move' (returns error code or SOAP_OK)
int OnvifClientImage::Move(_timg__Move* timg__Move, _timg__MoveResponse* timg__MoveResponse)
{
	SuperHttpCall(this, __super::Move(NULL, NULL, timg__Move, timg__MoveResponse), nResult);
}

/// Web service operation 'Stop' (returns error code or SOAP_OK)
int OnvifClientImage::Stop(_timg__Stop* timg__Stop, _timg__StopResponse* timg__StopResponse)
{
	SuperHttpCall(this, __super::Stop(NULL, NULL, timg__Stop, timg__StopResponse), nResult);
}

/// Web service operation 'GetStatus' (returns error code or SOAP_OK)
int OnvifClientImage::GetStatus(_timg__GetStatus* timg__GetStatus, _timg__GetStatusResponse* timg__GetStatusResponse)
{
	SuperHttpCall(this, __super::GetStatus(NULL, NULL, timg__GetStatus, timg__GetStatusResponse), nResult);
}

/// Web service operation 'GetMoveOptions' (returns error code or SOAP_OK)
int OnvifClientImage::GetMoveOptions(_timg__GetMoveOptions* timg__GetMoveOptions, _timg__GetMoveOptionsResponse* timg__GetMoveOptionsResponse)
{
	SuperHttpCall(this, __super::GetMoveOptions(NULL, NULL, timg__GetMoveOptions, timg__GetMoveOptionsResponse), nResult);
}
