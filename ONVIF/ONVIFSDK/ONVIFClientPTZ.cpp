#include "stdafx.h"
#include "onvifclientptz.h"

OnvifClientPTZ::OnvifClientPTZ(OnvifClientDevice& device, http_da_info* pHttpInfo)
	: m_Device(device),
	ptzProxy(SOAP_C_UTFSTRING)
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
	if (bHasCapabilities)
	{
		// Enable http digest authentication in PTZ
		/*if (!pHttpInfo && m_Device.GetHttpDa())
		{
			pHttpInfo = m_Device.GetHttpDa();
			Soap_Trace("%s Inherit httpinfo from Device or Media Object!\n", __FUNCTION__);
		}

		if (pHttpInfo)
		{
			bool bDigestAuth = true;
			soap_register_plugin_arg(&ptzProxy, http_da, &bDigestAuth);

			httpinfo = new http_da_info;
			memcpy(httpinfo, pHttpInfo, sizeof(http_da_info));
		}*/
		if (Initialize() != SOAP_OK)
			throw std::exception("Initialize PTZ failed,the specified device may not support PTZ!");
	}
	else
		throw std::exception("The Failed in Getting Capabilities from device!");
}

OnvifClientPTZ::~OnvifClientPTZ()
{
	if (httpinfo)
	{
		http_da_release(&ptzProxy, httpinfo);
		delete httpinfo;
	}
}

int  OnvifClientPTZ::Initialize()
{
	_tptz__GetConfigurationsResponse GetConfigsResp;

	if (GetConfigurations(GetConfigsResp) != 0)
		return -1;

	if (GetConfigsResp.PTZConfiguration[0]->DefaultAbsolutePantTiltPositionSpace)
		m_bHasAbsolutePTMove = TRUE;
	else
		m_bHasAbsolutePTMove = FALSE;
	if (GetConfigsResp.PTZConfiguration[0]->DefaultAbsoluteZoomPositionSpace)
		m_bHasAbsoluteZoomMove = TRUE;
	else
		m_bHasAbsoluteZoomMove = FALSE;
	if (GetConfigsResp.PTZConfiguration[0]->DefaultRelativePanTiltTranslationSpace)
		m_bHasRelativePTMove = TRUE;
	else
		m_bHasRelativePTMove = FALSE;
	if (GetConfigsResp.PTZConfiguration[0]->DefaultRelativeZoomTranslationSpace)
		m_bHasRelativeZoomMove = TRUE;
	else
		m_bHasRelativeZoomMove = FALSE;
	if (GetConfigsResp.PTZConfiguration[0]->DefaultContinuousPanTiltVelocitySpace)
		m_bHasContinuousPTMove = TRUE;
	else
		m_bHasContinuousPTMove = FALSE;
	if (GetConfigsResp.PTZConfiguration[0]->DefaultContinuousZoomVelocitySpace)
		m_bHasContinuousZoomMove = TRUE;
	else
		m_bHasContinuousZoomMove = FALSE;

	return SOAP_OK;
}

int  OnvifClientPTZ::Stop(_tptz__StopResponse& StopResponse, bool PanTilt, bool Zoom, string profileToken)
{
	_tptz__Stop StopReq;
	StopReq.PanTilt = &PanTilt;
	StopReq.ProfileToken = profileToken;
	StopReq.Zoom = &Zoom;
	HttpCall(ptzProxy, Stop(&StopReq, &StopResponse), nResult);
}

int  OnvifClientPTZ::AbsoluteMove(_tptz__AbsoluteMoveResponse& AbsoluteMoveResponse, tt__PTZSpeed& Speed, tt__PTZVector& position, string profileToken)
{
	if (!m_bHasAbsolutePTMove && !m_bHasAbsoluteZoomMove)
	{
		Soap_Trace("%s device does not support Absolute movement\n", __FUNCTION__);
		return -1;
	}
	_tptz__AbsoluteMove AbsoluteMoveReq;
	AbsoluteMoveReq.ProfileToken = profileToken;
	AbsoluteMoveReq.Position = &position;
	AbsoluteMoveReq.Speed = &Speed;

	HttpCall(ptzProxy, AbsoluteMove(&AbsoluteMoveReq, &AbsoluteMoveResponse), nResult);
}

