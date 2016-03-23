@set OLD_PATH=%PATH%

set PATH=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW\x64\bin;%OLD_PATH%
call "make-windows-inner.bat" x64

set PATH=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW\x86\bin;%OLD_PATH%
call "make-windows-inner.bat" x86

pause