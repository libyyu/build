@echo off
set OLD_PATH=%PATH%
set MinGW=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW

set PATH=%MinGW%\x64\bin;%OLD_PATH%
call "make-windows-inner.bat" x64

copy x64\slua.dll ..\slua.dll

set PATH=%MinGW%\x86\bin;%OLD_PATH%
call "make-windows-inner.bat" x86

pause