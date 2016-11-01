/*
*      Copyright (C) 2016 Billion Plus Computing Pvt. Ltd.
*      http://retv.in
*
*/

#include <ctime>
#include <string.h>
#include <sstream>
#include "ReTV.h"
#include "filesystem/CurlFile.h"
#include "interfaces/AnnouncementManager.h"
#include "utils/log.h"
#include "utils/JSONVariantParser.h"
#include "utils/JSONVariantWriter.h"
#include "utils/Variant.h"
#include "interfaces/legacy/SubscriptionInfo.h"
#include "utils/SystemInfo.h"
#include "guiinfo/GUIInfoLabels.h"
#include "Application.h"
#include "network/Network.h"
#include "filesystem/File.h"
#include "filesystem/SpecialProtocol.h"
#include "utils/StringUtils.h"
#include "system.h"
#include "settings/AdvancedSettings.h"
#include "addons/AddonDatabase.h"

using namespace XBMCAddon::retv;
using namespace XFILE;

ReTV g_retv;

const std::string ReTV::Author = "Billion Plus Computing Pvt. Ltd.";

std::string ReTV::m_updateRepoUsername = "retv";
std::string ReTV::m_updateRepoPassword = "dsd234SGFD823hd234";

int ReTV::torrentProgress = 0;


ReTV::ReTV()
{
	CLog::Log(LOGNOTICE, "Initialized ReTV Object");
}

ReTV::~ReTV()
{
	Cleanup();
}

void ReTV::Cleanup()
{

}

std::string ReTV::makeApiURL(std::string api)
{
	return m_apiUrl + api;
}

std::string ReTV::makeMediaApiURL(std::string api)
{
    return m_mediaUrl + api;
}


void ReTV::Initialize()
{
	CLog::Log(LOGNOTICE, "Initializing ReTV");

	// Live
	api_type = g_advancedSettings.m_apiType;

	if (api_type == API_TYPE_LIVE){
		m_apiUrl = m_apiUrlLive;
		m_mediaUrl = m_mediaUrlLive;
	}
	// Staging
	else{
		m_apiUrl = m_apiUrlStaging;
		m_mediaUrl = m_mediaUrlStaging;
	}

	// Read the platform Info
	readPlatformInfo();

	m_initialized = true;

	// Done for now
}


bool ReTV::isActivated()
{
	return m_isActivated;
}


std::string ReTV::registerDevice(const char* mobileNumber)
{
	if (!m_initialized)
		// 700 = Not Initialized
		return "{\"successcode\": 700 } ";

	XFILE::CCurlFile http;

	if (!http.IsInternet())
		// Curl No internet
		return "{\"errorcode\": 0 } ";

	http.SetRequestHeader("Authorization", m_headerAuthorization);
	http.SetRequestHeader("Content-Type", m_headerContentType);

	m_mobileNumber = (std::string)mobileNumber;

	std::string postData = getDeviceActivationJSON();

	std::string content;

	CLog::Log(LOGNOTICE, "Post Data : %s", postData.c_str());

	CLog::Log(LOGNOTICE, "URL : %s", makeApiURL(m_api_Activate).c_str());
	if (!http.Post(makeApiURL(m_api_Activate), postData, content, true)){
		CLog::Log(LOGNOTICE, "ReTV: Couldn't contact Activation URL");
		return "{\"errorcode\": 0 } ";
	}

	CLog::Log(LOGNOTICE, "ReTV: Activation response : %s", content.c_str());

	return content.c_str();
}


std::string ReTV::validateNumber(const char* authCode)
{
	if (!m_initialized)
		// 700 = Not Initialized
		return "{\"successcode\": 700 } ";

	XFILE::CCurlFile http;

	if (!http.IsInternet())
		// Curl No internet
		return "{\"errorcode\": 0 } ";

	http.SetRequestHeader("Authorization", m_headerAuthorization);
	http.SetRequestHeader("Content-Type", m_headerContentType);

	std::string postData = "{\"mobile\": \"" + m_mobileNumber + "\", \"devicecode\": \"" + m_platformInfo.m_macAddress + "\" ,\"authcode\":\"" + authCode + "\"}";

	std::string content;

	CLog::Log(LOGNOTICE, "Post Data : %s", postData.c_str());

	CLog::Log(LOGNOTICE, "URL : %s", makeApiURL(m_api_Validate).c_str());
	if (!http.Post(makeApiURL(m_api_Validate), postData, content, true)){
		CLog::Log(LOGNOTICE, "ReTV: Couldn't contact Validation URL");
		return "{\"errorcode\": 0 } ";
	}

	CLog::Log(LOGNOTICE, "ReTV: Validation response : %s", content.c_str());

	// We need to parse this and create our Registration file
	// Parse the response
	int result = parseValidationResponse(content);

	return content.c_str();
}

