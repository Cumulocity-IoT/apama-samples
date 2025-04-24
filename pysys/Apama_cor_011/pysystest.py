__pysys_title__   = r""" Starting correlator initializing from a YAML config file """
#                        ================================================================================

__pysys_purpose__ = r""" This test demonstrates starting a correlator with a YAML configuration file
	(most likely generated using the engine_deploy tool which contains the configuration to initialise 
	itself such as injections, event sending, etc.
	"""

__pysys_authors__ = "sample"

# Copyright (c) 2016, 2018, 2020-2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

import pysys.basetest, pysys.mappers
from pysys.constants import *
from apama.testplugin import ApamaHelper

class PySysTest(ApamaHelper, pysys.basetest.BaseTest):

	def execute(self):
		# start the correlator using the init.yaml file for injections and event sending
		correlator = self.apama.startCorrelator('testCorrelator', config=[self.input+'/init.yaml'])

		# wait for all events to be processed
		correlator.waitForLogGrep("Received simple event with message", condition=">=2")
		
	def validate(self):
		# look for log statements in the correlator log file
		self.assertGrep('testCorrelator.log', expr='(ERROR|FATAL) .*', contains=False)
		
		self.assertThat('events == expected', 
			events=self.grepAll('testCorrelator.log', r'Received simple event with message - .*'),
			expected=[
				'Received simple event with message - This is the first simple event',
				'Received simple event with message - This is the second simple event',
			])

