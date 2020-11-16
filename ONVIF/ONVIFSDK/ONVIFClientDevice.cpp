#include "stdafx.h"
#include "onvifclientdevice.h"

OnvifClientDevice::OnvifClientDevice(string strIP, string strUser, string strPass, unsigned short nPort)
	:m_strDevIP(strIP), m_strUser(strUser)
	, m_strPass(strPass), m_hasGetCap(false)
	, DeviceBindingProxy(SOAP_C_UTFSTRING)
	//,ImagingBindingProxy(*this)
	//,MediaBindingProxy(*this)
{
	char szURL[1024] = { 0 };
	if (nPort == 80)
		sprintf_s(szURL, 1024, "http://%s/onvif/device_service", strIP.c_str());
	else
		sprintf_s(szURL, 1024, "http://%s:%d/onvif/device_service", strIP.c_str(), nPort);
	m_strUrl = szURL;
	m_strDevIP = strIP;
	m_hasMedia = false;
	m_hasPTZ = false;
	m_hasImaging = false;
	m_hasReceiver = false;
	m_hasRecording = false;
	m_hasSearch = false;
	m_hasReplay = false;
	m_hasEvent = false;
}

OnvifClientDevice::~OnvifClientDevice()
{
	http_da_release(this, &httpinfo);
}

void OnvifClientDevice::SaveCapabilities(_tds__GetCapabilitiesResponse& cap)
{
	/* Media */
	if (cap.Capabilities->Media != NULL)
	{
		m_hasMedia = TRUE;
		m_strMedia = cap.Capabilities->Media->XAddr;
	}
	/* PTZ */
	if (cap.Capabilities->PTZ != NULL)
	{
		m_hasPTZ = TRUE;
		m_strPTZ = cap.Capabilities->PTZ->XAddr;
	}

	/* Event */
	if (cap.Capabilities->Events != NULL)
	{
		m_hasEvent = TRUE;
		m_strEvent = cap.Capabilities->Events->XAddr;
	}

	/* Imaging */
	if (cap.Capabilities->Imaging != NULL)
	{
		m_hasImaging = TRUE;
		m_strImaging = cap.Capabilities->Imaging->XAddr;
	}

	/* Extension */
	if (cap.Capabilities->Extension != NULL)
	{
		/* Receiver */
		if (cap.Capabilities->Extension->Receiver != NULL)
		{
			m_hasReceiver = TRUE;
			m_strReceiver = cap.Capabilities->Extension->Receiver->XAddr;
		}
		/* Recording */
		if (cap.Capabilities->Extension->Recording != NULL)
		{
			m_hasRecording = TRUE;
			m_strRecording = cap.Capabilities->Extension->Recording->XAddr;
		}
		/* Search */
		if (cap.Capabilities->Extension->Search != NULL)
		{
			m_hasSearch = TRUE;
			m_strSearch = cap.Capabilities->Extension->Search->XAddr;
		}
		/* Replay */
		if (cap.Capabilities->Extension->Replay != NULL)
		{
			m_hasReplay = TRUE;
			m_strReplay = cap.Capabilities->Extension->Replay->XAddr;
		}
	}

	m_hasGetCap = TRUE;
}

