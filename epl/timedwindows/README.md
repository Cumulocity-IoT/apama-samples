# Demonstration of timed windows in EPL


## DESCRIPTION

   This sample tracks the total value of shares traded for a particular symbol
   in a given period of time and sends alerts if that exceeds a given
   threshold.  After the first alert for a given stock the threshold is
   increased before another alert is given.

   Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

## FILES

  Sample contains:

    README.md            This file
    timed.mon            The simple monitor
    timed-feed.evt       Sample event data


## RUNNING THE SAMPLE

   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 
   
   Now run these commands from the sample's directory.

   1. Start the Apama Correlator:

      > correlator 
    
   2. (in a separate terminal) inject the simple monitor:
   
      > engine_inject timed.mon
      
   3. (in a separate terminal) register the event receiver and 
      write events to stdout:
   
      > engine_receive -c output
   
   4. (in a separate terminal) send the events:
   
      > engine_send timed-feed.evt

   The output of the engine_receive window should match the sample_output.txt
   file (ignoring BATCH tags).

