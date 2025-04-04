# Sample Mapper connectivity plugin using the C++ Plugin Development Kit


## DESCRIPTION

   A Connectivity Plugin which reshapes the data in message payloads and
   metadata to match two differing message schema.  Packaged as a sample to
   demonstrate creating plugins using the Connectivity Plugins C++ API.
   
   Copyright (c) 2016-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 


## FILES

  Sample files are:

    README.md                      This file
    MapperCodec.cpp                Source for the Mapper Codec
    Makefile                       GNU Make compatible makefile
    mapper-codec.yaml              Configuration file to load the codec


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
   this can be overidden by setting the CXX variable on the make command line.
   For example:

      $ make CXX=/usr/bin/gcc-4.8.5/g++

   would select a GNU C++ compiler installed under /usr/bin/gcc-4.8.5

   A successful build will produce one output file in the Release
   subdirectory:

      libMapperSample.so


## RUNNING THE SAMPLES

   This sample only demonstrates creation of a single codec. You will need
   a transport to demonstrate the use of it.

   The HTTP client sample is an end-to-end sample which demonstrates the 
   usage of this codec as well. It is visible in the parent directory of this
   sample.

   Running of the samples requires access to the Correlator and Apama command 
   line tools.
   
   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 

   1. Create a connectivity configuration YAML file. Loading is handled by
   mapper-codec.yaml.

   In the startChains stanza add the mapper codec to a chain and configure
   the mapper codec for the events in that chain:

       startChains:
         myChain:
          - apama.eventMap
          - mapperCodec:
              TypeA:
                towardsHost:
                  mapFrom:
                    - payload.fieldName: metadata.fieldName
                    - metadata.fieldName: payload.fieldName
              "*":
                towardsTransport:
                  defaultValue:
                    - metadata.fieldName: value
          - myTransport
   
   2. Start the Apama Correlator by running:

      > correlator --config mapper-codec.yaml --config connectivity.yaml
          
   3. Inject your application monitor code which sends/receives events to the
   chain.
   
   For more information see the documentation for Using Connectivity Chains, 
   Developing Connectivity Plugins and for the product version of the
   MapperCodec.
