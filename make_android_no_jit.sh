echo "Compiling NativeCode..."

$ndk/ndk-build NDK_PEOJECT_PATH=. NDK_APPLICATION_MK=Application.mk $*

echo "Cleaning up / removing build folders"


echo "Done!"