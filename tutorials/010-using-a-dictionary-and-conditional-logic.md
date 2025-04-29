# Using a Dictionary and Conditional Logic

Your current EPL program sends all Alert events sent to it from the SensorMonitor. It does not do any checking of the Alert messages before it sends them to external receivers.  
  
In this lesson, you'll use a dictionary and an if condition to add some filtering to your program to avoid sending duplicate Alerts or Alerts that have lower telemetric readings than Alerts that have already been received (and sent). 

### Add a dictionary

Dictionaries use the following syntax:  
  
```
dictionary <key_type, data_type> identifier;
```
  
In the AlertManager monitor, add a dictionary to contain the Alert events it receives (coming from the SensorMonitor monitor):  
  
1. Open the **AlertManager.mon** file.  
  
2. Define a monitor-scope dictionary variable with a key type of **string** and a value type of **Alert** (that is, the Alert event type). Use **alerts** as the identifier for the dictionary.  
  
3. Add the following print statement to the **on all Alert ()** action, before the **send** statement:  
  
```
print alerts.toString();
```
  
4. Save your changes. 

### Add an if conditional statement

Follow these steps to add an if statement to the **on all Alert** listener in AlertManager.mon file (before the **send** statement):
  
1. This conditional statement should first check to see if any of the following are true:  

Does the current Alert event match an existing Alert event already in the dictionary (to avoid duplicate alert messages)?  
``` if not alerts.hasKey(a.sensorId) or ```  

Does the dictionary hold an Alert with a higher temperature reading than the current Alert?  
``` a.temperature > alerts[a.sensorId].temperature or ```  
  
Does the dictionary hold an Alert with a higher pressure reading than the current Alert?    
``` a.pressure > alerts[a.sensorId].pressure ```  
  
2. If any of these conditions are true, then the program adds or updates the dictionary with the current Alert event and sends the current Alert message:  

```  
{  
    alerts.add(a.sensorId, a);  
    send a to "AlertChannel";  
}  
```

{{< c8y-admon-info >}}
If none of these conditions are true, the program does not update the dictionary nor does it send the current Alert message.  
{{< /c8y-admon-info >}}  
Save the file. 

After you have updated your program with the dictionary and if conditional statement, the contents of the AlertManager.mon file should look like this:  
``` 
monitor AlertManager {
    dictionary <string, Alert> alerts;
    action onload {
        monitor.subscribe("AlertManager");
        on all Alert() as a {
            print alerts.toString();
            // If no alerts for sensor or alert with higher temperature or pressure
            // then add/update alerts and send alert to clients.
            if not alerts.hasKey(a.sensorId) or
            a.temperature > alerts[a.sensorId].temperature or 
            a.pressure > alerts[a.sensorId].pressure {
                alerts.add(a.sensorId, a);
                send a to "AlertChannel";
            }
        }
    }
} 
```
1. Run the project.  
  
2. Use **Engine Receive**.  
  
#### You can import the completed solution for this lesson:  

In the [Apama Samples](https://github.com/Cumulocity-IoT/apama-samples) Git repository, navigate to the **tutorials/EPL_Fundamentals_Completed** directory.