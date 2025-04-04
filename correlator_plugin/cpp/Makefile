#
# Makefile
# 
# GNU Make compatible makefile for the example plugins.
#
#
# $Copyright(c) 2002, 2004-2006, 2009-2010 Progress Software Corporation (PSC). All rights reserved.$
# $Copyright (c) 2013, 2014, 2016-2017, 2019, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
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
CPPFLAGS += -I. -I"$(APAMA_HOME)/include"

# Compiler flags
CXXFLAGS += $(DEBUG_FLAGS) $(DEFINES) -std=c++2a

# Linker flags -- used when building executables
LDFLAGS += -L"$(APAMA_HOME)/lib" 



# =============================
# Plugin library build targets
# =============================

ALL := libprimitives_plugin.so libcomplex_plugin.so libevents_plugin.so libexception_plugin.so 

all: $(ALL)


# -----------------------
# Primitives plugin
# -----------------------
libprimitives_plugin.so: primitives_plugin.o
	$(CXX) $(LDFLAGS) $(CXX_LDFLAGS) -o $@ $^ $(LIBS)

primitives_plugin.o: primitives_plugin.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

# -----------------------
# Complex plugin
# -----------------------
libcomplex_plugin.so: complex_plugin.o
	$(CXX) $(LDFLAGS) $(CXX_LDFLAGS) -o $@ $^ $(LIBS)

complex_plugin.o: complex_plugin.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

# -----------------------
# Events plugin
# -----------------------
libevents_plugin.so: events_plugin.o
	$(CXX) $(LDFLAGS) $(CXX_LDFLAGS) -o $@ $^ $(LIBS)

events_plugin.o: events_plugin.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

# -----------------------
# Exception plugin
# -----------------------
libexception_plugin.so: exception_plugin.o
	$(CXX) $(LDFLAGS) $(CXX_LDFLAGS) -o $@ $^ $(LIBS)

exception_plugin.o: exception_plugin.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

# ========
# Cleanup
# ========

RM := rm -f

clean:
	-$(RM) *.o
	-$(RM) $(ALL)
