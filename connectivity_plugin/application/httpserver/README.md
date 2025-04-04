# Sample for using the HTTP server transport connectivity plug-in
Also uses the HTTP client transport.

## DESCRIPTION

   A sample that demonstrates multiple uses of the HTTP server transport to send and receive
   JSON messages through various definitions. It uses the HTTP server transport, the JSON codec and the String codec.
   It also shows how to use the Diagnostic component to demonstrate what each part of the configuration
   does to the message being generated or parsed.
   
   Copyright (c) 2017-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

## FILES

  Contains files:

    README.md                       This file
    HTTPServer.mon                  An EPL receiver to receive events
    HTTPClient.mon                  An EPL sender to send events
    HTTPServer.properties           Defines server correlator port
    ServerDefaultEventType.yaml     Server connectivity configuration for a default event type
    ServerEventTypeInPayload.yaml   Server connectivity configuration for the event type being specified in the payload
    ServerEventTypeInHeader.yaml    Server connectivity configuration for the event type being specified in the http header
    ClientDefaultEventType.yaml     Client connectivity configuration for a default event type
    ClientEventTypeInPayload.yaml   Client connectivity configuration for the event type being specified in the payload
    ClientEventTypeInHeader.yaml    Client connectivity configuration for the event type being specified in the http header
    SwaggerDefault.json             Describes the REST API as exposed by ServerDefaultEventType.yaml
    SwaggerBody.json                Describes the REST API as exposed by ServerEventTypeInPayload.yaml
    SwaggerHeader.json              Describes the REST API as exposed by ServerEventTypeInHeader.yaml

   The server and client configuration files are intended to be used as matched pairs when running the samples.

## RUNNING THE SAMPLE

   Running of the samples requires access to the Correlator and Apama command 
   line tools.
   
   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` 
   script has been sourced. Now run these commands from the sample's directory.
   
   1. CD to the sample directory. Start the Apama Server Correlator by running:

      > correlator --config HTTPServer.properties --config ServerDefaultEventType.yaml

      N.B. config can be one of ServerDefaultEventType.yaml, ServerEventTypeInPayload.yaml, 
      ServerEventTypeInHeader.yaml depending on the sample required.
      
      This is the receiver correlator which we will use to receive events.

      If you wanted to change the correlator listening port, edit HTTPServer.properties.

      N.B.  Skip (2), Parts of (3) and (5) below if you intend to only use curl to inject events. 
     
   2. Start the Apama HTTP client Correlator by running:

          > correlator --port 15904 --config HTTPServer.properties --config ClientDefaultEventType.yaml
      
      N.B. config can be one of ClientDefaultEventType.yaml, ClientEventTypeInPayload.yaml, 
      ClientEventTypeInHeader.yaml corresponding to the server config above.

      This is the sender correlator which we will use to send events

   3. Inject the connectivity plugins support EPL from the monitors/ directory
   of your Apama installation (Skip the inject for the client if you intend to only use curl):
       
          > engine_inject "$APAMA_HOME/monitors/ConnectivityPluginsControl.mon" "$APAMA_HOME/monitors/ConnectivityPlugins.mon"
          > engine_inject --port 15904 "$APAMA_HOME/monitors/ConnectivityPluginsControl.mon" "$APAMA_HOME/monitors/ConnectivityPlugins.mon"
          > engine_inject --port 15904 "$APAMA_HOME/monitors/AnyExtractor.mon" "$APAMA_HOME/monitors/HTTPClientEvents.mon"
      
   4. Inject the httpServerReceiver from HTTPServer.mon into the receiver correlator

          > engine_inject HTTPServer.mon

   5. Inject the httpServerSender from httpServerSender.mon into the sender correlator (can be skipped if only using curl)

          > engine_inject HTTPClient.mon --port 15904
          > engine_send SendHttpRequest.evt --port 15904

   6. (Optional) Use curl to inject an event directly to the server correlator.

      N.B. config will depend on behavior required, one of ServerDefaultEventType.yaml, ServerEventTypeInPayload.yaml, 
      ServerEventTypeinHeader.yaml will have been used to start the server Correlator. The HTTPServer.properties
      will contain the correlator listening port to use in the curl command line (default 8080). 

      For the serverDefaultEventType config use:

          curl -X PUT http://localhost:8080/ -d '{"sensorId":666, "sensorName":"FooBar", "temperature":"3.14","extra":{"A":"alpha","B":"beta"}}' -H "Content-Type:application/json"

      For the serverEventTypeInPayload config use:
      
          curl -X PUT http://localhost:8080/ -d '{"type":"apamax.httpserversample.Temperature", "channel":"httpServerSample", "data":{"sensorId":666, "sensorName":"FooBar", "temperature":3.14,"extra":{"A":"alpha"}} }' -H "Content-Type:application/json"

      For the serverEventTypeInHeader config use:

          curl -X PUT -H "X-ApamaEventType:apamax.httpserversample.Temperature"  -H "X-ApamaChannel:httpServerSample" http://localhost:8080/ -d '{"sensorId":666, "sensorName":"FooBar", "temperature":3.14,"extra":{"A":"alpha"} }'  -H "Content-Type:application/json"

## SAMPLE OUTPUT

   You will see a message being received by the receiver correlator (HTTP server) in the log file similar to the below text:
   
      Received event: apamax.httpserversample.Temperature(42,"Machine A Temperature",91.34,{"A":any(string,"alpha"),"B":any(string,"beta")})
   
   In the sender correlator (HTTP client) log you will see similar to the below text:
   
      Received response from HTTPServer: apamax.httpserversample.HTTPResponse(1,"","202","Accepted")
   
   with the -v option (verbose) for curl you will see a response similar to this (empty) after the send:

      < HTTP/1.1 202 Accepted
      < Date: Wed, 09 Aug 2017 14:37:40 GMT
      < Connection: Keep-Alive
      < Server: Apama/10.1 (amd64-rhel7)
      < Content-Length: 0
