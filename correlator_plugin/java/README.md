# Sample correlator plugins using the Java Plugin Development Kit


## DESCRIPTION

   Library source and associated EPL programs to demonstrate the
   development of EPL plugins written in Java. 
   

   Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 


## FILES

  Sample files are:

    README.md                This file
    build.xml                 ANT build file for all plugins
   
    SimplePlugin.java         Source for the simple example, using the Java API
    SimplePlugin.mon          EPL code for the Java simple example 
    SimplePluginSample.txt    Reference output for the simple plugin
   
    ComplexPlugin.java        Source for the complex example, using the Java API
    ComplexPlugin.mon         EPL code for the Java complex example
    ComplexPluginSample.txt   Reference output for the complex plugin
    
    SendPlugin.java           Source for the send example, using the Java API
    SendPlugin.mon            EPL code for the Java send example
    SendPluginSample.txt      Reference output for the send plugin

    SubscribePlugin.java      Source for the subscribe example, using the Java API
    SubscribePlugin.mon       EPL code for the Java subscribe example
    SubscribePluginSample.txt Reference output for the subscribe plugin


## BUILDING THE SAMPLES

   It is recommended that you copy this sample to an area of your APAMA_WORK 
   directory rather than running it directly from the installation directory. 

   The Java samples use the Java EPL plugin API. 
   
   Note also that Apache ANT is also required to build the plugins using the
   supplied build.xml.

   Run ant in the current directory:

     $ ant

   A successful build will produce four output files (and some intermediate compilation files):

      simple_plugin.jar
      complex_plugin.jar
      send_plugin.jar
      subscribe_plugin.jar
   
## RUNNING THE SAMPLES

   Running of the samples requires access to the Correlator and Apama command 
   line tools. 

   To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the bin/apama_env script has been sourced. 
   Unix, sourced). 
   
   1. Start the Apama Correlator by
      running:

      > correlator -j

   2. In a new terminal navigate to the folder where you built it,
	  and inject the simple_plugin test JAR, followed by the corresponding EPL code:
   
      > engine_inject -j -v simple_plugin.jar
      > engine_inject -v SimplePlugin.mon
      
   3. Repeat the previous step with the other plugins.

      
## SAMPLE OUTPUT

   SimplePlugin.mon should produce output similar to the file SimplePluginSample.txt

   ComplexPlugin.mon should produce output similar to the file ComplexPluginSample.txt

   SendPlugin.mon should produce output similar to the file SendPluginSample.txt

   SubscribePlugin.mon should produce output similar to the file SubscribePluginSample.txt

