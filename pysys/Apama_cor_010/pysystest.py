__pysys_title__   = r""" Starting the correlator with a connectivity plugin """
#                        ================================================================================

__pysys_purpose__ = r""" This demonstrates a simple example of connectivity plugin system test, using the codec at 
	samples/connectivity_plugin/java/JSON-Codec. 
	
	The injected EPL application is sending out events to the connectivity chain, which the correlator
	presents as maps to the chain. The JSON codec sees these maps and renders them as JSON documents
	into a string payload of the message before passing it on to the transport.
	
	In reply to each message, the transport sends one JSON document from a text file back towards
	the correlator. The JSON codec sees these as strings, and parses them into maps. The correlator then
	interprets these as events for the EPL application.
	
	The verification is being done by checking performing a file
	comparison with a reference file for the events received and written to file by the transport.
	"""

__pysys_authors__ = "sample"

__pysys_groups__ = "ant"

# Copyright (c) 2015-2016, 2018, 2020-2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

import pysys.basetest, pysys.mappers
from pysys.constants import *
from apama.testplugin import ApamaHelper

class PySysTest(ApamaHelper, pysys.basetest.BaseTest):

	def execute(self):
		appHome = self.project.appHome + '/connectivity_plugin/java/JSON-Codec'

		# build the sample, putting the results into the testcase output dir
		self.apama.antBuild(appHome + '/build.xml', args=['-Doutput-dir=%s/Release'%self.output])

		# create a correlator
		correlator = self.apama.startCorrelator('correlator', 
			config=[self.input+'/test-config.yaml'], 
			
			# these properties are used within the test .yaml file to specify input/output locations
			configPropertyOverrides={
				'TEST_OUTPUT_DIR': self.output, 
				'APP_HOME': appHome,
			})
		correlator.injectEPL([
			self.project.APAMA_HOME+'/monitors/ConnectivityPluginsControl.mon', 
			self.project.APAMA_HOME+'/monitors/ConnectivityPlugins.mon',
			appHome+'/demo/DemoApp.mon',
			])

		# wait until the correlator receives the events (aborting if an error occurs)
		correlator.waitForLogGrep("Sending special offer", condition="==2")
		
		# wait until 2 messages are received in this file
		self.waitForGrep('specialoffers-output.json.txt', expr="metadata=", condition="==2")
		
	def validate(self):
		# check the received events against the reference 
		appHome = self.project.appHome + '/connectivity_plugin/java/JSON-Codec'
		self.assertDiff('specialoffers-output.json.txt', 'reference-specialoffers-output.json.txt', 
			filedir2=appHome+'/demo')
