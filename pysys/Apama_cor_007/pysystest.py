__pysys_title__   = r""" Connecting a correlator source to a main correlator for sending of events """
#                        ================================================================================

__pysys_purpose__ = r""" Demonstrates connecting a correlator as a source of events to a main 
	application correlator. 
	
	In the test two instances of the CorrelatorHelper class are created and started. In the first 
	correlator an EPL file is injected to receive and log ForwardEvents. In the second 
	correlator a forwarding EPL file is inject to receive TestEvents, annotate them 
	with the sender ID, and send onto a test channel. The second correlator is connected as a 
	source of events to the first correlator on this channel. The test validates that forwarded 
	events are received by the first correlator over the connection.
	"""

__pysys_authors__ = "sample"

# Copyright (c) 2015-2016, 2018, 2020-2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

import pysys.basetest, pysys.mappers
from pysys.constants import *
from apama.testplugin import ApamaHelper

class PySysTest(ApamaHelper, pysys.basetest.BaseTest):

	def execute(self):
		# create the first correlator helper, start it and inject the log monitor
		correlator1 = self.apama.startCorrelator('correlator1')
		correlator1.injectEPL(filenames='log.mon')
	
		# create the second correlator helper, start it and inject the forward monitor
		correlator2 = self.apama.startCorrelator('correlator2')
		correlator2.injectEPL(filenames='forward.mon')
	
		# connect correlator2 as a source of events to correlator1
		correlator1.connect(source=correlator2, channel='TestChannel')
		
		# send some events to correlator2 to be forwarded to correlator1
		correlator2.send(filenames='forward.evt')
		
		# wait for signals in correlator1 logfile
		correlator1.waitForLogGrep('Received ForwardEvent', condition='>=5')
		
		# disconnect correlator2 as a source of events
		correlator1.disconnect(source=correlator2, channel='TestChannel', arguments=['-v'])
	
		# send some more events to correlator2
		correlator2.send(filenames='forward.evt')

	def validate(self):
		# check the first batch of events were received by correlator1
		self.assertThat('events == expected', 
			events=self.grepAll('correlator1.log', r'LogMonitor \[\d+\] (Received ForwardEvent.*)'),
			expected=[
				'Received ForwardEvent, ForwardEvent("Source correlator","Message 1")',
				'Received ForwardEvent, ForwardEvent("Source correlator","Message 2")',
				'Received ForwardEvent, ForwardEvent("Source correlator","Message 3")',
				'Received ForwardEvent, ForwardEvent("Source correlator","Message 4")',
				'Received ForwardEvent, ForwardEvent("Source correlator","Message 5")',
			])

		# check the second batch of events were not received by correlator1
		self.assertLineCount(file='correlator1.log', expr='Received ForwardEvent', condition='==5')

