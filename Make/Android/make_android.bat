
echo ""
echo "Compiling NativeCode..."

@set out_name=FengEngine

ndk-build.cmd  NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk OUT_MODULE_NAME=%out_name%
@rem #mv libs/armeabi/libAutoPF.so ./AutoPF.so

echo ""
echo "Cleaning up / removing build folders..."  #optional..
@rem #rm -rf libs
@rem #rm -rf obj

echo ""
echo "Done!"


cp ./libs/armeabi-v7a/lib%out_name%.so ../../../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/armeabi-v7a/lib%out_name%.so

cp ./libs/x86/lib%out_name%.so ../../../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/x86/lib%out_name%.so


pause
