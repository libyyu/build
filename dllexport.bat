@echo off
echo ���ڲ鿴dll�ļ�

SET DLL_PATH="%1"
SET DLL_PATH=%DLL_PATH:"=%
IF "DLL_PATH" == "" GOTO EXIT
set path=%path%;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin

dumpbin /exports %DLL_PATH% /out:%DLL_PATH%.txt


echo ��ȡ�ɹ���
:EXIT