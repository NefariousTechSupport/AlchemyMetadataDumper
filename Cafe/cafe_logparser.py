import os
import re

os.chdir(os.path.dirname(os.path.abspath(__file__))) # changes the cwd to be where the script is

print("Remember to delete the log file before starting!!")
logFilePath = "D:/Emulators/Cemu/log.txt"
print("Using log file path \"" + logFilePath + "\"")

fileHandles = {}

with open(logFilePath, 'r') as file:
	lines = file.readlines()

	currentLineIndex = 0
	while not("dumping metadata now" in lines[currentLineIndex]):
		currentLineIndex += 1

	currentLineIndex += 1

	linePrefixLen = lines[currentLineIndex].find("[OSConsole]") + 12

	everything = ""
	while not("all done!" in lines[currentLineIndex]):
		if "[OSConsole]" in lines[currentLineIndex]:
			everything += lines[currentLineIndex][linePrefixLen : ].replace("\n", "")
		currentLineIndex += 1

	currentIndex = 0
	fileName = ""
	while currentIndex < len(everything):
		if everything[currentIndex] == "m": # metadatadumper file fileName.xml
			fileName = ""
			currentIndex += 20 # length of "metadatadumper file "
			while not(fileName.endswith(".xml")):
				fileName += everything[currentIndex]
				currentIndex += 1
			currentIndex += 1
			
			if fileName not in fileHandles:
				fileHandle = open(fileName, 'wb')
				fileHandles[fileName] = fileHandle
		else:
			hexChar = everything[currentIndex : currentIndex + 2]
			fileHandles[fileName].write(bytes.fromhex(hexChar))
			currentIndex += 2

print("Done!")