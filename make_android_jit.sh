#!/bin/sh
echo ""
echo "Compiling NativeCode..."

NDK=D:/AndriodTools/adt-bundle-windows/ndk-r8d

echo "build libluajit-arm-v7.a"
rm -f ./libluajit-arm-v7.a

cd luajit-2.0.4/src
rm -f ./libluajit.a
rm -f *.o *.exe
NFK=$NDK/toolchains/arm-linux-androideabi-4.6/prebuilt/windows/bin/arm-linux-androideabi-

make clean
make HOST_CC="gcc -m32 -ffast-math -O3" \
CROSS=$NFK \
TARGET_SYS=Linux \
TARGET_FLAGS="--sysroot $NDK/platforms/android-14/arch-arm -march=armv7-a -Wl,--fix-cortex-a8"
cp ./libluajit.a ../../libluajit-arm-v7.a
cd ../../


echo ""
echo "Success to build libluajit-arm-v7"


echo ""
echo ""
echo "build libluajit-x86.a"
rm -f ./libluajit-x86.a

cd luajit-2.0.4/src
rm -f ./libluajit.a
rm -f *.o *.exe
NFK=$NDK/toolchains/x86-4.6/prebuilt/windows/bin/i686-linux-android-

make clean
make HOST_CC="gcc -m32 -ffast-math -O3" \
CROSS=$NFK \
TARGET_SYS=Linux \
TARGET_FLAGS="--sysroot $NDK/platforms/android-14/arch-x86 -Wl,--fix-cortex-a8"
cp ./libluajit.a ../../libluajit-x86.a
cd ../../

echo ""
echo "Success to build libluajit-x86"
