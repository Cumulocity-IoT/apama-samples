# Sample connectivity plugins monitoring application

## DESCRIPTION

   A full application using the HTTPClient, JSON-Codec, mapper and
   classifier codec plugins to implement a correlator which monitors 
   other Apama components and sends alerts when there are issues. 
   This sample uses dynamically created connectivity chains created 
   by EPL on demand.
   
   Copyright (c) 2016-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

## FILES

  Sample files are:

    README.md                   This file
    MonitoringEvents.mon        EPL implementation - events sent in by the user or to/from chains
    MonitoringAggregates.mon    EPL implementation - Aggregates used by ComponentMonitor
    ComponentMonitor.mon        EPL implementation - ComponentMonitor class
    MonitoringApp.mon           EPL implementation - the main application
    monitoring.yaml             Configuration for connectivity plugins for this application
    product-plugins.yaml        Configuration for product connectivity plugins
    monitor-feed.evt            Sample input events
    sample_output.txt           Sample output

## BUILDING THE SAMPLES

   It is recommended that you copy this sample folder to an area of your 
   APAMA_WORK directory rather than running it directly from the installation 
   directory.

## RUNNING THE SAMPLES

   Running of the samples requires access to the Correlator and Apama command 
   line tools.
   
   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the bin/apama_env script 
   has been sourced. 

   1. Start some other correlators which will be monitored by running: 

          > correlator -p <port number>
      
   Make sure to use some other port numbers that are not the default port.

   If you add an application to these correlator which processes events then
   it will produce more interesting status lines

   2. Replace the events in monitor-feed.evt with the addresses and port
   numbers of your other correlators.

   3. Start the monitoring correlator. This will
   load the config file from this sample, together with the required codecs 
   built into the product.

          > correlator -f monitoring.log --config product-plugins.yaml --config monitoring.yaml
      
   4. Inject the connectivity plugins support EPL from the monitors/ directory
   of your Apama installation:

          > engine_inject "$APAMA_HOME/monitors/ConnectivityPluginsControl.mon" "$APAMA_HOME/monitors/ConnectivityPlugins.mon"
      
   5. Inject the monitoring application from the supplied monitor files

          > engine_inject MonitoringEvents.mon MonitoringAggregates.mon ComponentMonitor.mon MonitoringApp.mon  

   6. Attach a receiver to the monitoring correlator

          > engine_receive

   7. Send in the sample events from monitor-feed.evt

          > engine_send monitor-feed.evt

  Make sure that the port numbers listed in the monitor-feed.evt file match the port numbers of your components

## SAMPLE OUTPUT

   The monitoring correlator will print the status of the other correlators
   that were configured via the monitor-feed.evt file every second. If you
   stop one of those correlators it should print warning messages instead and
   send ComponentDownWarning events to the attached receiver.

   If it succeeds the correlator should produce output similar to that in
   sample_output.txt. Any errors will be reported in the log file
   monitoring.log.

