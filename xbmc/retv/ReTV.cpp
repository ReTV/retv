/*
*      Copyright (C) 2016 Billion Plus Computing Pvt. Ltd.
*      http://retv.in
*
*/

#include <string.h>
#include "ReTV.h"
#include "filesystem/CurlFile.h"
#include "interfaces/AnnouncementManager.h"
#include "utils/log.h"
#include "utils/JSONVariantParser.h"
#include "utils/Variant.h"
#include "interfaces\legacy\SubscriptionInfo.h"

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

	CLog::Log(LOGNOTICE, "Post Data : %s",postData);
	CLog::Log(LOGNOTICE, "URL : %s", makeApiURL(m_api_Login));
	if (!http.Post(makeApiURL(m_api_Login), postData, content, true)){
		CLog::Log(LOGNOTICE, "ReTV: Couldn't login");
		return false;
	}


	// Parse the response
	CVariant loginResponse = CJSONVariantParser::Parse((const unsigned char *)content.c_str(), content.size());

	bool result = parseLoginResponse(loginResponse);

	if (!result)
		CLog::Log(LOGNOTICE, "ReTV: Login response : %s", content);

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


	int64_t expiry = loginResponse["expiry"].asInteger();
	//m_subInfo.m_subscriptionEnd = expiry;


	m_loginTime = loginResponse["time"].asDouble();

	CLog::Log(LOGNOTICE, "Sub details : %s - %ld - %f", m_authToken, (long)expiry, m_loginTime);

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


	m_loggedIn = true;

	//CLog::Log(LOGNOTICE, "Sub details : %s - %s ", m_planName, m_planCode);
	CLog::Log(LOGNOTICE, "Sub details : %s - %s -%s", m_userNameF, m_userNameL, m_userEmail);

	return true;
}




std::string ReTV::callAPI(const char* endPoint, const char* postVars)
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
		return "";

	http.SetRequestHeader("Authorization", m_headerAuthorization);
	http.SetRequestHeader("Content-Type", m_headerContentType);

	std::string postData = "{\"authtoken\": \"" + m_authToken + "\", \"devicecode\":\"" + m_deviceCode + "\"";

	if (strlen(postVars) > 0)
		postData = postData + ", " + postVars + "}";
	else
		postData = postData + "}";

	std::string content;

	CLog::Log(LOGNOTICE, "Post Data : %s", postData);
	CLog::Log(LOGNOTICE, "URL : %s", makeApiURL(endPoint));
	if (!http.Post(makeApiURL(endPoint), postData, content, true)){
		CLog::Log(LOGNOTICE, "ReTV: Couldn't make API Request");
		return "";
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


SubscriptionInfo* ReTV::getSubscriptionInfo()
{
	return &m_subInfo;
}

void Cleanup();