std::string ReTV::login(const char* mobileNumber)
{
	CLog::Log(LOGNOTICE, "Called Login");

	if (!m_initialized)
		return "{\"successcode\": 700 } ";

	m_mobileNumber = mobileNumber;

	XFILE::CCurlFile http;

	if (!http.IsInternet())
		return "{\"errorcode\": 0 } ";

	http.SetRequestHeader("Authorization", m_headerAuthorization);
	http.SetRequestHeader("Content-Type", m_headerContentType);
	
	std::string postData = getLoginJSON();

	std::string content;

	CLog::Log(LOGNOTICE, "Post Data : %s",postData.c_str());
	CLog::Log(LOGNOTICE, "URL : %s", makeApiURL(m_api_Login).c_str());
	if (!http.Post(makeApiURL(m_api_Login), postData, content, true)){
		CLog::Log(LOGNOTICE, "ReTV: Couldn't login");
		return "{\"errorcode\": 0 } ";
	}

	CLog::Log(LOGNOTICE, "ReTV: Login Response : %s",content.c_str());


	return parseLoginResponse(content);
	
}

std::string ReTV::parseLoginResponse(std::string loginResponseString)
{
	// Parse the response
	CVariant loginResponse = CJSONVariantParser::Parse((const unsigned char *)loginResponseString.c_str(), loginResponseString.size());

	if (!loginResponse.isObject()){
		CLog::Log(LOGNOTICE, "Couldn't parse response");
		return "{\"successcode\": 210 } ";
	}

	if (!loginResponse.isMember("successcode")){
		CLog::Log(LOGNOTICE, "Success code not found");
		return "{\"successcode\": 210 } ";
	}

	int successCode = loginResponse["successcode"].asInteger();

	if (successCode != 100){
		CLog::Log(LOGNOTICE, "Success code not proper");
		return loginResponseString;
	}

	CLog::Log(LOGNOTICE, "Success code is proper");

	m_authToken = loginResponse["authtoken"].asString();
	loginResponse["authtoken"].clear();	// Don't send this back to python

	CVariant data = loginResponse["data"];
	CVariant account = data["account"];
	CVariant accountData = account[m_deviceCode];

	CLog::Log(LOGNOTICE, "Types : %d - %d - %d", accountData.type(), account.type(), data["account"].type());


	CLog::Log(LOGNOTICE, "Device Code: %d", account.isMember(m_deviceCode));
	CLog::Log(LOGNOTICE, "Plan Name: %d", accountData.isMember("planname"));


	m_expiryTime = loginResponse["expiry"].asInteger();
	//m_subInfo.m_subscriptionEnd = expiry;


	m_loginTime = loginResponse["time"].asDouble();

	//CVariant userData = data["user"];

	//m_subInfo.setSubData(accountData["planname"].asString(), accountData["plancode"].asString(), accountData["startdate"].asInteger(), accountData["enddate"].asInteger(), accountData["fastforward"].asDouble());

	if (loginResponse.isMember("us")){
		ReTV::m_updateRepoUsername = loginResponse["us"].asString();
		loginResponse["us"].clear(); // Don't send this back to python
	}

	if (loginResponse.isMember("up")){
		ReTV::m_updateRepoPassword = loginResponse["up"].asString();
		loginResponse["us"].clear(); // Don't send this back to python
	}


#if defined(TARGET_ANDROID)
	// Initialize java rpc, only for Android
	initRPC();
#endif

	m_loggedIn = true;

	//CLog::Log(LOGNOTICE, "Sub details : %s - %s ", m_planName, m_planCode);
	CLog::Log(LOGNOTICE, "Sub details : %s - %s -%s", m_userNameF.c_str(), m_userNameL.c_str(), m_userEmail.c_str());

	return CJSONVariantWriter::Write(loginResponse,true);
}