int OnvifClientDevice::GetCapabilities(_tds__GetCapabilitiesResponse& cap)
{
	soap_endpoint = m_strUrl.c_str();
	_tds__GetCapabilities capabilities;
	capabilities.Category.push_back(tt__CapabilityCategory__All);
	if (bHttpda)
		http_da_restore(this, &httpinfo);
	int nResult = __super::GetCapabilities(&capabilities, &cap);
	if (SOAP_OK == nResult)
	{
		if (cap.Capabilities)
		{
			SaveCapabilities(cap);
			return nResult;
		}
		else
			return SOAP_ERR;
	}
	else
	{
		if (400 == status)
		{
			// bad http request ,retry  in basic authenticate
			soap_wsse_add_Security(this);
			soap_wsse_add_UsernameTokenDigest(this, nullptr, m_strUser.c_str(), m_strPass.c_str());
			userid = soap_strdup(nullptr, m_strUser.c_str());
			passwd = soap_strdup(nullptr, m_strPass.c_str());
			nResult = __super::GetCapabilities(&capabilities, &cap);
			if (SOAP_OK == nResult && cap.Capabilities != nullptr)
			{
				SaveCapabilities(cap);
				return nResult;
			}
		}
		if (401 == status)
		{
			// retry in digest authenticate
			soap_register_plugin(this, http_da);

			userid = m_strUser.c_str();
			passwd = m_strUser.c_str();
			http_da_save(this, &httpinfo, authrealm, userid, passwd);
			nResult = __super::GetCapabilities(&capabilities, &cap);
			if (SOAP_OK == nResult && !cap.Capabilities)
				SaveCapabilities(cap);

			return nResult;
		}
	}
	return SOAP_ERR;

}

int OnvifClientDevice::GetCapabilities()
{
	_tds__GetCapabilitiesResponse cap;
	return GetCapabilities(cap);
}

bool OnvifClientDevice::GetMediaUrl(string& strUrl)
{
	if (m_hasGetCap == false || m_hasMedia == false)
	{
		return false;
	}

	strUrl = m_strMedia;

	return true;
}

bool OnvifClientDevice::GetPTZUrl(string& strUrl)
{
	if (m_hasGetCap == false || m_hasPTZ == false)
	{
		return false;
	}

	strUrl = m_strPTZ;

	return true;
}

bool OnvifClientDevice::GetImagingUrl(string& strUrl)
{
	if (m_hasGetCap == false || m_hasImaging == false)
	{
		return false;
	}

	strUrl = m_strImaging;

	return true;
}

bool OnvifClientDevice::GetReceiverUrl(string& strUrl)
{
	if (m_hasGetCap == false || m_hasReceiver == false)
	{
		return false;
	}

	strUrl = m_strReceiver;

	return true;
}

bool OnvifClientDevice::GetRecordingUrl(string& strUrl)
{
	if (m_hasGetCap == false || m_hasRecording == false)
	{
		return false;
	}

	strUrl = m_strRecording;

	return true;
}

bool OnvifClientDevice::GetSearchUrl(string& strUrl)
{
	if (m_hasGetCap == false || m_hasSearch == false)
	{
		return false;
	}

	strUrl = m_strSearch;

	return true;
}

bool OnvifClientDevice::GetReplayUrl(string& strUrl)
{
	if (m_hasGetCap == false || m_hasReplay == false)
	{
		return false;
	}

	strUrl = m_strReplay;

	return true;
}

bool OnvifClientDevice::GetEventUrl(string& strUrl)
{
	if (m_hasGetCap == false || m_hasEvent == false)
	{
		return false;
	}

	strUrl = m_strEvent;

	return true;
}


//Device Service Functions
int OnvifClientDevice::GetDeviceInformation(_tds__GetDeviceInformationResponse& DeviceInformationResponse)
{
	_tds__GetDeviceInformation DeviceInformationReq;

	//string strUrl;

	soap_endpoint = m_strUrl.c_str();

	return __super::GetDeviceInformation(&DeviceInformationReq, &DeviceInformationResponse);
}

int OnvifClientDevice::GetSystemSupportInformation(_tds__GetSystemSupportInformationResponse& Response)
{
	_tds__GetSystemSupportInformation Request;
	soap_endpoint = m_strUrl.c_str();
	return __super::GetSystemSupportInformation(&Request, &Response);
}

int OnvifClientDevice::GetSystemDateAndTime(_tds__GetSystemDateAndTimeResponse& SystemDateAndTimeResponse)
{
	_tds__GetSystemDateAndTime SystemDateAndTime;

	soap_endpoint = m_strUrl.c_str();
	return __super::GetSystemDateAndTime(&SystemDateAndTime, &SystemDateAndTimeResponse);
}

