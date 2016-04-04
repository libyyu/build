APP_OPTIM      		:= release
APP_ABI        		:= armeabi-v7a x86
APP_PLATFORM   		:= android-14
APP_BUILD_SCRIPT    := Android-Agent.mk
APP_STL             := gnustl_static
APP_CPPFLAGS        += -fexceptions
APP_USE_CPP0X       := true