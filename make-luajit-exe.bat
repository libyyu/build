@echo off
@set OLD_PATH=%PATH%
@set MinGW=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW

@echo Build luajit Windows X64
set PATH=%MinGW%\x64\bin;%OLD_PATH%

gcc luajit-console.c ^
	-o window\x86_64\luajit-x64.exe ^
	-I./ ^
	-Iluajit-2.0.4/src ^
	-Wl,--whole-archive window\x86_64\slua.dll ^
	-Wl,--no-whole-archive -static-libgcc -static-libstdc++ -lstdc++
@echo Leave build X64

@echo Build luajit Windows X86
set PATH=%MinGW%\x86\bin;%OLD_PATH%

gcc luajit-console.c ^
	-o window\x86\luajit-x86.exe ^
	-I./ ^
	-Iluajit-2.0.4/src ^
	-Wl,--whole-archive window\x86\slua.dll ^
	-Wl,--no-whole-archive -static-libgcc -static-libstdc++
@echo Leave build X86

pause