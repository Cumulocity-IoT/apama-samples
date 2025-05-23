#
# Makefile
# 
# GNU Make compatible makefile for the example plugins.
#
# $Copyright (c) 2018-2019, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
# 


# ===========================================================================
# Apama installation root -- modify or override on command line as necessary
# ===========================================================================

ifndef APAMA_HOME
home_not_set:
	@echo Please set the APAMA_HOME environment variable
endif


# What OS are we running?
OS := $(shell uname)


ifeq ($(OS), Linux)

CXX := /usr/bin/g++


#DEBUG_FLAGS := -ggdb -Wall -Werror -pedantic
DEBUG_FLAGS := -O3 -Wall -Werror -pedantic
CXXFLAGS := -fPIC -std=c++2a
LDFLAGS = -shared -Wl,-soname=$(@F)
LIBS :=
DEFINES := -D__linux__ -D__unix__ -D__OSVERSION__=2 -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS

endif  # Linux

ARCH := $(shell uname -m)

ifeq ($(ARCH), x86_64)
DEFINES := $(DEFINES) -D__x86_64__ -D__i86__
endif # x86_64

ifeq ($(ARCH), armv7l)
DEFINES := $(DEFINES) -D__arm__ -mfloat-abi=hard
endif # armv7l


# Include paths
CPPFLAGS += -I. -I"$(APAMA_HOME)/include"

# Compiler flags
CXXFLAGS += $(DEBUG_FLAGS) $(DEFINES)

# Linker flags -- used when building executables
LDFLAGS += "-L$(APAMA_HOME)/lib" -lapclient



# =============================
# Plugin library build targets
# =============================

ALL := Release/libPrometheusSample.so

all: $(ALL)



# -----------------------
# Simple plugin, C++ API
# -----------------------
Release/libPrometheusSample.so: Release/int/PrometheusSample.o
	mkdir -p Release
	$(CXX) $(LDFLAGS) $(CXX_LDFLAGS) -o $@ $^ $(LIBS)

Release/int/PrometheusSample.o: SampleTransport.cpp
	mkdir -p Release/int
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<


# ========
# Cleanup
# ========

RM := rm -f

clean:
	-$(RM) Release