int ReTV::parseValidationResponse(std::string validationResponseString)
{
	// We need to parse this and create our Registration file
	// Parse the response
	CVariant validationResponse = CJSONVariantParser::Parse((const unsigned char *)validationResponseString.c_str(), validationResponseString.size());


	if (!validationResponse.isObject()){
		CLog::Log(LOGNOTICE, "Couldn't parse response");
		// API_NO_ACCESS CODE
		return 202;
	}

	if (!validationResponse.isMember("successcode")){
		CLog::Log(LOGNOTICE, "Success code not found");
		// API_NO_ACCESS CODE
		return 202;
	}

	int successCode = validationResponse["successcode"].asInteger();

	if (successCode != 100){
		CLog::Log(LOGNOTICE, "Success code not proper");
		return successCode;
	}

	CLog::Log(LOGNOTICE, "Success code is proper");


	// Validated & Activated
	m_isActivated = true;
	return 100;

}


#if defined(TARGET_ANDROID)
bool ReTV::initRPC(){
	if (m_platformInfo.m_platformId != ReTVPlatform::ReTVDevice)
		return false;
    std::stringstream rpc_payload;
    std::string content;
    XFILE::CCurlFile http;
    rpc_payload << "{\"jsonrpc\": \"2.0\", \"params\": [\"" << m_deviceCode <<"\", \""<< m_authToken <<"\", "<< (int)m_loginTime <<", " << api_type << "], \"method\": \"initAPI\"}";
    return http.Post(rpc_url, rpc_payload.str(), content, true);
}
#endif

std::string ReTV::callAPI(const char* endPoint, const char* postVars, int customTimeout)
{
	if (strlen(endPoint) < 1)
		return "";

	// 1. Make API URL
	// 2. Call API URL
	// 3. 

	if (!m_initialized)
		return "";

	XFILE::CCurlFile http;

	if (!http.IsInternet())
		return "{\"message\":\"\",\"errorcode\":0,\"data\":\"\"}";

	http.SetRequestHeader("Authorization", m_headerAuthorization);
	http.SetRequestHeader("Content-Type", m_headerContentType);

	std::string postData = "{\"authtoken\": \"" + m_authToken + "\", \"devicecode\":\"" + m_deviceCode + "\"";

	if (strlen(postVars) > 0)

		postData = postData + ", " + postVars + "}";
	else
		postData = postData + "}";

	std::string content;

	CLog::Log(LOGNOTICE, "URL : %s", makeApiURL(endPoint).c_str());
	if (!http.Post(makeApiURL(endPoint), postData, content, true, customTimeout)){
        CLog::Log(LOGNOTICE, "ReTV: Couldn't make API Request");
        CLog::Log(LOGNOTICE, "Response Code: %d", http.responseCode);
        std::stringstream strstream;
        strstream << "{\"message\":\"\",\"errorcode\":" << http.responseCode << ",\"data\":\"\"}";
        return strstream.str();
    }
    CLog::Log(LOGNOTICE, "Response Code: %d", http.responseCode);

	return content;
}

std::string ReTV::callMediaAPI(const char* endPoint, const char* postVars, int customTimeout)
{
    if (strlen(endPoint) < 1)
        return "";

    // 1. Make API URL
    // 2. Call API URL
    // 3.

    if (!m_initialized)
        return "";

    XFILE::CCurlFile http;

    if (!http.IsInternet())
        return "{\"message\":\"\",\"errorcode\":0,\"data\":\"\"}";

    http.SetRequestHeader("Authorization", m_headerAuthorization);
    http.SetRequestHeader("Content-Type", m_headerContentType);

    std::string postData = "{\"authtoken\": \"" + m_authToken + "\", \"devicecode\":\"" + m_deviceCode + "\"";

    if (strlen(postVars) > 0)
        postData = postData + ", " + postVars + "}";
    else
        postData = postData + "}";

    std::string content;

    CLog::Log(LOGNOTICE, "URL : %s", makeMediaApiURL(endPoint).c_str());
	if (!http.Post(makeMediaApiURL(endPoint), postData, content, true, customTimeout)){
        CLog::Log(LOGNOTICE, "ReTV: Couldn't make API Request");
        std::stringstream strstream;
		strstream << "{\"message\":\"\",\"errorcode\":" << http.responseCode << ",\"data\":\"\"}";
        return strstream.str();
    }

    return content;
}


int ReTV::getSubEndTime()
{
	return m_subscriptionEnd;
}

float ReTV::getFFData()
{
	return m_fastForwardData;
}

