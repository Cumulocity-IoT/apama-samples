# Simple generic EPL application for sending and receiving test messages
(for use with any connectivity plug-in)

## DESCRIPTION

   A simple application that logs all events received on a specific transport channel, 
   and also sends test events on a specific transport channel (once the AppReady event is received).

   This is useful for testing, diagnostics, and getting started with a new connectivity plug-in.
 
## COPYRIGHT NOTICE

   Copyright (c) 2018-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this 
   file except in compliance with the License. You may obtain a copy of the License at
   http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software distributed under the
   License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
   either express or implied. 
   See the License for the specific language governing permissions and limitations under the License.

## FILES

  Sample contains:

     README.md                       This file
     monitors/SendReceiveSample.mon  The monitor that logs incoming events and sends test messages
     events/AppReady.evt             An event that is sent when the application has been 
                                     fully injected to indicate it is ready to start receiving messages
     initialization.yaml             A YAML configuration file to initialize a correlator with this 
                                     application on the command line. 

## EDITING THE SAMPLE EPL FILE

   By default this sample logs received events from the "SampleInputChannel" channel and 
   sends test events to "SampleOutputChannel".
   
   However if the connectivity transport you wish to use is configured 
   to use channels with a specific prefix (e.g. "um:" or "mqtt:") you will need to 
   perform this initial step to configure the channels to be used:
   
   - Edit monitors/SendReceiveSample.mon and search for "TODO". 
   
   - At the indicated line, change it to use the desired input and output 
   channel names, with the prefix corresponding to your YAML configuration. 
   
   If you are not using external transport-specific tools to send and 
   receive test messages, you may wish to use the same channel for both, 
   for example using "um:MyChannelName" for both channels would allow sending 
   an event to the transport and then receiving the event back from the 
   transport without the need for any external send/receive utilities. 


## RUNNING THE SAMPLE
   
   1. Copy this project directory to a suitable location such as APAMA_WORK. 
   
   2. Write some .yaml and .properties configuration files for the connectivity plug-in 
   you wish to use into the config/ directory, using the documentation as a reference. 
   
   Depending on the chain configuration you are using you may also need to 
   configure YAML mapper and/or classifier codec rules for the 
   mypackage.MySampleInput and mypackage.MySampleOutput event definitions used 
   by the sample
   
   3. To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been sourced. 
   
   4. Start the correlator, specifying both the connectivity configuration 
   directory and the provided initialization.yaml, for example:

     correlator --config initialization.yaml --config config/myconnectivity
   
   5. This will launch the correlator, inject the sample EPL application and 
   send the AppReady event. This will cause it to send a MySampleOutput 
   containing "Hello world" to the configured channel. 
   
   Any received events will be written to the correlator's log which is 
   displayed on its standard output by default; see below for more details 
   on the expected output. 
   

## EXPECTED OUTPUT

   The correlator will log any messages received from the transport 
   on the configured input channel(s), so if you send some test messages 
   using a transport-specific utility or administration tool you should 
   see them in the correlator log output when they are received by Apama. 

   The sample can also be used for 'round-trip' testing, which means it can 
   send and then receive the same message itself, without the need for external 
   transport-specific tools. The sample sends a "Hello world" MySampleOutput 
   event to the configured output channel(s) at startup. 
   Therefore if you configure the sample's input and output channels to be the 
   same, the messages sent to the transport will then be received from the 
   transport, resulting in "Hello world" messages being received on 
   the specified input channel. Note that setting the output and input channel 
   to be the same is only appropriate if your connectivity YAML file specifies 
   suppressLoopback: true, since otherwise events sent to the channel would 
   be logged as having been received as a result of the correlator's channel 
   subscription mechanism, without using the external transport at all. 
   When the transport is working correctly and events are round-tripped 
   using "SampleInputChannel" as the input and output channel name, the 
   correlator will log a message similar to the following:
   
      --- Received sample event from channel 'SampleInputChannel': mypackage.MySampleOutput("Hello world sample message 1","myParameterValue",{})

   The type of the received event depends on your transport and codec 
   configuration. 
   
   If you are doing round-trip testing and your transport and codec 
   configuration allows the Apama event type to be included in the outgoing 
   messages (perhaps in a transport-specific header, property or tag) and 
   used to determine the towards-transport "metadata.sag.type", then 
   received messages will be of type MySampleOutput. 
   
   If the received events do not come from an Apama correlator, there 
   would usually be configuration in the classifierCodec or eventMap 
   rules to set an appropriate Apama event type for each incoming 
   message, perhaps using a different event type to the outgoing 
   messages such as MySampleInput. 


## SENDING ADDITIONAL EVENTS

   An optional capability of this sample is to send additional events 
   of any type to a channel of your choice. To do this, run 
   engine_send. 
   
   Wrap the event you wish to send in a mypackage.SendIt, specifying 
   the channel you wish to send it to, and adding any(typename, ...) 
   around the event itself. 
   
   For example, to send a MySampleOutput to the "SampleOutputChannel" 
   channel, send this event into the correlator on the 
   default channel (""):
   
      mypackage.SendThis("SampleOutputChannel", any(mypackage.MySampleOutput, mypackage.MySampleOutput("My SendThis message", "param", {"foo":any(string, "bar")})))

   This results in a log message such as:
   
      --- Sending specified sample output event to channel 'SampleOutputChannel': mypackage.MySampleOutput("My SendThis message","param",{"foo":any(string,"bar")})
