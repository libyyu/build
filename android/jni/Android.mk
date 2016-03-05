LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libluajit
LOCAL_SRC_FILES := libluajit.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_MODULE := slua
LOCAL_C_INCLUDES := $(LOCAL_PATH)/luajit/src 
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../lpeg
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../pbc-master
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../lua-cjson-2.1.0
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../luasocket-2.0.2
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../sproto-master
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../sqlite-amalgamation-3081101
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../lsqlite3-master
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../

LOCAL_CPPFLAGS := -03 -ffast-math
LOCAL_SRC_FILES := ../../slua.c \
				   ../../pb.c \
				   ../../lpeg/lpeg.c \
				   ../../pbc-master/src/alloc.c \
				   ../../pbc-master/src/array.c \
				   ../../pbc-master/src/bootstrap.c \
				   ../../pbc-master/src/context.c \
				   ../../pbc-master/src/decode.c \
				   ../../pbc-master/src/map.c \
				   ../../pbc-master/src/pattern.c \
				   ../../pbc-master/src/proto.c \
				   ../../pbc-master/src/register.c \
				   ../../pbc-master/src/rmessage.c \
				   ../../pbc-master/src/stringpool.c \
				   ../../pbc-master/src/varint.c \
				   ../../pbc-master/src/wmessage.c \
				   ../../pbc-master/binding/lua/pbc-lua.c \
				   ../../lua-cjson-2.1.0/fpconv.c \
				   ../../lua-cjson-2.1.0/strbuf.c \
				   ../../lua-cjson-2.1.0/lua_cjson.c \
				   ../../luasocket-2.0.2/src/auxiliar.c \
				   ../../luasocket-2.0.2/src/buffer.c \
				   ../../luasocket-2.0.2/src/except.c \
				   ../../luasocket-2.0.2/src/inet.c \
				   ../../luasocket-2.0.2/src/io.c \
				   ../../luasocket-2.0.2/src/luasocket.c \
				   ../../luasocket-2.0.2/src/mime.c \
				   ../../luasocket-2.0.2/src/options.c \
				   ../../luasocket-2.0.2/src/select.c \
				   ../../luasocket-2.0.2/src/tcp.c \
				   ../../luasocket-2.0.2/src/timeout.c \
				   ../../luasocket-2.0.2/src/udp.c \
				   ../../luasocket-2.0.2/src/unix.c \
				   ../../luasocket-2.0.2/src/usocket.c \
				   ../../sproto-master/lsproto.c \
				   ../../sproto-master/sproto.c \
				   ../../sqlite-amalgamation-3081101/sqlite3.c \
				   ../../lsqlite3-master/lsqlite3.c 

LOCAL_WHOLE_STATIC_LIBRARIES += libluajit
include $(BUILD_SHARED_LIBRARY)