int OnvifClientDevice::SetSystemDateAndTime(_tds__SetSystemDateAndTimeResponse& SetSystemDateAndTimeResponse, tt__SetDateTimeType& DateTimeType, bool DayLightSavings, tt__TimeZone& Timezone, tt__DateTime& UTCDateTime)
{
	_tds__SetSystemDateAndTime SetDateTimeReq;
	SetDateTimeReq.DateTimeType = (tt__SetDateTimeType)1;// DateTimeType;
	SetDateTimeReq.DaylightSavings = DayLightSavings;
	SetDateTimeReq.TimeZone = &Timezone;
	SetDateTimeReq.UTCDateTime = &UTCDateTime;

	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}

	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	return __super::SetSystemDateAndTime(&SetDateTimeReq, &SetSystemDateAndTimeResponse);
}

int OnvifClientDevice::GetHostname(_tds__GetHostnameResponse& GetHostnameResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__GetHostname GetHostnameReq;

	return __super::GetHostname(&GetHostnameReq, &GetHostnameResponse);
}

int OnvifClientDevice::SetHostname(_tds__SetHostnameResponse& SetHostnameResponse, string Name)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__SetHostname SetHostnameReq;
	SetHostnameReq.Name = Name;
	return __super::SetHostname(&SetHostnameReq, &SetHostnameResponse);
}

int OnvifClientDevice::GetDNS(_tds__GetDNSResponse& GetDNSResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__GetDNS GetDNSReq;
	return __super::GetDNS(&GetDNSReq, &GetDNSResponse);
}

int OnvifClientDevice::SetDNS(_tds__SetDNSResponse& SetDNSResponse, bool FromDHCP, vector<string, allocator<string>> SearchDomain, vector<tt__IPAddress*, allocator<tt__IPAddress*>>& DNSManual)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__SetDNS SetDNSReq;
	SetDNSReq.FromDHCP = FromDHCP;
	SetDNSReq.SearchDomain = SearchDomain;
	SetDNSReq.DNSManual = DNSManual;
	return __super::SetDNS(&SetDNSReq, &SetDNSResponse);
}

int OnvifClientDevice::GetNTP(_tds__GetNTPResponse& GetNTPResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__GetNTP GetNTPReq;
	return __super::GetNTP(&GetNTPReq, &GetNTPResponse);
}

int OnvifClientDevice::SetNTP(_tds__SetNTPResponse& SetNTPResponse, bool FromDHCP, std::vector<tt__NetworkHost* >& NTPManual)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__SetNTP SetNTPReq;
	SetNTPReq.FromDHCP = FromDHCP;
	SetNTPReq.NTPManual = NTPManual;
	return __super::SetNTP(&SetNTPReq, &SetNTPResponse);
}

int OnvifClientDevice::GetDynamicDNS(_tds__GetDynamicDNSResponse& GetDynamicDNSResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__GetDynamicDNS GetDynamicDNSReq;
	return __super::GetDynamicDNS(&GetDynamicDNSReq, &GetDynamicDNSResponse);
}

int OnvifClientDevice::SetDynamicDNS(_tds__SetDynamicDNSResponse& SetDynamicDNSResponse, tt__DynamicDNSType& Type, tt__DNSName& Name, LONG64& durationTTL)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__SetDynamicDNS SetDynamicDNSReq;
	SetDynamicDNSReq.Name = &Name;
	SetDynamicDNSReq.TTL = &durationTTL;
	SetDynamicDNSReq.Type = Type;
	return __super::SetDynamicDNS(&SetDynamicDNSReq, &SetDynamicDNSResponse);
}

