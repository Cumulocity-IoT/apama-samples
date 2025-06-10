# Basic sample for Cumulocity Connectivity

   This EPL sample demonstrates how to receive notifications from Cumulocity IoT using 
   the Notifications 2.0 mechanism. 
   
   The sample itself listens for measurements from the platform and creates alarms which will be displayed 
   in Cumulocity IoT if those measurements breach a threshold.

   Copyright (c) 2018-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
   Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.


## FILES

  Sample files:

    README.md                              This file
    BasicSample.mon                        EPL for communicating with Cumulocity IoT
    CumulocityIoTREST.properties           Configuration file connecting to Cumulocity IoT platform using REST interface
    CumulocityNotifications2.properties    Configuration file connecting to Cumulocity IoT platform using Notifications 2.0

## PREREQUISITES

   It is recommended that you copy this sample folder to an area of your APAMA_WORK directory rather
   than running it directly from the installation directory.

   To use Notifications 2.0 to connect to CumulocityIoT ensure that the Notifications 2.0
   feature flag is enabled on your tenant. (Notifications 2.0 is currently in private preview.
   If you would like to have it enabled for your tenant contact Cumulocity IoT Operations.)
   
   In addition, you will need to create the ROLE_NOTIFICATION_2_ADMIN global role and add it 
   to your user.

   Provide the credentials for connecting to Cumulocity IoT platform in the 
   config/connectivity/CumulocityClient/CumulocityIoTREST.properties file as outlined below.
   
   CUMULOCITY_SERVER_URL is the URL of the Cumulocity IoT platform that you log into including the
   http:// or https:// protocol prefix. You only need to set CUMULOCITY_AUTHORITY_FILE if you're using 
   a private instance whose certificate is not signed by a public authority.
   
   CUMULOCITY_USERNAME, CUMULOCITY_PASSWORD and CUMULOCITY_TENANT are the username, password and tenant 
   ID that are used to login to the Cumulocity IoT platform that was specified in CUMULOCITY_SERVER_URL.
   
   CUMULOCITY_APPKEY is the key of an application configured within Cumulocity. Creating an application 
   for testing purposes can be done by following these steps:
   
   * Log into Cumulocity IoT platform, 
   * Select Administration from the Application switcher,
   * Expand Ecosystem in the Navigator,
   * Select Applications,
   * Select Add application,
   * Select External application,
   * Provide a Name, Application key and External URL,
   * Click Save. 
     
   Once the application has been added, update CUMULOCITY_APPKEY to the Application key specified above.

   Provide additional details for connecting to Cumulocity IoT platform using Notifications 2.0
   in the config/connectivity/CumulocityClient/CumulocityNotifications2.properties file as outlined below.

   CUMULOCITY_NOTIFICATIONS_SUBSCRIBER_NAME is a unique alphanumeric name to identify the subscriber. 
   CUMULOCITY_NOTIFICATIONS_SUBSCRIPTION_NAME is the alphanumeric name of the subscription to use or create. 
   (These can be anything alphanumeric for the purposes of this sample.)
   

   The sample assumes the availability of a device which provides any of the following measurements and 
   can optionally support a display or a relay. The measurements are: 

   - Temperature Measurement (c8y_Temperature)
   - Analog Measurement (c8y_AnalogMeasurement)
   - Acceleration Measurement (c8y_MotionMeasurement)
   - Signal Quality Measurement (c8y_SignalStrength)

   For example, an Mbed Ublox device meets all the above requirements. Before starting, make sure you 
   have installed the Cumulocity Agent on this device, typically by downloading a suitable agent using 
   the device catalog from the Cumulocity GmbH Cloud website. You will also need to register the device id 
   (typically, the IMEI number) with the Cumulocity cloud. 
     
   Alternatively, a simulator can be used to generate measurements. A Simulator can be created by 
   following these steps:
     
   * Log into Cumulocity IoT platform, 
   * Select Device management from the Application switcher,
   * Expand Devices in the Navigator,
   * Select Simulators,
   * Select Add simulator from the Top menu bar,
   * Select Temperature measurements from the Presets dropdown,
   * Click Add, 
   * Select the newly created Simulator,
   * Select the Instructions tab,
   * If you want to generate alarms, modify some of the Create custom measurement steps for the
     simulator to generate values greater than MAX_TEMP that is defined in src/BasicSample.mon,
   * To view the relay being toggled on the device, switch to the simulator's Supported operations tab,
   * Click on Add custom operation and enter "c8y_Relay" for the operation type.
   * Select Simulators in the Navigator, 
   * Enable the simulator.
       
   When using a simulator, any alarms generated by the sample can be seen in the Alarms tab of the 
   device associated with the simulator (In the DeviceManagement app, select Devices, then All devices, and
   select the device associated with the simulator). Note: the Alarms tab for the simulator will
   only display any alarms relating to the simulator itself, not those generated by the application 
   as a result of any temperature measurements. To view the relay being toggled, select the Control
   tab for the device. Each toggling of the relay will be displayed in the Operation list.

## DESCRIPTION

   The sample performs the following tasks:                                                

   1. Receives temperature measurement fragments from Cumulocity IoT and generates alarms for devices 
      whose temperature exceeds MAX_TEMP. These alarms can be viewed in Cumulocity IoT. (Make sure the 
      fragment is the same in both simulator and sample.)
         
   2. Toggles a device relay in response to an alarm.

