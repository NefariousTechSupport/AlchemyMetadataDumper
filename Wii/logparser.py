import os
import re

print("Remember to delete the log file before starting!!")
AppDataPath = os.environ['AppData']
logFilePath = AppDataPath + "/Dolphin Emulator/Logs/dolphin.log"
print("Using log file path \"" + logFilePath + "\"")

fileHandles = {}

with open(logFilePath, 'r') as file:
	for line in file:
		result = re.search("^[0-9]{2}:[0-9]{2}:[0-9]+ Core\\\\HLE\\\\HLE_OS\\\\.cpp:[0-9]+ N\\[OSREPORT_HLE\\]: [0-9a-f]+->[0-9a-f]+| ([A-z\\.]+):([0-9A-F]+)$", line)

		if (result != None):
			fileName = result.group(1)
			hexData = result.group(2)

			if fileName not in fileHandles:
				fileHandle = open(fileName, 'wb')
				fileHandles[fileName] = fileHandle

			fileHandles[fileName].write(bytes.fromhex(hexData))