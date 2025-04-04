# Sample C++ transport written using the Chain Manager API


## DESCRIPTION

   Sample C++ transport written using the Chain Manager API. This can be 
   used as a starting point for creating new C++ transports that take 
   responsibility for creating their own chains.

   
## COPYRIGHT NOTICE

   Copyright (c) 2017-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

## FILES

  Sample contents:

    README.md                             This file
    MyTransport.cpp                       Source for the sample transport
    Makefile                              GNU Make compatible makefile
    MyConnectivity.yaml                   Sample YAML file

## BUILDING THE SAMPLES

   It is recommended that you copy this sample folder to an area of your 
   APAMA_WORK directory rather than running it directly from the installation 
   directory. 

   GNU Make is required in order to build the samples using the supplied Makefile.

   Source the apama_env file located in the installation bin directory to set 
   the environment variables required for building sample. Run make in the 
   current directory:  

    $ make

   If the C++ compiler is not in the default location assumed by the Makefile,
   this can be overridden by setting the CXX variable on the make command line.
   For example:

    $ make CXX=/usr/bin/gcc-4.8.5/g++

   would select a GNU C++ compiler installed under /usr/bin/gcc-4.8.5

   A successful build will produce one output file in the Release subdirectory:

    libMyTransport.so

  
## RUNNING THE SAMPLES

   The sample creates chain dynamically by a transport chain manager plug-in, 
   using chain definitions specified in dynamicChains. In our sample, 
   manager ("MyTransportChainManager") creates a chain in response to an EPL
   application subscribing or sending to a channel with a specific name or
   prefix("myTransport").
   
   Running the samples requires access to the correlator and Apama command 
   line tools.
   
   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 

   1. CD to the sample directory. Start the Apama server correlator by running:

    > correlator --port 15903 --config MyConnectivity.yaml

   2. Inject the connectivity plug-ins support EPL from the monitors/ 
   directory of your Apama installation :

    > engine_inject --port 15903 "$APAMA_HOME/monitors/ConnectivityPluginsControl.mon" "$APAMA_HOME/monitors/ConnectivityPlugins.mon"
      
  3. Inject the TestMonitor.mon into the correlator

    > engine_inject --port 15903 TestMonitor.mon 

  You will see messages in the correlator for the event sent from the 
  correlator to the transport:

    <connectivity.MyTransport.TestManager-ToTransport[myTransport:testChain]> Sending message towards transport: Hello World



  and for the event sent from the transport to the correlator:
  
    TestMonitor [1] Message received from Transport: Hello World
  
  


