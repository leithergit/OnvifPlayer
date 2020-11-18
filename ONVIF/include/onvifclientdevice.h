#ifndef __ONVIF_CLIENT_DEVICE__
#define __ONVIF_CLIENT_DEVICE__

#include <string>
#include <map>
#include <iostream>
#include <list>
#include <ctime>
#include "soapStub.h"
#include "soapDeviceBindingProxy.h"
#include "soapPTZBindingProxy.h"
#include "soapImagingBindingProxy.h"
#include "soapMediaBindingProxy.h"
#include "wsseapi.h"
#include "httpdadefine.h"
using namespace std;
enum  DevFuncAuthentic
{

};
class  OnvifClientDevice :public DeviceBindingProxy
	/*,public PTZBindingProxy
	 ,public ImagingBindingProxy
	 ,public MediaBindingProxy*/
{
public:
	Declare_ClassName(OnvifClientDevice);
	OnvifClientDevice(string strIP, string strUser, string strPass, unsigned short nPort = 80);
	~OnvifClientDevice();

	void SaveCapabilities(_tds__GetCapabilitiesResponse& cap);

	int GetCapabilities(_tds__GetCapabilitiesResponse& cap);

	int GetServiceCapabilites(_tds__GetServiceCapabilitiesResponse& ServiceCapRes);

	void SaveServiceCapabilites(tds__DeviceServiceCapabilities* pCapabilities);

	int GetCapabilities();

	bool GetMediaUrl(string& strUrl);

	bool GetPTZUrl(string& strUrl);

	bool GetImagingUrl(string& strUrl);

	bool GetReceiverUrl(string& strUrl);

	bool GetRecordingUrl(string& strUrl);

	bool GetSearchUrl(string& strUrl);

	bool GetReplayUrl(string& strUrl);

	bool GetEventUrl(string& strUrl);

	int  GetSystemLog(_tds__GetSystemLog& tds__GetSystemLog, _tds__GetSystemLogResponse& tds__GetSystemLogResponse);

	//Device Service Functions
	int GetDeviceInformation(_tds__GetDeviceInformationResponse& Response);

	int GetSystemSupportInformation(_tds__GetSystemSupportInformationResponse& Response);

	int GetSystemDateAndTime(_tds__GetSystemDateAndTimeResponse& SystemDateAndTimeResponse);

	int SetSystemDateAndTime(_tds__SetSystemDateAndTimeResponse& SetSystemDateAndTimeResponse, tt__SetDateTimeType& DateTimeType, bool DayLightSavings, tt__TimeZone& Timezone, tt__DateTime& UTCDateTime);

	int GetHostname(_tds__GetHostnameResponse& GetHostnameResponse);

	int SetHostname(_tds__SetHostnameResponse& SetHostnameResponse, string Name);

	int GetDNS(_tds__GetDNSResponse& GetDNSResponse);

	int SetDNS(_tds__SetDNSResponse& SetDNSResponse, bool FromDHCP, vector<string, allocator<string>> SearchDomain, vector<tt__IPAddress*, allocator<tt__IPAddress*>>& DNSManual);

	int GetNTP(_tds__GetNTPResponse& GetNTPResponse);

	int SetNTP(_tds__SetNTPResponse& SetNTPResponse, bool FromDHCP, std::vector<tt__NetworkHost* >& NTPManual);

	int GetDynamicDNS(_tds__GetDynamicDNSResponse& GetDynamicDNSResponse);

	int SetDynamicDNS(_tds__SetDynamicDNSResponse& SetDynamicDNSResponse, tt__DynamicDNSType& Type, tt__DNSName& Name, LONG64& durationTTL);

	int GetNetworkInterfaces(_tds__GetNetworkInterfacesResponse& GetNetworkInterfacesResponse);

	int SetNetworkInterfaces(_tds__SetNetworkInterfacesResponse& SetNetworkInterfacesResponse, string InterfaceToken, tt__NetworkInterfaceSetConfiguration& NetworkInterface);

	int GetNetworkProtocols(_tds__GetNetworkProtocolsResponse& GetNetworkProtocolsResponse);

	int SetNetworkProtocols(_tds__SetNetworkProtocolsResponse& SetNetworkProtocolsResponse, vector<tt__NetworkProtocol*, allocator<tt__NetworkProtocol*>>& NetworkProtocols);

	int GetNetworkDefaultGateway(_tds__GetNetworkDefaultGatewayResponse& GetNetworkDefaultGatewayResponse);

	int SetNetworkDefaultGateway(_tds__SetNetworkDefaultGatewayResponse& SetNetworkDefaultGatewayResponse, vector<string, allocator<string>>& IPv4, vector<string, allocator<string>>& IPv6);

	int SystemReboot(_tds__SystemRebootResponse& SystemRebootResponse);

	int SetSystemFactoryDefault(_tds__SetSystemFactoryDefault& Request, _tds__SetSystemFactoryDefaultResponse& Response);

	int SynchronizeDateAndTimeWithCamera(string& strUrl, string& strUser, string& strPass, _tds__SetSystemDateAndTimeResponse& SetSystemDateAndTimeResponse);

	int SynchronizeDateAndTimeWithCamera(_tds__SetSystemDateAndTimeResponse& SetSystemDateAndTimeResponse);

	bool GetUserPasswd(string& strUser, string& strPass);

	bool SetUrlUserPasswd(string& strUrl, string& strUser, string& strPass);

	int GetScopes(_tds__GetScopes& tds__GetScopes, _tds__GetScopesResponse& tds__GetScopesResponse);

	bool GetUrl(string& _strUrl);
	/* used to find time offset inside SynchronizeDateAndTimeWithCamera()  */
	double findDiffTime(struct tm local_sys, tt__DateTime cameraTime, bool isDST);

	/* Most of this function is taken from the function 'soap_wsse_add_UsernameTokenDigest()' defined in wsseapi.cpp //
	// Used to alter the soap request for an offset time (for authorization purposes - replay attack protection) */
	int LocalAddUsernameTokenDigest(struct soap* soapOff, double cam_pc_offset);

	http_da_info& GetHttpDa();
	bool& HttpdaEnabled();
	const char* GetDevIP();
	bool GetMessage(char* szMessageBuff, int nBufferSize);
	// ServiceCapabilites 
	bool m_bTLS1_x002e0;	/* optional attribute */
	bool m_bTLS1_x002e1;	/* optional attribute */
	bool m_bTLS1_x002e2;	/* optional attribute */
	bool m_bOnboardKeyGeneration;	/* optional attribute */
	bool m_bAccessPolicyConfig;	/* optional attribute */
	bool m_bDefaultAccessPolicy;	/* optional attribute */
	bool m_bDot1X;	/* optional attribute */
	bool m_bRemoteUserHandling;	/* optional attribute */
	bool m_bX_x002e509Token;	/* optional attribute */
	bool m_bSAMLToken;	/* optional attribute */
	bool m_bKerberosToken;	/* optional attribute */
	bool m_bUsernameToken;	/* optional attribute */
	bool m_bHttpDigest;	/* optional attribute */
	bool m_bRELToken;	/* optional attribute */
	std::string m_strSupportedEAPMethods;	/* optional attribute */
	int m_nMaxUsers;	/* optional attribute */
private:
	string m_strUrl;
	string m_strUser;
	string m_strPass;
	string m_strDevIP;
	bool m_hasGetCap;
	http_da_info httpinfo;
	bool m_bHttpda = false;
	const char* httpuserid = nullptr;
	const char* httppasswd = nullptr;
	/* If the Device support blow service */
	bool m_hasMedia;
	bool m_hasPTZ;
	bool m_hasImaging;
	bool m_hasReceiver;
	bool m_hasRecording;
	bool m_hasSearch;
	bool m_hasReplay;
	bool m_hasEvent;

	/* The Url of blow service */
	string m_strMedia;
	string m_strPTZ;
	string m_strImaging;
	string m_strReceiver;
	string m_strRecording;
	string m_strSearch;
	string m_strReplay;
	string m_strEvent;
	//DeviceBindingProxy deviceBindProxy;

};


#endif
