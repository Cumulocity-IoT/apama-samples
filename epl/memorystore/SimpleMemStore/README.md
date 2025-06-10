# Simple sample to demonstrate the use of in-memory MemoryStore

## DESCRIPTION

   This sample demonstrates the basics of interacting with a MemoryStore. It shows how to get/set data,
   and how to react to out of date data when committing.

   The MemoryStore used is dictated by the event sent to the correlator. 


## COPYRIGHT NOTICE

   Copyright (c) 2018-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.


## FILES

  Sample contains:

    README.md                           This file
    SimpleMemStore.mon                  The sample EPL application
    ConfigureSample.evt                 The event file to trigger monitor functionality

## DETAILS

   The monitor SimpleMemStore reacts to a PrepareInMemory 
   event, which will create a MemoryStore, write a row of
   data to the store, reliably commit the data to the store and then print it.

   Calls to prepare both the Store and the Table are asynchronous, but any
   interactions with the table are then synchronous until you try and commit
   any data back to the store.
   
   The sample uses tryCommitOrUpdate to demonstrate how to recover if the row was
   modified by another process or context between getting the current values and
   committing the change. To demonstrate this, 3 modifications are performed in
   parallel by different EPL contexts. Messages are written to the log to indicate
   whether any retries were required for each context.

## RUNNING THE SAMPLE

      1. Source the `bin/apama_env` script.
      
      2. Inject the MemoryStore EPL and the EPL for this sample using:

         > engine_inject "$APAMA_HOME/monitors/data_storage/MemoryStore.mon"
         > engine_inject src/SimpleMemStore.mon

      3.  Send the event file to the correlator to initiate MemoryStore operations:
         
         > engine_send src/ConfigureSample.evt
         
   Once the above steps have been performed, the Correlator log file will 
   contain a series of messages showing interactions with the MemoryStore.
   
    
## SAMPLE OUTPUT


Sample output is:

    SimpleMemStore [2] Preparing in-memory MemoryStore: InMemoryStore.
    SimpleMemStore [2] Preparing table MyData.
    SimpleMemStore [2] Opening table MyData and performing first write.
    SimpleMemStore [5] *** Successfully committed changes to MemoryStore 'InMemoryStore' row 'my-example-key': {"stringfield":any(string,"Hello world"),"timestamp":any(float,1524582728.3),"value":any(integer,1)}
    SimpleMemStore [3] *** Retrying MemoryStore commit for 'my-example-key', attempt 2 for this modification
    SimpleMemStore [4] *** Retrying MemoryStore commit for 'my-example-key', attempt 2 for this modification
    SimpleMemStore [4] *** Successfully committed changes to MemoryStore 'InMemoryStore' row 'my-example-key': {"stringfield":any(string,"Hello world"),"timestamp":any(float,1524582728.3),"value":any(integer,2)}
    SimpleMemStore [3] *** Retrying MemoryStore commit for 'my-example-key', attempt 3 for this modification
    SimpleMemStore [3] *** Successfully committed changes to MemoryStore 'InMemoryStore' row 'my-example-key': {"stringfield":any(string,"Hello world"),"timestamp":any(float,1524582728.3),"value":any(integer,3)}
