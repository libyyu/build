#!/usr/bin/env bash
cd "$( dirname "${BASH_SOURCE[0]}" )"

cd FengEngine
xcodebuild clean
xcodebuild -configuration=Release
cp -r build/Release/FengEngine.bundle ../../../../LuaGame/ProjectUnity/Assets/Plugins/
echo "build success."