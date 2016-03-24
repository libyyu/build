
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	LOCAL_PATH:=$(call my-dir)
	include $(CLEAR_VARS)
	LOCAL_MODULE 		:= libluajit-arm-v7
	LOCAL_SRC_FILES		:= libluajit-arm-v7.a
	THIRD_MODULS 		:= $(LOCAL_MODULE)
	include $(PREBUILT_STATIC_LIBRARY)
else ifeq ($(TARGET_ARCH_ABI),x86)
	LOCAL_PATH:=$(call my-dir)
	include $(CLEAR_VARS)
	LOCAL_MODULE 		:= libluajit-x86
	LOCAL_SRC_FILES		:= libluajit-x86.a
	THIRD_MODULS 		:= $(LOCAL_MODULE)
	include $(PREBUILT_STATIC_LIBRARY)
else
	THIRD_MODULS :=
endif

$(warning $(LOCAL_PATH))
$(warning $(LOCAL_MODULE))
$(warning $(NDK_PROJECT_PATH))

include $(CLEAR_VARS)
LOCAL_FORCE_STATIC_EXECUTABLE := true

# override strip command to strip all symbols from output library; no need to ship with those ...
# cmd-strip = $(TOOLCHAIN_PREFIX)strip $1

LOCAL_ARM_MODE  := arm
LOCAL_MODULE    := slua

LOCAL_PATH      := $(NDK_PROJECT_PATH)
LUA_PATH        := $(LOCAL_PATH)/luajit-2.0.4/src
JSON_PATH       := $(LOCAL_PATH)/lua-cjson-2.1.0
LPEG_PATH       := $(LOCAL_PATH)/lpeg
SQLITE_PATH     := $(LOCAL_PATH)/sqlite-amalgamation-3081101
LSQLITE_PATH    := $(LOCAL_PATH)/lsqlite3-master
SOCKET_PATH     := $(LOCAL_PATH)/luasocket-2.0.2/src

$(warning "LUA_PATH" $(LUA_PATH))
LOCAL_CPP_FEATURES := rtti exceptions
#LOCAL_CFLAGS   += -D _ANDROID -fvisibility=hidden
LOCAL_CFLAGS   += -D _ANDROID -D ANDROID
#LOCAL_CFLAGS   += -D _ANDROID -D __OpenBSD__
#LOCAL_CFLAGS   += -fexceptions
#LOCAL_CFLAGS   += -frtti
LOCAL_CPPFLAGS  += -03 -ffast-math

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LUA_PATH)
LOCAL_C_INCLUDES += $(LPEG_PATH)
LOCAL_C_INCLUDES += $(JSON_PATH)
LOCAL_C_INCLUDES += $(SQLITE_PATH)
LOCAL_C_INCLUDES += $(LSQLITE_PATH)
LOCAL_C_INCLUDES += $(SOCKET_PATH)

LOCAL_SRC_FILES := \
$(LOCAL_PATH)/slua.c \
$(LOCAL_PATH)/pb.c \
$(LOCAL_PATH)/bit.c \
$(JSON_PATH)/fpconv.c \
$(JSON_PATH)/strbuf.c \
$(JSON_PATH)/lua_cjson.c \
$(LPEG_PATH)/lpeg.c \
$(SQLITE_PATH)/sqlite3.c \
$(LSQLITE_PATH)/lsqlite3.c \
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
$(SOCKET_PATH)/usocket.c

LOCAL_SRC_FILES  += $(THIRD_SRCS)

LOCAL_SHARE_LIBARIES  := \
libcutils \
libdl

LOCAL_STATIC_LIBARIES := $(THIRD_MODULS)
LOCAL_LDLIBS          += -landroid -llog

LOCAL_WHOLE_STATIC_LIBRARIES += $(LOCAL_STATIC_LIBARIES)

#cmd-strip = $(ndk)/arm-linux-androideabi-4.8/prebuild/strip -s --strip-debug -x $1

include $(BUILD_SHARED_LIBRARY)