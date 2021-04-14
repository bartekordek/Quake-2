#!/usr/bin/python3

import sys
import ntpath
import glob, os

print ('Number of arguments:', len(sys.argv), 'arguments.')
print ('Argument List:', str(sys.argv))


for root, dirs, files in os.walk("."):
    for file in files:
        if file.find( "CMakeC" ) != -1:
            continue
        oldFile = os.path.join(root, file)
        if file.endswith(".h"):
            newFileName = oldFile.replace( ".h", ".hpp" )
        elif file.endswith(".c"):
            newFileName = oldFile.replace( ".c", ".cpp" )
        else:
            continue
        print( "Old file: " + oldFile )
        print( "New file: " + newFileName )
        os.rename(oldFile,newFileName)