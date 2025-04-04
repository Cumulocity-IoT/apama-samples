__pysys_title__   = r""" Hot Redeployment Sample """
#                        ================================================================================

__pysys_purpose__ = r""" 
	"""

__pysys_authors__ = ""
#__pysys_groups__  = "myGroup; inherit=true"
#__pysys_skipped_reason__   = "Skipped until Bug-1234 is fixed"

# Hot Redeployment sample
# Copyright (c) 2017 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors. 
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

from pysys.constants import *
from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper

import time

class PySysTest(BaseTest):
	def execute(self):
		
		correlator = CorrelatorHelper(self)
		# Use 'arguments=["-P"]' to enable correlator persistence 
		correlator.start(logfile='correlator.log')

		# inject Memory Store bundle
		correlator.injectEPL(filenames='MemoryStore.mon', filedir=os.path.join(self.project.APAMA_HOME, 'monitors', 'data_storage'))
		
		# inject the monitors
		correlator.injectEPL(filenames=['Events.mon', 'Upgrader.mon', 'Simulator.mon'])
		
		# Inject version 1
		correlator.injectEPL(filenames='Counter_v1.mon')
		
		# Create some instances in v1
		correlator.sendEventStrings('com.apama.sample.redeploy.Create("main", 1)')
		correlator.sendEventStrings('com.apama.sample.redeploy.Create("process", 111)')
		correlator.sendEventStrings('com.apama.sample.redeploy.Create("v1", 111111)')
		
		# Let v1 run
		time.sleep(5)
		
		# Stop the Simulator
		correlator.sendEventStrings('com.apama.sample.redeploy.StopSimulator()')
		correlator.flush()
		
		# Save v1 state
		#correlator.sendEventStrings('com.apama.sample.redeploy.Upgrade(2)', channel='DATA')
		#correlator.flush()
		
		# Inject version 2
		correlator.injectEPL(filenames='Counter_v2.mon')
		
		# Create some instances in v2
		correlator.sendEventStrings('com.apama.sample.redeploy.Create("v2", 2222)')

		# Start the Simulator
		correlator.sendEventStrings('com.apama.sample.redeploy.StartSimulator()')
		
		# Let v2 run
		time.sleep(5)

	def validate(self):
		self.assertLineCount('correlator.out', expr='Saving state com.apama.sample.redeploy.v1.State', condition='==3')
		self.assertLineCount('correlator.out', expr='state restored to com.apama.sample.redeploy.v2.State', condition='==3')
