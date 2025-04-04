'''
	A simple python plugin that has a method that can generate a UUID and return it to EPL
 
 	$Copyright (c) 2018 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$ 
 	Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 
 
@author: SGOL
'''

import uuid
from apama.eplplugin import EPLAction, EPLPluginBase


class SimplePluginClass(EPLPluginBase):

	def __init__(self,init):
		super(SimplePluginClass,self).__init__(init)
		self.getLogger().info("SimplePluginClass initialised")

	@EPLAction("action< > returns string ")
	def generateUUID(self):
		retUuid = uuid.uuid4()
		return str(retUuid)

