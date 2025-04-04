# Sample HTTP Client transport using the C++ Plugin Development Kit


## DESCRIPTION

   A Connectivity transport which can send HTTP GET requests to websites and
   send back the contents of the page in a response. Packaged as a sample to
   demonstrate creating transports using the Connectivity Plugins C++ API.
   
   Copyright (c) 2016-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

## FILES

  Sample files are:

    README.md                             This file
    HTTPClient.cpp                        Source for the HTTPClient transport
    Makefile                              GNU Make compatible makefile
    DemoApp.mon                           An EPL program to use this transport
    httpclient-transport.yaml             Configuration file to load the transport
    httpclient.yaml                       End-to-end sample configuration
    httpclient.properties                 End-to-end sample configuration
    httpclient-feed.evt                   Sample input events
    sample_output.txt                     Sample output

## BUILDING THE SAMPLES

   It is recommended that you copy this sample folder to an area of your 
   APAMA_WORK directory rather than running it directly from the installation 
   directory. 

   GNU Make is required in order to build the samples using the supplied Makefile.

   Source the `bin/apama_env` file to set the environment variables required for 
   building the sample. Run make in the current directory:  

      $ make

   If the C++ compiler is not in the default location assumed by the Makefile,
   this can be overridden by setting the CXX variable on the make command line.
   For example:

      $ make CXX=/usr/bin/gcc-4.8.5/g++

   would select a GNU C++ compiler installed under /usr/bin/gcc-4.8.5

   A successful build will produce one output file in the Release subdirectory:

      libHTTPClientSample.so

   
## RUNNING THE SAMPLES

   Running of the samples requires access to the Correlator and Apama command 
   line tools.
   
   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 

   This sample also makes use of various built in codec plugins and the mapper 
   sample. See the README instructions in the mapper sample directory 
   to build that sample before continuing. You will refer to the 
   config file from that directory later.

   1. Start the Apama Correlator:

      > correlator -f correlator.log --config ../mapper/mapper-codec.yaml --config httpclient-transport.yaml --config httpclient.yaml --config httpclient.properties
      
   2. Inject the connectivity plugins support EPL from the monitors/ directory
   of your Apama installation:

      > engine_inject "$APAMA_HOME/monitors/ConnectivityPluginsControl.mon" "$APAMA_HOME/monitors/ConnectivityPlugins.mon"
      
   3. Inject the demo application from DemoApp.mon

      > engine_inject DemoApp.mon

   4. Send in the sample events from httpclient-feed.evt

      > engine_send httpclient-feed.evt

## SAMPLE OUTPUT

   The sample event in httpclient-feed.evt requests the current location of
   the ISS from a public feed. You will need to be running on a machine with
   an internet connection for this to work. If it succeeds the correlator
   should produce a JSON document with the ISS's location on its standard
   output which looks something like the output in sample_output.txt.

   You can send in events with any http URL and the correlator will print the
   content of that page. Any errors will be reported in the log file
   correlator.log.
