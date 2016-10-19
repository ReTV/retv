#pragma once

/*
*      Copyright (C) 2016 Billion Plus Computing Pvt. Ltd.
*      http://retv.in
*
*/

#include "utils/JSONVariantParser.h"
#include "utils/Variant.h"
#include "interfaces/legacy/SubscriptionInfo.h"


const int REQUEST_TIMEOUT = 10;

class ReTV
{
public:
	/*!
	\brief The only way through which the global instance of the ReTV should be accessed.
	\return the global instance.
	*/
	static ReTV& GetInstance();

	/*!
	\brief Initialize ReTV - pass whether we want to use staging or live server
	*/
	bool initialize(int type, const char* deviceCode, const char* mobileNumber);

	bool login();

	std::string callAPI(const char* endPoint, const char* postVars, int timeout=REQUEST_TIMEOUT);
	std::string callMediaAPI(const char* endPoint, const char* postVars, int timeout=REQUEST_TIMEOUT);

	void Cleanup();

    std::string makeApiURL(std::string api);
    std::string makeMediaApiURL(std::string api);

	XBMCAddon::retv::SubscriptionInfo* getSubscriptionInfo();

	int getSubEndTime();

    bool isLoggedIn();

	float getFFData();

private:
	// private construction, and no assignments; use the provided singleton methods
	ReTV();
	ReTV(const ReTV&) = delete;
	ReTV const& operator=(ReTV const&) = delete;
	~ReTV();

	bool m_initialized = false;

	bool m_loggedIn = false;

	std::string m_authToken;
	std::string m_deviceCode;
	std::string m_mobileNumber;

	XBMCAddon::retv::SubscriptionInfo m_subInfo;

	unsigned int m_expiryTime;
    int api_type;
    const int API_TYPE_LIVE = 0;
    const int API_TYPE_STAGING = 1;
	double m_loginTime;

	std::string m_planName;
	std::string m_planCode;
	unsigned int m_subscriptionStart;
	unsigned int m_subscriptionEnd;
	double m_fastForwardData;

	std::string m_userNameF;
	std::string m_userNameL;
	std::string m_userAddress;
	std::string m_userCountry;
	std::string m_userCity;
	std::string m_userEmail;


	const std::string m_headerAuthorization = "Basic cmV0dkFQSUNvbnN1bWVyOlIkdF4xNg==";
	const std::string m_headerContentType = "application/json";//; charset=UTF-8";

	std::string m_apiUrl;
	std::string m_mediaUrl;

	const std::string m_apiUrlLive = "https://api.retv.in/index.php/api/";
	const std::string m_apiUrlStaging = "https://staging.retv.in/api.retv.in/index.php/api/";
	
	const std::string m_mediaUrlLive = "http://files.retv.in/";
	const std::string m_mediaUrlStaging = "http://staging.retv.in/files.retv.in/";

    const std::string rpc_url = "http://localhost:4000/api";

	// API end points
	const std::string m_api_Login = "user/login";

	bool parseLoginResponse(CVariant loginResponse);
    bool initRPC();
};
