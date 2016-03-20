rm ./libluajit-arm-v7.a

cd luajit/src
NDK=D:/AndriodTools/adt-bundle-windows/ndk-r8d
NFK=$NDK/toolchains/arm-linux-androideabi-4.6/prebuilt/windows/bin/arm-linux-androideabi-
make clean

make HOST_CC="gcc -m32 -ffast-math -O3" \
CROSS=$NFK \
TARGET_SYS=Linux \
TARGET_FLAGS="--sysroot $NDK/platforms/android-14/arch-arm -march=armv7-a -Wl,--fix-cortex-a8"
cp ./libluajit.a ../../libluajit-arm-v7.a
cd ../../../
