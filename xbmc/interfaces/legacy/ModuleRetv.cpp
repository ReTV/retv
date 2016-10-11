/*
*      Copyright (C) 2016 Billion Plus Computing Pvt. Ltd.
*      http://retv.in
*
*/

#include "ModuleRetv.h"

#include "filesystem/PluginDirectory.h"
#include "FileItem.h"
#include "retv/ReTV.h"
#include "SubscriptionInfo.h"


namespace XBMCAddon
{

  namespace retv
  {
    String test(const String& url)
    {
		return url;
    }



	bool initialize(int type, const char* deviceCode, const char* mobileNumber)
	{
		return ReTV::GetInstance().initialize(type, deviceCode, mobileNumber);
	}


	bool login()
	{
		return ReTV::GetInstance().login();
	}

	String callAPI(const char* endPoint, const char* postData)
	{
		return ReTV::GetInstance().callAPI(endPoint, postData);
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
		return ReTV::GetInstance().getSubEndTime();
	}

	float getFastForwardData()
	{
		return ReTV::GetInstance().getFFData();
	}

	XBMCAddon::retv::SubscriptionInfo* getLoginInfo()
	{
		//return ReTV::GetInstance().getSubscriptionInfo();
		return &SubscriptionInfo();
	}

  }
}