int OnvifClientDevice::GetNetworkInterfaces(_tds__GetNetworkInterfacesResponse& GetNetworkInterfacesResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__GetNetworkInterfaces GetNetworkInterfacesReq;
	return __super::GetNetworkInterfaces(&GetNetworkInterfacesReq, &GetNetworkInterfacesResponse);
}

int OnvifClientDevice::SetNetworkInterfaces(_tds__SetNetworkInterfacesResponse& SetNetworkInterfacesResponse, string InterfaceToken, tt__NetworkInterfaceSetConfiguration& NetworkInterface)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__SetNetworkInterfaces SetNetworkInterfacesReq;
	SetNetworkInterfacesReq.InterfaceToken = InterfaceToken;
	SetNetworkInterfacesReq.NetworkInterface = &NetworkInterface;
	return __super::SetNetworkInterfaces(&SetNetworkInterfacesReq, &SetNetworkInterfacesResponse);
}

int OnvifClientDevice::GetNetworkProtocols(_tds__GetNetworkProtocolsResponse& GetNetworkProtocolsResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__GetNetworkProtocols GetNetworkProtocolsReq;
	return __super::GetNetworkProtocols(&GetNetworkProtocolsReq, &GetNetworkProtocolsResponse);
}

int OnvifClientDevice::SetNetworkProtocols(_tds__SetNetworkProtocolsResponse& SetNetworkProtocolsResponse, vector<tt__NetworkProtocol*, allocator<tt__NetworkProtocol*>>& NetworkProtocols)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__SetNetworkProtocols SetNetworkProtocolsReq;
	SetNetworkProtocolsReq.NetworkProtocols = NetworkProtocols;
	return __super::SetNetworkProtocols(&SetNetworkProtocolsReq, &SetNetworkProtocolsResponse);
}

int OnvifClientDevice::GetNetworkDefaultGateway(_tds__GetNetworkDefaultGatewayResponse& GetNetworkDefaultGatewayResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__GetNetworkDefaultGateway GetNetworkDefaultGatewayReq;
	return __super::GetNetworkDefaultGateway(&GetNetworkDefaultGatewayReq, &GetNetworkDefaultGatewayResponse);
}

int OnvifClientDevice::SetNetworkDefaultGateway(_tds__SetNetworkDefaultGatewayResponse& SetNetworkDefaultGatewayResponse, vector<string, allocator<string>>& IPv4, vector<string, allocator<string>>& IPv6)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__SetNetworkDefaultGateway SetNetworkDefaultGatewayReq;
	SetNetworkDefaultGatewayReq.IPv4Address = IPv4;
	SetNetworkDefaultGatewayReq.IPv6Address = IPv6;
	return __super::SetNetworkDefaultGateway(&SetNetworkDefaultGatewayReq, &SetNetworkDefaultGatewayResponse);
}

int OnvifClientDevice::SystemReboot(_tds__SystemRebootResponse& SystemRebootResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	soap_endpoint = strUrl.c_str();
	soap_wsse_add_Security(this);
	soap_wsse_add_UsernameTokenDigest(this, "Id", strUser.c_str(), strPass.c_str());

	_tds__SystemReboot SystemRebootReq;
	return __super::SystemReboot(&SystemRebootReq, &SystemRebootResponse);
}

