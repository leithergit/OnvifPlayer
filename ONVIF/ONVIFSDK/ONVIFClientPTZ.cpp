#include "stdafx.h"
#include "onvifclientptz.h"

OnvifClientPTZ::OnvifClientPTZ(OnvifClientDevice& device )
	: m_Device(device)
	, PTZBindingProxy(SOAP_C_UTFSTRING)
	/*, httpinfo(device.GetHttpDa())
	, bHttpda(device.HttpdaEnabled())*/
{
	bool bHasCapabilities = false;
	if (!m_Device.GetPTZUrl(m_strUrl))
	{
		bHasCapabilities = false;
	}
	else
		bHasCapabilities = true;
	if (bHasCapabilities)
	{
		if (Initialize() != SOAP_OK)
			throw std::exception("Initialize PTZ failed,the specified device may not support PTZ!");
	}
	else
		throw std::exception("The specified device may not support PTZ!");
}

OnvifClientPTZ::~OnvifClientPTZ()
{
	http_da_release(this, &httpinfo);
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
	HttpdaCall(this, __super::Stop(&StopReq, &StopResponse), nResult);
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

	HttpdaCall(this, __super::AbsoluteMove(&AbsoluteMoveReq, &AbsoluteMoveResponse), nResult);
}

int  OnvifClientPTZ::RelativeMove(_tptz__RelativeMoveResponse& relMoveResponse, tt__PTZVector& Translation, tt__PTZSpeed& Speed, string profileToken)
{
	_tptz__RelativeMove relMoveReq;
	relMoveReq.ProfileToken = profileToken;
	relMoveReq.Translation = &Translation;
	relMoveReq.Speed = &Speed;
	HttpdaCall(this, __super::RelativeMove(&relMoveReq, &relMoveResponse), nResult);
}


int  OnvifClientPTZ::GetPresets(_tptz__GetPresetsResponse& tptz__GetPresetsResponse, string profileToken)
{
	_tptz__GetPresets GetPresetsReq;
	GetPresetsReq.ProfileToken = profileToken;
	HttpdaCall(this, __super::GetPresets(&GetPresetsReq, &tptz__GetPresetsResponse), nResult);
}

int  OnvifClientPTZ::GotoPreset(string strPresetToken, _tptz__GotoPresetResponse& tptz__GotoPresetResponse, string profileToken)
{
	_tptz__GotoPreset GotoPresetReq;
	GotoPresetReq.ProfileToken = profileToken;
	GotoPresetReq.PresetToken = strPresetToken;

	HttpdaCall(this, __super::GotoPreset(&GotoPresetReq, &tptz__GotoPresetResponse), nResult);
}

int  OnvifClientPTZ::SetPreset(string* pstrPrsetName, string* pstrPresetToken, _tptz__SetPresetResponse& tptz__SetPresetResponse, string profileToken)
{
	_tptz__SetPreset tptz__SetPreset;
	tptz__SetPreset.ProfileToken = profileToken;
	tptz__SetPreset.PresetName = pstrPrsetName;
	tptz__SetPreset.PresetToken = pstrPresetToken;

	HttpdaCall(this, __super::SetPreset(&tptz__SetPreset, &tptz__SetPresetResponse), nResult);
}

int  OnvifClientPTZ::RemovePreset(string strPresetToken, _tptz__RemovePresetResponse& tptz__SetPresetResponse, string profileToken)
{
	_tptz__RemovePreset tptz__RemovePreset;
	tptz__RemovePreset.PresetToken = strPresetToken;
	tptz__RemovePreset.ProfileToken = profileToken;
	_tptz__RemovePresetResponse tptz__RemovePresetResponse;
	HttpdaCall(this, __super::RemovePreset(&tptz__RemovePreset, &tptz__SetPresetResponse), nResult);
}

int  OnvifClientPTZ::GotoHomePosition(_tptz__GotoHomePositionResponse& HomePositionResponse, string profileToken)
{
	_tptz__GotoHomePosition  HomePosition;
	HomePosition.ProfileToken = profileToken;

	HttpdaCall(this, __super::GotoHomePosition(&HomePosition, &HomePositionResponse), nResult);
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

	HttpdaCall(this, __super::ContinuousMove(&ContMoveReq, &ContMoveResponse), nResult);
}

