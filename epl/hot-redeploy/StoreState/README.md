# Sample application to illustrate upgrading a monitor version while preserving state

## DESCRIPTION

   An application which contains 2 versions of a monitor that process input
   Process() events.  It shows how an upgrade from v1 to v2 could be performed
   and how state can be stored and restored from MemoryStore.
   
## COPYRIGHT NOTICE

   Copyright (c) 2017-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

## FILES

  Sample contains:

    README.md                   This file.
    Input/Events.mon            The EPL event definitions used by this application.
    Input/Simulator.mon         An input event simulator.
    Input/Upgrader.mon          Helper to upgrade an applications state.
    Input/Counter_v1.mon        Version 1 of this sample application.
    Input/Counter_v2.mon        Version 2 of this sample application.
    run.py                      PySys test to run sample.


## SAMPLE EXPLANATION

   This sample is driven by a PySys test in run.py and reviewing this file is
   recommended.  It does the following steps:
   
   1. Starts a correlator.
   2. Injects required base EPL services (MemoryStore, Simulator,
      Upgrader and Events).
   3. Injects v1 of the Counter application and sends some 'Create()' events
      to create some monitor instances. These process input events which are
      sent from the simulator, and increments an integer count by 1.
   4. Waits 5 seconds for v1 processing.
   5. Stops the input events and flushes all queues.
   6. Injects v2 of the Counter application which will send an Upgrade()
      event. This informs the v1 monitor that an upgrade is happening and it
      should save its state and its listeners should be destroyed.
   7. Once v1 has finished processing Upgrade() it will iterate the
      MemoryStore table and load each monitors state. The v2 monitors take over
      the processing of the input events from v1, and increment a decimal
      counter by a random amount.
   8. Starts the input events.
   9. Waits 5 seconds for v2 processing.
   

## RUNNING THE SAMPLE

   Running of the samples requires access to the correlator and Apama command 
   line tools.
   
   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 
      
   This sample uses the PySys test framework to manage starting correlators,
   injecting code, sending events and setting up receivers.

   1. Execute 'pysys run'.
   2. The sample takes ~10s to run.


## SAMPLE OUTPUT

   A file will be created called `Output/<platform>/correlator.out`.  This file
   will show when an input event is processed and when a monitors state is
   stored and recovered.
   
   You should see the first 5 seconds of events being processed with counts
   being incremented by 1.  Then the update begins, the v1 state is stored,
   v2 restores the saved state and continues to process the input events.
   V2 will be incrementing the now decimal count by a different amount.
