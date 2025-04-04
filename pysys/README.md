# Sample PySys tests for Apama

## Description

   A selection of PySys testcases which demonstrate the various uses of 
   the Apama extensions to PySys

## Copyright notice

   Copyright(c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

## Running the tests

  It is recommended that you copy this sample folder to an area of your 
  APAMA_WORK directory rather than running it directly from the installation 
  directory. 
   
  The tests must be run in an Apama environment - so "source" the
  `bin/apama_env` script. Then simply run the pysys command to execute the 
  tests:

      > pysys run

  To print the set of tests available execute this command:

      > pysys print


  Each subdirectory of this one represents a single test. In each of 
  those directories there is a pysystest.py file which contains python code 
  which is executed to run the test, under the management of the PySys
  framework.


## EPL code coverage reporting

   To enable EPL code coverage while running PySys tests, set the codeCoverage 
   property, e.g.:
    
      > pysys run -XcodeCoverage
    
  This causes every Correlator that PySys starts to write a ".eplcoverage" 
  file during shutdown, and at the end of the test run automatically runs the 
  epl_coverage tool to generate a merged.eplcoverage file from all executed 
  test coverage files, and an HTML report showing which executable lines 
  were exercised by one or more testcases. This report can be very useful for 
  spotting any code paths that are not adequately covered by the existing 
  tests. 
   
  Note: coverage report generation relies on the Correlator being started 
  from PySys, and shutdown cleanly at the end of the testcase rather than 
  killed. 
