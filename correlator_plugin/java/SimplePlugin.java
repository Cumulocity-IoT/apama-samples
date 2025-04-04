// Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
// Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

@com.apama.epl.plugin.annotation.ApplicationJar(
	name = "SimplePluginJar",
	classpath = ""
)

/**
	A very simple plugin with one method that takes and prints out a string,
	and returns another string.
*/
@com.apama.epl.plugin.annotation.EPLPlugin(
	name="SimplePlugin",
	description="A test plugin"
)
public class SimplePlugin
{
	private static final org.slf4j.Logger logger = org.slf4j.LoggerFactory.getLogger("plugins.SimplePlugin");
	
	public static final String TEST_STRING = "Hello, World";

	public static String test(String arg)
	{
		logger.info("Called test function in SimplePlugin");
		logger.info("SimplePlugin function test called");
		logger.info("arg = "+arg);
		logger.info("return value = "+TEST_STRING);
		return TEST_STRING;
	}
}
