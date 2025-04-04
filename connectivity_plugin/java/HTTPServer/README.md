# Sample Java Connectivity Plug-in implementing an HTTP server transport

## DESCRIPTION

   Transport source code and associated EPL application to demonstrate the
   ability to use HTTP PUT requests to send events into the correlator via
   this transport.

   Copyright (c) 2016-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

## FILES

  Sample files are:

    README.md                   This file
    build.xml                   ANT build file

    src/**/HTTPServer.java      Source for the HTTP server transport
    src/**/index.html           Content to serve for GET requests
    DemoApp.mon                 EPL code for the demonstration
    connectivity.yaml           Connectivity plug-ins configuration file
    httpserver.properties       Properties file defining substitutions for the
                                configuration file

## BUILDING THE SAMPLE

   It is recommended that you copy this sample folder to an area of your 
   APAMA_WORK directory rather than running itdirectly from the installation directory.

   Running and building of the sample requires access to the Correlator and
   Apama command line tools.

   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 

   To build the sample, run ant in the current directory:

   > ant

   A successful build will produce an output file httpserver-sample.jar
   containing the compiled HTTP server transport.

## RUNNING THE SAMPLE

   1. Start the Apama Correlator specifying the connectivity config file and
      properties file

      > correlator --config httpserver.properties --config connectivity.yaml

   2. Inject the demo application and the connectivity plug-in EPL

      > engine_inject "$APAMA_HOME/monitors/ConnectivityPluginsControl.mon" "$APAMA_HOME/monitors/ConnectivityPlugins.mon" DemoApp.mon
      
   3. Open a web browser and navigate to http://localhost:8080/
      (this instruction assumes that you are running the correlator on the
       same machine as your browser)
   
   4. Use the text box to write a JSON-formatted string:

          {"stringField":"asdf", "integerField":40, "seqField":[1.0, 2.0, 3.0]}


      Click the button to submit it via an HTTP 'PUT' request into the HTTP
      server plug-in that is currently running in the correlator.


## SAMPLE OUTPUT

   The HTTP server transport turns HTTP 'PUT' requests into messages that are
   sent towards the host. The message contains HTTP headers in its metadata,
   and also the contents of the 'PUT' request in the message's string payload.

   The JSON codec is placed between the host and the HTTP server transport
   so that any JSON that is 'PUT' into the chain this way gets mapped into
   structured data that the correlator can interpret as an EPL event.

   The EPL monitor 'ListenForJSON' in this demo listens for these events and
   logs their contents. By default the correlator log goes to the console, and
   if you submit the JSON from the instructions above, you'll see a line in
   the log like:

       ListenForJSON [1] Got an event FromJSON([1,2,3],40,"asdf")
