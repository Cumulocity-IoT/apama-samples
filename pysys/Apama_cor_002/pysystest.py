__pysys_title__   = r""" More advanced EPL testing sample, using Xclock """
#                        ================================================================================

__pysys_purpose__ = r""" Demonstrates sending events into a correlator and use of external clocking to control time 
	inside the correlator.
	"""

__pysys_authors__ = "sample"

# Copyright (c) 2015-2016, 2018-2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

import pysys.basetest, pysys.mappers
from pysys.constants import *
from apama.testplugin import ApamaHelper

class PySysTest(ApamaHelper, pysys.basetest.BaseTest):

	def execute(self):
		correlator = self.apama.startCorrelator('myCorrelator',
			# External clocking means that correlator time is controlled externally by &TIME(...) events instead of the
			# real system clock
			Xclock=True, 
			# The input log can sometimes be a useful debugging aid in complex tests
			inputLog='myCorrelator.input.log', 
		)
		
		# Write events received from EchoChannel to a file, for debugging purposes
		correlator.receive(filename='receive.evt', channels=['EchoChannel'])

		# This can be useful if you need to debug a tricky EPL bug
		correlator.applicationEventLogging(enable=True)

		# Inject the simple monitor into the correlator
		correlator.injectEPL(filenames=['simple.mon'])
		
		# Send events to the correlator from the test
		correlator.sendEventStrings(
			# since we are using Xclock, we manually control the correlator's 
			# 'currentTime' with &TIME pseudo-events. 
			'&TIME(1000.1)',
			'SimpleEvent("This is the first simple event")',
			
			'&TIME(1005.2)',
			'SimpleEvent("This is the second simple event")',
			channel='TestChannel',
		)
		
		# Wait for all events to be processed (the flush() approach works well for simple cases, but if you need to 
		# wait for any external events or time listeners then you'll need waitForGrep instead). 
		correlator.flush()
		
	def validate(self):
		# Look for the log statements in the correlator log file
		self.assertThat('logMessages == expected', 
			logMessages=self.grepAll('myCorrelator.log', 'Received simple event with message - .*'),
			expected=[
				'Received simple event with message - This is the first simple event; correlator currentTime=1000.1',
				'Received simple event with message - This is the second simple event; correlator currentTime=1005.2',

			])
