#!/bin/sh
# AUTO-GENERATED FILE, DO NOT EDIT!
if [ -f $1.org ]; then
  sed -e 's!^E:/WorkSpace/Game/MinGW/MinGW/x86/msys/1\.0/!/!ig;s! E:/WorkSpace/Game/MinGW/MinGW/x86/msys/1\.0/! /!ig;s!^E:/WorkSpace/Game/MinGW/MinGW/x86/msys/1\.0!/usr!ig;s! E:/WorkSpace/Game/MinGW/MinGW/x86/msys/1\.0! /usr!ig;s!^C:/Users/LIEACE~1\.FEN/AppData/Local/Temp!/tmp!ig;s! C:/Users/LIEACE~1\.FEN/AppData/Local/Temp! /tmp!ig;s!^d:/mingw/x86!/mingw!ig;s! d:/mingw/x86! /mingw!ig;s!^z:!/z!ig;s! z:! /z!ig;s!^e:!/e!ig;s! e:! /e!ig;s!^d:!/d!ig;s! d:! /d!ig;s!^c:!/c!ig;s! c:! /c!ig;' $1.org > $1 && rm -f $1.org
fi
