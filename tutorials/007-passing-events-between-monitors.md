# Passing Events Between Monitors (Routing)

In this lesson, you update your program so that the SensorMonitor creates new derived events from the incoming events and routes these derived events (Alerts) to a new monitor called AlertManager for further handling. 

### Add an event definition

You must define any event that you want the correlator to process - whether the event originates from an external source or is routed internally by your EPL program.  
  
Your program will generate an alert message (output event) if it encounters temperature or pressure values that exceed normal parameters.  
  
Follow these steps to define an event type definition for these Alert events:  
  
1. Open the **eventdefinitions/SensorMonitorEvents.mon** file.  
  
2. Add an event definition for the **Alert** event type with the following fields, in the following order:  
  
* type (string)  
* sensorId (string)  
* temperature (float)  
* pressure (float)  
  
{{< c8y-admon-info >}} 
The **type** field is a textual description of the alert condition. The **sensorId** is a unique identifier for the sensors in the fictional industrial process. The fields **temperature** and **pressure** identify the temperature in celsius and the pressure in kilopascals.  
{{< /c8y-admon-info >}}
  
3. Save your changes. To edit the SensorMonitor to route Alert events:  

### Route an event

To edit the SensorMonitor to route Alert events

1. Open the SensorMonitor.mon file.  
  
2. Change the print statement to route statement as follows:  

```
Change:  
print "TEMP_PRESSURE_RISE: " + temperature.toString() + " " + pressure.toString();  
To:  
route Alert("TEMP_PRESSURE_RISE", sensorId, temperature.temperature, pressure.pressure);  
```
3. Save the file. 

### Create a monitor to listen for the routed event

Create a new monitor, in a separate file, that listens and acts on the Alert events:  
  
1. Right-click on the **monitors** folder and select new file.  
  
2. Enter "AlertManager.mon" for the file name.  
  
3. Edit the generated monitor as below so that it listens for the routed Alert event and prints it:  

```
monitor AlertManager {  
    action onload () {  
        on all Alert() as a {  
            print a.toString();  
        }  
    }  
}  
```
5. Save your changes.  

6. Run the project.  
  
You should see **Alert** messages for the S001 and S003 sensors.  
  
#### You can import the completed solution for this lesson:  

In the [Apama Samples](https://github.com/Cumulocity-IoT/apama-samples) Git repository, navigate to the **tutorials/EPL_Fundamentals_Completed** directory.