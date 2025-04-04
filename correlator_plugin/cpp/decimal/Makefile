#
# Makefile
# 
# GNU Make compatible makefile for the example plugins.
#
#
# $Copyright (c) 2016-2017, 2019, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
# 


# ===========================================================================
# Apama installation root -- modify or override on command line as necessary
# ===========================================================================

ifndef APAMA_HOME
home_not_set:
	@echo Please set the APAMA_HOME environment variable
endif

ifndef LIB_IDFP_INCLUDE
	LIB_IDFP_DEFINED=false
endif
ifndef LIB_IDFP_LIB_DIR
	LIB_IDFP_DEFINED=false
endif
ifdef LIB_IDFP_DEFINED
lib_idfp_not_set:
	@echo Cannot find required Intel Decimal FP library.
	@echo Please define LIB_IDFP_INCLUDE and LIB_IDFP_LIB_DIR
endif

# What OS are we running?
OS := $(shell uname)


ifeq ($(OS), Linux)

CXX := /usr/bin/g++


#DEBUG_FLAGS := -ggdb -Wall -Werror -pedantic
DEBUG_FLAGS := -O3 -Wall -Werror -pedantic
CXXFLAGS := -fPIC 
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
CPPFLAGS += -I. "-I$(APAMA_HOME)/include" "-I$(LIB_IDFP_INCLUDE)"

# Compiler flags
CXXFLAGS += $(DEBUG_FLAGS) $(DEFINES) -std=c++2a

# Linker flags -- used when building executables
LDFLAGS += "-L$(APAMA_HOME)/lib" "-L$(LIB_IDFP_LIB_DIR)" -lbid


# =============================
# Plugin library build targets
# =============================

ALL := libdecimal_plugin.so

all: $(ALL)

# ------------------------
# Decimal plugin, C++ API
# ------------------------
libdecimal_plugin.so: decimal_plugin.o
	$(CXX) $(LDFLAGS) $(CXX_LDFLAGS) -o $@ $^ $(LIBS)

decimal_plugin.o: decimal_plugin.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<


# ========
# Cleanup
# ========

RM := rm -f

clean:
	-$(RM) *.o
	-$(RM) $(ALL)
