#!/usr/bin
cd "$( dirname "${BASH_SOURCE[0]}" )"

LUAJIT=../../luajit-2.1.0
cd $LUAJIT

cd slua
xcodebuild clean
xcodebuild -configuration=Release
cp -f ./build/Release-iphoneos/libslua.a ../../../LuaGame/ProjectUnity/Assets/Plugins/iOS/
cd ..

