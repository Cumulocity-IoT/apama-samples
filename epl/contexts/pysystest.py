__pysys_title__   = r""" Contexts Sample """
#                        ================================================================================

__pysys_purpose__ = r""" 
	"""

__pysys_authors__ = ""
#__pysys_groups__  = "myGroup; inherit=true"
#__pysys_skipped_reason__   = "Skipped until Bug-1234 is fixed"

# Contexts sample
# Copyright (c) 2017 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors. 
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

from pysys.constants import *
from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper

class PySysTest(BaseTest):
	def execute(self):
		
		# Generate Data
		correlatorGen = CorrelatorHelper(self)
		correlatorGen.start(logfile='correlator-gen.log')
		self.log.info("***** Generating data")
		receiveProcess = correlatorGen.receive(filename='data.evt', logChannels=True)
		correlatorGen.injectEPL(filenames=['evt.mon', 'gen.mon'])
		correlatorGen.send(filenames=['gen.evt'])
		correlatorGen.flush(count=3)
		correlatorGen.send(filenames=['fin.evt'])
		correlatorGen.flush(count=3)
		correlatorGen.shutdown()

		# Analyse in Serial
		correlatorSerial = CorrelatorHelper(self)
		correlatorSerial.start(logfile='correlator-serial.log')
		self.log.info("***** Analysing in serial")
		correlatorSerial.injectEPL(filenames='TimeFormatEvents.mon', filedir=os.path.join(self.project.APAMA_HOME, 'monitors'))
		correlatorSerial.injectEPL(filenames=['evt.mon', 'analyse-serial.mon'])
		receiveProcess = correlatorSerial.receive(filename='serial-results.evt')
		correlatorSerial.watch(filename='watch-serial.dat')
		correlatorSerial.send(filedir=self.output, filenames=['data.evt'])
		correlatorSerial.flush(count=3, timeout=300)
		correlatorSerial.shutdown()

		# Analyse in Parallel
		correlatorParallel = CorrelatorHelper(self)
		correlatorParallel.start(logfile='correlator-parallel.log')
		self.log.info("***** Analysing in parallel")
		correlatorParallel.injectEPL(filenames='TimeFormatEvents.mon', filedir=os.path.join(self.project.APAMA_HOME, 'monitors'))
		correlatorParallel.injectEPL(filenames=['evt.mon', 'analyse-parallel.mon'])
		receiveProcess = correlatorParallel.receive(filename='parallel-results.evt')
		correlatorParallel.watch(filename='watch-parallel.dat')
		correlatorParallel.send(filedir=self.output, filenames=['data.evt'])
		correlatorParallel.flush(count=3, timeout=300)
		correlatorParallel.shutdown()

	def validate(self):
		pass
