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
#include <sstream>
#include <string.h>

#if defined(TARGET_ANDROID)
#include "platform/android/activity/XBMCApp.h"
#include "filesystem/File.h"
#endif

using namespace XFILE;

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
		return g_retv.secureCheck();
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
	void initAPI(std::string authtoken, unsigned int expiry, const char* username, const char* password){
		return g_retv.initAPI(authtoken, expiry, username, password);
	}
    
    String getDownloadDir()
    {
        std::string path = "";
#if defined(TARGET_ANDROID)
        
        if (!CXBMCApp::GetExternalStorage(path, "downloads") || path.empty() || !CFile::Exists(path))
        {
          path = "";
        }
#endif
        return path;
    }

	void setFallbackDeviceCode(std::string fallbackCode)
	{
		g_retv.setFallbackDeviceCode(fallbackCode);
	}
    
    String getMacAddress(){
        return g_retv.getPlatformInfo().m_macAddress;
    }
    
    std::vector<String> getPlatformInfo(){
        
        std::vector<String> infoToSend;
        
        ReTVPlatformInfo info = g_retv.getPlatformInfo();
        
        /*
        ReTVPlatform m_platformId;
        std::string m_arch;
        int m_bitness;
        std::string m_os;
        std::string m_osVersion;
        std::string m_manufacturer;
        std::string m_channel;
        std::string m_brand;
        std::string m_model;
        std::string m_product;
        std::string m_deviceName;
        std::string m_macAddress;
        std::string m_versionCore;
        std::string m_versionScript;
        std::string m_versionSkin;
        */
        std::stringstream id;
        id << info.m_platformId;
        infoToSend.push_back(id.str());
        
        infoToSend.push_back(info.m_arch);
        
        std::stringstream bit;
        bit << info.m_bitness;
        infoToSend.push_back(bit.str());
        
        infoToSend.push_back(info.m_arch);
        infoToSend.push_back(info.m_os);
        infoToSend.push_back(info.m_osVersion);
        infoToSend.push_back(info.m_manufacturer);
        infoToSend.push_back(info.m_channel);
        infoToSend.push_back(info.m_brand);
        infoToSend.push_back(info.m_model);
        infoToSend.push_back(info.m_product);
        infoToSend.push_back(info.m_deviceName);
        infoToSend.push_back(info.m_macAddress);
        infoToSend.push_back(info.m_versionCore);
        infoToSend.push_back(info.m_versionScript);
        infoToSend.push_back(info.m_versionSkin);
        
        return infoToSend;
    }
    
  }
}
