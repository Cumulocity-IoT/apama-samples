__pysys_title__   = r""" Generating JSON strings towardsTransport - successful mappings """
#                        ================================================================================

__pysys_purpose__ = r""" Check that successful (well-formed) JSON can be generated.
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
		correlator.waitForLogGrep(expr="UnitTestHarness output.*final message", 
			errorExpr=[' (ERROR .*|FATAL .*|WARN .*MyTestChain.*)'])
		
	def validate(self):
		self.assertGrep('test-correlator.log', expr='( ERROR .*| WARN .*MyTestChain.*)', contains=False)
		self.assertDiff('output-towards-transport.txt', 'ref-output-towards-transport.txt')
		