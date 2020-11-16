#include "stdafx.h"
#include "onvifclientmedia.h"

OnvifClientMedia::OnvifClientMedia(OnvifClientDevice& device)
	: m_Device(device)
	, MediaBindingProxy(SOAP_C_UTFSTRING)
	/*, httpinfo(device.GetHttpDa())
	, bHttpda(device.HttpdaEnabled())*/
{
	device.GetMediaUrl(m_strUrl);
}

OnvifClientMedia::~OnvifClientMedia()
{
	http_da_release(this, &httpinfo);
}


int OnvifClientMedia::GetMetadataConfigurationOptions(_trt__GetMetadataConfigurationOptionsResponse& GetMetadataConfigurationOptionsResponse, string ConfigToken, string profileToken)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetMetadataConfigurationOptions GetMetadataConfigurationOptionsReq;
	GetMetadataConfigurationOptionsReq.ConfigurationToken = &ConfigToken;
	GetMetadataConfigurationOptionsReq.ProfileToken = &profileToken;
	return __super::GetMetadataConfigurationOptions(&GetMetadataConfigurationOptionsReq, &GetMetadataConfigurationOptionsResponse);
}

int OnvifClientMedia::GetMetadataConfiguration(_trt__GetMetadataConfigurationResponse& GetMetadataConfigurationResponse, string profileToken)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetMetadataConfiguration GetMetadataConfigurationReq;
	GetMetadataConfigurationReq.ConfigurationToken = profileToken;
	return __super::GetMetadataConfiguration(&GetMetadataConfigurationReq, &GetMetadataConfigurationResponse);
}

int OnvifClientMedia::GetMetadataConfigurations(_trt__GetMetadataConfigurationsResponse& GetMetadataConfigurationsResponse)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetMetadataConfigurations GetMetadataConfigurationsReq;

	return __super::GetMetadataConfigurations(&GetMetadataConfigurationsReq, &GetMetadataConfigurationsResponse);
}

int OnvifClientMedia::GetVideoSourceConfigurationOptions(_trt__GetVideoSourceConfigurationOptionsResponse& GetVideoSourceConfigOptionsResponse, string ConfigurationToken, string profileToken)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetVideoSourceConfigurationOptions GetVideoSourceConfigOptionsReq;
	GetVideoSourceConfigOptionsReq.ConfigurationToken = &ConfigurationToken;
	GetVideoSourceConfigOptionsReq.ProfileToken = &profileToken;

	return __super::GetVideoSourceConfigurationOptions(&GetVideoSourceConfigOptionsReq, &GetVideoSourceConfigOptionsResponse);
}

int OnvifClientMedia::SetVideoSourceConfiguration(_trt__SetVideoSourceConfigurationResponse& SetVideoSourceConfigResponse)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__SetVideoSourceConfiguration SetVideoSourceConfigReq;

	return __super::SetVideoSourceConfiguration(&SetVideoSourceConfigReq, &SetVideoSourceConfigResponse);
}

int OnvifClientMedia::GetVideoSourceConfiguration(_trt__GetVideoSourceConfigurationResponse& GetVideoSourceConfigResponse, string ConfigurationToken)
{
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetVideoSourceConfiguration GetVideoSourceConfigReq;
	GetVideoSourceConfigReq.ConfigurationToken = ConfigurationToken;
	return __super::GetVideoSourceConfiguration(&GetVideoSourceConfigReq, &GetVideoSourceConfigResponse);
}

int OnvifClientMedia::GetVideoSourceConfigurations(_trt__GetVideoSourceConfigurationsResponse& GetVideoSourceConfigResponse)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetVideoSourceConfigurations GetVideoSourceConfigReq;

	return __super::GetVideoSourceConfigurations(&GetVideoSourceConfigReq, &GetVideoSourceConfigResponse);
}

int OnvifClientMedia::GetVideoSources(_trt__GetVideoSourcesResponse& GetVideoSourcesResponse)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetVideoSources GetVideoSourcesReq;

	return __super::GetVideoSources(&GetVideoSourcesReq, &GetVideoSourcesResponse);
}

