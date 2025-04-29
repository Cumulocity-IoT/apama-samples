# Monitoring Complex Events

In this lesson, you will extend your simple EPL program to listen for more complex events. You will update it to monitor the fictional industrial process's event data for both Temperature and Pressure values that fall within specific ranges, and within a specified time frame.

In a previous lesson you created a simple listener that matched all Temperature events from a particular sensor. Follow these steps to create a listener that matches only high temperature readings:  
  
1. Open **SensorMonitor.mon** and delete the contents of the 'onload' action created in the previous lesson except ``` monitor.subscribe("Factory1") ```
  
2. At the beginning of the **SensorMonitor** monitor, define a float variable called **targetTemperature** to hold the Target Temperature value (in Celsius) and set its value to 100.0:  
```
float targetTemperature:=100.0;
```  
3. Add a listener under 'onload' action that monitors the events for the S001 sensor for a **temperature** values greater than or equal to the mean, multiplied by 1.10):  
```
on all Temperature (sensorId="S001", temperature >= targetTemperature * 1.10) as temperature  
{  
}  
```  
{{< c8y-admon-info >}}
This step makes use of the Temperature event definition (in **eventdefinitions/SensorMonitorEvents.mon**) that were defined in a previous lesson.  
{{< /c8y-admon-info >}}
  
4. Between the ```{...}``` braces in the Temperature listener action, add a print statement to indicate that the value is a high temperature reading:  
```
print "TEMP_HIGH:" + temperature.toString();  
```  
5. Save your changes and run it.

You should see two **TEMP_HIGH** messages.  
  
Using the Temperature listener as an example, create a similar listener for **Pressure** events:  
  
1. Create an monitor-scope, float variable called **targetPressure** (for target Pressure) with an initial value of 800.0 pressure.  
  
2. Create a listener so that it monitors the events for the S001 sensor for a pressure reading that is greater than or equal to the target, multiplied by 1.10, and then use 'as' operator to copy the matched event to the **pressure** variable.  
  
{{< c8y-admon-info >}}
This step makes use of the Pressure event definition (in **eventdefinitions/SensorMonitorEvents.mon**) that was defined in a previous lesson.  
{{< /c8y-admon-info >}}
  
3. Add a print statement.  
  
4. Save your changes.  
  
Your program now has two listeners: one monitors the events (of a single sensor) for high temperature readings, the other listener does the same for a high pressure readings.  
  
{{< c8y-admon-info >}} These two listeners are running concurrently in the same monitor. EPL permits multiple concurrent listeners running in the same monitor or different monitors.  
After you complete the "Update the Temperature listener" and "Add a listener for Pressure events" lessons, 
{{< /c8y-admon-info >}}
Your EPL program should look like this:  
```
monitor SensorMonitor {
    float targetTemperature:=100.0;
    float targetPressure:=800.0;
    action onload() {
        monitor.subscribe("Factory1");
        // Temperature high rule
        on all Temperature (sensorId="S001", temperature >= targetTemperature * 1.10) as temperature {
            print "TEMP_HIGH: " + temperature.toString();
        }
        // Pressure high rule
        on all Pressure (sensorId="S001", pressure >= targetPressure * 1.10) as pressure {
            print "PRESSURE_HIGH: " + pressure.toString();
        }
    }
}
```

1. Create a third listener that adds another level of complexity, by looking for a high temperature reading followed by a high pressure reading within a specified period of time (3 seconds):  
```
on all Temperature (sensorId="S001", temperature >= targetTemperature * 1.02) as temperature -> 
Pressure (sensorId="S001", pressure >= targetPressure * 1.05) as pressure within (3.0) { 
    print "TEMP_PRESSURE_RISE: " + temperature.toString() + " " + pressure.toString(); 
}
```
2. Save your changes.  
  
{{< c8y-admon-info >}}   
The first line of this temporal listener introduces the followed-by operator, denoted by ->. The followed-by operator allows pattern matching for sequential events.  
In this case, there is also a **within** clause, which specifies that two sequential events must be detected within a set amount of time. 

{{< /c8y-admon-info >}}

3. Run the project.  
  
You should now see three types of messages: **TEMP_HIGH**, **PRESSURE_HIGH**, and **TEMP_PRESSURE_RISE**.  
  

#### You can import the completed solution for this lesson:  

In the [Apama Samples](https://github.com/Cumulocity-IoT/apama-samples) Git repository, navigate to the **tutorials/EPL_Fundamentals_Completed** directory.