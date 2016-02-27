#!/bin/bash
#
# OS X 32-bit
# Copyright (C) polynation games ltd - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Christopher Redden, December 2013

cd luajit-2.1.0
make clean
make CC="gcc -m32" BUILDMODE=static
cp src/libluajit.a ../luajit-osx/libluajit_x86.a

make clean
make CC="gcc" BUILDMODE=static
cp src/libluajit.a ../luajit-osx/libluajit_x86_64.a

cd ../luajit-osx/
xcodebuild
cd ..

mkdir -p ./Plugins
cp -r ./luajit-osx/DerivedData/slua/Build/Products/Release/slua.bundle ./Plugins/
