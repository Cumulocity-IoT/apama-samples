__pysys_title__   = r""" Test the CRUD HTTP Server sample """
#                        ================================================================================

__pysys_purpose__ = r""" Test the CRUD HTTP Server sample
	"""

__pysys_authors__ = "sample"
#__pysys_groups__  = "myGroup; inherit=true"
#__pysys_skipped_reason__   = "Skipped until Bug-1234 is fixed"

# Sample PySys testcase
# Copyright (c) 2018, 2021-2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors. 
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

import http.client

from pysys.constants import *
from pysys.basetest import BaseTest

from apama.testplugin import ApamaHelper

class PySysTest(ApamaHelper, BaseTest):

	def execute(self):
		port = self.getNextAvailableTCPPort()
		correlator = self.apama.startCorrelator("sample-correlator", 
						config=[self.project.appHome],
						# override the port to avoid accidental clashes while running the test
						arguments=['-DHTTPServer_port=%s' % port])
		correlator.waitForLogGrep('HTTPServer ready to handle requests')

		# test inserts
		self.do_request('create1', correlator.host, port, 'PUT', '/create', '{"name":"Resource 1", "value":42}')
		self.do_request('create2', correlator.host, port, 'PUT', '/create', '{"name":"Resource 2", "value":666}')
		
		# test retrieval
		self.do_request('read1', correlator.host, port, 'GET', '/objects/1')
		
		# test update
		self.do_request('update', correlator.host, port, 'PUT', '/objects/1', '{"name":"Resource 1", "value":3.14}')
		self.do_request('read2', correlator.host, port, 'GET', '/objects/1')
		
		# test delete
		self.do_request('delete', correlator.host, port, 'DELETE', '/objects/1')
		self.do_request('read3', correlator.host, port, 'GET', '/objects/1')
		
	def do_request(self, name, host, port, method, URL, data=None):
		connection = http.client.HTTPConnection("%s:%s" % (host, port))
		connection.request(method, URL, data, headers={'Content-Type': 'application/json'} if data else {})
		response = connection.getresponse()
		with open(self.output+'/'+name+'.out', 'w') as f:
			f.write(response.read().decode('utf-8'))
		with open(self.output+'/'+name+'.status', 'w') as f:
			f.write("%s %s\n" % (response.status, response.reason))
		
	def validate(self):
		# check the log file for warnings / errors
		self.assertGrep(file='sample-correlator.log', expr='ERROR', contains=False)
		self.assertGrep(file='sample-correlator.log', expr='WARN', contains=False, ignores=['RLIMIT_CORE'])
		
		# check that the application logs the expected things
		self.assertGrep(file='sample-correlator.log', expr='apamax.CRUDHandler ... Starting CRUD handle')
		self.assertGrep(file='sample-correlator.log', expr='apamax.CRUDHandler ... Added new object with path /objects/1')
		self.assertGrep(file='sample-correlator.log', expr='apamax.CRUDHandler ... Added new object with path /objects/2')
		self.assertGrep(file='sample-correlator.log', expr='apamax.CRUDHandler ... Updated object at /objects/1')
		self.assertGrep(file='sample-correlator.log', expr='apamax.CRUDHandler ... Deleted object at /objects/1')
		
		# check the status from the requests is what we expect
		for f in ['create1', 'create2', 'read1', 'update', 'read2', 'delete']:
			self.assertGrep(file='%s.status' % f, expr='200 OK')
		self.assertGrep(file='read3.status', expr='404 Not Found')
		
		# check the output from the requests is what we expect
		self.assertGrep(file='create1.out', expr='Successfully added new object at /objects/1')
		self.assertGrep(file='create2.out', expr='Successfully added new object at /objects/2')
		self.assertGrep(file='read1.out', expr='{"name":"Resource 1","value":42}')
		self.assertGrep(file='update.out', expr='Successfully updated object at /objects/1')	
		self.assertGrep(file='read2.out', expr='{"name":"Resource 1","value":3.14}')
		self.assertGrep(file='delete.out', expr='Successfully deleted object at /objects/1')
		self.assertGrep(file='read3.out', expr='No object at /objects/1')
