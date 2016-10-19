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
#include "utils/Variant.h"
#include "interfaces/legacy/SubscriptionInfo.h"

using namespace XBMCAddon::retv;

ReTV& ReTV::GetInstance()
{
	static ReTV retv;
	return retv;
}


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


/*
 Initialize the ReTV system. Store user data. Will change later
*/
bool ReTV::initialize(int type, const char*  deviceCode, const char*  mobileNumber)
{
	CLog::Log(LOGNOTICE, "Initialize : %d - %s - %s", type, deviceCode, mobileNumber);

	// Live
	if (type == 0){
		m_apiUrl = m_apiUrlLive;
		m_mediaUrl = m_mediaUrlLive;
	}
	// Staging
	else{
		m_apiUrl = m_apiUrlStaging;
		m_mediaUrl = m_mediaUrlStaging;
	}

	this->m_deviceCode = deviceCode;
	this->m_mobileNumber = mobileNumber;

	//m_subInfo = SubscriptionInfo();
	//m_subInfo.setPhone(mobileNumber);

	m_initialized = true;

	// Done for now
	return true;
}

bool ReTV::login()
{
	CLog::Log(LOGNOTICE, "Called Login");
	if (!m_initialized)
		return false;

	XFILE::CCurlFile http;

	if (!http.IsInternet())
		return false;

	
	http.SetRequestHeader("Authorization", m_headerAuthorization);
	http.SetRequestHeader("Content-Type", m_headerContentType);
	
	std::string postData = "{\"mobile\": \"" + m_mobileNumber + "\", \"devicecode\":\"" + m_deviceCode + "\"}";
	//postData = "Hello World";
	std::string content;

	CLog::Log(LOGNOTICE, "Post Data : %s",postData.c_str());
	CLog::Log(LOGNOTICE, "URL : %s", makeApiURL(m_api_Login).c_str());
	if (!http.Post(makeApiURL(m_api_Login), postData, content, true)){
		CLog::Log(LOGNOTICE, "ReTV: Couldn't login");
		return false;
	}


	// Parse the response
	CVariant loginResponse = CJSONVariantParser::Parse((const unsigned char *)content.c_str(), content.size());

	bool result = parseLoginResponse(loginResponse);

	if (!result)
		CLog::Log(LOGNOTICE, "ReTV: Login response : %s", content.c_str());

	return result;
}

bool ReTV::parseLoginResponse(CVariant loginResponse)
{
	if (!loginResponse.isObject()){
		CLog::Log(LOGNOTICE, "Couldn't parse response");
		return false;
	}

	if (!loginResponse.isMember("successcode")){
		CLog::Log(LOGNOTICE, "Success code not found");
		return false;
	}


	int successCode = loginResponse["successcode"].asInteger();

	if (successCode != 100){
		CLog::Log(LOGNOTICE, "Success code not proper");
		return false;
	}

	CLog::Log(LOGNOTICE, "Success code is proper");

	m_authToken = loginResponse["authtoken"].asString();

	CVariant data = loginResponse["data"];
	CVariant account = data["account"];
	CVariant accountData = account[m_deviceCode];

	CLog::Log(LOGNOTICE, "Types : %d - %d - %d", accountData.type(), account.type(), data["account"].type());


	CLog::Log(LOGNOTICE, "Device Code: %d", account.isMember(m_deviceCode));
	CLog::Log(LOGNOTICE, "Plan Name: %d", accountData.isMember("planname"));


	m_expiryTime = loginResponse["expiry"].asInteger();
	//m_subInfo.m_subscriptionEnd = expiry;


	m_loginTime = loginResponse["time"].asDouble();

	CLog::Log(LOGNOTICE, "Sub details : %s - %d - %f", m_authToken.c_str(), m_expiryTime, m_loginTime);

	CVariant userData = data["user"];

	//m_subInfo.setSubData(accountData["planname"].asString(), accountData["plancode"].asString(), accountData["startdate"].asInteger(), accountData["enddate"].asInteger(), accountData["fastforward"].asDouble());
	/*m_subInfo.m_planName = accountData["planname"].asString();
	m_subInfo.m_planCode = accountData["plancode"].asString();

	m_subInfo.m_subscriptionStart = accountData["startdate"].asInteger();
	m_subInfo.m_subscriptionEnd = accountData["enddate"].asInteger();
	m_subInfo.m_fastForwardData = accountData["fastforward"].asDouble();*/
	
	//m_subInfo.setUserData(userData["firstname"].asString(), userData["lastname"].asString(), userData["address"].asString(), userData["country"].asString(), userData["city"].asString(), userData["email"].asString());
	/*m_subInfo.m_userNameF = userData["firstname"].asString();
	m_subInfo.m_userNameL = userData["lastname"].asString();
	m_subInfo.m_userAddress = userData["address"].asString();
	m_subInfo.m_userCountry = userData["country"].asString();
	m_subInfo.m_userCity = userData["city"].asString();
	m_subInfo.m_userEmail = userData["email"].asString();*/


    // Initialize java rpc
    initRPC();
	m_loggedIn = true;

	//CLog::Log(LOGNOTICE, "Sub details : %s - %s ", m_planName, m_planCode);
	CLog::Log(LOGNOTICE, "Sub details : %s - %s -%s", m_userNameF.c_str(), m_userNameL.c_str(), m_userEmail.c_str());

	return true;
}

bool ReTV::initRPC(){
    std::stringstream rpc_payload;
    std::string content;
    XFILE::CCurlFile http;
    rpc_payload << "{\"jsonrpc\": \"2.0\", \"params\": [\"" << m_deviceCode <<"\", \""<< m_authToken <<"\", "<< (int)m_loginTime <<"], \"method\": \"initAPI\"}";
    CLog::Log(LOGNOTICE, "RPC payload : %s", rpc_payload.str().c_str());
    return http.Post(rpc_url, rpc_payload.str(), content, true);
}


std::string ReTV::callAPI(const char* endPoint, const char* postVars, int timeout)
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

	CLog::Log(LOGNOTICE, "Post Data : %s", postData.c_str());
	CLog::Log(LOGNOTICE, "URL : %s", makeApiURL(endPoint).c_str());
	if (!http.Post(makeApiURL(endPoint), postData, content, true, timeout)){
        CLog::Log(LOGNOTICE, "ReTV: Couldn't make API Request");
        CLog::Log(LOGNOTICE, "Response Code: %d", http.responseCode);
        std::stringstream strstream;
        strstream << "{\"message\":\"\",\"errorcode\":" << http.responseCode << ",\"data\":\"\"}";
        return strstream.str();
    }
    CLog::Log(LOGNOTICE, "Response Code: %d", http.responseCode);

	return content;
}

std::string ReTV::callMediaAPI(const char* endPoint, const char* postVars, int timeout)
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

    CLog::Log(LOGNOTICE, "Post Data : %s", postData.c_str());
    CLog::Log(LOGNOTICE, "URL : %s", makeMediaApiURL(endPoint).c_str());
    if (!http.Post(makeMediaApiURL(endPoint), postData, content, true, timeout)){
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
    CLog::Log(LOGNOTICE, "Auth Token: %s \nCurrent Time: %ld \nToken Expiry: %ld", m_authToken.c_str(), (long)current_time, (long)m_expiryTime);
    if ( !m_authToken.empty() && current_time < m_expiryTime)
    {
        CLog::Log(LOGNOTICE, "User is logged in");
        return true;
    }
    CLog::Log(LOGNOTICE, "Not logged in or token expired.");
    return false;
}

SubscriptionInfo* ReTV::getSubscriptionInfo()
{
	return &m_subInfo;
}

void Cleanup();