int  OnvifClientPTZ::SetConfiguration(_tptz__SetConfigurationResponse& SetConfigurationResponse, tt__PTZConfiguration& PTZConfiguration, bool ForcePersist)
{
	_tptz__SetConfiguration SetConfigurationReq;
	SetConfigurationReq.PTZConfiguration = &PTZConfiguration;
	SetConfigurationReq.ForcePersistence = ForcePersist;

	HttpdaCall(this, __super::SetConfiguration(&SetConfigurationReq, &SetConfigurationResponse), nResult);
}

int  OnvifClientPTZ::GetServiceCapabilities(_tptz__GetServiceCapabilitiesResponse& ServiceCapResponse)
{
	_tptz__GetServiceCapabilities ServiceCapReq;

	HttpdaCall(this, __super::GetServiceCapabilities(&ServiceCapReq, &ServiceCapResponse), nResult);
}

int  OnvifClientPTZ::GetStatus(_tptz__GetStatusResponse& StatusResponse, string profileToken)
{
	_tptz__GetStatus StatusReq;
	StatusReq.ProfileToken = profileToken;

	HttpdaCall(this, __super::GetStatus(&StatusReq, &StatusResponse), nResult);
}

int  OnvifClientPTZ::GetNode(_tptz__GetNodeResponse& GetNodeResponse, string nodeToken)
{
	_tptz__GetNode GetNodeReq;
	GetNodeReq.NodeToken = nodeToken;

	HttpdaCall(this, __super::GetNode(&GetNodeReq, &GetNodeResponse), nResult);
}

int  OnvifClientPTZ::GetNodes(_tptz__GetNodesResponse& GetNodesResponse)
{
	_tptz__GetNodes GetNodesReq;

	HttpdaCall(this, __super::GetNodes(&GetNodesReq, &GetNodesResponse), nResult);
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
	userid = soap_strdup(nullptr, strUser.c_str());
	passwd = soap_strdup(nullptr, strPass.c_str());
	soap_endpoint = strUrl.c_str();
	if (bHttpda)
		http_da_restore(this, &httpinfo);
	/*soap_wsse_add_Security(&this);
	soap_wsse_add_UsernameTokenDigest(&this, "Id",	strUser.c_str(), strPass.c_str());*/
	int nResult = __super::GetConfigurations(&configurationsReq, &configurationsResp);
	if (nResult != SOAP_OK &&
		status == 401)
	{
		// 重新认证
		userid = soap_strdup(nullptr, strUser.c_str());
		passwd = soap_strdup(nullptr, strPass.c_str());
		// 能过Auth Digest 一般不需要wsse认证？
		if (!bHttpda)
		{
			soap_register_plugin(this, http_da);
			http_da_save(this, &httpinfo, authrealm, userid, passwd);
		}
		else
		{
			http_da_restore(this, &httpinfo);
		}
		return __super::GetConfigurations(&configurationsReq, &configurationsResp);
	}
	else
		return nResult;
	//_tptz__GetConfigurations	configurationsReq;
	//HttpdaCall(this, GetConfigurations(&configurationsReq, &configurationsResp), nResult);
}

int  OnvifClientPTZ::GetConfiguration(_tptz__GetConfigurationResponse& configurationResp)
{
	_tptz__GetConfiguration	configurationReq;

	HttpdaCall(this, __super::GetConfiguration(&configurationReq, &configurationResp), nResult);
}

int  OnvifClientPTZ::GetConfigurationOptions(_tptz__GetConfigurationOptionsResponse& configOptions, string configToken)
{
	_tptz__GetConfigurationOptions	configurationOptionsReq;
	configurationOptionsReq.ConfigurationToken = configToken;

	HttpdaCall(this, __super::GetConfigurationOptions(&configurationOptionsReq, &configOptions), nResult);
}
