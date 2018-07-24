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
    const int REQUEST_TIMEOUT = 10;
    String test(const String& url);

	/**
	* version() -- Gets ReTV api/system version
	*/
	int version();

	/**
	* isRegistered() -- Checks whether we are registered with ReTV
	*/
	bool isRegistered();

	/**
	* isSecure() -- Checks whether system is secure, is being accessed from allowed network
	*/
	bool isSecure();

	/**
	* registerDevice(mobileNumber) -- Calls the Registration API for ReTV
	*
	* mobileNumber    : string - 10 digit mobile numer\n
	*
	*/
	String registerDevice(const char* mobileNumber);

    
    /**
	* registerDeviceCode(activationCode) -- Calls the Registration API for ReTV
	*
	* activationCode    : string - Activate Code\n
	*
	*/
	String registerDeviceCode(const char* activationCode);
    
	/**
	* validateNumber(mobileNumber, authCode) -- Calls the Verification API for ReTV
	*
	* authCode	      : string - Authcode received on the mobile number
	*
	*/
	String validateNumber(const char* authCode);



	String login(const char* mobileNumber);

	String callAPI(const char* endPoint, const char* postData, int timeout=REQUEST_TIMEOUT);
	String callMediaAPI(const char* endPoint, const char* postData, int timeout=REQUEST_TIMEOUT);
	bool callDownloadAPI(const char* fileId, const char* filePath);
    String getLinkByToken(const char* token);
    String getBaseUrl();
	void initAPI(std::string authtoken, unsigned int expiry, const char* username, const char* password);

    int getSubscriptionEndTime();
	
	float getFastForwardData();

    bool isLoggedIn();

	XBMCAddon::retv::SubscriptionInfo* getLoginInfo();

	bool hasInternet();

	int getPlatform();

	void setTorrentProgress(int percent);
    
    String getDownloadDir();

    String getDeviceId();
    
	void setFallbackDeviceCode(std::string fallbackCode);
  }
}
