# Sending Events to External Components (Sending to receivers)

In this lesson, you will update your EPL program to send Alert events and make them available to receivers that are external to the correlator via a named channel. 

### Edit the AlertManager monitor to send events

The **send** command can also send events to external software via a named channel.  
  
To update your program to send Alert events instead of printing them:  
  
1. Open the **AlertManager.mon** file.  
  
2. Change the line of code that prints Alert events to instead use the send command to send Alert events to external software via a channel named "AlertChannel":  

```
Change:  
print a.toString();    
To:  
send a to "AlertChannel";  
```

{{< c8y-admon-info >}}
The Alert event is being published on the **AlertChannel** channel. For an external application component to receive the Alert event it must register itself as an event receiver and subscribe to the AlertChannel channel. 
{{</ c8y-admon-info >}}

To see the results of sending an event (which is being output from the correlator), use the Engine Receive.  
  
### Start Engine Receive

1. Before Running your project, you need to start the engine_receive to listen for events on a specific channel.
Open a terminal in VS Code and run the following command:

```
engine_receive -c AlertChannel -p 5000
```

2. Run the project i.e inject mon files and send events

3. With the project running and events sent, 
you should now see alert messages appear in the terminal running engine_receive.

```
Alert("TEMP_PRESSURE_RISE","S003",150,1500)
```
  
#### You can import the completed solution for this lesson:  

In the [Apama Samples](https://github.com/Cumulocity-IoT/apama-samples) Git repository, navigate to the **tutorials/EPL_Fundamentals_Completed** directory.