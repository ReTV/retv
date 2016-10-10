/*
*      Copyright (C) 2016 Billion Plus Computing Pvt. Ltd.
*      http://retv.in
*
*/


%module(directors="1") retv

%{
#include "interfaces/legacy/ModuleRetv.h"
#include "interfaces/legacy/SubscriptionInfo.h"

using namespace XBMCAddon;
using namespace retv;

#if defined(__GNUG__) && (__GNUC__>4) || (__GNUC__==4 && __GNUC_MINOR__>=2)
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

%}

%feature("knownbasetypes") XBMCAddon::retv "AddonClass"
%feature("knownapitypes") XBMCAddon::retv "XBMCAddon::retv::SubscriptionInfo"


%include "interfaces/legacy/swighelper.h"

%feature("director") SubscriptionInfo;

%include "interfaces/legacy/AddonString.h"
%include "interfaces/legacy/ModuleRetv.h"

%include "interfaces/legacy/SubscriptionInfo.h"
