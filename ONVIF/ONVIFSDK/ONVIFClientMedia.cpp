#include "stdafx.h"
#include "onvifclientmedia.h"

OnvifClientMedia::OnvifClientMedia(OnvifClientDevice& device)
	: m_Device(device), mediaProxy(SOAP_C_UTFSTRING)
{
	/*if (device.GetHttpDa())
	{
		httpinfo = new http_da_info;
		memcpy(httpinfo, device.GetHttpDa(), sizeof(http_da_info));
		soap_register_plugin(&mediaProxy, http_da);
	}*/
}

OnvifClientMedia::~OnvifClientMedia()
{
	if (httpinfo)
	{
		http_da_release(&mediaProxy, httpinfo);
		delete httpinfo;
	}
}

http_da_info* OnvifClientMedia::GetHttpDa()
{
	return httpinfo;
}
int OnvifClientMedia::GetMetadataConfigurationOptions(_trt__GetMetadataConfigurationOptionsResponse& GetMetadataConfigurationOptionsResponse, string ConfigToken, string profileToken)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetMetadataConfigurationOptions GetMetadataConfigurationOptionsReq;
	GetMetadataConfigurationOptionsReq.ConfigurationToken = &ConfigToken;
	GetMetadataConfigurationOptionsReq.ProfileToken = &profileToken;
	return mediaProxy.GetMetadataConfigurationOptions(&GetMetadataConfigurationOptionsReq, &GetMetadataConfigurationOptionsResponse);
}

int OnvifClientMedia::GetMetadataConfiguration(_trt__GetMetadataConfigurationResponse& GetMetadataConfigurationResponse, string profileToken)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetMetadataConfiguration GetMetadataConfigurationReq;
	GetMetadataConfigurationReq.ConfigurationToken = profileToken;
	return mediaProxy.GetMetadataConfiguration(&GetMetadataConfigurationReq, &GetMetadataConfigurationResponse);
}

int OnvifClientMedia::GetMetadataConfigurations(_trt__GetMetadataConfigurationsResponse& GetMetadataConfigurationsResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetMetadataConfigurations GetMetadataConfigurationsReq;

	return mediaProxy.GetMetadataConfigurations(&GetMetadataConfigurationsReq, &GetMetadataConfigurationsResponse);
}

int OnvifClientMedia::GetVideoSourceConfigurationOptions(_trt__GetVideoSourceConfigurationOptionsResponse& GetVideoSourceConfigOptionsResponse, string ConfigurationToken, string profileToken)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetVideoSourceConfigurationOptions GetVideoSourceConfigOptionsReq;
	GetVideoSourceConfigOptionsReq.ConfigurationToken = &ConfigurationToken;
	GetVideoSourceConfigOptionsReq.ProfileToken = &profileToken;

	return mediaProxy.GetVideoSourceConfigurationOptions(&GetVideoSourceConfigOptionsReq, &GetVideoSourceConfigOptionsResponse);
}

int OnvifClientMedia::SetVideoSourceConfiguration(_trt__SetVideoSourceConfigurationResponse& SetVideoSourceConfigResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__SetVideoSourceConfiguration SetVideoSourceConfigReq;

	return mediaProxy.SetVideoSourceConfiguration(&SetVideoSourceConfigReq, &SetVideoSourceConfigResponse);
}

int OnvifClientMedia::GetVideoSourceConfiguration(_trt__GetVideoSourceConfigurationResponse& GetVideoSourceConfigResponse, string ConfigurationToken)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetVideoSourceConfiguration GetVideoSourceConfigReq;
	GetVideoSourceConfigReq.ConfigurationToken = ConfigurationToken;
	return mediaProxy.GetVideoSourceConfiguration(&GetVideoSourceConfigReq, &GetVideoSourceConfigResponse);
}

int OnvifClientMedia::GetVideoSourceConfigurations(_trt__GetVideoSourceConfigurationsResponse& GetVideoSourceConfigResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetVideoSourceConfigurations GetVideoSourceConfigReq;

	return mediaProxy.GetVideoSourceConfigurations(&GetVideoSourceConfigReq, &GetVideoSourceConfigResponse);
}

int OnvifClientMedia::GetVideoSources(_trt__GetVideoSourcesResponse& GetVideoSourcesResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetVideoSources GetVideoSourcesReq;

	return mediaProxy.GetVideoSources(&GetVideoSourcesReq, &GetVideoSourcesResponse);
}

int OnvifClientMedia::CreateProfile(_trt__CreateProfileResponse& CreateProfileResponse, string Name, string token)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__CreateProfile CreateProfileReq;
	CreateProfileReq.Name = Name;
	CreateProfileReq.Token = &token;

	return mediaProxy.CreateProfile(&CreateProfileReq, &CreateProfileResponse);
}

int OnvifClientMedia::StopMulticastStreaming(_trt__StopMulticastStreamingResponse& StopMulticastResponse, string profileToken)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__StopMulticastStreaming StopMulticastReq;
	StopMulticastReq.ProfileToken = profileToken;

	return mediaProxy.StopMulticastStreaming(&StopMulticastReq, &StopMulticastResponse);

}

int OnvifClientMedia::StartMulticastStreaming(_trt__StartMulticastStreamingResponse& StartMulticastResponse, string profileToken)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__StartMulticastStreaming StartMulticastReq;
	StartMulticastReq.ProfileToken = profileToken;
	return mediaProxy.StartMulticastStreaming(&StartMulticastReq, &StartMulticastResponse);

}

