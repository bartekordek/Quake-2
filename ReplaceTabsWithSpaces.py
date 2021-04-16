#!/usr/bin/python3

import sys
import ntpath
import glob, os

print ('Number of arguments:', len(sys.argv), 'arguments.')
print ('Argument List:', str(sys.argv))

def replaceTabsWithSpaces( filePath ):
    inputFile = open( filePath,"r" )
    tempFileName = "temp"
    exportFile = open( tempFileName, "w" )
    for line in inputFile:
        new_line = line.replace('\t', "    ")
        exportFile.write(new_line)

    inputFile.close()
    exportFile.close()

    os.remove(filePath)
    os.rename(tempFileName,filePath)

for root, dirs, files in os.walk("."):
    for file in files:
        if "CMake" in file:
            continue
        if file.endswith(".h") or file.endswith(".c") or file.endswith(".hpp") or file.endswith(".cpp"):
            filePath = os.path.join(root, file)
            replaceTabsWithSpaces( filePath )