int  OnvifClientPTZ::RelativeMove(_tptz__RelativeMoveResponse& relMoveResponse, tt__PTZVector& Translation, tt__PTZSpeed& Speed, string profileToken)
{
	_tptz__RelativeMove relMoveReq;
	relMoveReq.ProfileToken = profileToken;
	relMoveReq.Translation = &Translation;
	relMoveReq.Speed = &Speed;
	HttpCall(ptzProxy, RelativeMove(&relMoveReq, &relMoveResponse), nResult);
}


int  OnvifClientPTZ::GetPresets(_tptz__GetPresetsResponse& tptz__GetPresetsResponse, string profileToken)
{
	_tptz__GetPresets GetPresetsReq;
	GetPresetsReq.ProfileToken = profileToken;
	HttpCall(ptzProxy, GetPresets(&GetPresetsReq, &tptz__GetPresetsResponse), nResult);
}

int  OnvifClientPTZ::GotoPreset(string strPresetToken, _tptz__GotoPresetResponse& tptz__GotoPresetResponse, string profileToken)
{

	_tptz__GotoPreset GotoPresetReq;
	GotoPresetReq.ProfileToken = profileToken;
	GotoPresetReq.PresetToken = strPresetToken;

	HttpCall(ptzProxy, GotoPreset(&GotoPresetReq, &tptz__GotoPresetResponse), nResult);
}

int  OnvifClientPTZ::SetPreset(string* pstrPrsetName, string* pstrPresetToken, _tptz__SetPresetResponse& tptz__SetPresetResponse, string profileToken)
{
	_tptz__SetPreset tptz__SetPreset;
	tptz__SetPreset.ProfileToken = profileToken;
	tptz__SetPreset.PresetName = pstrPrsetName;
	tptz__SetPreset.PresetToken = pstrPresetToken;

	HttpCall(ptzProxy, SetPreset(&tptz__SetPreset, &tptz__SetPresetResponse), nResult);
}

int  OnvifClientPTZ::RemovePreset(string strPresetToken, _tptz__RemovePresetResponse& tptz__SetPresetResponse, string profileToken)
{
	_tptz__RemovePreset tptz__RemovePreset;
	tptz__RemovePreset.PresetToken = strPresetToken;
	tptz__RemovePreset.ProfileToken = profileToken;
	_tptz__RemovePresetResponse tptz__RemovePresetResponse;
	HttpCall(ptzProxy, RemovePreset(&tptz__RemovePreset, &tptz__SetPresetResponse), nResult);
}

int  OnvifClientPTZ::GotoHomePosition(_tptz__GotoHomePositionResponse& HomePositionResponse, string profileToken)
{
	_tptz__GotoHomePosition  HomePosition;
	HomePosition.ProfileToken = profileToken;

	HttpCall(ptzProxy, GotoHomePosition(&HomePosition, &HomePositionResponse), nResult);
}

int  OnvifClientPTZ::ContinuousMove(_tptz__ContinuousMoveResponse& ContMoveResponse, tt__PTZSpeed& Speed, LONG64& Timeout, string profileToken)
{
	if (!m_bHasContinuousPTMove && !m_bHasContinuousZoomMove)
	{
		Soap_Trace("%s device does not support Continuous movement\n", __FUNCTION__);
		return -1;
	}

	_tptz__ContinuousMove ContMoveReq;
	ContMoveReq.ProfileToken = profileToken;
	ContMoveReq.Velocity = &Speed;
	ContMoveReq.Timeout = &Timeout;

	HttpCall(ptzProxy, ContinuousMove(&ContMoveReq, &ContMoveResponse), nResult);
}

