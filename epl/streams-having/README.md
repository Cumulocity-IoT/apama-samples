# A streams sample, taking advantage of 'having'

## DESCRIPTION

   This application receives a stream of Tick events and calculates the
   average price over the last 10 events. If this average changes more than a
   threshold then it sends an event containing the price difference

   Compare this sample with the 'streams' sample to see how 'having'
   can help in writing simpler streaming code.

   Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 


## FILES

  Sample contains:

    README.md               This file
    streams-having.mon       The simple monitor
    streams-having-feed.evt  Sample event data


## RUNNING THE SAMPLE

   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been sourced. Now run these commands from the sample's directory.

   1. Start the Apama Correlator:

      > correlator 
    
   2. (in a terminal) inject the simple monitor:
   
      > engine_inject streams-having.mon
      
   3. (in a terminal) register the event receiver and 
      write events to stdout:
   
      > engine_receive -c output
   
   4. (in a terminal) send the events:
   
      > engine_send streams-having-feed.evt

   The output of the engine_receive window should match the sample_output.txt
   file (ignoring BATCH tags).

