# Sample correlator plugins using the C++ Plugin Development Kit


## DESCRIPTION

   Library source and associated EPL programs to demonstrate the
   development of Correlator plugins using C++.
   
   
   Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 


## FILES

  Sample files are:

    README.md                     This file
    primitives_plugin.cpp          Source for the primitives plugin example, using the C++ API
    primitives_plugin.mon          EPL code for the C++ primitives plugin example
    complex_plugin.cpp             EPL for the complex plugin example, using the C++ API
    complex_plugin.mon             EPL code for the C++ complex plugin example
    events_plugin.cpp              Source for the events plugin example, using the C++ API
    events_plugin.mon              EPL code for the C++ events plugin example
    exception_plugin.cpp           Source for the exception plugin example, using the C++ API
    exception_plugin.mon           EPL code for the C++ exception plugin example
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
   
   Source the apama_env file located in the installation bin directory to set 
   the environment variables required for building sample. Run make in the 
   current directory:  

      $ make

   If the C++ compiler is not in the default location assumed by the Makefile,
   this can be overidden by setting the CXX variable on the make command line.
   For example:

      $ make CXX=/usr/bin/gcc-4.8.5/g++

   would select a GNU C++ compiler installed under /usr/bin/gcc-4.8.5

   A successful build will produce four output files:

      libprimitives_plugin.so
      libcomplex_plugin.so
      libevents_plugin.so
      libexception_plugin.so

  
## RUNNING THE SAMPLES

   Running of the samples requires access to the Correlator and Apama command 
   line tools. 
   
   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 

   1. The Correlator can only load native plug-ins from directories in the 
      PATH/LD_LIBRARY_PATH environment of the correlator, such as the 
      APAMA_WORK/lib directory. So to make the plug-in binaries from this sample 
      available to the correlator, copy them to this location:   
      
      > cp *.so "$APAMA_WORK/lib/"
         
   2. Start the Apama Correlator:

      > correlator     
      
   2. Inject the primitives_plugin test EPL:
   
      > engine_inject primitives_plugin.mon
      
   3. Repeat the previous step with the other plugins. Remember that the test
      code will only inject correctly if the corresponding plugin was actually
      built!

      
## SAMPLE OUTPUT

   primitives_plugin.mon should produce output similar to the file primitives_plugin_sample.txt

   complex_plugin.mon should produce output similar to the file complex_plugin_sample.txt

   events_plugin.mon should produce output similar to the file events_plugin_sample.txt

   exception_plugin.mon should produce output similar to the file exception_plugin_sample.txt

