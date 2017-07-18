/*
*      Copyright (C) 2016 Billion Plus Computing Pvt. Ltd.
*      http://retv.in
*
*/

#include "ModuleRetv.h"

#include "filesystem/PluginDirectory.h"
#include "FileItem.h"
#include "retv/ReTV.h"
#include "utils/log.h"
#include "SubscriptionInfo.h"
#include "utils/SystemInfo.h"


namespace XBMCAddon
{

  namespace retv
  {
    String test(const String& url)
    {
		return url;
    }

	int version()
	{
		return ReTV::RETV_VERSION;
	}

	bool isRegistered()
	{
		return g_retv.isActivated();
	}

	bool isSecure()
	{
		return ReTV::isSecure;
	}
	
	void secureCheck()
	{
		g_retv.secureCheck();
	}

	String registerDevice(const char* mobileNumber)
	{
		return g_retv.registerDevice(mobileNumber);
	}

	String validateNumber(const char* authCode)
	{
		return g_retv.validateNumber(authCode);
	}


	String login(const char* mobileNumber)
	{
		return g_retv.login(mobileNumber);
	}

	String callAPI(const char* endPoint, const char* postData, int timeout)
	{
		return g_retv.callAPI(endPoint, postData, timeout);
	}

    String callMediaAPI(const char* endPoint, const char* postData, int timeout)
    {
		return g_retv.callMediaAPI(endPoint, postData, timeout);
    }

	bool callDownloadAPI(const char* fileId, const char* filePath)
	{
		return g_retv.callDownloadAPI(fileId, filePath);
	}

    String getLinkByToken(const char* token)
    {
		return g_retv.getLinkByToken(token);
    }

    String getBaseUrl()
    {
		return g_retv.getBaseUrl();
    }

	/*Tuple<String,String> getLoginInfo()
	{
		std::string strTitle = "Hello";
		std::string strYear = "World";
		Tuple<String, String> subData = Tuple<String, String>(strTitle, strYear);

		return subData;
		/*
		subData->setProperty("loggedIn", "true");
		subData->setProperty("authToken", "sgdfgdfgghoshgrhgurhgiurehireg");

		subData->setProperty("sub_end", "20");
		subData->setProperty("ff", "30.03");

		return subData;*/
	//}

	int getSubscriptionEndTime()
	{
		return g_retv.getSubEndTime();
	}

	float getFastForwardData()
	{
		return g_retv.getFFData();
	}

    bool isLoggedIn()
    {
		return g_retv.isLoggedIn();
    }

	XBMCAddon::retv::SubscriptionInfo* getLoginInfo()
	{
		return g_retv.getSubscriptionInfo();
//		return &SubscriptionInfo();
	}

	bool hasInternet()
	{
		CLog::Log(LOGNOTICE, "Has Internet : %d", g_sysinfo.HasInternet());
		return g_sysinfo.HasInternet();
	}

	int getPlatform()
	{
		return g_retv.getPlatform();
	}

	void setTorrentProgress(int percent)
	{
		ReTV::torrentProgress = percent;
	}
	void initAPI(std::string authtoken, unsigned int expiry){
		return g_retv.initAPI(authtoken, expiry);
	}
  }
}
