-include ../../Makefile.include

VERSION.TXT := $(XBMCROOT)/version.txt
APP_NAME=$(shell awk '/APP_NAME/ {print tolower($$2)}' $(VERSION.TXT))

SOURCE=../../../../


export CXXFLAGS+=-O3
export CFLAGS+=-O3

# configuration settings
CONFIGURE = cp -f $(CONFIG_SUB) $(CONFIG_GUESS) build-aux/ ;\
  ./configure --prefix=$(PREFIX) $(DEBUG)

ifeq ($(OS),android)
CONFIGURE += --enable-codec=amcodec --enable-breakpad --disable-libcec

ifeq ($(lock),)
$(info No Lock defined)
else
$(info Locking to manufacturer - $(lock) )
export CXXFLAGS+=-DRETV_ANDROID_LOCK="\"$(lock)"\"
endif

endif

ifeq ($(channel),)
$(info No Channel defined)
else
$(info App Channel - $(channel) )
export CXXFLAGS+=-DAPP_CHANNEL="\"$(channel)"\"
endif

ifeq ($(Configuration),Release)
DEBUG = --enable-debug=no
endif

CONFIGURE += $(CONFIG_EXTRA)

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
