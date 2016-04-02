
echo ""
echo "Compiling NativeCode..."
ndk-build.cmd  NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk

echo ""
echo "Done!"

pause
