LUAJIT=../../luajit-2.1.0
BUILD_DIR=$LUAJIT/build

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR

cd $LUAJIT

rm *.a 1>/dev/null 2>/dev/null

LIPO="xcrun -sdk iphoneos lipo"
STRIP="xcrun -sdk iphoneos strip"

IXCODE=`xcode-select -print-path`
ISDK=$IXCODE/Platforms/iPhoneOS.platform/Developer
ISDKVER=iPhoneOS9.1.sdk
ISDKP=$IXCODE/usr/bin/
ISDKD=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/
echo build luajit-ios

if [ ! -e $ISDKP/ar ]; then
  sudo cp $ISDKD/usr/bin/ar $ISDKP
fi

if [ ! -e $ISDKP/ranlib ]; then
  sudo cp $ISDKD/usr/bin/ranlib $ISDKP
fi

if [ ! -e $ISDKP/strip ]; then
  sudo cp $ISDKD/usr/bin/strip $ISDKP
fi

echo =================================================

make clean
ISDKF="-arch armv7 -isysroot $ISDK/SDKs/$ISDKVER"
make HOST_CC="gcc -m32 -std=c99" CROSS="$ISDKP" TARGET_FLAGS="$ISDKF" TARGET=armv7 TARGET_SYS=iOS LUAJIT_A=libluajitv7.a

make clean
ISDKF="-arch armv7s -isysroot $ISDK/SDKs/$ISDKVER"
make HOST_CC="gcc -m32 -std=c99" CROSS="$ISDKP" TARGET_FLAGS="$ISDKF" TARGET=armv7s TARGET_SYS=iOS LUAJIT_A=libluajitv7s.a

make clean
ISDKF="-arch arm64 -isysroot $ISDK/SDKs/$ISDKVER"
make HOST_CC="gcc -m64 -std=c99" CROSS="$ISDKP" TARGET_FLAGS="$ISDKF" TARGET=arm64 TARGET_SYS=iOS LUAJIT_A=libluajit64.a

cd src
lipo libluajitv7.a -create libluajitv7s.a libluajit64.a -output libluajit.a

cp libluajitv7.a ../../Make/iOS/libluajitv7.a
cp libluajitv7s.a ../../Make/iOS/libluajitv7s.a
cp libluajit64.a ../../Make/iOS/libluajit64.a
cp libluajit.a ../../Make/iOS/libluajit.a