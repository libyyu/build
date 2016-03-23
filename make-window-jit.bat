@echo off

@set OLD_PATH=%PATH%

echo "Build LuaJit For x64"
PATH=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW\x64\bin;%OLD_PATH%

@del libluajit-window-x64.a
cd luajit-2.0.4
@del src\*.o src\host\*.o src\*.obj src\*.manifest src\libluajit.a src\host\minilua.exe src\host\buildvm.exe src\luajit.exe
mingw32-make clean
mingw32-make BUILDMODE=static CC="gcc -m64"
copy src\libluajit.a ..\libluajit-window-x64.a /y
cd ..
PATH=%OLD_PATH%

echo "Successful Build LuaJit For Window X64"

echo "Build LuaJit For x86"
PATH=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW\x86\bin;%OLD_PATH%

@del libluajit-window-x86.a
cd luajit-2.0.4
@del src\*.o src\host\*.o src\*.obj src\*.manifest src\libluajit.a src\host\minilua.exe src\host\buildvm.exe src\luajit.exe
mingw32-make clean
mingw32-make BUILDMODE=static CC="gcc -m32"
copy src\libluajit.a ..\libluajit-window-x86.a /y
cd ..
PATH=%OLD_PATH%

echo "Successful Build LuaJit For Window X86"
pause
