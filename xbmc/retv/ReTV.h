#pragma once

/*
*      Copyright (C) 2016 Billion Plus Computing Pvt. Ltd.
*      http://retv.in
*
*/

#include "utils/JSONVariantParser.h"
#include "utils/Variant.h"
#include "interfaces/legacy/SubscriptionInfo.h"
#include "GUIInfoManager.h"


enum ReTVPlatform { ReTVDevice = 1, Android = 2, iOS  = 3, Windows = 4, OSX = 5, Linux = 6, FreeBSD = 7, Others = 8 };

struct ReTVPlatformInfo
{
	ReTVPlatform m_platformId;
	std::string m_arch;
	int m_bitness;
	std::string m_os;
	std::string m_osVersion;
	std::string m_manufacturer;
	std::string m_brand;
	std::string m_model;
	std::string m_product;
	std::string m_deviceName;
	std::string m_macAddress;
	std::string m_versionCore;
	std::string m_versionScript;
	std::string m_versionSkin;
};

class ReTV
{
public:
	/*!
	\brief The only way through which the global instance of the ReTV should be accessed.
	\return the global instance.
	*/
	//static ReTV& GetInstance();

	ReTV();
	~ReTV();

	static const std::string Author;

	static std::string makeAuthenticatedUrl(std::string url);
	std::string makeReTVAddonUrl(std::string url);

	static int torrentProgress;

	static const int API_TYPE_LIVE = 0;
	static const int API_TYPE_STAGING = 1;

	/*!
	\brief Initialize ReTV
	*/
	void Initialize();

	/*!
	\brief Set User data
	*/
	bool setUser(const char* mobileNumber);

	bool isActivated();

	std::string registerDevice(const char* mobileNumber);

	std::string validateNumber(const char* authCode);

	int getPlatform();

	std::string login(const char* mobileNumber);

    std::string callAPI(const char* endPoint, const char* postVars, int customTimeout=-1);
    std::string callMediaAPI(const char* endPoint, const char* postVars, int customTimeout=-1);
	bool callDownloadAPI(const char* fileId, const char* filePath);
    std::string getLinkByToken(const char* token);
    std::string getBaseUrl();

    void Cleanup();

    std::string makeApiURL(std::string api);
    std::string makeMediaApiURL(std::string api);

	XBMCAddon::retv::SubscriptionInfo* getSubscriptionInfo();

	int getSubEndTime();

    bool isLoggedIn();

	float getFFData();

private:
	// private construction, and no assignments; use the provided singleton methods
	//ReTV();
	//ReTV(const ReTV&) = delete;
	//ReTV const& operator=(ReTV const&) = delete;
	//~ReTV();

	bool m_isActivated = false;

	bool m_initialized = false;

	bool m_loggedIn = false;

	std::string m_authToken;
	std::string m_deviceCode;
	std::string m_mobileNumber;

	XBMCAddon::retv::SubscriptionInfo m_subInfo;

	unsigned int m_expiryTime;
    int api_type;

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

	ReTVPlatformInfo m_platformInfo;

	const std::string c_retvManufacturer = "BnPlus";

	const std::string m_headerAuthorization = "Basic cmV0dkFQSUNvbnN1bWVyOlIkdF4xNg==";
	const std::string m_headerContentType = "application/json";//; charset=UTF-8";

	static std::string m_updateRepoUsername;
	static std::string m_updateRepoPassword;

	std::string m_apiUrl;
	std::string m_mediaUrl;
	std::string m_downloadUrl;

    std::string api_username = "retvAPIConsumer";
    std::string api_password = "R$t^16";

	const std::string c_retvSkin	= "skin.retv.2";
	const std::string c_retvScript	= "script.retv";




	const std::string m_apiUrlLive = "https://api.retv.in/index.php/api/";
	const std::string m_apiUrlStaging = "http://staging.retv.in/api.retv.in/index.php/api/";
	
	const std::string m_mediaUrlLive = "http://files.retv.in/";
	const std::string m_mediaUrlStaging = "http://staging.retv.in/files.retv.in/";

	const std::string m_downloadUrlLive = "https://api.retv.in/index.php/download/";
	const std::string m_downloadUrlStaging = "http://staging.retv.in/api.retv.in/index.php/download/";

    const std::string rpc_url = "http://localhost:4000/api";

	// API end points
	const std::string m_api_Login = "user/login";

	const std::string m_api_Activate = "activation/sendcode";
	const std::string m_api_Validate = "activation/validatecode";

	std::string parseLoginResponse(std::string loginResponseString);

	int parseValidationResponse(std::string validationResponse);

	void readPlatformInfo();

	void readSubscriptionInfo();

	std::string getLoginJSON();
	std::string getDeviceActivationJSON();

#if defined(TARGET_ANDROID)
    bool initRPC();
#endif
};


extern ReTV g_retv;
extern CGUIInfoManager g_infoManager;