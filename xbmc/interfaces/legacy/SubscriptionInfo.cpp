/*
*      Copyright (C) 2005-2013 Billion Plus Computing Private Limited
*      http://retv.in
*
*
*/

#include "AddonClass.h"
#include "SubscriptionInfo.h"

namespace XBMCAddon
{
	namespace retv
	{
		SubscriptionInfo::SubscriptionInfo()
		{
			m_userNameF = "Test User";
			m_planName = "Test plan";
		}

		SubscriptionInfo::~SubscriptionInfo()
		{
			
		}

		String SubscriptionInfo::getFirstName()
		{
			return m_userNameF;
		}
		/**
		* getLastName() -- returns a string.\n
		*/
		String SubscriptionInfo::getLastName()
		{
			return m_userNameL;
		}
		/**
		* getName() -- returns a string.\n
		*/
		String SubscriptionInfo::getName()
		{
			return m_userNameF+" "+ m_userNameL;
		}
		/**
		* getFastForward() -- returns a string.\n
		*/
		float SubscriptionInfo::getFastForward()
		{
			return m_fastForwardData;
		}
		/**
		* getSubscriptionEnd() -- returns a string.\n
		*/
		int SubscriptionInfo::getSubscriptionEnd()
		{
			return m_subscriptionEnd;
		}
		/**
		* getPlanName() -- returns a string.\n
		*/
		String SubscriptionInfo::getPlanName()
		{
			return m_planName;
		}
		/**
		* getPlanCode() -- returns an integer.\n
		*/
		String SubscriptionInfo::getPlanCode()
		{
			return m_planCode;
		}
		/**
		* getMobileNumber() -- returns an integer.\n
		*/
		String SubscriptionInfo::getMobileNumber()
		{
			return m_mobileNumber;
		}

		void SubscriptionInfo::setPhone(std::string phone){}
		void SubscriptionInfo::setSubData(std::string planName, std::string planCode, int subStart, int subEnd, int validity, double ffData){}
		void SubscriptionInfo::setUserData(std::string fname, std::string lname, std::string email, std::string address, std::string city, std::string country){}
	}
}

