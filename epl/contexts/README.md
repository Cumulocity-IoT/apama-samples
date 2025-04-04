# Application using contexts to demonstrate their use comparing parallel and serial usage

## DESCRIPTION

   This application receives Tick events as an input feed, and searches for 
   events where the price is greater than 10.0 arbitrary units. Matching 
   events are output.


## COPYRIGHT NOTICE

   Copyright (c) 2015-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 


## FILES

  Sample contains:

    README.md                    This file
    Input/analyse-parallel.mon   Parallel implementation using contexts
    Input/analyse-serial.mon     Comparison serial implementation with no contexts
    Input/evt.mon                Definition of all the events used in the sample
    Input/gen.mon                Event generator monitor to generate fake input events
    Input/gen.evt                Event to start running the sample
    Input/fin.evt                Event to stop running the sample
    run.py                       PySys test to run sample

## RUNNING THE SAMPLE

   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 
      
   This sample uses the PySys test framework to manage starting correlators,
   injecting code, sending events and setting up receivers.

   To run use the command:

      > pysys run

## SAMPLE OUTPUT

   The sample will create several output files under an Output directory. Look
   for the Result events at the end of each file

   serial-results.evt    The results from the non-context version. 
   parallel-results.evt  The results from the context-using version.

   The parallel results should be around 6-8 times smaller (it's time taken)
   than the serial results.
