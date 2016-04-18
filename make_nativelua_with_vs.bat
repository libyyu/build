@echo off

set build_config=%1
if "%1" == "" (goto vs2015 ) 

if /i "%build_config%" == "vs2015" ( goto vs2015 ) else ( goto check_vs2013 )

:check_vs2013
if /i "%build_config%" == "vs2013" ( goto vs2013 ) else ( goto check_vs2012 )

:check_vs2012
if /i "%build_config%" == "vs2012" ( goto vs2012 ) else ( goto check_vs2010 )

:check_vs2010
if /i "%build_config%" == "vs2010" ( goto vs2010 ) else ( goto check_vs2008 )

:check_vs2008
if /i "%build_config%" == "vs2008" ( goto vs2008 ) else ( goto missing )

:vs2015
if exist "%VS140COMNTOOLS%" (
	set VCVARS="%VS140COMNTOOLS%..\..\VC\bin\"
	goto build
	) ^
else (goto missing)

:vs2013
if exist "%VS120COMNTOOLS%" (
	set VCVARS="%VS120COMNTOOLS%..\..\VC\bin\"
	goto build
	) ^
else (goto missing)

:vs2012
if exist "%VS110COMNTOOLS%" (
	set VCVARS="%VS110COMNTOOLS%..\..\VC\bin\"
	goto build
	) ^
else (goto missing)

:vs2010
if exist "%VS100COMNTOOLS%" (
	set VCVARS="%VS100COMNTOOLS%..\..\VC\bin\"
	goto build
	) ^
else (goto missing)

:vs2008
if exist "%VS90COMNTOOLS%" (
	set VCVARS="%VS90COMNTOOLS%..\..\VC\bin\"
	goto build
	) ^
else (goto missing)


:build
echo "build begin...%1"

@set CUR_DIR=%cd%

@set PROJECT_PATH=%cd%\Project-Source
@set LUAPATH=lua-5.1.5

echo Use %VCVARS%

@set ENV32="%VCVARS%vcvars32.bat"
@set ENV64="%VCVARS%amd64\vcvars64.bat"

call "%ENV32%"
echo "Swtich to x86 build env"
cd %LUAPATH%\src

echo -------------------------------1
echo build lua debug static x86
call msvcbuild.bat debug static
mkdir %PROJECT_PATH%\lib\x86\Debug
mkdir %PROJECT_PATH%\bin\native\x86\Debug
copy /Y lua51_d.lib %PROJECT_PATH%\lib\x86\Debug\liblua.lib
copy /Y lua.exe %PROJECT_PATH%\bin\native\x86\Debug\lua.exe
copy /Y luac.exe %PROJECT_PATH%\bin\native\x86\Debug\luac.exe

echo -------------------------------2
echo build lua release static x86
call msvcbuild.bat static
mkdir %PROJECT_PATH%\lib\x86\Release
mkdir %PROJECT_PATH%\bin\native\x86\Release
copy /Y lua51.lib %PROJECT_PATH%\lib\x86\Release\liblua.lib
copy /Y lua.exe %PROJECT_PATH%\bin\native\x86\Release\lua.exe
copy /Y luac.exe %PROJECT_PATH%\bin\native\x86\Release\luac.exe

cd %CUR_DIR%

call "%ENV64%"
echo "Swtich to x64 build env"
cd %LUAPATH%\src

echo -------------------------------3
echo build lua debug static x64
call msvcbuild.bat debug static
mkdir %PROJECT_PATH%\lib\x64\Debug
mkdir %PROJECT_PATH%\bin\native\x64\Debug
copy /Y lua51_d.lib %PROJECT_PATH%\lib\x64\Debug\liblua.lib
copy /Y lua.exe %PROJECT_PATH%\bin\native\x64\Debug\lua.exe
copy /Y luac.exe %PROJECT_PATH%\bin\native\x64\Debug\luac.exe

echo -------------------------------4
echo build lua release static x64
call msvcbuild.bat static
mkdir %PROJECT_PATH%\lib\x64\Release
mkdir %PROJECT_PATH%\bin\native\x64\Release
copy /Y lua51_d.lib %PROJECT_PATH%\lib\x64\Release\liblua.lib
copy /Y lua.exe %PROJECT_PATH%\bin\native\x64\Release\lua.exe
copy /Y luac.exe %PROJECT_PATH%\bin\native\x64\Release\luac.exe

cd %CUR_DIR%


goto buildfinily

:missing
echo Can't find Visual Studio 2008-2015.
goto result

:buildfinily
echo =======================Build Success=======================

:result
pause