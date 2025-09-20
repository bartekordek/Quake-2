@echo off

set CONFIGURATION=%1
set TARGET_DIR=quake_data

rem Assumption 00: You were using Build-Windows as your solution root.
rem Assumption 01: Your Quake 2 Data is inside of quake_data dir.

echo Copying file for %CONFIGURATION%

xcopy Build-Windows\%CONFIGURATION%\*.dll %TARGET_DIR%\ /Y
xcopy Build-Windows\%CONFIGURATION%\*.exe %TARGET_DIR%\ /Y
xcopy Build-Windows\%CONFIGURATION%\*.pdb %TARGET_DIR%\ /Y