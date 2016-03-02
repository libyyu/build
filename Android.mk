
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	LOCAL_PATH := $(call my-dir)
	include $(CLEAR_VARS)
	THIRD_MODULS :=
else ifeq ($(TARGET_ARCH_ABI),x86)
	LOCAL_PATH := $(call my-dir)
	include $(CLEAR_VARS)
	THIRD_MODULS :=
else
	THIRD_MODULS :=
endif

include $(CLEAR_VARS)

# override strip command to strip all symbols from output library; no need to ship with those ...
# cmd-strip = $(TOOLCHAIN_PREFIX)strip $1

LOCAL_ARM_MODE  := arm
LOCAL_PATH      := $(NDK_PROJECT_PATH)
LUA_PATH        := $(LOCAL_PATH)/lua-5.1.5/src
#JSON_PATH       := $(LOCAL_PATH)/cjson
LOCAL_MODULE    := FLua
LOCAL_CPP_FEATURES := rtti exceptions
#LOCAL_CFLAGS   += -D _ANDROID -fvisibility=hidden
LOCAL_CFLAGS   += -D _ANDROID -D ANDROID
#LOCAL_CFLAGS   += -D _ANDROID -D __OpenBSD__
#LOCAL_CFLAGS   += -fexceptions
#LOCAL_CFLAGS   += -frtti

LOCAL_SRC_FILES := \
$(LOCAL_PATH)/slua.c \
$(LOCAL_PATH)/pb.c \
$(LUA_PATH)/lapi.c \
$(LUA_PATH)/lauxlib.c \
$(LUA_PATH)/lbaselib.c \
$(LUA_PATH)/lcode.c \
$(LUA_PATH)/ldblib.c \
$(LUA_PATH)/ldebug.c \
$(LUA_PATH)/ldo.c \
$(LUA_PATH)/ldump.c \
$(LUA_PATH)/lfunc.c \
$(LUA_PATH)/lgc.c \
$(LUA_PATH)/linit.c \
$(LUA_PATH)/liolib.c \
$(LUA_PATH)/llex.c \
$(LUA_PATH)/lmathlib.c \
$(LUA_PATH)/lmem.c \
$(LUA_PATH)/loadlib.c \
$(LUA_PATH)/lobject.c \
$(LUA_PATH)/lopcodes.c \
$(LUA_PATH)/loslib.c \
$(LUA_PATH)/lparser.c \
$(LUA_PATH)/lstate.c \
$(LUA_PATH)/lstring.c \
$(LUA_PATH)/lstrlib.c \
$(LUA_PATH)/ltable.c \
$(LUA_PATH)/ltablib.c \
$(LUA_PATH)/ltm.c \
$(LUA_PATH)/lundump.c \
$(LUA_PATH)/lvm.c \
$(LUA_PATH)/lzio.c \
$(LUA_PATH)/print.c \


LOCAL_SRC_FILES  += $(THIRD_SRCS)

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
$(LOCAL_PATH)/lua-5.1.5/src


LOCAL_SHARE_LIBARIES  := \
libcutils \
libdl

LOCAL_STATIC_LIBARIES := $(THIRD_MODULS)
LOCAL_LDLIBS          += -landroid -llog

#cmd-strip = $(ndk)/arm-linux-androideabi-4.8/prebuild/strip -s --strip-debug -x $1

include $(BUILD_SHARED_LIBRARY)