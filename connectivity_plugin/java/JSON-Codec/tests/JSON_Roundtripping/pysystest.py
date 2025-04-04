__pysys_title__   = r""" Bidirectional roundtripping JSON strings - message to JSON string back to message """
#                        ================================================================================

__pysys_purpose__ = r""" Pass the same messages through the codec in both directions (start with Message to JSON). 
	
	This is more efficient than creating separate uni-directional testcases.
	"""

__pysys_authors__ = "sample"

# Copyright (c) 2016, 2020-2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors. 
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
		
		# because we set logOutput=true we can wait for completion by checking the log file, 
		# which also allows aborting quickly if there's an error, or a warning 
		# that's related to the chain we're testing
		# we need to wait until it is logged twice, once towards the host, then once back 
		# towards the transport
		correlator.waitForLogGrep("UnitTestHarness output.*final message", condition='==2')
		
	def validate(self):
		self.assertGrep('test-correlator.log', expr='( ERROR .*| WARN .*MyTestChain.*)', contains=False)
		# as long as we're careful to use the same formatting as the UnitTestHarness's payload_json 
		# uses, we can can re-use our input file as the reference to compare the output against
		self.assertDiff('output-towards-transport.txt', 'input-towards-host.txt', filedir2=self.input)
		