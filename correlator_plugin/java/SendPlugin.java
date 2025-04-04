// Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
// Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

import com.apama.epl.plugin.Context;
import com.apama.epl.plugin.Correlator;

@com.apama.epl.plugin.annotation.ApplicationJar(
	name = "SendPluginJar",
	classpath = ""
)

/**
	Demonstrates passing contexts around and sending events
*/
@com.apama.epl.plugin.annotation.EPLPlugin(
	name="SendPlugin",
	description="A test plugin"
)
public class SendPlugin
{
	public static void sendEventToChannel(String event, String channel)
	{
		// Send the event to the channel
		Correlator.sendTo(event, channel);
	}
	public static void sendEventTo(String event, Context context)
	{
		// Get the current context we're running in
		Context current = Context.getCurrent();

		// For some reason we don't want to let you do this. 
		// An exception will terminate the monitor instance
		if (current.equals(context)) 
		{
			throw new IllegalArgumentException("Please don't use this function to send events to the current context!");
		}

		// Send the event to the other context
		Correlator.sendTo(event, context);
	}
}