int OnvifClientDevice::SynchronizeDateAndTimeWithCamera(string& strUrl, string& strUser, string& strPass, _tds__SetSystemDateAndTimeResponse& SetSystemDateAndTimeResponse)
{
	m_strUrl = strUrl;
	m_strUser = strUser;
	m_strPass = strPass;

	tt__DateTime* diffTime_tt = new tt__DateTime;
	_tds__GetSystemDateAndTimeResponse DateAndTimeResp;
	this->GetSystemDateAndTime(DateAndTimeResp);
	tt__DateTime* camHolder = DateAndTimeResp.SystemDateAndTime->UTCDateTime;

	time_t timeNow = time(NULL);		struct tm* offsetStruct = gmtime(&timeNow); //offsetStruct->tm_year += 1900;
	double cam_pc_offset = findDiffTime(*offsetStruct, *camHolder, DateAndTimeResp.SystemDateAndTime->DaylightSavings);

	double diffhold = (double)timeNow; double difference = diffhold - cam_pc_offset;
	cout << "current time: " << diffhold << endl << "difference calced here: " << difference << endl;

	//Doing some soap work
	soap_wsse_add_Security(this);
	LocalAddUsernameTokenDigest(this, cam_pc_offset);

	// creating and setting parameter for the set date and time request
	_tds__SetSystemDateAndTime SetDateTimeReq;
	SetDateTimeReq.DateTimeType = (tt__SetDateTimeType)0;// DateTimeType - 0=manual, 1=NTP;
	SetDateTimeReq.DaylightSavings = (bool)offsetStruct->tm_isdst;// DayLightSavings;
	// *** Timezone stuff is messy... could be cleaned up potentially -john
	tt__TimeZone* TiZ = new tt__TimeZone;
	string TimeZ = "MST7MDT,M3.2.0,M11.1.0"; // TODO: Needs to be configurable depending on what time zone is being worked in. Set for Mountain time (Denver);
	TiZ->TZ = TimeZ;	SetDateTimeReq.TimeZone = TiZ;
	// set the camera time to match pc time for authentication simplicity
	tt__DateTime* UTCDateTime = new tt__DateTime;
	time_t NOW = time(NULL);	struct tm* noww = gmtime(&NOW);
	tt__Date thisDate;		thisDate.Day = noww->tm_mday;	thisDate.Month = noww->tm_mon + 1;	thisDate.Year = noww->tm_year + 1900; // added 1 to month and 1900 to year to account for how tm defines those values
	tt__Time thisTime;		thisTime.Hour = noww->tm_hour;	thisTime.Minute = noww->tm_min;	thisTime.Second = noww->tm_sec;
	UTCDateTime->Date = &thisDate;	UTCDateTime->Time = &thisTime;
	SetDateTimeReq.UTCDateTime = UTCDateTime;

	return __super::SetSystemDateAndTime(&SetDateTimeReq, &SetSystemDateAndTimeResponse);
}

int OnvifClientDevice::SynchronizeDateAndTimeWithCamera(_tds__SetSystemDateAndTimeResponse& SetSystemDateAndTimeResponse)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	return SynchronizeDateAndTimeWithCamera(strUrl, strUser, strPass, SetSystemDateAndTimeResponse);
}

bool OnvifClientDevice::GetUserPasswd(string& strUser, string& strPass)
{
	strUser = m_strUser;
	strPass = m_strPass;
	return true;
}
bool OnvifClientDevice::SetUrlUserPasswd(string& strUrl, string& strUser, string& strPass)
{
	m_strUrl = strUrl;
	m_strUser = strUser;
	m_strPass = strPass;
	return true;
}
bool OnvifClientDevice::GetUrl(string& _strUrl)
{
	_strUrl = m_strUrl;
	return true;
}
/* used to find time offset inside SynchronizeDateAndTimeWithCamera()  */
double OnvifClientDevice::findDiffTime(struct tm local_sys, tt__DateTime cameraTime, bool isDST)
{
	tt__Date* camDate = cameraTime.Date;
	tt__Time* camTime = cameraTime.Time;

	// Converting tt__DateTime to tm struct to find time offset
	struct tm* CamDateTime = new tm;
	CamDateTime->tm_hour = camTime->Hour; CamDateTime->tm_min = camTime->Minute; CamDateTime->tm_sec = camTime->Second;
	CamDateTime->tm_mday = camDate->Day; CamDateTime->tm_mon = camDate->Month - 1; CamDateTime->tm_year = camDate->Year - 1900;
	CamDateTime->tm_isdst = isDST;
	if (isDST)
		CamDateTime->tm_hour++;
	//convert camera and system tm to time_t
	time_t camera_time_t = mktime(CamDateTime);		time_t sys_time_t = mktime(&local_sys);

	double diff = difftime(sys_time_t, camera_time_t);
	cout << "camera epoch: " << camera_time_t << "\t\tSystem epoch: " << sys_time_t << endl << "returned diff: " << diff << "\t\tcalced here: " << sys_time_t - camera_time_t << endl;
	return diff;
}

