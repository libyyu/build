
#!/bin/sh
echo ""
echo "Compiling NativeCode..."

ndk=D:/AndriodTools/adt-bundle-windows/ndk-r8d

$ndk/ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk $*
#mv libs/armeabi/libAutoPF.so ./AutoPF.so

echo ""
echo "Cleaning up / removing build folders..."  #optional..
#rm -rf libs
#rm -rf obj

echo ""
echo "Done!"

cp ./libs/armeabi-v7a/libslua.so ../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/armeabi-v7a/libslua.so

cp ./libs/x86/libslua.so ../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/x86/libslua.so


