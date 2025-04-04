__pysys_title__   = r""" Simple EPL test showing injection, event sending, and validation of correlator output and log messages """
#                        ================================================================================

__pysys_purpose__ = r""" Demonstrates starting a correlator, injecting an EPL application, and sending in a series of 
	input events via event file. 
	
	This is a good sample test to copy from if you're working with EPL.
	"""

__pysys_authors__ = "sample"
#__pysys_groups__ = "myGroup"
#__pysys_skipped_reason__ = "Skipped until Bug-1234 is fixed"

# Copyright (c) 2015-2016, 2018-2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

import pysys.basetest, pysys.mappers
from pysys.constants import *
from apama.testplugin import ApamaHelper

class PySysTest(ApamaHelper, pysys.basetest.BaseTest):

	def execute(self):
		# Start an instance of the correlator (on an automatically generated free port) 
		correlator = self.apama.startCorrelator('testCorrelator')
		
		# This test monitor produces a line in the correlator log with a JSON representation of each event sent to the 
		# specified channel(s), which provides a very convenient way to check the results
		# In some tests it's helpful to include the channel containing the inputs (e.g. Factory1 in this case) as well 
		# as the outputs so you can see the ordering relationship between input and output clearly
		correlator.injectTestEventLogger(channels=['Alerts', 'Factory1'])

		# Inject the application EPL and any test monitors
		# In a real application this would come from a project property e.g. self.project.appHome+'/foo.mon'
		correlator.injectEPL(['SensorMonitorApp.mon'])
		
		# This is how to wait for a log message, with automatic aborting if an error occurs while waiting
		correlator.waitForLogGrep("Loaded SensorMonitor")
		
		# We can send in events to configure our sensors as we need them for the test
		correlator.sendEventStrings(
			'apamax.myapp.AddSensor("TempSensor001",100)',
			'apamax.myapp.AddSensor("TempSensor002",800)',
		)

		# Send in some representative sample data from .evt file in the Input/ directory, to exercise our application
		correlator.send(['TemperatureEvents.evt'])
			
		# Wait for all events to be processed (the flush() approach works well for simple cases, but if you need to 
		# wait for any external events or time listeners then you'll need waitForLogGrep instead). 
		correlator.flush()
		
		# Wait for the expected number of events to appear in the log file, aborting if there's an error or event 
		# parsing failure (NB: don't try to use regular expressions to parse the actual contents of your events) 
		correlator.waitForLogGrep('-- Got test event: .*apamax.myapp.Alert', condition=">=3+2")
	
	
	def validate(self):
		# Best practice is to always check for errors in the the correlator log file (you can add ignores for any 
		# that you are expecting). 
		self.assertGrep('testCorrelator.log', expr=' (ERROR|FATAL|Failed to parse) .*', contains=False, 
			ignores=self.apama.defaultLogIgnores+[])

		# The easiest way to validate the output events is usually to use extractEventLoggerOutput() to get events 
		# written by injectEventLogger() into a Python dictionary. The fields and events of interest can be easily 
		# extracted using the power of Python [...] list comprehensions
		sensor1_temps = [ 
			# Extract only the field value(s) we care about (allows us to ignore unimportant information, timestamps, etc):
			(evt['temperature']) for evt in self.apama.extractEventLoggerOutput('testCorrelator.log')
			
			# Filter to include the desired subset of events:
			if evt['.eventType']=='apamax.myapp.Alert' and evt['sensorId']=='TempSensor001'
			]
		self.assertThat('sensor1_temps == expected', sensor1_temps=sensor1_temps, expected=[
				111.0,
				120,
				145.2,
			])

		# It's easy to write sophisticated verification conditions using the values extracted from the events
		self.assertThat('min(sensor1_temps) >= expected', sensor1_temps=sensor1_temps, expected=100*1.02)
		self.assertThat('50 <= sensor1_temps[0] <= 200', sensor1_temps=sensor1_temps)

		# For cases where you have many events, or want to use most of the event fields not just one or two, 
		# it may be easier to generate a file containing the fields and events of interest and then diff them
		self.write_text('sensor2_output.txt', '\n'.join(
			str( {field:val for (field,val) in evt.items() if field not in ['time', 'requestId'] })
				for evt in self.apama.extractEventLoggerOutput('testCorrelator.log')
				if evt['.eventType']=='apamax.myapp.Alert' and evt['sensorId']=='TempSensor002'), 
			encoding='utf-8')
		self.assertDiff('sensor2_output.txt', encoding='utf-8')
