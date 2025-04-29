# Using Event Stream Processing Constructs

In the previous lesson, you wrote a simple program that listened for 
events that are all of the same type.

This lesson will introduce the concept of EPL **streams**, showing how a simple stream can be created from incoming events, and how EPL **stream queries** provide a simple and concise way to enrich incoming data using powerful relational operators such as joins, aggregation, projection, and sliding window calculations.  

### Calculate average temperature readings over the last minute
  
You will start by creating a stream from Temperature events coming into the correlator, and then add a stream query that uses this stream to calculate an average value. 

1. Open the **SensorMonitor.mon** file, and delete the existing contents of the ``` onload ``` action except ``` monitor.subscribe("Factory1") ``` statement(i.e. the Temperature event expression and listener you created in the previous lesson).

2. Declare a new variable in the 'onload' action, and initialize it to a stream of Temperature events defined by a simple Temperature event template:  

```
stream<Temperature> temperatures := all Temperature(sensorId="S001");
```

3. Then add a stream query that creates a 60 second sliding window of the most recent events from this stream:  
```
from t in temperatures within 60.0 
select Temperature("S001", mean(t.temperature)) as temperature {
    print temperature.toString();
} 
```
This query uses the built-in aggregation function 'mean' to calculate the average value and put it into a new Temperature event. Every time the stream produces a new Temperature event it is assigned to the variable 'temperature' and passed to the following code between the braces {...}.

4. Allow the 'mean' aggregation function to be known by its unqualified name by adding a 'using' statement at the top of the source file: 

```
using com.apama.aggregates.mean; 
```

Save SensorMonitor.mon, and run the project.(inject monitors and sent the event mon) You should see the averaged temperatures for S001 in the Temperature events printed to the terminal. 
<br>
If you wait for 1 minute (i.e. until the end of the query window period), you may notice that the average temperature value changes to 'NaN'. Because there are no temperature values left in the window, the average value cannot be calculated so it is shown as 'NaN', the floating point value that represents 'not a number'.

### Calculate average pressure readings over the last minute
Following the pattern above, create another stream query using Pressure events. 

1. In the SensorMonitor, subscribe to the **Factory1** channel.

2. At the end of the 'onload' action, add a new stream query that will perform the same task as the previous temperature query, but using Pressure events instead:  
```
stream<Pressure> pressures := all Pressure(sensorId="S001");
from p in pressures within 60.0
select Pressure("S001", mean(p.pressure)) as pressure
{
  print pressure.toString();
}
```

Save SensorMonitor.mon, and start the project. <br>
You should now see average pressures as well as temperatures for S001 in the Terminal. 

### Create a stream combining the temperature and pressure averages

This step will introduce a more complex query that combines values from two separate streams - the average temperature and 
average pressure - into one. Combining streams this way is known as a 'cross join'.

1. Open **eventdefinitions/SensorMonitorEvents.mon**, and add a new event type for the combined average readings: 
```
event AverageReadings {
  string sensorId;
  float meanTemperature;
  float meanPressure;
}
```
Save the file.

2. Open **monitors/SensorMonitor.mon** again. At the end of the 'onload' action, create streams for the average temperature and pressure values. These streams will only contain the raw 'float' values that are actually required (i.e. they are not wrapped in new Temperature or Pressure events as before):  
```
stream<float> meanTemperatures := from t in temperatures within 60.0 select mean(t.temperature);
stream<float> meanPressures := from p in pressures within 60.0 select mean(p.pressure);
```
3. Add a stream query with two 'from' clauses, to join the values from both streams:  
```
from avgT in meanTemperatures retain 1 
from avgP in meanPressures retain 1
select AverageReadings("S001", avgT, avgP) as averageReadings 
{
  print averageReadings.toString();
} 
```
The 'retain' keyword introduces a size-based window (similar to the time-based 'within' window you used earlier). In this case the retain window contains only 1 item, allowing us to combine exactly one 'most recent' average value from each stream.

Note that the entire query could have been written in the following single statement (there's no need to make this change to the code): 
```
from avgT in 
   from t in all Temperature(sensorId="S001") 
      within 60.0 
      select mean(t.temperature)
   retain 1
from avgP in 
   from p in all Pressure(sensorId="S001") 
      within 60.0 
      select mean(p.pressure) 
   retain 1
select AverageReadings("S001", avgT, avgP) as averageReadings 
{
   print averageReadings.toString();
} 
```
Save SensorMonitor.mon, and run the project. You should now see the AverageReadings events produced by the new stream in the terminal, every time a new Pressure or Temperature is received.
```
Temperature("S001",90)
Pressure("S001",750)
AverageReadings("S001",90,750)
Temperature("S001",95)
AverageReadings("S001",95,750)
Pressure("S001",775)
AverageReadings("S001",95,775)
Temperature("S001",100.33333333333333)
AverageReadings("S001",100.33333333333333,775)
Pressure("S001",800)
AverageReadings("S001",100.33333333333333,800)
Temperature("S001",105.25)
AverageReadings("S001",105.25,800)
Pressure("S001",825)
AverageReadings("S001",105.25,825)
```

#### You can import the completed solution for this lesson:  

In the [Apama Samples](https://github.com/Cumulocity-IoT/apama-samples) Git repository, navigate to the **tutorials/EPL_Fundamentals_Completed** directory.