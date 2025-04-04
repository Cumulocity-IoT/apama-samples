# Sample Java Connectivity Plug-in implementing an Object <-> JSON Codec

## DESCRIPTION

   Codec source code and associated EPL application to demonstrate the
   operation of the codec.

   Copyright (c) 2016-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

## FILES

  Sample files are:

    README.md                                    This file
    build.xml                                    ANT build file

    src/**/JSONCodec.java                        Source for the JSON codec
    json-codec.yaml                              Connectivity plug-ins configuration file for
                                                 loading the JSON codec
                                                  
    demo/json-demo-config.yaml                   Connectivity plug-ins configuration file for
                                                 running a demo using this plug-in
    demo/DemoApp.mon                             EPL code for the demonstration
    demo/customers-input.json.txt                Input file containing JSON
    demo/reference-correlator.out                Sample output of the correlator
    demo/reference-specialoffers-output.json.txt Sample output from the transport

## BUILDING THE SAMPLE

   It is recommended that you copy this sample folder to an area of your 
   APAMA_WORK directory rather than running it directly from the installation directory.

   Running and building of the sample requires access to the Correlator and
   Apama command line tools.

   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 

   To build the sample, run ant in the current directory:

   > ant

   A successful build will produce output files for the JSON codec,
   as well as a directory containing 3rd party libraries used
   by the JSON codec:

        Release/json-codec-sample.jar
        Release/lib/

## RUNNING THE SAMPLE

   1. Start the Apama Correlator specifying the connectivity config file

      > correlator --config json-codec.yaml --config demo/json-demo-config.yaml

   2. Inject the demo application and the connectivity plug-in EPL

      > engine_inject "$APAMA_HOME/monitors/ConnectivityPluginsControl.mon" "$APAMA_HOME/monitors/ConnectivityPlugins.mon" "demo/DemoApp.mon"
      
## SAMPLE OUTPUT

   This demo uses the standard UnitTestHarness codec plug-in, which reads 
   some JSON messages the text file demo/customers-input.json.txt containing 
   information about customers, which are parsed using the JSON codec and 
   sent into the Correlator (or "host") as Apama events. 

   The EPL demo application logs information about each customer. Certain 
   customers should be sent some special offers, and an Apama event is 
   sent from the Correlator into the connectivity chain with this information. 
   
   The JSON codec creates a JSON document with the special offer messages 
   it receives, and these are written to a text file called 
   demo/specialoffers-output.json.txt by the UnitTestHarness. In a real 
   application these messages would be sent to some external system by a 
   transport, but for this demo we use the NullTransport (which does nothing 
   with them). 

   The expected messages that should be in the correlator log file are shown 
   in demo/reference-correlator.out. The expected JSON strings for the 
   special offers are in demo/reference-specialoffers-output.json.txt. 


## RUNNING THE PYSYS TESTS

   This sample comes with a selection of PySys testcases that demonstrate 
   some of the typical testcases that would be required to check the 
   correctness of a bi-direction connectivity codec plug-in, and some best 
   practices and patterns for writing simple and efficient testcases. 
   
   These testcases all use the standard UnitTestHarness plug-in which makes it 
   easy to write testcases for a single plug-in in isolation, without 
   the need to configure or write code in the host (e.g. correlator) or 
   (for a codec plug-in like this one) to configure a transport, as would be 
   required for a system or integration testcase designed to check how all 
   the plug-ins in a chain work together. 
   
   The UnitTestHarness sits above and/or below the plug-in being tested 
   in the connectivity chain, and can send Message objects into the 
   chain from a text file, and can write Message objects out to a text file 
   and/or the host log file for checking the results. See the documentation 
   for more information about the UnitTestHarness. 
   
   To view the sample testcases for this plug-in (after building the sample 
   and setting up the environment as described above), change to the 
   tests/ subdirectory and execute:
   
      > pysys print
   
   To run all of the testcases execute:
   
      > pysys run
