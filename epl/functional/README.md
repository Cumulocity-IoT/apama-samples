## Some samples to demonstrate the use of the EPL functional library.

##  DESCRIPTION

   This project contains multiple samples that demonstrate the basics of using the 
   new EPL functional library, as well as some more complex samples,
   and shows how it can result in simpler code than using existing imperative-style
   programming.

## COPYRIGHT NOTICE

   Copyright (c) 2023-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.


## FILES

  Sample contains:

    README.md                           This file
    events/Start.evt                    Events that will trigger each of the samples.
    monitors/SimpleFunctional.mon       The simple sample EPL application.
    monitors/AverageCalc.mon            The average calculation of measurement values sample.
    monitors/ClearAllAlarms.mon         The sample that shows how to clear all alarms for a given Cumulocity device.
    monitors/Simulator.mon              This monitor will intercept calls to Cumulocity to avoid any setup in order to run these samples.
    sample_output.txt                   Reflects what the output should be in the correlator log from all three samples.


## DETAILS

   The SimpleFunctional sample compares imperative and functional coding approaches
   to carrying out the operation of filtering a sequence of integers up 
   to 9 for odds, multiplying them by 10, and summing the result. 
   In addition, two approaches to using the functional libraries are illustrated: 
   A somewhat naive approach in which multiple function calls are nested, and 
   an easier-to-read more fluent style, in which the calls are made 
   using the Functional wrapper event.

   The AverageCalc sample calculates the average of the value element of all Measurement values returned 
   from a FindMeasurement request which is restricted by fragment, series and a date range and 
   then groups per fragment/series and filters to ensure values counted are within a threshold.
   
   The ClearAllAlarms sample receives a list of ManagedObjects and will search for any alarms
   associated with those managed objects, it gathers them into a list and sends an event
   to update the status on each of those alarms to CLEARED.


## RUNNING THE SAMPLE

   The sample can be run using the engine_deploy tool. First, source the `bin/apama_env` script, then run:

     $ cd $APAMA_HOME/samples/epl
     $ engine_deploy --outputDeployDir deployed functional
     $ correlator --config deployed
    
## SAMPLE OUTPUT

   The output in the correlator log should contain the contents of sample_output.txt (without the timestamps).
