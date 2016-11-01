/*
*      Copyright (C) 2005-2013 Billion Plus Computing Private Limited
*      http://retv.in
*
*
*/

#include "AddonClass.h"
#include "AddonString.h"

#pragma once

namespace XBMCAddon
{
	namespace retv
	{
		/**
		* Subscription Info class to hold User Subscription data.\n
		*/
		class SubscriptionInfo : public AddonClass
		{
		private:

			std::string m_planName;
			std::string m_planCode;
			int m_subscriptionStart;
			int m_subscriptionEnd;
			double m_fastForwardData;

			std::string m_userNameF;
			std::string m_userNameL;
			std::string m_userAddress;
			std::string m_userCountry;
			std::string m_userCity;
			std::string m_userEmail;
			std::string m_userPinCode;

			std::string m_mobileNumber;


		public:
			SubscriptionInfo();
			virtual ~SubscriptionInfo();

			void setPhone(std::string phone);
			void setSubData(std::string planName, std::string planCode, int subStart, int subEnd, int validity, double ffData);
			void setUserData(std::string fname, std::string lname, std::string email, std::string address, std::string city, std::string country );

			/**
			* getURL() -- returns a string.\n
			*/
			String getFirstName();
			/**
			* getTitle() -- returns a string.\n
			*/
			String getLastName();
			/**
			* getArtist() -- returns a string.\n
			*/
			String getName();
			/**
			* getAlbum() -- returns a string.\n
			*/
			float getFastForward();
			/**
			* getAlbumArtist() -- returns a string.\n
			*/
			int getSubscriptionEnd();
			/**
			* getGenre() -- returns a string.\n
			*/
			String getPlanName();
			/**
			* getDuration() -- returns an integer.\n
			*/
			String getPlanCode();
			/**
			* getTrack() -- returns an integer.\n
			*/
			String getMobileNumber();


		};
	}
}