int OnvifClientMedia::GetServiceCapabilities(_trt__GetServiceCapabilitiesResponse& GetServiceCapResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", strUser.c_str(), strPass.c_str());

	_trt__GetServiceCapabilities GetServiceCapReq;

	return mediaProxy.GetServiceCapabilities(&GetServiceCapReq, &GetServiceCapResponse);

}

int OnvifClientMedia::GetStreamUri(_trt__GetStreamUriResponse& StreamUriResponse, tt__StreamSetup& StreamSetup, string profileToken)
{
	bool static bDigestAuth = false;
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}

	mediaProxy.soap_endpoint = strUrl.c_str();
	// here can't add wsse security 
	//soap_wsse_add_Security(&mediaProxy);
	//soap_wsse_add_UsernameTokenDigest(&mediaProxy, nullptr, strUser.c_str(), strPass.c_str());

	if (/*bDigestAuth &&*/ httpinfo)
		http_da_restore(&mediaProxy, httpinfo);

	_trt__GetStreamUri StreamUriReq;
	StreamUriReq.ProfileToken = profileToken;
	StreamUriReq.StreamSetup = &StreamSetup;
	int nResult = mediaProxy.GetStreamUri(&StreamUriReq, &StreamUriResponse);
	if (nResult != SOAP_OK &&
		mediaProxy.status == 401)
	{
		// 重新认证
		bDigestAuth = true;
		soap_wsse_add_Security(&mediaProxy);
		soap_wsse_add_UsernameTokenDigest(&mediaProxy, nullptr, strUser.c_str(), strPass.c_str());
		if (!httpinfo)
		{
			soap_register_plugin(&mediaProxy, http_da);
			httpinfo = new http_da_info;
			http_da_save(&mediaProxy, httpinfo, mediaProxy.authrealm, mediaProxy.userid, mediaProxy.passwd);
		}
		else
		{
			http_da_restore(&mediaProxy, httpinfo);
			//http_da_updatenonce(&mediaProxy);
		}

		return mediaProxy.GetStreamUri(&StreamUriReq, &StreamUriResponse);
	}
	else
		return nResult;
}



int OnvifClientMedia::GetProfiles(_trt__GetProfilesResponse* pProfilesResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	_trt__GetProfiles	profilesReq;

	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}

	mediaProxy.soap_endpoint = strUrl.c_str();
	mediaProxy.userid = soap_strdup(&mediaProxy, strUser.c_str());
	mediaProxy.passwd = soap_strdup(&mediaProxy, strPass.c_str());


	if (httpinfo)
		http_da_restore(&mediaProxy, httpinfo);
	/*else
	{
		soap_wsse_add_Security(&mediaProxy);
		soap_wsse_add_UsernameTokenDigest(&mediaProxy, nullptr, strUser.c_str(), strPass.c_str());
	}*/

	int nResult = mediaProxy.GetProfiles(&profilesReq, pProfilesResponse);
	if (nResult != SOAP_OK &&
		mediaProxy.status == 401)
	{
		// retry in digest authenticate
		/*soap_wsse_add_Security(&mediaProxy);
		soap_wsse_add_UsernameTokenDigest(&mediaProxy, nullptr, strUser.c_str(), strPass.c_str());*/

		if (!httpinfo)
		{
			soap_register_plugin(&mediaProxy, http_da);
			mediaProxy.userid = soap_strdup(&mediaProxy, strUser.c_str());
			mediaProxy.passwd = soap_strdup(&mediaProxy, strPass.c_str());
			httpinfo = new http_da_info;
			http_da_save(&mediaProxy, httpinfo, mediaProxy.authrealm, mediaProxy.userid, mediaProxy.passwd);
		}
		else
			http_da_restore(&mediaProxy, httpinfo);

		nResult = mediaProxy.GetProfiles(&profilesReq, pProfilesResponse);
	}

	return nResult;
}

inline int OnvifClientMedia::GetProfile(_trt__GetProfileResponse& profileResponse, string profileToken)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false || m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id",
		strUser.c_str(), strPass.c_str());

	_trt__GetProfile profileReq;
	profileReq.ProfileToken = profileToken;
	return mediaProxy.GetProfile(&profileReq, &profileResponse);
}

inline int OnvifClientMedia::GetSnapshotUri(_trt__GetSnapshotUriResponse& SnapshotUriResponse, string profileToken)
{
	string strUrl;
	string strUser;
	string strPass;
	if (m_Device.GetUserPasswd(strUser, strPass) == false
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}

	mediaProxy.soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, nullptr, strUser.c_str(), strPass.c_str());
	if (httpinfo)
		http_da_restore(&mediaProxy, httpinfo);

	_trt__GetSnapshotUri SnapshotUriReq;

	int nResult = mediaProxy.GetSnapshotUri(&SnapshotUriReq, &SnapshotUriResponse);
	if (nResult != SOAP_OK &&
		mediaProxy.status == 401)
	{
		// 重新认证
		soap_register_plugin(&mediaProxy, http_da);
		mediaProxy.userid = soap_strdup(&mediaProxy, strUser.c_str());
		mediaProxy.passwd = soap_strdup(&mediaProxy, strPass.c_str());
		if (!httpinfo)
			httpinfo = new http_da_info;
		http_da_save(&mediaProxy, httpinfo, mediaProxy.authrealm, mediaProxy.userid, mediaProxy.passwd);
		return mediaProxy.GetSnapshotUri(&SnapshotUriReq, &SnapshotUriResponse);
	}
	else
		return nResult;
}
