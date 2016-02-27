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

cd ../pbc/
make clean
make BUILDMODE=static CC="gcc -m64"
cp build/libpbc.a ../window/x86_64/libpbc.a

cd ../cjson/
make clean
make BUILDMODE=static CC="gcc -m64"
cp build/libcjson.a ../window/x86_64/libcjson.a

cd ..

gcc slua.c \
    pb.c \
    lpeg/lpeg.c \
    pbc/binding/lua/pbc-lua.c \
	cjson/lua_cjson.c \
    sqlite/src/sqlite3.c \
    sqlite/lsqlite3.c \
    luasocket/src/luasocket.c \
    luasocket/src/timeout.c \
    luasocket/src/buffer.c \
    luasocket/src/io.c \
    luasocket/src/auxiliar.c \
    luasocket/src/options.c \
    luasocket/src/inet.c \
    luasocket/src/tcp.c \
    luasocket/src/udp.c \
    luasocket/src/except.c \
    luasocket/src/select.c \
    luasocket/src/wsocket.c \
    -o Plugins/x86_64/slua.dll -m64 -shared \
    -I./ \
    -Iluajit-2.1.0/src/ \
    -Ilpeg \
    -Ipbc \
    -Icjson \
    -Isqlite/src \
    -Iluasocket/src \
    -Wl,--whole-archive \
    window/x86_64/libluajit.a \
    window/x86_64/libpbc.a \
    window/x86_64/libcjson.a \
    -Wl,--no-whole-archive -lwsock32 -static-libgcc -static-libstdc++


 cp Plugins/x86_64/slua.dll ../ProjectUnity/Assets/Plugins/x64/
