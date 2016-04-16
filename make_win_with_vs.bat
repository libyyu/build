@echo off

@set LUAJIT=luajit-2.1.0

if exist "%VS140COMNTOOLS%" (
	set VCVARS="%VS140COMNTOOLS%..\..\VC\bin\"
	goto build
	)
else (goto check2013)

:check2013
if exist "%VS120COMNTOOLS%" (
	set VCVARS="%VS120COMNTOOLS%..\..\VC\bin\"
	goto build
	) 
else (goto check2012)

:check2012
if exist "%VS110COMNTOOLS%" (
	set VCVARS="%VS110COMNTOOLS%..\..\VC\bin\"
	goto build
	) 
else (goto check2010)

:check2010
if exist "%VS100COMNTOOLS%" (
	set VCVARS="%VS100COMNTOOLS%..\..\VC\bin\"
	goto build
	) 
else (goto missing)

:build
echo Use %VCVARS%

@set ENV32="%VCVARS%vcvars32.bat"
@set ENV64="%VCVARS%amd64\vcvars64.bat"

call "%ENV32%"
echo Swtich to x86 build env
cd %LUAJIT%\src

echo build luajit debug static x86
call msvcbuild.bat debug static
copy /Y luajit51_d.lib ..\..\Project-Source\lib\x86\Debug\libluajit.lib
copy /Y luajit.exe ..\..\Project-Source\bin\x86\Debug\luajit.exe

echo build luajit release static x86
call msvcbuild.bat static
copy /Y luajit.lib ..\..\Project-Source\lib\x86\Release\libluajit.lib
copy /Y luajit.exe ..\..\Project-Source\bin\x86\Release\luajit.exe
cd ..\..

call "%ENV64%"
echo Swtich to x64 build env
cd %LUAJIT%\src

echo build luajit debug static x64
call msvcbuild.bat debug static
copy /Y luajit51_d.lib ..\..\Project-Source\lib\x64\Debug\libluajit.lib
copy /Y luajit.exe ..\..\Project-Source\bin\x64\Debug\luajit.exe

echo build luajit release static x64
call msvcbuild.bat static
copy /Y luajit.lib ..\..\Project-Source\lib\x64\Release\libluajit.lib
copy /Y luajit.exe ..\..\Project-Source\bin\x64\Release\luajit.exe
cd ..\..

goto buildfinily

:missing
echo Can't find Visual Studio 2010-2015.
goto result

:buildfinily
echo =======================Build Success=======================

:result
pause

