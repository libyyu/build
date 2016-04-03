export NDK=D:/AndriodTools/adt-bundle-windows/ndk-r8d

CUR_PATH=$(cd `dirname $0`; pwd)
echo $CUR_PATH

CUR_DIR=CUR_PATH

cd ../../../curl-7.48.0

echo "libcurl for x86 "
make clean

export CC="$NDK/toolchains/x86-4.6/prebuilt/windows/bin/i686-linux-android-gcc --sysroot $NDK/platforms/android-14/arch-x86 -Wl,--fix-cortex-a8"

./configure --host=x86-linux-androideabi

make

cp lib/.libs/libcurl.a $CUR_DIR/libcurl-x86.a
echo "................................"

echo "libcurl for arm-v7"
make clean

export CC="$NDK/toolchains/arm-linux-androideabi-4.6/prebuilt/windows/bin/arm-linux-androideabi-gcc --sysroot $NDK/platforms/android-14/arch-arm -march=armv7-a -Wl,--fix-cortex-a8"
./configure --host=arm-linux-androideabi

make

cp lib/.libs/libcurl.a $CUR_DIR/libcurl-arm-v7.a



