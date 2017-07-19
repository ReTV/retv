-include ../../Makefile.include

VERSION.TXT := $(XBMCROOT)/version.txt
APP_NAME=$(shell awk '/APP_NAME/ {print tolower($$2)}' $(VERSION.TXT))

SOURCE=../../../../


export CXXFLAGS+=-O3
export CFLAGS+=-O3

# configuration settings
CONFIGURE = cp -f $(CONFIG_SUB) $(CONFIG_GUESS) build-aux/ ;\
  ./configure --prefix=$(PREFIX) $(DEBUG)

# tvOS build?
ifeq ($(platform),tvos)
$(info Platform - tvOS )
export CXXFLAGS+=-DPF_TVOS
endif

ifeq ($(OS),android)
CONFIGURE += --enable-codec=amcodec --enable-breakpad --disable-libcec

# Locking to a Manufacturer
ifeq ($(lock),)
$(info No Lock defined)
else
$(info Locking to manufacturer - $(lock) )
export CXXFLAGS+=-DRETV_ANDROID_LOCK="\"$(lock)"\"
endif

# Android TV build
ifeq ($(platform),androidtv)
$(info Platform - Android TV )
export CXXFLAGS+=-DPF_ANDROID_TV
endif

# Generic Android Box build
ifeq ($(platform),androidbox)
$(info Platform - Android Box )
export CXXFLAGS+=-DPF_ANDROID_BOX
endif

endif

# Acquisition Channel
ifeq ($(channel),)
$(info No Channel defined)
else
$(info App Channel - $(channel) )
export CXXFLAGS+=-DAPP_CHANNEL="\"$(channel)"\"
endif

# Secure APK
ifeq ($(secure),)
$(info Open Build)
else
$(info Secure Build for ISPs - $(secure) )
export CXXFLAGS+=-DSECURE_BUILD
endif

ifeq ($(Configuration),Release)
DEBUG = --enable-debug=no
endif

CONFIGURE += $(CONFIG_EXTRA)

all: DEBUG=--enable-debug=no
all: $(SOURCE)/lib$(APP_NAME).so

release: DEBUG=--enable-debug=no
release: $(SOURCE)/lib$(APP_NAME).so

debug: DEBUG=--enable-debug=yes
debug: $(SOURCE)/lib$(APP_NAME).so

$(SOURCE)/lib$(APP_NAME).so:
	cd $(SOURCE); BOOTSTRAP_FROM_DEPENDS=yes ./bootstrap
	cd $(SOURCE); echo $(CONFIGURE)

../../Makefile.include:
	$(error Please run configure)

clean:
	cd $(SOURCE); $(MAKE) clean

distclean:
	cd $(SOURCE); $(MAKE) clean
