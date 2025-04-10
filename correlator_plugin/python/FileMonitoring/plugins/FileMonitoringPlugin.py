'''
	A File monitoring Python plugin that has methods to monitor a directory, get directory contents, and more.

	$Copyright (c) 2018-2019 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$ 
	Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH. 

'''

import sys
import os
import threading
import time
from apama.eplplugin import EPLAction, EPLPluginBase, Correlator, Event

def poll(plugin, searchDirs, searchDir, interval, channel):
	"""
	This method runs on a separate thread to monitor the directory for file changes.

	@param searchDir:	The name of the directory to monitor (not including the base path).
	@param interval:	The interval at which the thread should poll the directory for changes.
	@param channel:		The EPL channel which will receive Changes events.
	"""
	while (plugin.running):
		try:
			# check the current contents against self.OrigSeqFiles
			currentSeqFiles = os.listdir(searchDir)
			origSet = set(searchDirs[searchDir])
			currentSet = set(currentSeqFiles)

			additions = currentSet - origSet
			deleted = origSet - currentSet

			# If anything has changed then send this information to the Correlator
			if len(additions) > 0 or len(deleted) > 0:
				evtInstance = Event( 'apamax.filemonitoringsample.Changes' , {"searchDir":searchDir,"additions":sorted(additions),"deleted":sorted(deleted)})
				Correlator.sendTo(channel, evtInstance)
	
				#update the original list so that the we can determine any additional changes after this point
				searchDirs[searchDir] = currentSeqFiles

		except:
			plugin.getLogger().error("Exception: %s", sys.exc_info()[1])

		time.sleep(interval)


class FileMonitoringClass(EPLPluginBase):

	def __init__(self,init):
		super(FileMonitoringClass,self).__init__(init)
		self.getLogger().info("FileMonitoringClass initialised with config: %s" % self.getConfig())
		self.searchDirs = {}
		self.basePath = self.getConfig()["basePath"]
		self.running = True

	@EPLAction("action<string, integer, string> returns boolean")
	def monitorDirectory(self, searchDir, interval, channel):
		"""
		Activate a separate thread to monitor this directory for file changes.
		Assume the searchDir includes the base Path.

		@param searchDir:	The name of the directory to monitor (not including the base path).
		@param interval:	The interval at which the thread should poll the directory for changes.
		@param channel:		The channel which any changes should be reported back to EPL on.
		"""
		try:
			OrigSeqFiles = os.listdir(searchDir)
			self.searchDirs[searchDir] = OrigSeqFiles
			self.thread = threading.Thread(target=poll, args=(self, self.searchDirs, searchDir, interval, channel), name='Apama FileMonitoringPlugin polling thread')
			self.thread.start()
			return True
		except:
			self.getLogger().error("Exception: %s", sys.exc_info()[1])
			return False

	@EPLAction("action<>")
	def shutdown(self):
		self.running = False
		self.thread.join()

	@EPLAction("action<string, string> returns sequence<string> ")
	def readFile(self, fileDir, fileName):
		"""
		This method will read a file contents line by line and send back any event strings found
		If they are not proper events then the calling method should handle this.

		@param fileDir:	The name of the directory of the file.
		@param file:	The file to read.
		"""
		lines = []
		try:
			file = fileDir + "/" + fileName
			if os.path.isfile(file):
				if os.path.getsize(file) > 0:
					with open(file) as f:
						content = f.readlines()
						for line in content:
							lines.append(line)
					return lines
				else:
					self.getLogger().info("File %s is empty", file)
			else:
				raise Exception("File %s is invalid", file)
		except Exception as e:
			raise Exception("Exception thrown reading file: %s", e)

	@EPLAction('action<string> returns sequence<string> ')
	def getCurrentContents(self, searchDir):
		"""
		Returns a list of the names of the files and directories contained in the
		specified directory, or an empty list if it does not exist.
		
		Assume the searchDir includes the base Path

		@param searchDir:	The directory to retrieve the contents of.
		"""
		if os.path.exists(searchDir):
			return os.listdir(searchDir)
		else:
			return []

	@EPLAction('action<string> returns boolean ')
	def checkPathExists(self, path):
		"""
		Returns True if the path exists, False otherwise.
		"""
		return os.path.exists(path)
			
	@EPLAction('action<> returns string ')
	def getBasePath(self):
		"""
		Get the base path to use which is retrieved from the configuration file.
		"""
		return self.basePath




