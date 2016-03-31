
echo ""
echo "Compiling NativeCode..."
ndk-build.cmd  NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk OUT_MODULE_NAME=slua
@rem #mv libs/armeabi/libAutoPF.so ./AutoPF.so

echo ""
echo "Cleaning up / removing build folders..."  #optional..
@rem #rm -rf libs
@rem #rm -rf obj

echo ""
echo "Done!"

cp ./libs/armeabi-v7a/libslua.so ../../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/armeabi-v7a/libslua.so

cp ./libs/x86/libslua.so ../../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/x86/libslua.so


pause
