########################## 

LOCAL_PATH      := $(NDK_PROJECT_PATH)
#LOCAL_PATH := $(call my-dir)/../../../curl-7.48.0
PROJECT_PATH := $(NDK_PROJECT_PATH)/../../../curl-7.48.0

CFLAGS := -Wpointer-arith -Wwrite-strings -Wunused -Winline \
  -Wnested-externs -Wmissing-declarations -Wmissing-prototypes -Wno-long-long \
  -Wfloat-equal -Wno-multichar -Wsign-compare -Wno-format-nonliteral \
  -Wendif-labels -Wstrict-prototypes -Wdeclaration-after-statement \
  -Wno-system-headers -DHAVE_CONFIG_H -DCURL_STATICLIB -DANDROID -D_ANDROID

include $(CLEAR_VARS)
include $(PROJECT_PATH)/lib/Makefile.inc

LOCAL_C_INCLUDES := $(PROJECT_PATH)
LOCAL_C_INCLUDES += $(PROJECT_PATH)/include/curl 
LOCAL_C_INCLUDES += $(PROJECT_PATH)/lib

LOCAL_SRC_FILES := $(addprefix $(PROJECT_PATH)/lib/,$(CSOURCES))
LOCAL_CFLAGS += $(CFLAGS)

LOCAL_COPY_HEADERS_TO := $(PROJECT_PATH)lib/curl
LOCAL_COPY_HEADERS := $(addprefix $(PROJECT_PATH)/include/curl/,$(HHEADERS))

LOCAL_MODULE:= libcurl

LOCAL_SHARE_LIBARIES  := \
libcutils \
libdl

LOCAL_LDLIBS          += -landroid -llog -lc -lGLESv2

include $(BUILD_STATIC_LIBRARY)

#########################
# Build the curl binary

include $(CLEAR_VARS)
include $(PROJECT_PATH)/src/Makefile.inc
LOCAL_SRC_FILES := $(addprefix $(PROJECT_PATH)/src/,$(CURL_CFILES))

LOCAL_MODULE := curljni
LOCAL_MODULE_TAGS := optional
LOCAL_STATIC_LIBRARIES := libcurl
LOCAL_SYSTEM_SHARED_LIBRARIES := libc

LOCAL_C_INCLUDES += $(PROJECT_PATH)/include/curl $(PROJECT_PATH)/lib

# This may also need to include $(CURLX_CFILES) in order to correctly link
# if libcurl is changed to be built as a dynamic library
LOCAL_CFLAGS += $(CFLAGS)

LOCAL_SHARE_LIBARIES  := \
libcutils \
libdl

LOCAL_LDLIBS          += -landroid -llog -lc -lGLESv2

include $(BUILD_EXECUTABLE)