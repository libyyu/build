#!/bin/bash
#
# Linux 32-bit/64-bit

# Copyright (C) polynation games ltd - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Christopher Redden, December 2013

# 32 Bit Version
mkdir -p linux/x86

cd luajit
make clean

make BUILDMODE=static CC="gcc -fPIC -m32"
cp src/libluajit.a ../linux/x86/libluajit.a

cd ../pbc/
make clean
make BUILDMODE=static CC="gcc -fPIC -m32"
cp build/libpbc.a ../linux/x86/libpbc.a

cd ../cjson/
make clean
make BUILDMODE=static CC="gcc -fPIC -m32"
cp build/libcjson.a ../linux/x86/libcjson.a

cd ..
gcc -fPIC \
	lua_wrap.c \
	pb.c \
	lpeg.c \
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
    luasocket/src/unix.c \
    luasocket/src/except.c \
    luasocket/src/select.c \
    luasocket/src/usocket.c \
	-o Plugins/x86/libslua.so -shared -m32 \
	-I./ \
	-Iluajit/src \
	-Ipbc \
	-Icjson \
	-Isqlite/src \
	-Iluasocket/src \
	-Wl,--whole-archive \
	linux/x86/libluajit.a \
	linux/x86/libpbc.a \
	linux/x86/libcjson.a \
	-Wl,--no-whole-archive -static-libgcc -static-libstdc++
