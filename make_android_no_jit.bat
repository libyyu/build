@echo off

echo "Comiling NativeCode"
ndk-build.cmd NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk
@rem #mv libs/armeabi/libAutoPF.so ./AutoPF.so

echo "Cleaning up / removing build folders..." #optionl...
@rem #rm -rf libs
@rem #rm -rf obj

copy ./libs/armeabi-v7a/libslua.so ../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/armeabi-v7a/libslua.so /y
copy ./libs/x86/libslua.so ../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/x86/libslua.so /y

echo "Done"

pause