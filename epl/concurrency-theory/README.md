# Simple banking implementation to show concurrency issues


## DESCRIPTION

   There are multiple implementations of a simple consumer bank. 
   Two implementations suffer from simple concurrency design flaws,
   and one implements a bank correctly.

   See the section "Samples for implementing contexts" in the Apama 
   EPL documentation.


## COPYRIGHT NOTICE

   Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 


## FILES

  Sample files:

    README.md              This file
    Input/BankCommon.mon   Common event definitions
    Input/Bank-*.mon       Various implementations of a bank
    Input/*.evt            Event files used by run_sample.*
    run.py                 PySys test to run sample


## RUNNING THE SAMPLE
  
   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been run sourced. 
      
   This sample uses the PySys test framework to manage starting correlators,
   injecting code, sending events and setting up receivers.

   1. Execute 'pysys run -m <mode>' where <mode> is one of
         "Race", "Deadlock", "CompareSwap" or "ALL", for example:

      > pysys run -m Race
    

## SAMPLE OUTPUT

   The output of the PySys test run will show the 
   events showing what money has been transferred, and 
   the value of the accounts after all transactions.
   The correct numbers, as shown by the compareswap
   implementation, are:

     customera: 50, customerb: 125, customerc: 125

