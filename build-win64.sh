#!/bin/bash
#
# Windows 32-bit/64-bit

# Copyright (C) polynation games ltd - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Christopher Redden, December 2013

# 64 Bit Version
mkdir -p window/x86_64
mkdir -p Plugins/x86_64

# cp ../Assets/slua_src/slua.c slua.c

cd luajit-2.1.0
mingw32-make clean

mingw32-make BUILDMODE=static CC="gcc -m64"
cp src/libluajit.a ../window/x86_64/libluajit.a

cd ../pbc-win/
make clean
make BUILDMODE=static CC="gcc -m64"
cp build/libpbc.a ../window/x86_64/libpbc.a

cd ../lua-cjson-2.1.0/
make clean
make BUILDMODE=static CC="gcc -m64"
cp build/libcjson.a ../window/x86_64/libcjson.a

cd ..

gcc slua.c \
    pb.c \
    lpeg/lpeg.c \
    pbc-win/binding/lua/pbc-lua.c \
	lua-cjson-2.1.0/lua_cjson.c \
    sqlite-amalgamation-3081101/sqlite3.c \
    lsqlite3-master/lsqlite3.c \
    luasocket-2.0.2/src/luasocket.c \
    luasocket-2.0.2/src/timeout.c \
    luasocket-2.0.2/src/buffer.c \
    luasocket-2.0.2/src/io.c \
    luasocket-2.0.2/src/auxiliar.c \
    luasocket-2.0.2/src/options.c \
    luasocket-2.0.2/src/inet.c \
    luasocket-2.0.2/src/tcp.c \
    luasocket-2.0.2/src/udp.c \
    luasocket-2.0.2/src/except.c \
    luasocket-2.0.2/src/select.c \
    luasocket-2.0.2/src/wsocket.c \
    -o Plugins/x86_64/slua.dll -m64 -shared \
    -I./ \
    -Iluajit-2.1.0/src/ \
    -Ilpeg \
    -Ipbc-win \
    -Ilua-cjson-2.1.0 \
    -Isqlite-amalgamation-3081101 \
    -Ilsqlite3-master \
    -Iluasocket-2.0.2/src \
    -Wl,--whole-archive \
    window/x86_64/libluajit.a \
    window/x86_64/libpbc.a \
    window/x86_64/libcjson.a \
    -Wl,--no-whole-archive -lwsock32 -static-libgcc -static-libstdc++


 cp Plugins/x86_64/slua.dll ../ProjectUnity/Assets/Plugins/x64/
