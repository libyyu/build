
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
JSON_PATH       := $(LOCAL_PATH)/cjson
LPEG_PATH       := $(LOCAL_PATH)/lpeg
SQLITE_PATH     := $(LOCAL_PATH)/sqlite
SOCKET_PATH     := $(LOCAL_PATH)/luasocket/src
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
$(LOCAL_PATH)/bit.c \
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
$(JSON_PATH)/fpconv.c \
$(JSON_PATH)/strbuf.c \
$(JSON_PATH)/lua_cjson.c \
$(LPEG_PATH)/lpeg.c \
$(SQLITE_PATH)/src/sqlite3.c \
$(SQLITE_PATH)/lsqlite3.c \
$(SOCKET_PATH)/luasocket.c \
$(SOCKET_PATH)/timeout.c \
$(SOCKET_PATH)/buffer.c \
$(SOCKET_PATH)/io.c \
$(SOCKET_PATH)/auxiliar.c \
$(SOCKET_PATH)/options.c \
$(SOCKET_PATH)/inet.c \
$(SOCKET_PATH)/tcp.c \
$(SOCKET_PATH)/udp.c \
$(SOCKET_PATH)/unix.c \
$(SOCKET_PATH)/mime.c \
$(SOCKET_PATH)/except.c \
$(SOCKET_PATH)/select.c \
$(SOCKET_PATH)/usocket.c \


LOCAL_SRC_FILES  += $(THIRD_SRCS)

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
$(LUA_PATH) \
$(JSON_PATH) \
$(LPEG_PATH) \
$(SQLITE_PATH)/src \
$(SOCKET_PATH)


LOCAL_SHARE_LIBARIES  := \
libcutils \
libdl

LOCAL_STATIC_LIBARIES := $(THIRD_MODULS)
LOCAL_LDLIBS          += -landroid -llog

#cmd-strip = $(ndk)/arm-linux-androideabi-4.8/prebuild/strip -s --strip-debug -x $1

include $(BUILD_SHARED_LIBRARY)