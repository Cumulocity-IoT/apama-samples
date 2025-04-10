__pysys_title__   = r""" Concurreny Theory Sample """
#                        ================================================================================

__pysys_purpose__ = r""" 
	"""

__pysys_authors__ = ""
#__pysys_groups__  = "myGroup; inherit=true"
__pysys_modes__            = r""" lambda helper: helper.inheritedModes + [{'mode': 'Race'}, {'mode': 'Deadlock'}, {'mode': 'CompareSwap'}] """

#__pysys_skipped_reason__   = "Skipped until Bug-1234 is fixed"

# Warning from pysystest.xml->pysystest.py conversion: {w}

# Concurrency theory sample
# Copyright (c) 2017 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors. 
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

from pysys.constants import *
from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper

class PySysTest(BaseTest):
	def execute(self):
		
		# create the correlator helper, start the correlator and attach an 
		# engine_receive process listening to channel 'output'. The helper will 
		# automatically get an available port that will be used for all 
		# operations against it
		correlator = CorrelatorHelper(self)
		correlator.start(logfile='correlator.log')
		receiveProcess = correlator.receive(filename='%s.evt'%self.mode, channels=['output'], logChannels=True)

		# inject the monitors
		correlator.injectEPL(filenames='TimeFormatEvents.mon', filedir=os.path.join(self.project.APAMA_HOME, 'monitors'))
		correlator.injectEPL(filenames=['BankCommon.mon', 'Bank-%s.mon'%self.mode.lower()])
		
		# send in the events
		correlator.send(filenames=['setup.evt', 'expose.evt', 'transfer-%s.evt'%self.mode.lower()])
		
		# wait for all events to be processed
		correlator.flush(count=6)
		
		# send in the events
		correlator.send(filenames=['sendbalance.evt'])

		# wait for all events to be processed
		correlator.flush(count=2)
		
		# log received file
		self.logFileContents('%s.evt'%self.mode, maxLines=50)

	def validate(self):
		pass
