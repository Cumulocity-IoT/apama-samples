// Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
// Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

import com.apama.epl.plugin.Correlator;
import com.apama.epl.plugin.EventHandler;

@com.apama.epl.plugin.annotation.ApplicationJar(
	name = "SubscribePluginJar",
	classpath = ""
)

/**
	Demonstrates a plugin subscribing to channels to receive events
*/
@com.apama.epl.plugin.annotation.EPLPlugin(
	name="SubscribePlugin",
	description="A test plugin"
)
public class SubscribePlugin
{
	public static class SubscribeHandler implements EventHandler
	{
		public void handleEvent(String event, String channel)
		{
			System.out.println("Got event "+event+" on channel "+channel);
		}
	}
	public static Object createHandler(String channel)
	{
		SubscribeHandler h = new SubscribeHandler();
		Correlator.subscribe(h, channel);
		return h;
	}
}
