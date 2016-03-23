
echo ""
echo "Compiling NativeCode..."
ndk-build.cmd  NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk
@rem #mv libs/armeabi/libAutoPF.so ./AutoPF.so

echo ""
echo "Cleaning up / removing build folders..."  #optional..
@rem #rm -rf libs
@rem #rm -rf obj

echo ""
echo "Done!"

pause
