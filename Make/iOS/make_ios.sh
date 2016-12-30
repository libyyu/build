#!/usr/bin
cd "$( dirname "${BASH_SOURCE[0]}" )"

cd FengEngine
xcodebuild clean
xcodebuild -configuration=Release
cp -f ./build/Release-iphoneos/libFengEngine.a ../../../../LuaGame/ProjectUnity/Assets/Plugins/iOS/
cd ..

