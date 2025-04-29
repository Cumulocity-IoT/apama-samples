# Defining Event Types

**Events** are the fundamental elements for information exchange in an event processing application. An event captures important information about the state (or changes to state) of some entity. Examples of events include stock market trades and quotes, ATM transactions, radio-frequency identification (RFID) signals, and telemetry readings.  
  
The **event correlator** is Apama's core event processing and correlation engine. The correlator monitors input events and produces output events.  
  
Before you send event data to the correlator, your application needs to define the types (or structures) of events so that the correlator can interpret the events correctly when it receives them.

A typical Apama application processes streaming data coming from real-time custom or standards-based data feeds.  
  
For the purposes of this tutorial, the sample application you build will monitor simulated temperature and pressure telemetry readings (input events) from a fictional industrial process.  
  
**In this first lesson**, you will define the fields and data types for the temperature event and pressure event input from a simulated data feed.  
1. Git clone [Apama Samples](https://github.com/Cumulocity-IoT/apama-samples) repository, then open the **apama-samples/tutorials/EPL_Fundamentals** project in VS Code.
  
2. Right-click on the **eventdefinitions** folder and Create new file **SensorMonitorEvents.mon**.  
  
Here is the **syntax for an event type definition**:  

```
event <EventType> {  
  <fieldtype> <fieldName>; 
  ...  
}
```
  
The industrial process's **Temperature** readings have the following event fields in this order:  
* sensorId (string)  
* temperature (float)  
  
1. Add the following event definition for **Temperature** events in the **SensorMonitorEvents.mon** file:  
  
```
event Temperature {
    string sensorId;
    float temperature;  
}
```
  
2. Press **CTRL+S** to save your changes.  
  
{{< c8y-admon-info >}}
* Event and field names are **case-sensitive** and fields are strongly typed. You must use the names and types shown here for use later in this tutorial.  
* Field order is also important when you define an event - the order of fields in the event type definition must match the order of fields input from the incoming events (in this case, the SensorTest.evt file).  
* The **sensorId** is a unique identifier for the sensors in the fictional industrial process. The field **temperature** identifies the temperature in Celsius degrees. The field **pressure** identifies the pressure in kilopascals.
{{< /c8y-admon-info >}}
1. In the **SensorMonitorEvents.mon** file, after the definition of the Temperature event type, add a separate event definition for the **Pressure** event type.  
  
2. Save your changes. The contents of the **SensorMonitorEvents.mon** file should now look like this:  

```
/*  
 * Input temperature reading.  
 */  
event Temperature {  
    string sensorId;  
    float temperature;  
}

/*  
 * Input pressure reading.  
 */  
event Pressure {  
    string sensorId;  
    float pressure;  
}  
```
  
{{< c8y-admon-info >}} The **/\*\* Comment \*/** pattern is the recommended way to document the purpose and usage of event definitions in EPL. {{< /c8y-admon-info >}}
  
{{< c8y-admon-info >}} The **sensorId** is a unique identifier for the sensors in the fictional industrial process. The **targetTemperature** and **targetPressure** fields provide the mean values for temperature and pressure.
{{< /c8y-admon-info >}}  
  
1. In the **SensorMonitorEvents.mon** file, after the definition of the Pressure event type, add a separate event definition for the **AddSensor** event type. The AddSensor events have the following fields in this order: sensorId (string), targetTemperature (float), and targetPressure (float).  
  
2. Save your changes. The contents of the **SensorMonitorEvents.mon** file should now look like this:  

```
/*  
 * Input temperature reading.  
 */  
event Temperature {  
    string sensorId;  
    float temperature;  
}

/*  
 * Input pressure reading.  
 */  
event Pressure {  
    string sensorId;  
    float pressure;  
}  
  
/*  
 * Sensor configuration.
 */ 
event AddSensor {  
    string sensorId;  
    float targetTemperature;  
    float targetPressure;  
}
```
  
Now that you have added some event definitions, temperature and pressure telemetry readings (for the fictional industrial process that your sample application will monitor) are simulated via the **SensorTest.evt** file.  
  
1. Expand the **events** folder.  
  
2. open **SensorTest.evt** file.  
  
3. Look at the format of the data in this file. There are three types of events, corresponding to the events you have just defined: **AddSensor**, **Temperature**, and **Pressure**.  
  
Notice how the Temperature and Pressure events are each preceded by a string ("Factory1" or "Factory2"). This optional string defines the name of the channel that the event is sent on.  
  
{{< c8y-admon-info >}} 
The BATCH lines in the .evt file send events to the correlator in groups, timed in milliseconds. 
{{< /c8y-admon-info >}}
  
4. **Close** the **SensorTest.evt** file.
