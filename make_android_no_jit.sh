echo "Compiling NativeCode..."

$ndk/ndk-build NDK_PEOJECT_PATH=. NDK_APPLICATION_MK=Application.mk $*

echo "Cleaning up / removing build folders"

cp ./libs/armeabi-v7a/libslua.so ../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/armeabi-v7a/libslua.so
cp ./libs/x86/libslua.so ../LuaGame/ProjectUnity/Assets/Plugins/Android/libs/x86/libslua.so

echo "Done!"