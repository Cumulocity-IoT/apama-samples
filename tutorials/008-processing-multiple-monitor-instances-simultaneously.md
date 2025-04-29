# Processing Multiple Monitor Instances Simultaneously (Contexts and Parallelism)

When you start a correlator it has a single main context.  
  
To implement parallel processing:  
  
1. Define a new context (in the monitor you want to spawn) using this syntax: **context("contextName")**  
  
2. In the spawn statement, specify that it should be **to context("contextName")**.  
  
3. In the spawned monitor, subscribe to the channels that the data is sent on (as we did for single context processing).  
  
4. To pass events between monitor instances in different contexts, use the send...to EPL command, instead of the route command.  
  
The **send...to** command sends the event to a particular context or a channel, and thus to any contexts subscribed to that channel. The send...to command places the sent event at the back of the target context's queue (whereas the route command places it at the front of the queue). Any active listeners will eventually receive an event that are sent (using send...to) once the events make their way to the head of the input queue alongside normal events.  
  
{{< c8y-admon-info >}}
If you need your EPL program to process events in a predictable, serial manner, you should use the route command to send events, not the send...to commands.  
{{< /c8y-admon-info >}}
  
The SensorMonitor in your current program is an example of a monitor which can implement parallel processing because it performs calculations for a number of events (Temperature or Pressure) that are of the same type, but that have different identifiers (Sensor IDs).  
  
In this lesson, you update your program so that the correlator executes multiple instances of the SensorMonitor concurrently. To implement this behavior, you'll create a new context for each Sensor ID and create a new monitor instance in each context. You'll then use send...to statements (instead of route statements) to pass the Alert events between contexts. 

### Create a new context for each sensor

1. Open the **SensorMonitor.mon** file.  
  
2. Update the spawn statement to spawn to a new context, which is created using the **context** type. Change the SensorMonitor's spawn statement to include the **to context** option:  
  
spawn monitorSensor(sensor.sensorId, sensor.targetTemperature, sensor.targetPressure) **to context (sensor.sensorId);**  
  
{{< c8y-admon-info >}} 
In this line of code, **context(...)** creates a new context and spawns a new monitor instance into that context for each AddSensor event.  
{{< /c8y-admon-info >}}
  
3. Save your changes.  
Note that because each spawned monitor is listening to the Temperature and Pressure events, each context will receive the events that it needs.

### Send Alert events to all contexts

1. In the **monitorSensor()** action, update the temporal listener so that it sends the Alert events to the **AlertManager** channel, instead of routing them.

```
Change:    
route Alert("TEMP_PRESSURE_RISE", sensorId, temperature.temperature, pressure.pressure);  
to:  
send Alert("TEMP_PRESSURE_RISE", sensorId, temperature.temperature, pressure.pressure) to "AlertManager";  
```

2. In the **AlertManager** monitor, subscribe to the **AlertManager** channel.  

```
action onload () {  
    monitor.subscribe("AlertManager");  
    on all Alert() as a {  
        print a.toString();  
    }
}
```
3. Save your changes and run the project. 

You should see **Alert** messages for the S001 and S003 sensors.  
  
#### You can import the completed solution for this lesson:  

In the [Apama Samples](https://github.com/Cumulocity-IoT/apama-samples) Git repository, navigate to the **tutorials/EPL_Fundamentals_Completed** directory.