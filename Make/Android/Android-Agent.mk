
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	LOCAL_PATH:=$(call my-dir)
	include $(CLEAR_VARS)
	LOCAL_MODULE 		:= libluajit-arm-v7
	LOCAL_SRC_FILES		:= libluajit-arm-v7.a
	LOCAL_MODULE 		:= libcurl-arm-v7
	LOCAL_SRC_FILES		:= libcurl-arm-v7.a
	THIRD_MODULS 		:= $(LOCAL_MODULE)
	include $(PREBUILT_STATIC_LIBRARY)
	# THIRD_MODULS :=
	# LOCAL_LDLIBS += -lcurl-arm-v7 -lluajit-arm-v7
else ifeq ($(TARGET_ARCH_ABI),x86)
	LOCAL_PATH:=$(call my-dir)
	include $(CLEAR_VARS)
	LOCAL_MODULE 		:= libluajit-x86
	LOCAL_SRC_FILES		:= libluajit-x86.a
	LOCAL_MODULE 		:= libcurl-x86
	LOCAL_SRC_FILES		:= libcurl-x86.a
	THIRD_MODULS 		:= $(LOCAL_MODULE)
	include $(PREBUILT_STATIC_LIBRARY)
	# THIRD_MODULS :=
	# LOCAL_LDLIBS += -lcurl-x86 -lluajit-x86
else
	THIRD_MODULS :=
endif

# $(warning  " NDK_PROJECT_PATH =  $(NDK_PROJECT_PATH)")  
include $(CLEAR_VARS)
LOCAL_FORCE_STATIC_EXECUTABLE := true

# override strip command to strip all symbols from output library; no need to ship with those ...
# cmd-strip = $(TOOLCHAIN_PREFIX)strip $1

LOCAL_ARM_MODE  := arm
LOCAL_MODULE    := $(OUT_MODULE_NAME)


LOCAL_PATH      := $(NDK_PROJECT_PATH)
PROJECT_PATH    := $(NDK_PROJECT_PATH)/../..
LUA_PATH        := $(PROJECT_PATH)/luajit-2.0.4/src
CURL_PATH		:= $(PROJECT_PATH)/curl-7.48.0
PROJECT_SRC_PATH := $(PROJECT_PATH)/Project-Source

LOCAL_CPP_FEATURES := rtti exceptions
#LOCAL_CFLAGS   += -D _ANDROID -fvisibility=hidden
LOCAL_CFLAGS   += -D _ANDROID -D ANDROID -D CURL_STATICLIB
#LOCAL_CFLAGS   += -D _ANDROID -D __OpenBSD__
#LOCAL_CFLAGS   += -fexceptions
#LOCAL_CFLAGS   += -frtti
#LOCAL_CPPFLAGS  += -03 -ffast-math

LOCAL_C_INCLUDES := $(PROJECT_PATH)
LOCAL_C_INCLUDES += $(LUA_PATH)
LOCAL_C_INCLUDES += $(CURL_PATH)/include
LOCAL_C_INCLUDES += $(PROJECT_SRC_PATH)
LOCAL_C_INCLUDES += $(PROJECT_SRC_PATH)/Common
LOCAL_C_INCLUDES += $(PROJECT_SRC_PATH)/AnyLog


LOCAL_SRC_FILES := \
$(PROJECT_SRC_PATH)/AnyLog/ILog.cpp \
$(PROJECT_SRC_PATH)/Common/FAssist.cpp \
$(PROJECT_SRC_PATH)/FLuaExport.cpp \
$(PROJECT_SRC_PATH)/VersionMan/VersionMan.cpp \
$(PROJECT_SRC_PATH)/Agent/src/TestAgent.cpp


LOCAL_SRC_FILES  += $(THIRD_SRCS)

LOCAL_SHARE_LIBARIES  := \
libcutils \
libdl

LOCAL_STATIC_LIBARIES := $(THIRD_MODULS)
LOCAL_LDLIBS          += -landroid -llog -lc -lGLESv2

LOCAL_WHOLE_STATIC_LIBRARIES += $(LOCAL_STATIC_LIBARIES)

#cmd-strip = $(ndk)/arm-linux-androideabi-4.8/prebuild/strip -s --strip-debug -x $1

include $(BUILD_SHARED_LIBRARY)