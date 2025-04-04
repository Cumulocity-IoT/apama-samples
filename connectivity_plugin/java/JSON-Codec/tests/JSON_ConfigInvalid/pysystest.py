__pysys_title__   = r""" Configuration - error messages for invalid config """
#                        ================================================================================

__pysys_purpose__ = r""" Check that invalid plug-in configuration results in appropriate error messages
	"""

__pysys_authors__ = "sample"

# Copyright (c) 2016, 2020-2021 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors. 
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

from pysys.constants import *
from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper

class PySysTest(BaseTest):

	def execute(self):
		# we expect the correlator not to start
		subtest = 'unexpected-arg'		
		correlator = CorrelatorHelper(self, name='test-correlator-'+subtest)
		correlator.start(logfile=correlator.name+'.log', 
			config=[self.input+'/%s.yaml'%subtest, self.project.appHome+'/json-codec.yaml'], 
			ignoreExitStatus=True, waitForServerUp=False)
		self.waitProcess(correlator.process, timeout=TIMEOUTS['WaitForSignal'], abortOnError=True)
		self.logFileContents(correlator.name+'.log', tail=True, includes=[' ERROR .*', ' WARN .*'])
		
		# we could add similar subtests below for any other incorrect configurations we wish to test
		
	def validate(self):
		# check error messages are correct
		
		# do not be more precise than necessary, just check that the required 
		# error message is present
		
		self.assertGrep('test-correlator-unexpected-arg.log', expr=' ERROR .*Found unexpected items in configuration')
