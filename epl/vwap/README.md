# VWAP sample application

## DESCRIPTION

   The Event Correlator computes Volume Weighted Average Price (VWAP) data for
   given stocks and sends events when consecutive VWAP calculations differ by
   a given percentage. In this example the Event Correlator outputs all
   floating point values to six significant figures.

   Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

## FILES

  Sample contains:

    README.md              This file
    vwap.mon               The VWAP monitor
    vwap-feed.evt          Simulation event data


## RUNNING THE SAMPLE

   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 
   
   Now run these commands from the sample's directory.

   1. Start the Apama Correlator:

      > correlator 

   2. (in a separate terminal) inject the VWAP monitor:
   
      > engine_inject vwap.mon
      
   3. (in a separate terminal) register the event receiver and 
      write events to stdout:
   
      > engine_receive -c output
   
   4. (in a separate terminal) send the events:
   
      > engine_send vwap-feed.evt
      
   The output of the engine_receive window should match the sample_output.txt
   file (ignoring BATCH tags).

