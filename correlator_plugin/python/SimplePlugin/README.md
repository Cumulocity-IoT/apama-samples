# Simple application that shows how to invoke a python plugin 


## DESCRIPTION

   A simple application that calls a method on a python plugin to generate a UUID and return it to EPL.
      
## COPYRIGHT NOTICE

   Copyright (c) 2018-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this 
   file except in compliance with the License. You may obtain a copy of the License at
   http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software distributed under the
   License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
   either express or implied. 
   See the License for the specific language governing permissions and limitations under the License.

### FILES

  Sample files:

    README.md                       This file
    SimplePluginSample.txt          An example of the what the output is expected to look like after 
                                    running the application.
    monitors/SimplePlugin.mon       The monitor that calls out to the python plugin and logs the 
                                    returned UUID.
    plugins/SimplePlugin.py         The python file that represents the python plugin which has a 
                                    method to generate a UUID.
    config/CorrelatorConfig.yaml    A YAML configuration file to initialize the correlator with the 
                                    python plugin settings.


## RUNNING THE SAMPLE
   
   1. Copy this project directory to a suitable location such as APAMA_WORK. 
   
   2. To ensure that the environment is configured correctly for Apama, all the 
   commands below should be executed from a shell where the `bin/apama_env` script 
   has been sourced, at the top level of the project.
   
   3. Start the correlator, specifying configuration file CorrelatorConfig.yaml, 
   for example:

   > correlator --config config/CorrelatorConfig.yaml
   
   4. This will launch the correlator with the python plugin details. you can then inject 
   the sample EPL application. This will cause it to call the Python plugin method generateUuid.
   
   > engine_inject monitors/SimplePlugin.mon
   
   Any output will be written to the correlator's log which is 
   displayed on its standard output by default; see below for more details 
   on the expected output. 
   

## EXPECTED OUTPUT

Output will be similar to:

    apamax.simplepluginsample.SimplePluginMonitor [1] UUID generated: 285859c8-be46-49b5-bbb1-c256b003fdfd
   