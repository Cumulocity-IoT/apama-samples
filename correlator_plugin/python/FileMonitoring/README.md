# Python plug-in sample

   This sample shows how to write and invoke a Python EPL plug-in that has file 
   access capabilities, can monitor directories, and when new event files are created 
   in those directories, read and send those events on another channel.

   This is a more complex Python sample than SimplePlugin. 
   
   The sample application calls a method on a Python plug-in to allow EPL to easily 
   monitor various directories.

   EPL detects when event files are created within a monitored directory, and then 
   asks the plug-in to read the contents of that file. The EPL then converts this 
   string into an event that is sent on a separate channel, which another monitor is 
   subscribed to.
   
   Some useful things demonstrated in this sample are:
   * accessing settings in a configuration file from a Python plug-in
   * the plug-in's ability to write to the Correlator log file
   * being able to send events to a Correlator's channel from the 
     Python plug-in, 
   * raising Python exceptions that can be caught in EPL,
   * being able to create background threads in Python plug-ins.
    
   Note: The sample will log an exception if asked to monitor a directory that does 
   not already exist. This could be easily changed in the plug-in's poll method.
   
## COPYRIGHT NOTICE

   Copyright (c) 2018-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

## FILES

  Sample files:

    README.md                              This file
    sample_output.txt                       An example of the what the output is expected 
                                            to look like after running the application.
    monitors/FileMonitoring.mon             The monitor that calls out to the Python 
                                            plug-in to monitor directories and listens 
                                            out for additions, in such cases asking the 
                                            plug-in to read those files and sending these 
                                            events onto another channel.  
    plugins/FileMonitoringPlugin.py         The Python file that implements the plug-in. 
                                            This plug-in can monitor a directory, and make 
                                            the file contents available to EPL.
    config/CorrelatorConfig.yaml            A YAML configuration file to initialize the 
                                            correlator with the Python plug-in settings.
    events/start.evt                        Sets up which directories are being monitored
    events/SampleEventsForPluginToRead.txt  An example new events file, containing events 
                                            which a receiver is listening out for.


## RUNNING THE SAMPLE
   
   1. Copy this project directory to a suitable location such as APAMA_WORK. 
   
   2. To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been sourced, at the top level of the project.

   3. Before running the application you could create the directories to be monitored, as 
   per the contents of start.evt. In this case we will create only one of them
      
   * For example create an empty directory called test2 in the same directory 
     as this README file.
   * The base directory for these are set in the configuration file, currently this is: 
     `${PARENT_DIR}/../..`
   * The directory to be monitored must already exist, and an exception will be thrown 
     if it doesn't. This could be easily changed in the plug-in's poll method if needed. 
   
   4. Start the correlator, specifying configuration file CorrelatorConfig.yaml, 
   for example:

    > correlator --config config/CorrelatorConfig.yaml
   
   * Note: If you change the port the correlator is running on you must use -p and 
     the new port number in the following commands.
   
   5. This will launch the correlator. You can then inject the sample EPL application which 
   loads the Python plug-in in another terminal. In a separate terminal
   write the following command:
   
    > engine_inject monitors/FileMonitoring.mon monitors/ProcessNewExpectedEvtsMonitor.mon
   
   6. If you wish you can set up an additional engine_receive to listen for any events 
   read in from new files.
   
    > engine_receive -c fileMonitoringReadEventsChannel 
   
   7. You can then send the events to start monitoring from start.evt. In a separate terminal
   write the following command:
   
    > engine_send events/start.evt
   
   This will cause in the EPL application to output the current files in each directory 
   (or an exception if the directory doesn't exist)
   
   8. Copy the SampleEventsForPluginToRead.txt into the test2 directory being monitored
   This will result in a log message indicating a new file exists.
   The file is then read and the event is sent back to EPL, which you can see in the logging. 
   EPL then processes and sends this event to anything listening on 
   channel: fileMonitoringReadEventsChannel 
   (which this monitor is subscribed to and you will therefore see in the logging). 
   
   Any output will be written to the correlator's log which is 
   displayed on its standard output by default; see below for more details 
   on the expected output. 
   
## EXPECTED OUTPUT

   sample_output.txt has detailed expected output
   

