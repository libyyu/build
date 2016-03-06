#!/usr/bin/env bash
cd "$( dirname "${BASH_SOURCE[0]}" )"
#cp slua.c luajit-2.1.0/src/
cd luajit-2.1.0
LIPO="xcrun -sdk iphoneos lipo"
STRIP="xcrun -sdk iphoneos strip"

IXCODE=`xcode-select -print-path`
ISDK=$IXCODE/Platforms/iPhoneOS.platform/Developer
ISDKVER=iPhoneOS9.1.sdk
ISDKP=$IXCODE/usr/bin/
ISDKD=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/

if [ ! -e $ISDKP/ar ]; then 
  sudo cp $ISDKD/usr/bin/ar $ISDKP
fi

if [ ! -e $ISDKP/ranlib ]; then
  sudo cp $ISDKD/usr/bin/ranlib $ISDKP
fi

if [ ! -e $ISDKP/strip ]; then
  sudo cp $ISDKD/usr/bin/strip $ISDKP
fi

make clean
ISDKF="-arch armv7 -isysroot $ISDK/SDKs/$ISDKVER"
make HOST_CC="gcc -m32 -std=c99" CROSS="$ISDKP" TARGET_FLAGS="$ISDKF" TARGET=armv7 TARGET_SYS=iOS LUAJIT_A=libsluav7.a


make clean
ISDKF="-arch armv7s -isysroot $ISDK/SDKs/$ISDKVER"
make HOST_CC="gcc -m32 -std=c99" CROSS="$ISDKP" TARGET_FLAGS="$ISDKF" TARGET=armv7s TARGET_SYS=iOS LUAJIT_A=libsluav7s.a

make clean
ISDKF="-arch arm64 -isysroot $ISDK/SDKs/$ISDKVER"
make HOST_CC="gcc -std=c99" CROSS="$ISDKP" TARGET_FLAGS="$ISDKF" TARGET=arm64 TARGET_SYS=iOS LUAJIT_A=libslua64.a

cd src
lipo libsluav7.a -create libsluav7s.a libslua64.a -output libslua.a
cd ../slua
xcodebuild clean
xcodebuild -configuration=Release
cp -f ./build/Release-iphoneos/libslua.a ../../../LuaGame/ProjectUnity/Assets/Plugins/iOS/
cd ..