int  OnvifClientPTZ::SetConfiguration(_tptz__SetConfigurationResponse& SetConfigurationResponse, tt__PTZConfiguration& PTZConfiguration, bool ForcePersist)
{
	_tptz__SetConfiguration SetConfigurationReq;
	SetConfigurationReq.PTZConfiguration = &PTZConfiguration;
	SetConfigurationReq.ForcePersistence = ForcePersist;

	HttpCall(ptzProxy, SetConfiguration(&SetConfigurationReq, &SetConfigurationResponse), nResult);
}

int  OnvifClientPTZ::GetServiceCapabilities(_tptz__GetServiceCapabilitiesResponse& ServiceCapResponse)
{
	_tptz__GetServiceCapabilities ServiceCapReq;

	HttpCall(ptzProxy, GetServiceCapabilities(&ServiceCapReq, &ServiceCapResponse), nResult);
}

int  OnvifClientPTZ::GetStatus(_tptz__GetStatusResponse& StatusResponse, string profileToken)
{
	_tptz__GetStatus StatusReq;
	StatusReq.ProfileToken = profileToken;

	HttpCall(ptzProxy, GetStatus(&StatusReq, &StatusResponse), nResult);
}

int  OnvifClientPTZ::GetNode(_tptz__GetNodeResponse& GetNodeResponse, string nodeToken)
{
	_tptz__GetNode GetNodeReq;
	GetNodeReq.NodeToken = nodeToken;

	HttpCall(ptzProxy, GetNode(&GetNodeReq, &GetNodeResponse), nResult);
}

int  OnvifClientPTZ::GetNodes(_tptz__GetNodesResponse& GetNodesResponse)
{
	_tptz__GetNodes GetNodesReq;

	HttpCall(ptzProxy, GetNodes(&GetNodesReq, &GetNodesResponse), nResult);
}

int  OnvifClientPTZ::GetConfigurations(_tptz__GetConfigurationsResponse& configurationsResp)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetPTZUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	_tptz__GetConfigurations	configurationsReq;
	ptzProxy.userid = soap_strdup(nullptr, strUser.c_str());
	ptzProxy.passwd = soap_strdup(nullptr, strPass.c_str());
	ptzProxy.soap_endpoint = strUrl.c_str();
	if (httpinfo)
		http_da_restore(&ptzProxy, httpinfo);
	/*soap_wsse_add_Security(&ptzProxy);
	soap_wsse_add_UsernameTokenDigest(&ptzProxy, "Id",	strUser.c_str(), strPass.c_str());*/
	int nResult = ptzProxy.GetConfigurations(&configurationsReq, &configurationsResp);
	if (nResult != SOAP_OK &&
		ptzProxy.status == 401)
	{
		// 重新认证
		ptzProxy.userid = soap_strdup(nullptr, strUser.c_str());
		ptzProxy.passwd = soap_strdup(nullptr, strPass.c_str());
		// 能过Auth Digest 一般不需要wsse认证？
		if (!httpinfo)
		{
			soap_register_plugin(&ptzProxy, http_da);
			httpinfo = new http_da_info;
			http_da_save(&ptzProxy, httpinfo, ptzProxy.authrealm, ptzProxy.userid, ptzProxy.passwd);
		}
		else
		{
			http_da_restore(&ptzProxy, httpinfo);
			//http_da_updatenonce(&mediaProxy);
		}
		return ptzProxy.GetConfigurations(&configurationsReq, &configurationsResp);
	}
	else
		return nResult;
	//_tptz__GetConfigurations	configurationsReq;
	//HttpCall(ptzProxy, GetConfigurations(&configurationsReq, &configurationsResp), nResult);
}

int  OnvifClientPTZ::GetConfiguration(_tptz__GetConfigurationResponse& configurationResp)
{
	_tptz__GetConfiguration	configurationReq;

	HttpCall(ptzProxy, GetConfiguration(&configurationReq, &configurationResp), nResult);
}

int  OnvifClientPTZ::GetConfigurationOptions(_tptz__GetConfigurationOptionsResponse& configOptions, string configToken)
{
	_tptz__GetConfigurationOptions	configurationOptionsReq;
	configurationOptionsReq.ConfigurationToken = configToken;

	HttpCall(ptzProxy, GetConfigurationOptions(&configurationOptionsReq, &configOptions), nResult);
}
