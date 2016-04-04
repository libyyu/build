
echo ""
echo "Compiling NativeCode..."
ndk-build.cmd  NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application-Agent.mk OUT_MODULE_NAME=agent
@rem #mv libs/armeabi/libAutoPF.so ./AutoPF.so

echo ""
echo "Cleaning up / removing build folders..."  #optional..
@rem #rm -rf libs
@rem #rm -rf obj

echo ""
echo "Done!"

pause
