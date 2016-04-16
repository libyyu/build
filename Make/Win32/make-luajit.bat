@echo off

@set OLD_PATH=%PATH%
@set MinGW=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW
@set LUAJIT_PATH=luajit-2.0.4
::保存当前工作目录
@set CUR_DIR=%cd%

echo "build luajit for x64"
PATH=%MinGW%\x64\bin;%OLD_PATH%

@del libluajit-x64.a
::进入主工程目录
cd ..\..
::进入LuaJIT目录
cd %LUAJIT_PATH%
@del src\*.o src\host\*.o src\*.obj src\*.manifest src\libluajit.a src\host\minilua.exe src\host\buildvm.exe src\luajit.exe
mingw32-make clean
mingw32-make BUILDMODE=static CC="gcc -m64"
copy src\libluajit.a %CUR_DIR%\libluajit-x64.a /y
copy src\luajit.exe %CUR_DIR%\x64\luajit.exe /y
::还原到工作目录
cd %CUR_DIR%
PATH=%OLD_PATH%

echo "Successful Build LuaJit For Window X64"
echo "---------------------------------------------------------"
echo "build luajit for x86"
PATH=%MinGW%\x86\bin;%OLD_PATH%

@del libluajit-x86.a
::进入主工程目录
cd ..\..
::进入LuaJIT目录
cd %LUAJIT_PATH%
@del src\*.o src\host\*.o src\*.obj src\*.manifest src\libluajit.a src\host\minilua.exe src\host\buildvm.exe src\luajit.exe
mingw32-make clean
mingw32-make BUILDMODE=static CC="gcc -m32"
copy src\libluajit.a %CUR_DIR%\libluajit-x86.a /y
copy src\luajit.exe %CUR_DIR%\x86\luajit.exe /y
::还原到工作目录
cd %CUR_DIR%
PATH=%OLD_PATH%

echo "Successful Build LuaJit For Window X86"

pause
