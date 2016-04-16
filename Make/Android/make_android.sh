
#!/bin/sh
echo ""
echo "Compiling NativeCode..."

ndk=D:/AndriodTools/adt-bundle-windows/ndk-r8d
out_name=FengEngine

$ndk/ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk $* OUT_MODULE_NAME=$out_name
#mv libs/armeabi/libAutoPF.so ./AutoPF.so

echo ""
echo "Cleaning up / removing build folders..."  #optional..
#rm -rf libs
#rm -rf obj

echo ""
echo "Done!"

cp ./libs/armeabi-v7a/lib${out_name}.so ../../../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/armeabi-v7a/lib${out_name}.so

cp ./libs/x86/lib${out_name}.so ../../../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/x86/lib${out_name}.so


