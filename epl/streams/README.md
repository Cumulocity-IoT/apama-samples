# Simple application using EPL's stream queries feature


## DESCRIPTION

   This application receives a stream of stock price Tick events and calculates the
   average price over the last 10 events. If this average changes more than a
   threshold then it sends an event containing the price difference


## COPYRIGHT NOTICE

   Copyright (c) 2013-persent Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 


## FILES

  Sample files:

    README.md              This file
    streams.mon            The simple monitor
    streams-feed.evt       Sample event data


## RUNNING THE SAMPLE

   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 
   
   Now run these commands from the sample's directory:

   1. Start the Apama Correlator by running:

      > correlator 
    
   2. (in a separate terminal) inject the simple monitor:
   
      > engine_inject streams.mon
      
   3. (in a separate terminal) register the event receiver and 
      write events to stdout:
   
      > engine_receive -c output
   
   4. (in a separate terminal) send the events:
   
      > engine_send streams-feed.evt


## SAMPLE OUTPUT

   The output of the engine_receive window should match the sample_output.txt
   file (ignoring BATCH tags).

