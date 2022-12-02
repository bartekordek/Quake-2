#!/usr/bin/python3

import sys
import ntpath
import glob, os

print ('Number of arguments:', len(sys.argv), 'arguments.')
print ('Argument List:', str(sys.argv))

def line_prepender(filename, line):
    with open(filename, 'r+') as f:
        content = f.read()
        f.seek(0, 0)
        f.write(line.rstrip('\r\n') + '\n' + content)

def FixHeaderFile( filePath ):
    guard = filePath.replace( ".", "_" )
    guard = guard.replace( "/", "_" )
    guard = guard.replace( "\\", "_" )
    guard = guard.upper()
    guard = "__" + guard + "__"

    with open(filePath) as f:
        firstLine = "#ifndef " + guard
        secondLine = "#define " + guard
        if firstLine in f.read():
            print("Guard exists in " + filePath)
        else:
            print( "There is no guard in " + filePath + ", adding." )
            line_prepender( filePath, "\n" )
            line_prepender( filePath, secondLine )
            line_prepender( filePath, firstLine )

            with open(filePath, "a") as file:
                file.write("\n\n#endif // " + guard)

for root, dirs, files in os.walk("."):
    for file in files:
        if file.endswith(".h"):
            headerPath = os.path.join(root, file)
            if "deps" not in headerPath:
                FixHeaderFile( headerPath )