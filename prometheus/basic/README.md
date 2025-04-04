# Prometheus Monitoring of the Apama Correlator

## DESCRIPTION

Basic monitoring of a single correlator, demonstrating both in-built metrics and user defined metrics.

Copyright (c) 2018-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

## FILES

Sample contains:

    README.md                             This file
    SampleTransport.cpp                   Source for a demo transport
    Makefile                              GNU Make compatible makefile
    Demo.mon                              An EPL demo program
    connectivity.yaml                     Configuration file to load the transport
    prometheus.yml                        Configuration file to pass to Prometheus

## ASSUMED KNOWLEDGE

Connectivity Plugin basics

## BUILDING THE SAMPLE

It is possible to run this sample without building the associated Connectivity Plug-in, thus this step may be skipped (though the end demo will have reduced functionality).

It is recommended that you copy this sample folder to an area of your APAMA_WORK directory rather than running it directly from the installation directory. 

GNU Make is required in order to build the samples using the supplied Makefile.

Source the apama_env file located in the installation bin directory to set the environment variables required for building sample. Run make in the current directory:  

	$ make

If the C++ compiler is not in the default location assumed by the Makefile, this can be overridden by setting the CXX variable on the make command line. For example:

	$ make CXX=/usr/bin/gcc-4.8.5/g++

would select a GNU C++ compiler installed under /usr/bin/gcc-4.8.5

A successful build will produce one output file in the Release subdirectory:

	libPrometheusSample.so

## RUNNING THE SAMPLE

Running the sample requires access to the correlator and Apama command line tools.

Additionally, a Prometheus server executable is required.

To ensure that the environment is configured correctly for Apama, all the commands below should be executed from a shell where the `bin/apama_env` script has been sourced. 

1.	Start the Prometheus server, pointing it towards the provided 'prometheus.yml' configuration file. This sample assumes the Prometheus server will run on the same host as the Apama Correlator, and that Apama Correlator will be running on the default port.

2.	Start the Apama Correlator:

		> correlator --config initialization.yaml

	Or, if using the associated Connectivity Plug-in:

		> correlator --config initialization.yaml --config connectivity.yaml

## SAMPLE OUTPUT

The demo itself will not output any files or logging, however navigating to a means of monitoring the Prometheus server, such as localhost:9090/graph, will show the range of metrics exposed by the Apama Correlator.

	sag_apama_correlator_uptime_seconds	                          This metric represents how long this Correlator has been running for - it will steadily increase
	sag_apama_correlator_consumers_total                          This metric represents the number of external receivers attached to the Correlator - it will measure 1 if using the Connectivity Plug-in or 0 if otherwise
	sag_apama_correlator_listeners_total                          This metric represents the number of listeners active within the Correlator - it will have a baseline of 2 due to the 'timer' and 'any' listeners, but vary above that due to spawning and termination of new 'send_message' contexts
	sag_apama_correlator_physical_memory_bytes                    This metric represents the amount of physical memory used by the Correlator - it should remain stable
	sag_apama_correlator_user_listener_one_received               This metric represents the amount of events received by the 'any' listener. It is a user defined status. It will steadily increase
	sag_apama_correlator_user_sampleTransport_messages_received   This metric represents the amount of events received by the Connectivity Plug-in. It is a user defined status, and will not be present if not using the plugin. It will steadily increase
