# Demonstration of grouped aggregates in EPL

## DESCRIPTION

   Monitors updates from temperature sensors and sends out alerts if the new
   temperature mean is more than some standard deviations above the mean for
   the last 10 minutes or exceeds a fixed threshold. The alert contains the
   most recent temperature reading and average.

## COPYRIGHT NOTICE

   Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

## FILES

  Sample files:

    README.md            This file
    groups.mon           The simple monitor
    groups-feed.evt      Sample event data


## RUNNING THE SAMPLE

   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 
   
   Now run these commands from the sample's directory.

   1. Start the Apama Correlator by running:

      > correlator
      
   2. (in a separate terminal) inject the simple monitor:
   
      > engine_inject groups.mon
      
   3. (in a separate terminal) register the event receiver and 
      write events to stdout:
   
      > engine_receive -c output
   
   4. (in a separate terminal) send the events:
   
      > engine_send groups-feed.evt


## SAMPLE OUTPUT

   The output of the engine_receive window should match the sample_output.txt
   file (ignoring BATCH tags).

