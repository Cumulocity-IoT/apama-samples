# Sample for using the HTTP server transport connectivity plug-in with EPL-supplied responses

## DESCRIPTION
   
   Demonstrates use of the HTTP server transport capability to generate responses
   in EPL. The sample implements a generic Create Retrieve Update Delete (CRUD) 
   store in the correlator which is accessed via a REST API. This can be accessed
   programmatically with any REST client or through a trivial built-in web form
   which is also served from the server. There is a matching swagger file for 
   auto-generating API code.
   
   The sample can be deployed on the command line or
   as a Docker container. There is a test suite written in Pysys.  
   
   Copyright (c) 2018-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

## FILES

  Sample files are:

    README.md                                             This file.
    config/connectivity/HTTPServer/HTTPServer.yaml        Connectivity plugins configuration for the HTTP server and mapping rules.
    config/connectivity/HTTPServer/HTTPServer.properties  Properties used by the above configuration which can be overridden at run-time.
    config/connectivity/HTTPServer/index.html             Simple web form for interacting with the sample.
    config/connectivity/HTTPServer/swagger.json           Swagger definition of the REST API for generating API code in various languages.
    eventdefinitions/CRUDEvents.mon                       Event definitions which the mapping configuration creates and consumes, for use in the CRUD monitor.
    monitors/CRUDHandler.mon                              EPL code for managing the CRUD database in response to the events from the HTTP server.
    tests/TestCRUD                                        Pysys-based test for the sample.
    Dockerfile                                            Instructions to deploy this sample as a Docker container.
    
## RUNNING THE SAMPLE

The sample can be run using the engine_deploy tool:
 
    $ source `bin/apama_env`
    $ cd $APAMA_HOME/samples/connectivity_plugin/application
    $ engine_deploy --outputDeployDir deployed httpserver-responses
    $ correlator --config deployed

As soon as correlator status lines start to appear there, you will be able to connect a browser to "http://localhost:8080/" to access the sample.

The simple web form allows you to create, update and delete resources. To update and delete you will need to know the path for the resource, which will be returned when you create it. This can also be used as the URL to get the current value of the resource. For example, if the path is "/objects/1" then you can get the resource with "http://localhost:8080/objects/1".

You can also interact with the sample via command line tools such as curl:

 * To create a resource: curl -H "Content-Type: application/json" -X PUT http://localhost:8080/create -d "{\"name\":\"foo\"}"
 * To read a resource:   curl http://localhost:8080/objects/1
 * To update a resource: curl -H "Content-Type: application/json" -X PUT http://localhost:8080/objects/1 -d "{\"name\":\"bar\"}"
 * To delete a resource: curl -X DELETE http://localhost:8080/objects/1

In addition, you can build and deploy a Docker container using the project. The project has already had support for Docker added to it, so to deploy it you just need an environment with access to the Docker command-line tools configured for a Linux-based Docker host:

    $ docker build -t crud-sample httpserver-responses
    $ docker run --rm -d -P --name crud-sample crud-sample
    
This will start a background docker container. You can manage it with several docker commands:

    $ docker logs crud-sample # access the correlator logs
    $ docker port crud-sample # show the external port which port 8080 has been mapped to
    $ docker stop crud-sample # shutdown and delete the container
    
To access the sample in the running container, you will need the host of your docker server and the external port which docker chose to map our application to, which you can get from the docker port command above. With those details you can use any of the above methods to access the sample.

## GENERATING APIS WITH SWAGGER

The Sample also comes with an OpenAPI specification per the definition at http://swagger.io. You can use any compatible tool, including the ones provided by swagger.io, to consume this API description and create bindings for different languages. You can either use the file provided within the sample, or you can access it from the running correlator by using the URL http://localhost:8080/swagger.json

## RUNNING THE TESTS

To run the tests you must first deploy the project, then run the tests from within the deployment:

    $ engine_deploy --outputDeployDir deployed httpserver-responses
    $ cd deployed/tests
    $ pysys run
    
Alternatively, when you build the Docker container, the tests will be run as part of the validation of the image. If the tests fail, building the image will also fail.

Pysys will expect a Python3 installation, and if there isn't one Apama\third_party\ you may need to install it, and include its location in PATH. 

## SAMPLE OUTPUT

Substitute the appropriate host and port depending on your start method. These commands use curl as an example, but you can run the equivalent using any other method.

    $ curl -H "Content-Type: application/json" -X PUT http://localhost:8080/create -d "{\"name\":\"Resource 1\", \"value\": 42}"
    Successfully added new object at /objects/1
    
    $ curl -H "Content-Type: application/json" -X PUT http://localhost:8080/create -d "{\"name\":\"Resource 2\", \"value\": 666}"
    Successfully added new object at /objects/2
    
    $ curl http://localhost:8080/objects/1
    {"name":"Resource 1","value": 42}
    
    $ curl -H "Content-Type: application/json" -X PUT http://localhost:8080/objects/1 -d "{\"name\":\"Resource 1\", \"value\": 3.14}"
    Successfully updated object at /objects/1    
    
    $ curl http://localhost:8080/objects/1
    {"name":"Resource 1","value": 3.14}
    
    $ curl -X DELETE http://localhost:8080/objects/1
    Successfully deleted object at /objects/1
    
    $ curl http://localhost:8080/objects/1
    No object at /objects/1
    
