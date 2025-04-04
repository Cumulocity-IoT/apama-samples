__pysys_title__   = r""" Perform doRequest operations with the engine_management tool """
#                        ================================================================================

__pysys_purpose__ = r""" Performs the applicationEventLogging, applicationLogging, 
	toStringAll and profiling operations against the Event Correlator using the 
	engine_management wrapper methods.
	"""

__pysys_authors__ = "sample"

# Copyright (c) 2015-2016, 2018, 2020-2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

import pysys.basetest, pysys.mappers
from pysys.constants import *
from apama.testplugin import ApamaHelper

class PySysTest(ApamaHelper, pysys.basetest.BaseTest):

	def execute(self):
		# create the correlator helper, start the correlator 
		correlator = self.apama.startCorrelator('correlator')
		
		# enable application event logging
		correlator.applicationEventLogging(enable=True)
		
		# set the application log file and verbosity
		correlator.setApplicationLogFile('application.log')
		correlator.setApplicationLogLevel('DEBUG')
				
		# switch profiling on
		correlator.profilingOn()
		
		# inject the simple.mon monitor (directory defaults to the testcase input)
		correlator.injectEPL(filenames=['simple.mon'])
		
		# wait a few seconds and then get the profiling information
		self.waitForGrep('application.log', expr="Number of events received", condition=">100", errorExpr=[' (ERROR|FATAL) .*'])
		correlator.profilingGet('profile.log')
		
		# get a dump of the stringified state
		correlator.toStringAll('tostringall.log')
		
	def validate(self):
		self.addOutcome(INSPECT, 'Please manually review the profile and toStringAll output')
