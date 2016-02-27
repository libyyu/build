#!/bin/bash
#
# OS X 32-bit

# Copyright (C) polynation games ltd - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Christopher Redden, December 2013

cd luajit

IXCODE=`xcode-select -print-path`
ISDK=$IXCODE/Platforms/iPhoneOS.platform/Developer
ISDKVER=iPhoneOS8.1.sdk
ISDKP=`xcrun -find -sdk iphoneos ar | xargs dirname`/
ISDKF="-arch armv7 -isysroot $ISDK/SDKs/$ISDKVER"
IGCC=`xcrun -find -sdk iphoneos gcc`

cd ..
$IGCC -arch armv7 -isysroot $ISDK/SDKs/$ISDKVER -c ../slua.c -o ios/slua.o -Iluajit/src
"$ISDKP"ar rcs ios/slua.a ios/slua.o

$IGCC -arch arm64 -isysroot $ISDK/SDKs/$ISDKVER -c ../slua.c -o ios/slua.o -Iluajit/src
"$ISDKP"ar rcs ios/slua.a ios/slua.o

libtool -static -o ios/sLua.a ios/slua.a ios/libluajit.a


