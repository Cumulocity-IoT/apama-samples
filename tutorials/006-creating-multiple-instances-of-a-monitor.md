# Creating Multiple Instances of a Monitor (Spawning)

You spawn a monitor instance by specifying the **spawn** keyword followed by an action. When the correlator spawns a monitor instance, it does the following:  
  
1. Creates a monitor instance that is almost identical to the monitor instance that is spawning. The only difference is that the correlator does not clone any active listeners.  
  
2. Passes the current values of the monitor instance's variables to the new monitor instance.  
  
3. Executes the named action in the new monitor instance.  
  
By spawning, you can use a monitor as a template to create multiple monitor instances, and in this way you can scale your EPL application to handle a large number of events at one time.  

### Review the AddSensor event type definition
In this lesson, you will update your EPL program to monitor the temperature and pressure events for each sensor in the industrial process. In this tutorial's first lesson, you created event type definitions for three input events, coming from the **events/SensorTest.evt** file: **Temperature**, **Pressure**, and **AddSensor**.  
  
To review the data types and order of the AddSensor fields:  
  
1. Open the **eventdefinitions/SensorMonitorEvents.mon** event definition file.  
  
2. Note that the AddSensor events have the following fields: sensorId (string), targetTemperature (float), and targetPressure (float).  
  
3. Close the file. 

### Create a listener for AddSensor events

1. Open the **monitors/SensorMonitor.mon** file.  
  
2. Create a new action, below the onload() action, and call it **monitorSensor**.  
It will look like as :  
```  
action monitorSensor (string sensorId, float targetTemperature, float targetPressure) { }
```
  
3. Move the channel subscriptions and three listeners in the current action onload() construct into the new **monitorSensor** action.  
  
4. In the **onload()** action, create a new listener for all AddSensor events and assign the AddSensor events to the **sensor** variable. For the listener action, specify to spawn **monitorSensor**:  
```
on all AddSensor() as sensor {  
    spawn monitorSensor(sensor.sensorId, sensor.targetTemperature, sensor.targetPressure);  
}
``` 
5. Save your changes. 

### Update the spawned listeners to handle all sensors

1. In the **SensorMonitor** monitor, remove the targetTemperature and targetPressure monitor-level variables as this information is now coming from the event file.  
  
2. In each of the listeners, update the code so that instead of looking for a hard-coded ID, it looks for the value of the current AddSensor event. Change all instances of **sensorId="S001"** to **sensorId=sensorId.**  
  
{{< c8y-admon-info >}} Each spawned monitor instance, will listen for only those events whose sensor **sensorId** matches the value of the **sensor** variable.  {{< /c8y-admon-info >}}
  
3. Save your changes. 

The contents of the **SensorMonitor.mon** file should now look like this:  
```
monitor SensorMonitor {
    action onload () {
        // Spawn a monitor instance for each sensor
        on all AddSensor() as sensor {
          spawn monitorSensor(sensor.sensorId, sensor.targetTemperature, sensor.targetPressure);
        }
    }
    action monitorSensor (string sensorId, float targetTemperature, float targetPressure) {
        monitor.subscribe("Factory1"); 
        // Temperature high rule
        on all Temperature (sensorId = sensorId, temperature >= targetTemperature * 1.10) as temperature {
            print "TEMP_HIGH: " + temperature.toString();
        }
        // Pressure high rule
        on all Pressure (sensorId = sensorId, pressure >= targetPressure * 1.10) as pressure {
            print "PRESSURE_HIGH: " + pressure.toString();
        }
        // Temporal rule, temperature high followed by pressure high
        on all Temperature (sensorId = sensorId, temperature >= targetTemperature * 1.02) as temperature -> 
            Pressure (sensorId = sensorId, pressure >= targetPressure * 1.05) as pressure within (3.0) {
                print "TEMP_PRESSURE_RISE: " + temperature.toString() + " " + pressure.toString();
        }
    }
}
```
Save everything and run the project.  

In the terminal, you should see messages for sensor S003, as well as S001.  
  
#### You can import the completed solution for this lesson:  

In the [Apama Samples](https://github.com/Cumulocity-IoT/apama-samples) Git repository, navigate to the **tutorials/EPL_Fundamentals_Completed** directory.

An alternative approach to handling multiple concerns (for example, multiple Sensor IDs in the SensorMonitor monitor), is to use a dictionary . A **dictionary** is a data type that stores and retrieves data based on a key.  
  
Often the dictionary approach to handling multiple concerns is preferred to spawning. This is particularly true when the spawning approach includes an unmatched event expression, which is vulnerable to maintenance issues if someone else loads a listener for a pattern that you expect to have no other matches.  
  
There is also a lesson in this tutorial see [Using a Dictionary and Conditional Logic](/apama-tutorials/using-a-dictionary-and-conditional-logic/) that shows you how to use a dictionary in your sample program.