/* Most of this function is taken from the function 'soap_wsse_add_UsernameTokenDigest()' defined in wsseapi.cpp //
// Used to alter the soap request for an offset time (for authorization purposes - replay attack protection) */
int OnvifClientDevice::LocalAddUsernameTokenDigest(struct soap* soapOff, double cam_pc_offset)
{
	string strUrl;
	string strUser;
	string strPass;
	if (this->GetUserPasswd(strUser, strPass) == false
		|| this->GetUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	/* start soap_wsse_add_UsernameTokenDigest; Taken from wsseapi.cpp*/
	/* All of this is taken from the function soap_wsse_add_UsernameTokenDigest() defined in wsseapi.cpp */
	_wsse__Security* security = soap_wsse_add_Security(soapOff);
	time_t now = time(NULL);
	now -= (time_t)cam_pc_offset; //offset so digest comes out correctly (synced times between cam and pc);
	const char* created = soap_dateTime2s(soapOff, now);
	char HA[SOAP_SMD_SHA1_SIZE], HABase64[29];
	char nonce[20], * nonceBase64;

	/*start calc_nonce(soapOff, nonce); Taken from wsseapi.cpp */
	time_t r = time(NULL);
	cout << "now time: " << r << endl;
	r -= (time_t)cam_pc_offset; //offset so digest comes out correctly (synced times between cam and pc);
	cout << "now time minus offset: " << r << endl;
	memcpy(nonce, &r, 4);
	for (int i = 4; i < 20; i += 4)
	{
		r = soap_random;
		memcpy(nonce + i, &r, 4);
	}
	/*end calc_nonce(soapOff, nonce); */

	nonceBase64 = soap_s2base64(soapOff, (unsigned char*)nonce, NULL, 20);

	/* start calc_digest(soapOff, created, nonce, 20, strPass, HA);  Taken from wsseapi.cpp */
	struct soap_smd_data context;
	/* use smdevp engine */
	soap_smd_init(soapOff, &context, SOAP_SMD_DGST_SHA1, NULL, 0);
	soap_smd_update(soapOff, &context, nonce, 20);
	soap_smd_update(soapOff, &context, created, strlen(created));
	soap_smd_update(soapOff, &context, strPass.c_str(), strlen(strPass.c_str()));
	soap_smd_final(soapOff, &context, HA, NULL);
	/* end calc_digest(soapOff, created, nonce, 20, strPass, HA); */

	soap_s2base64(soapOff, (unsigned char*)HA, HABase64, SOAP_SMD_SHA1_SIZE);
	/* populate the UsernameToken with digest */
	soap_wsse_add_UsernameTokenText(soapOff, "Id", strUser.c_str(), HABase64);
	/* populate the remainder of the password, nonce, and created */
	security->UsernameToken->Password->Type = (char*)wsse_PasswordDigestURI;
	security->UsernameToken->Nonce = nonceBase64;
	security->UsernameToken->wsu__Created = soap_strdup(soapOff, created);
	/* end soap_wsse_add_UsernameTokenDigest */
	return SOAP_OK;
}

http_da_info& OnvifClientDevice::GetHttpDa()
{
	return httpinfo;
}

bool& OnvifClientDevice::HttpdaEnabled()
{
	return bHttpda;
}
const char* OnvifClientDevice::GetDevIP()
{
	if (m_strDevIP.size())
		return m_strDevIP.c_str();
	else
		return nullptr;
}
