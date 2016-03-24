@echo off
@set OLD_PATH=%PATH%
@set MinGW=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW

echo "Build libpbc for X64"
set PATH=%MinGW%\x64\bin;%OLD_PATH%
mingw32-make clean
mingw32-make BUILDMODE=static CC="gcc -m64"
copy build\libpbc.a ..\window\x86_64\libpbc.a /y
copy build\libpbc.a ..\libpbc-window-x64.a /y

echo "Build libpbc for X86"
set PATH=%MinGW%\x86\bin;%OLD_PATH%
mingw32-make clean
mingw32-make BUILDMODE=static CC="gcc -m32"
copy build\libpbc.a ..\window\x86\libpbc.a /y
copy build\libpbc.a ..\libpbc-window-x86.a /y

pause