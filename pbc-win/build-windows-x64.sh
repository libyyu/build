echo "Build libpbc for X64"
make clean
make BUILDMODE=static CC="gcc -m64"
cp build/libpbc.a ../window/x86_64/libpbc.a
cp build/libpbc.a ../libpbc-window-x64.a