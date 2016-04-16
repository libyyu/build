@echo off
set OLD_PATH=%PATH%
set MinGW=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW
set module_name=FengEngine

set PATH=%MinGW%\x64\bin;%OLD_PATH%
call "make-windows-inner.bat" %module_name% x64

copy x64\%module_name%.dll ..\..\..\LuaGame\ProjectUnity\Assets\Plugins\x64\%module_name%.dll /y

set PATH=%MinGW%\x86\bin;%OLD_PATH%
call "make-windows-inner.bat" %module_name% x86

copy x86\%module_name%.dll ..\..\..\LuaGame\ProjectUnity\Assets\Plugins\x86\%module_name%.dll /y

pause