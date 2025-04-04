# Samples for the in-process external connectivity framework

## COPYRIGHT NOTICE

   Copyright (c) 2017-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 


## DESCRIPTION

   This directory tree contains several different samples in both C++ and Java
   for using the Connectivity Plug-ins framework to create in-process adapters
   for connecting to external systems.

   Because of the nature of this plug-in framework some of the samples use the
   plug-ins from other samples as part of the demonstration. As a result there
   is an order that you need to build the plug-ins in.


## CONTENTS

  * cpp - Sample codecs and transports written in C++
    *  httpclient - A transport that connects to external HTTP servers, 
        to demonstrate how to create transports. For production use, we 
        recommend using the standard (non-sample) HTTPClient binary included 
        with the product
      
     *  skeleton_chainmanager - A starting point for creating new transports 
        that manage chain creation themselves

     * mapper - A codec for moving data between fields in events. For 
        production use, we recommend using the standard (non-sample) 
        MapperCodec binary included with the product
        
   * java - Sample codecs and transport written in Java
   
      * JSON-Codec - A codec for transforming data between JSON strings and maps 
        to demonstrate how to create codecs in Java. For production use, we 
        recommend using the standard (non-sample) JSON codec binary included 
        with the product
        
      * HTTPServer - A transport that acts as HTTP server, to demonstrate how to 
        create transports. For production use, we recommend using the 
        standard (non-sample) HTTPServer binary included with the product


   * application - Sample EPL applications demonstrating use of connectivity 
      plug-ins from EPL, and of the C++ and Java sample plug-ins
   
      * genericsendreceive - A simple general-purpose EPL monitor that can send and 
        receive messages to/from any transport. This is a great way to get started 
        with any connectivity plug-in. 
        
      * monitoring - An application for monitoring the status of other Apama components, 
        demonstrating the HTTPClient, JSON-Codec and mapper sample plug-ins
        
      * weather - An application that looks up the weather in your location, 
        demonstrating the HTTPClient, JSON-Codec and mapper samples
        
      * httpserver - An application that shows how to use the standard 
        (non-sample) HTTP server transport, including the serving of swagger 
        files and various approaches to message mapping. 
      
## SAMPLE BUILD ORDER

   To build the application samples you must first build all the samples under
   the cpp and java directories.

   Within the cpp directory you must build the mapper before building 
   the httpclient samples.

   Within the Java directory you must build the JSON-Codec sample before
   building any others.

   Each sample has instructions on how to build it within that directory.
