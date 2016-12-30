#!/usr/bin/env bash

cd "$( dirname "${BASH_SOURCE[0]}" )"
#cp slua.c luajit-2.1.0/src/

cd ../../lua-5.1.5
cd osxproject/lua
xcodebuild -configuration=Release
cp -r Build/Release/liblua.a ../../../Make/MacWithLua/