bool ReTV::isLoggedIn()
{
    time_t current_time = std::time(0);

    CLog::Log(LOGNOTICE, "Checking if user is logged in.");
    if ( !m_authToken.empty() && current_time < m_expiryTime)
    {
        CLog::Log(LOGNOTICE, "User is logged in");
        return true;
    }
    CLog::Log(LOGNOTICE, "Not logged in or token expired.");
    return false;
}

std::string ReTV::getLinkByToken(const char* token)
{
    std::string url;
    if (api_type == API_TYPE_LIVE)
        url = m_mediaUrlLive;
    else
        url = m_mediaUrlStaging;

    url.replace(6, 1, "/" + api_username + ":" + api_password + "@");
    return url + "stream/?token=" + token;
}

std::string ReTV::getBaseUrl()
{
    if (api_type == API_TYPE_LIVE)
        return m_apiUrlLive;
    return m_apiUrlStaging;
}

SubscriptionInfo* ReTV::getSubscriptionInfo()
{
	return &m_subInfo;
}

void Cleanup();


/* Reads initial Subscription info */
void ReTV::readSubscriptionInfo()
{
	m_isActivated = false;
	/*
	// Read in the subscription data
	CReTVDatabase database;
	database.Open();

	m_isActivated = database.hasRegistrationData();

	if (!m_isActivated){
		CLog::Log(LOGNOTICE, "Registration table found, but no data in it");

		return;
	}

	m_isActivated = database.readSubscriptionData(m_subInfo);

	if (!m_isActivated){
		CLog::Log(LOGNOTICE, "Error reading subscription data");

		return;
	}

	this->m_mobileNumber = m_subInfo.getMobileNumber();
	
	database.Close();
	*/
}



/* Reads platform information*/
void ReTV::readPlatformInfo()
{
	// Get App Version
	m_platformInfo.m_versionCore = g_sysinfo.GetVersion();

	// Get Addon & Skin Version
	CAddonDatabase database;
	database.Open();

	m_platformInfo.m_versionScript	= database.GetAddonVersion(c_retvScript).first.Upstream();
	m_platformInfo.m_versionSkin	= database.GetAddonVersion(c_retvSkin).first.Upstream();
	database.Close();

	m_platformInfo.m_os = g_sysinfo.GetBuildTargetPlatformName();
	
	m_platformInfo.m_osVersion = g_sysinfo.GetOsVersion();

	m_platformInfo.m_manufacturer = g_sysinfo.GetManufacturerName();

	m_platformInfo.m_model = g_sysinfo.GetModelName();

	m_platformInfo.m_deviceName = g_sysinfo.GetDeviceName();

	m_platformInfo.m_arch = g_sysinfo.GetKernelCpuFamily();

	m_platformInfo.m_bitness = g_sysinfo.GetKernelBitness();

#if defined(TARGET_FREEBSD)
	m_platformInfo.m_platformId = ReTVPlatform::FreeBSD;
#elif defined(TARGET_DARWIN_IOS)
	m_platformInfo.m_platformId = ReTVPlatform::iOS;
#elif defined(TARGET_DARWIN_OSX)
	m_platformInfo.m_platformId = ReTVPlatform::OSX;
#elif defined (TARGET_ANDROID)
	if (m_platformInfo.m_manufacturer == c_retvManufacturer)
		m_platformInfo.m_platformId = ReTVPlatform::ReTVDevice;
	else
		m_platformInfo.m_platformId = ReTVPlatform::Android;
#elif defined(TARGET_LINUX)
	m_platformInfo.m_platformId = ReTVPlatform::Linux;
#elif defined (TARGET_WINDOWS)
	m_platformInfo.m_platformId = ReTVPlatform::Windows;
#else
	m_platformInfo.m_platformId = ReTVPlatform::Others;
#endif

	// Force Device ID if set in AdvancedSettings
	if (!g_advancedSettings.m_forcedDeviceId.empty() ){
		m_platformInfo.m_macAddress = g_advancedSettings.m_forcedDeviceId;
		m_deviceCode = g_advancedSettings.m_forcedDeviceId;
		return;
	}

	XBMC_TRACE;

	// THe code below doesnt work if the Interface is not connected

	// Always get the 1st connected Interface
	// What happens when there is not interface
	// Or what happens when another interface is added
	// On all devices, except ReTV, this shouldn' be a problem
	// But on ReTV, we might need to activate this again
	/*CNetworkInterface* iface = g_application.getNetwork().GetFirstConnectedInterface();
	if (iface)
		//return iface->GetCurrentIPAddress();
		m_platformInfo.m_macAddress = iface->GetMacAddress();
	else
		m_platformInfo.m_macAddress = "";

	StringUtils::ToUpper(m_platformInfo.m_macAddress);

	m_deviceCode = m_platformInfo.m_macAddress;

	CLog::Log(LOGNOTICE, "Device Code : %s", m_deviceCode);*/

	// We rather get the 1st Interface in the list
	// We cannot register if there are no network interfaces on this device
	// If the 1st interface is disabled, then this approach won't work
	// We need to inform the user that due to some hardware change the IDs have changed

	std::vector<CNetworkInterface*> list = g_application.getNetwork().GetInterfaceList();

	if (list.size() > 0){

		m_platformInfo.m_macAddress = list[0]->GetMacAddress();
		StringUtils::ToUpper(m_platformInfo.m_macAddress);
	}
	else{
		m_platformInfo.m_macAddress = "";
	}

	m_deviceCode = m_platformInfo.m_macAddress;

	CLog::Log(LOGNOTICE, "Device Code : %s", m_deviceCode.c_str()); 

	/*std::vector<CNetworkInterface*>::iterator it;

	int i = 0;

	for (it = list.begin(); it < list.end(); it++, i++) {
		
		CNetworkInterface* a = *it;
		CLog::Log(LOGNOTICE, "Attached Networks : %d - %s", i, a->GetName());
	}*/

}

