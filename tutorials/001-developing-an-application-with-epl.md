# Developing an Application with EPL

This tutorial provides step-by-step instructions for how to create an event processing application with **EPL**, Apama's Event Programming Language.  
  
This tutorial shows you how to build a **sample application** that monitors a fictional industrial process. Sensors in the manufacturing plant continuously produce telemetry data on the operational temperature and pressure of the industrial process. You will write a program that monitors the temperature and pressure data and sends an alert when there is a violation of expected operational values.  
  
As you build this sample application you will learn:  
* What an event is and how to define different types of events  
* How to use EPL to identify and respond to event patterns  
* How to persist application information
  
The end result will be an application which processes a continuous sequence of sensor events, generates alerts on violations, and presents real-time sensor readings and alerts in an interactive user-interface.  
  
This tutorial is written for software developers with some programming experience, but with little or no prior knowledge of EPL.

{{< c8y-admon-info >}}
This tutorial uses [Apama extension for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=ApamaCommunity.apama-extensions). Please ensure you have it installed and configured before continuing. All instructions in this and related tutorials assume you're working in VS Code with this extension.
{{< /c8y-admon-info >}}