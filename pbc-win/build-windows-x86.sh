echo "Build libpbc for X86"
make clean
make BUILDMODE=static CC="gcc -m32"
cp build/libpbc.a ../window/x86/libpbc.a
cp build/libpbc.a ../libpbc-window-x86.a