int OnvifClientMedia::CreateProfile(_trt__CreateProfileResponse& CreateProfileResponse, string Name, string token)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__CreateProfile CreateProfileReq;
	CreateProfileReq.Name = Name;
	CreateProfileReq.Token = &token;

	return __super::CreateProfile(&CreateProfileReq, &CreateProfileResponse);
}

int OnvifClientMedia::StopMulticastStreaming(_trt__StopMulticastStreamingResponse& StopMulticastResponse, string profileToken)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass) )
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__StopMulticastStreaming StopMulticastReq;
	StopMulticastReq.ProfileToken = profileToken;

	return __super::StopMulticastStreaming(&StopMulticastReq, &StopMulticastResponse);

}

int OnvifClientMedia::StartMulticastStreaming(_trt__StartMulticastStreamingResponse& StartMulticastResponse, string profileToken)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__StartMulticastStreaming StartMulticastReq;
	StartMulticastReq.ProfileToken = profileToken;
	return __super::StartMulticastStreaming(&StartMulticastReq, &StartMulticastResponse);
}

int OnvifClientMedia::GetServiceCapabilities(_trt__GetServiceCapabilitiesResponse& GetServiceCapResponse)
{
	string strUser;
	string strPass;
	if (!m_Device.GetUserPasswd(strUser, strPass))
	{
		return SOAP_ERR;
	}
	soap_endpoint = m_strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetServiceCapabilities GetServiceCapReq;

	return __super::GetServiceCapabilities(&GetServiceCapReq, &GetServiceCapResponse);
}

int OnvifClientMedia::GetStreamUri(_trt__GetStreamUriResponse& StreamUriResponse, tt__StreamSetup& StreamSetup, string profileToken)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}

	soap_endpoint = strUrl.c_str();
	

	if (!bHttpda)
	{
		soap_wsse_add_Security(this);
		soap_wsse_add_UsernameTokenDigest(this, nullptr, strUser.c_str(), strPass.c_str());
	}
		
	else
	{
		userid = strUser.c_str();
		passwd = strPass.c_str();
		http_da_restore(this, &httpinfo);
	}

	_trt__GetStreamUri StreamUriReq;
	StreamUriReq.ProfileToken = profileToken;
	StreamUriReq.StreamSetup = &StreamSetup;
	int nResult = __super::GetStreamUri(&StreamUriReq, &StreamUriResponse);
	if (nResult != SOAP_OK &&
		status == 401)
	{
		userid = strUser.c_str();
		passwd = strPass.c_str();
		if (!bHttpda)
		{
			soap_register_plugin(this, http_da);
			bHttpda = true;
			http_da_save(this, &httpinfo, authrealm, userid, passwd);
		}
		else
			http_da_restore(this, &httpinfo);
		

		return __super::GetStreamUri(&StreamUriReq, &StreamUriResponse);
	}
	else
		return nResult;
}



int OnvifClientMedia::GetProfiles(_trt__GetProfilesResponse* pProfilesResponse)
{
	_trt__GetProfiles	profilesReq;

	HttpdaCall(this, __super::GetProfiles(&profilesReq, pProfilesResponse), nResult);

}

inline int OnvifClientMedia::GetProfile(_trt__GetProfileResponse& profileResponse, string profileToken)
{
	_trt__GetProfile profileReq;
	profileReq.ProfileToken = profileToken;
	HttpdaCall(this, __super::GetProfile(&profileReq, &profileResponse), nResult);
}

inline int OnvifClientMedia::GetSnapshotUri(_trt__GetSnapshotUriResponse& SnapshotUriResponse, string profileToken)
{
	_trt__GetSnapshotUri SnapshotUriReq;
	SnapshotUriReq.ProfileToken = profileToken;

	HttpdaCall(this, __super::GetSnapshotUri(&SnapshotUriReq, &SnapshotUriResponse), nResult);
}
