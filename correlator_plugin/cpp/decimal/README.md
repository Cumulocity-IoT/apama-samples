# Sample decimal correlator plugins using the C++ Plugin Development Kit


## DESCRIPTION

   Library source and associated EPL programs to demonstrate the
   development of Correlator plugins using C++ and the Intel Decimal FP
   library (IEEE 754-2008 decimal).
   
   
## COPYRIGHT NOTICE

   Copyright (c) 2016-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

## FILES

  Sample contains:

    README.md                      This file
    decimal_plugin.cpp             Source for the decimal example, using the C++ API
    decimal_plugin.mon             EPL code for the C++ decimal example
    Makefile                       GNU Make compatible makefile


## BUILDING THE SAMPLES

   It is recommended that you copy this sample folder to an area of your 
   APAMA_WORK directory rather than running it directly from the installation 
   directory. 

   The samples use the C++ plugin API and have only been built and run on the  
   platform/compiler combinations specified in the Apama supported platforms 
   list. Most other compilers are known not to work with this sample, due to 
   differences in C++ name mangling and runtime library implementations.
   
   GNU Make is required in order to build the samples using the supplied Makefile.

   This sample specifically shows the user how to use the decimal type from
   within a plugin.  The decimal format used is IEEE 754-2008 decimal of
   which the Intel Decimal Floating Point library is an implementation.
   
   This sample depends on the Intel Decimal Floating Point library
   which must be obtained separately.  It can be found here:
   https://www.intel.com/content/www/us/en/developer/articles/tool/intel-decimal-floating-point-math-library.html

   Change to the LIBRARY subdirectory of the Intel Decimal Floating Point library and 
   run the following command:

   $ make CC='gcc -m64 -fPIC' CALL_BY_REF=0 GLOBAL_RND=0 GLOBAL_FLAGS=0 UNCHANGED_BINARY_FLAGS=0

   Before building the sample, export 2 environment variables:
   
   * LIB_IDFP_INCLUDE - Which should be the path to the LIBRARY/src directory of the 
     downloaded Intel Decimal FP library.
   * LIB_IDFP_LIB_DIR - Which should be the path to the LIBRARY directory
     of the downloaded Intel Decimal FP library.
   
   Source the apama_env file located in the installation bin directory to set 
   the environment variables required for building sample. Run make in the 
   current directory:  

      $ make

   If the C++ compiler is not in the default location assumed by the Makefile,
   this can be overidden by setting the CXX variable on the make command line.
   For example:

      $ make CXX=/usr/bin/gcc-4.8.5/g++

   would select a GNU C++ compiler installed under /usr/bin/gcc-4.8.5

   A successful build will produce an output file:

      libdecimal_plugin.so

  
## RUNNING THE SAMPLES

   Running of the samples requires access to the Correlator and Apama command 
   line tools. 
   
   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been sourced. 

   1. The Correlator can only load native plug-ins from directories in the 
      PATH/LD_LIBRARY_PATH environment of the correlator, such as the 
      APAMA_WORK/lib directory. So to make the plug-in binaries from this sample 
      available to the correlator, copy them to this location:   
      
      > cp *.so "$APAMA_WORK/lib/"
         
   2. Start the Apama Correlator:

      > correlator
             
   3. Inject the simple_plugin test EPL code:
   
      > engine_inject decimal_plugin.mon

      
## SAMPLE OUTPUT

   decimal_plugin.mon should produce output similar to the file decimal_plugin_sample.txt