int ReTV::getPlatform()
{
	return m_platformInfo.m_platformId;
}

/*Gets the Login JSON as string*/
std::string ReTV::getLoginJSON()
{
	std::string loginJSON = "{";

	loginJSON += "  \"mobile\": \"" + m_mobileNumber + "\"";
	loginJSON += ", \"devicecode\":\"" + m_deviceCode + "\"";

	/*loginJSON += ", \"core_sw_ver\":\"" + m_platformInfo.m_versionCore + "\"";
	loginJSON += ", \"script_sw_ver\":\"" + m_platformInfo.m_versionScript + "\"";
	loginJSON += ", \"skin_sw_ver\":\"" + m_platformInfo.m_versionSkin + "\"";*/

	loginJSON += "}";

	return 	loginJSON;
	;
}

/*Gets the Activation JSON as string*/
std::string ReTV::getDeviceActivationJSON()
{
	// Form the JSON here
	// from the Platform Info
	std::string activationJSON = "{";
	
	activationJSON += "  \"mobile\": \""		+ m_mobileNumber + "\"";
	activationJSON += ", \"devicecode\":\""		+ m_platformInfo.m_macAddress + "\"";
    
    std::stringstream ss;
    ss << m_platformInfo.m_platformId;
	activationJSON += ", \"id\":\""				+ ss.str() + "\"";
	activationJSON += ", \"arch\":\""			+ m_platformInfo.m_arch + "\"";
    
    ss << m_platformInfo.m_bitness;
	activationJSON += ", \"bitness\":\""		+ ss.str() + "\"";
	activationJSON += ", \"os\":\""				+ m_platformInfo.m_os + "\"";
	activationJSON += ", \"os_version\":\""		+ m_platformInfo.m_osVersion + "\"";
	activationJSON += ", \"manufacturer\":\""	+ m_platformInfo.m_manufacturer + "\"";
	activationJSON += ", \"brand\":\""			+ m_platformInfo.m_model + "\"";
	activationJSON += ", \"device_model\":\""	+ m_platformInfo.m_model + "\"";
	activationJSON += ", \"device_name\":\""	+ m_platformInfo.m_deviceName + "\"";

	activationJSON += ", \"core_sw_ver\":\"" + m_platformInfo.m_versionCore + "\"";
	activationJSON += ", \"script_sw_ver\":\"" + m_platformInfo.m_versionScript + "\"";
	activationJSON += ", \"skin_sw_ver\":\"" + m_platformInfo.m_versionSkin + "\"";

	activationJSON += "}";

	return activationJSON;
}


std::string ReTV::makeAuthenticatedUrl(std::string url)
{
	size_t iPos = url.find("://");

	// No http header found, directly add our authentication values
	if (iPos == std::string::npos)
	{
		return m_updateRepoUsername + ":" + m_updateRepoUsername + "@" + url;
	}
	else{

		std::string authUrl  = url.substr(0, iPos + 3); // Makes http(s)://

		authUrl				+= m_updateRepoUsername + ":" + m_updateRepoUsername + "@";
		authUrl				+= url.substr(iPos + 3);

		return authUrl;
	}
}
