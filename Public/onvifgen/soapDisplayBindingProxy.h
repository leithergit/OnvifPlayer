/* soapDisplayBindingProxy.h
   Generated by gSOAP 2.8.17r from onvif.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapDisplayBindingProxy_H
#define soapDisplayBindingProxy_H
#include "soapH.h"

class SOAP_CMAC DisplayBindingProxy : public soap
{ public:
	/// Endpoint URL of service 'DisplayBindingProxy' (change as needed)
	const char *soap_endpoint;
	/// Constructor
	DisplayBindingProxy();
	/// Construct from another engine state
	DisplayBindingProxy(const struct soap&);
	/// Constructor with endpoint URL
	DisplayBindingProxy(const char *url);
	/// Constructor with engine input+output mode control
	DisplayBindingProxy(soap_mode iomode);
	/// Constructor with URL and input+output mode control
	DisplayBindingProxy(const char *url, soap_mode iomode);
	/// Constructor with engine input and output mode control
	DisplayBindingProxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~DisplayBindingProxy();
	/// Initializer used by constructors
	virtual	void DisplayBindingProxy_init(soap_mode imode, soap_mode omode);
	/// Delete all deserialized data (with soap_destroy and soap_end)
	virtual	void destroy();
	/// Delete all deserialized data and reset to default
	virtual	void reset();
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Put SOAP Header in message
	virtual	void soap_header(struct _wsse__Security *wsse__Security, char *wsa__MessageID, struct wsa__Relationship *wsa__RelatesTo, struct wsa__EndpointReferenceType *wsa__From, struct wsa__EndpointReferenceType *wsa__ReplyTo, struct wsa__EndpointReferenceType *wsa__FaultTo, char *wsa__To, char *wsa__Action, struct wsdd__AppSequenceType *wsdd__AppSequence, char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action, struct chan__ChannelInstanceType *chan__ChannelInstance);
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Force close connection (can kill a thread blocked on IO)
	virtual	int soap_force_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
#ifndef WITH_COMPAT
	virtual	void soap_stream_fault(std::ostream&);
#endif

	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_tls__GetServiceCapabilities *tls__GetServiceCapabilities, _tls__GetServiceCapabilitiesResponse *tls__GetServiceCapabilitiesResponse) { return this->GetServiceCapabilities(NULL, NULL, tls__GetServiceCapabilities, tls__GetServiceCapabilitiesResponse); }
	virtual	int GetServiceCapabilities(const char *endpoint, const char *soap_action, _tls__GetServiceCapabilities *tls__GetServiceCapabilities, _tls__GetServiceCapabilitiesResponse *tls__GetServiceCapabilitiesResponse);

	/// Web service operation 'GetLayout' (returns error code or SOAP_OK)
	virtual	int GetLayout(_tls__GetLayout *tls__GetLayout, _tls__GetLayoutResponse *tls__GetLayoutResponse) { return this->GetLayout(NULL, NULL, tls__GetLayout, tls__GetLayoutResponse); }
	virtual	int GetLayout(const char *endpoint, const char *soap_action, _tls__GetLayout *tls__GetLayout, _tls__GetLayoutResponse *tls__GetLayoutResponse);

	/// Web service operation 'SetLayout' (returns error code or SOAP_OK)
	virtual	int SetLayout(_tls__SetLayout *tls__SetLayout, _tls__SetLayoutResponse *tls__SetLayoutResponse) { return this->SetLayout(NULL, NULL, tls__SetLayout, tls__SetLayoutResponse); }
	virtual	int SetLayout(const char *endpoint, const char *soap_action, _tls__SetLayout *tls__SetLayout, _tls__SetLayoutResponse *tls__SetLayoutResponse);

	/// Web service operation 'GetDisplayOptions' (returns error code or SOAP_OK)
	virtual	int GetDisplayOptions(_tls__GetDisplayOptions *tls__GetDisplayOptions, _tls__GetDisplayOptionsResponse *tls__GetDisplayOptionsResponse) { return this->GetDisplayOptions(NULL, NULL, tls__GetDisplayOptions, tls__GetDisplayOptionsResponse); }
	virtual	int GetDisplayOptions(const char *endpoint, const char *soap_action, _tls__GetDisplayOptions *tls__GetDisplayOptions, _tls__GetDisplayOptionsResponse *tls__GetDisplayOptionsResponse);

	/// Web service operation 'GetPaneConfigurations' (returns error code or SOAP_OK)
	virtual	int GetPaneConfigurations(_tls__GetPaneConfigurations *tls__GetPaneConfigurations, _tls__GetPaneConfigurationsResponse *tls__GetPaneConfigurationsResponse) { return this->GetPaneConfigurations(NULL, NULL, tls__GetPaneConfigurations, tls__GetPaneConfigurationsResponse); }
	virtual	int GetPaneConfigurations(const char *endpoint, const char *soap_action, _tls__GetPaneConfigurations *tls__GetPaneConfigurations, _tls__GetPaneConfigurationsResponse *tls__GetPaneConfigurationsResponse);

	/// Web service operation 'GetPaneConfiguration' (returns error code or SOAP_OK)
	virtual	int GetPaneConfiguration(_tls__GetPaneConfiguration *tls__GetPaneConfiguration, _tls__GetPaneConfigurationResponse *tls__GetPaneConfigurationResponse) { return this->GetPaneConfiguration(NULL, NULL, tls__GetPaneConfiguration, tls__GetPaneConfigurationResponse); }
	virtual	int GetPaneConfiguration(const char *endpoint, const char *soap_action, _tls__GetPaneConfiguration *tls__GetPaneConfiguration, _tls__GetPaneConfigurationResponse *tls__GetPaneConfigurationResponse);

	/// Web service operation 'SetPaneConfigurations' (returns error code or SOAP_OK)
	virtual	int SetPaneConfigurations(_tls__SetPaneConfigurations *tls__SetPaneConfigurations, _tls__SetPaneConfigurationsResponse *tls__SetPaneConfigurationsResponse) { return this->SetPaneConfigurations(NULL, NULL, tls__SetPaneConfigurations, tls__SetPaneConfigurationsResponse); }
	virtual	int SetPaneConfigurations(const char *endpoint, const char *soap_action, _tls__SetPaneConfigurations *tls__SetPaneConfigurations, _tls__SetPaneConfigurationsResponse *tls__SetPaneConfigurationsResponse);

	/// Web service operation 'SetPaneConfiguration' (returns error code or SOAP_OK)
	virtual	int SetPaneConfiguration(_tls__SetPaneConfiguration *tls__SetPaneConfiguration, _tls__SetPaneConfigurationResponse *tls__SetPaneConfigurationResponse) { return this->SetPaneConfiguration(NULL, NULL, tls__SetPaneConfiguration, tls__SetPaneConfigurationResponse); }
	virtual	int SetPaneConfiguration(const char *endpoint, const char *soap_action, _tls__SetPaneConfiguration *tls__SetPaneConfiguration, _tls__SetPaneConfigurationResponse *tls__SetPaneConfigurationResponse);

	/// Web service operation 'CreatePaneConfiguration' (returns error code or SOAP_OK)
	virtual	int CreatePaneConfiguration(_tls__CreatePaneConfiguration *tls__CreatePaneConfiguration, _tls__CreatePaneConfigurationResponse *tls__CreatePaneConfigurationResponse) { return this->CreatePaneConfiguration(NULL, NULL, tls__CreatePaneConfiguration, tls__CreatePaneConfigurationResponse); }
	virtual	int CreatePaneConfiguration(const char *endpoint, const char *soap_action, _tls__CreatePaneConfiguration *tls__CreatePaneConfiguration, _tls__CreatePaneConfigurationResponse *tls__CreatePaneConfigurationResponse);

	/// Web service operation 'DeletePaneConfiguration' (returns error code or SOAP_OK)
	virtual	int DeletePaneConfiguration(_tls__DeletePaneConfiguration *tls__DeletePaneConfiguration, _tls__DeletePaneConfigurationResponse *tls__DeletePaneConfigurationResponse) { return this->DeletePaneConfiguration(NULL, NULL, tls__DeletePaneConfiguration, tls__DeletePaneConfigurationResponse); }
	virtual	int DeletePaneConfiguration(const char *endpoint, const char *soap_action, _tls__DeletePaneConfiguration *tls__DeletePaneConfiguration, _tls__DeletePaneConfigurationResponse *tls__DeletePaneConfigurationResponse);
};
#endif
