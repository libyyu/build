@echo off

echo "Build libluajit.a For x64"
PATH=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW\x64\bin;%PATH%
mkdir window\x86_64

cd luajit-2.0.4
mingw32-make clean
mingw32-make BUILDMODE=static CC="gcc -m64"
copy src\libluajit.a ..\window\x86_64\libluajit.a /y
cd ..

pause
