# Denial of Service sample application


## DESCRIPTION

   The Correlator is used to detect Denial of Service attacks from one 
   or more hosts through detection of high densities of 'Ping' events. 
   Periodic alerts are sent with the address of the originating host and 
   the number of Ping events detected

## COPYRIGHT NOTICE

   Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

## FILES

  Sample contains:

    README.md              This file
    dos.mon                The DenialOfService monitor
    dos-feed.evt           Simulation event data


## RUNNING THE SAMPLE

   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 
   
   Now run these commands from the sample's directory:

   1. Start the Apama Correlator by running:

      > correlator
        
   2. (in a separate terminal) inject the DoS monitor:
   
      > engine_inject dos.mon
      
   3. (in a separate terminal) register the event receiver and
      write events to stdout:
   
      > engine_receive -c output
   
   4. (in a separate terminal) send the events:
   
      > engine_send dos-feed.evt
      
## SAMPLE OUTPUT

   The output of the engine_receive window should match the sample_output.txt
   file (ignoring BATCH tags).

