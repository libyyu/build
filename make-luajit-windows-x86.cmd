@echo off

echo "Build libluajit.a For x86"
PATH=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW\x86\bin;%PATH%
mkdir window\x86

cd luajit-2.0.4
mingw32-make clean
mingw32-make BUILDMODE=static CC="gcc -m32"
copy src\libluajit.a ..\window\x86\libluajit.a /y
cd ..

pause
