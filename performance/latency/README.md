# Latency-testing framework for the correlator

## Description

   This is a latency-testing framework for the correlator. It works by using 3
   correlators. Correlator A gates your input events to a fixed rate. You want
   to send to this correlator as fast as possible from your input source (e.g.
   using engine send). It also picks a random subset of events to bracket with
   'timing' events, used to calculate the latency of that one event.
   Correlator B is the correlator under test and should contain your
   application. Correlator C receives the bracketed timing events and uses
   them to calculate end-to-end latencies. It also does all the aggregation
   and produces results for latency.

   You can also use this framework without testing latency, just for
   controlling the event rate into the correlator under test by setting the
   latency sampling rate to 0.

   Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$ 
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

## Files

  Sample contains:

    README.md                This file
    latency.mon              Event definitions
    sender.mon               Rate-gating sender monitor
    forwarder.mon            Monitor that forwards timing events
    receiver.mon             Timing and aggregation monitor
    user-events.mon          Sample user event definitions
    user-forwarder.mon       Sample user-event forwarding monitor
    user-application.mon     Sample user application
    user-events.evt          Sample user events
    sample-output.txt        Samples events produced by the analysis correlator

## Usage

   From the sample's directory you need to start three correlators. Connect correlator A to correlator B
   on all channels. Connect correlator B to correlator C on channel `com.apama.latency.monitor`.
   Each correlator's output will be available in the corresponding file passed on their command line.
   
      > correlator -p 15903 -f sender-correlator.log
      > correlator -p 15904 -f application-correlator.log
      > correlator -p 15905 -f analysis-correlator.log
      > engine_connect -tp 15904 -sp 15903
      > engine_connect -tp 15905 -sp 15904 -c com.apama.latency.monitor

   Inject files to the following correlators:

   Correlator A (replacing user-events.mon with your event types): 

       > engine_inject -p 15903 "$APAMA_HOME/monitors/TimeFormatEvents.mon" latency.mon sender.mon user-events.mon

   Correlator B (replacing user-events.mon with your event types, and user-application.mon with your application monitors): 

       > engine_inject -p 15904 latency.mon forwarder.mon user-events.mon user-application.mon

   Correlator C:
   
       > engine_inject -p 15905 "$APAMA_HOME/monitors/TimeFormatEvents.mon" latency.mon receiver.mon 

## Configuration

   The forwarding monitor also provides configuration in a LatencyRate event:

      event LatencyRate
      {
         integer hz;
         integer probability;
      }

   Where: hz is the sending rate (events per second), and 
   probability is the fraction of events used for testing latency (0 = don't
   test latency. 100 = test 1 in every 100 events, randomly selected)

   If the test needs to vary the rate then it is probably best to restart it
   with a new rate, however, you can send another LatencyRate event into the
   sender correlator which will reset all the counts and send subsequent
   events with the new rate and sample rate. 
   
   If a new rate is required, please edit it in user-event.evt
   and change the values within the brackets:
   com.apama.latency.LatencyRate(1000, 1), where in this example 1000 represents hz
   and 1 the probability;
   
## Output

   You need to receive on `com.apama.latency.results` from Correlator C:
   
      > engine_receive -p 15905 -c com.apama.latency.results

   Finally, update user-events.evt with at least 1000 events and send it to correlator A
   Do the following from the sample's directory in a new terminal:

      > engine_send -p 15903 user-events.evt
   
   It will produce the following events from the engine_receive (as seen in sample-output.txt):

      event LatencyResult
      {
         float mean; // average latency of those sampled
         float stddev; // latency deviation of those sampled
         integer count; // number sampled
         float min; // minimum latency sampled
         float max; // maximum latency sampled
         float nth; // Nth percentile sampled latency (aka 'the most useful number', defaults to 95%)
      }

   By default the framework will track the 95th percentile latency and output
   that. This can be changed by sending the following event into correlator C,
   after injecting receiver.mon:

      com.apama.latency.LatencyResultConfig(0.99)

   This should be a floating point number between 0 and 1.0 and will report
   the latency under which that percentage of events sampled fall. Other good
   numbers to select are 0.99 and 0.9.

   Each latency sample will produce a results output, but that includes the
   results aggregated over all the previous samples, so only the last one will
   be of interest.

## Contexts

   This explicitly only tests single-context applications. If you don't have
   everything in the main context then the bracketing events may be processed
   before your application has finished processing, yielding incorrect
   results.


