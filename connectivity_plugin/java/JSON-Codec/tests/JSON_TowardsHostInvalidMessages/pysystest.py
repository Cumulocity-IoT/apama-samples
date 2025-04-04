__pysys_title__   = r""" Parsing JSON strings towardsHost - invalid JSON """
#                        ================================================================================

__pysys_purpose__ = r""" Check that malformed messages result in error messages, 
	and do not cause later parsing teh fail.
	"""

__pysys_authors__ = "sample"

# Copyright (c) 2016-2017, 2020-2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors. 
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

from pysys.constants import *
import pysys.basetest

from apama.correlator import CorrelatorHelper
from apama.testplugin import ApamaHelper

class PySysTest(ApamaHelper, pysys.basetest.BaseTest):

	def execute(self):
		correlator = CorrelatorHelper(self, name='test-correlator')
		correlator.start(logfile=correlator.name+'.log', 
			config=[self.input+'/json-test-connectivity.yaml', self.project.appHome+'/json-codec.yaml'], 
			arguments=['-DTEST_INPUT_DIR=%s'%self.input])
		
		correlator.waitForLogGrep("UnitTestHarness output.*final message", ignores=[' ERROR .*'])
		
	def validate(self):
		# check for no errors or test chain warnings we were not expecting
		self.assertGrep('test-correlator.log', expr='( ERROR .*| WARN .*MyTestChain.*)', contains=False, 
			ignores=[' WARN .*jsonCodec.MyTestChain> .*failed to transform message'])
			
		# check for the errors we were expected
		# no need to over-specify the error message - just check that the required info is present
		self.assertLineCount('test-correlator.log', 
			expr=' WARN .*jsonCodec.MyTestChain> .*failed to transform message.*aim.*payload=', 
			condition='==2')

		# check that valid messages can be parsed after failures
		self.assertGrep('test-correlator.log', 
			expr="UnitTestHarness output.*Safely handling empty payloads.*\{\}")

		# check that valid messages can be parsed after failures
		self.assertGrep('test-correlator.log', 
			expr="UnitTestHarness output.*final message.*Hello world")

		# two valid message should have been received
		self.assertLineCount('test-correlator.log', expr="UnitTestHarness output.*", condition='==2')
