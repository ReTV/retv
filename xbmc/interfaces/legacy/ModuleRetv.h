/*
*      Copyright (C) 2016 Billion Plus Computing Pvt. Ltd.
*      http://retv.in
*
*/


#include "AddonString.h"
#include "swighelper.h"
#include "SubscriptionInfo.h"

namespace XBMCAddon
{
  namespace retv
  {
#ifndef SWIG
	  // This is a bit of a hack to get around a SWIG problem
	  extern const int lLOGNOTICE;
#endif

    /**
     * test(url) -- Test method that returns the supplied string
     * 
     * url         : string - any string\n
     * 
     */
    String test(const String& url);


	/**
	* initialize(type, deviceCode, mobileNumber) -- Initializes ReTV
	*
	* url         : string - any string\n
	*
	*/
	bool initialize(int type, const char* deviceCode, const char* mobileNumber);


	bool login();

	String callAPI(const char* endPoint, const char* postData);
	String callMediaAPI(const char* endPoint, const char* postData);

	int getSubscriptionEndTime();
	
	float getFastForwardData();

	//XBMCAddon::retv::SubscriptionInfo* getLoginInfo();
  }
}
