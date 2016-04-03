########################## 

LOCAL_PATH      := $(NDK_PROJECT_PATH)
#LOCAL_PATH := $(call my-dir)/../../../curl-7.48.0
PROJECT_PATH := $(NDK_PROJECT_PATH)/../../../curl-7.48.0

common_CFLAGS := -Wpointer-arith -Wwrite-strings -Wunused -Winline -Wnested-externs -Wmissing-declarations -Wmissing-prototypes -Wno-long-long -Wfloat-equal -Wno-multichar -Wsign-compare -Wno-format-nonliteral -Wendif-labels -Wstrict-prototypes -Wdeclaration-after-statement -Wno-system-headers -DHAVE_CONFIG_H

#########################
# Build the libcurl library

include $(CLEAR_VARS)
include $(PROJECT_PATH)/lib/Makefile.inc
CURL_HEADERS := \
	curlbuild.h \
	curl.h \
	curlrules.h \
	curlver.h \
	easy.h \
	mprintf.h \
	multi.h \
	stdcheaders.h \
	typecheck-gcc.h

LOCAL_SRC_FILES := $(addprefix $(PROJECT_PATH)/lib/,$(CSOURCES))
LOCAL_C_INCLUDES += $(PROJECT_PATH)/include/
LOCAL_C_INCLUDES += $(PROJECT_PATH)/lib/
LOCAL_CFLAGS += $(common_CFLAGS)

#LOCAL_COPY_HEADERS_TO := libcurl/curl
#LOCAL_COPY_HEADERS := $(addprefix $(PROJECT_PATH)/include/curl/,$(CURL_HEADERS))

LOCAL_MODULE:= curl
LOCAL_MODULE_FILENAME := libpbc
LOCAL_MODULE_TAGS := optional

# Copy the licence to a place where Android will find it.
# Actually, this doesn't quite work because the build system searches
# for NOTICE files before it gets to this point, so it will only be seen
# on subsequent builds.
# ALL_PREBUILT += $(PROJECT_PATH)/NOTICE
# $(PROJECT_PATH)/NOTICE: $(PROJECT_PATH)/COPYING | $(ACP)
# 	$(copy-file-to-target)

LOCAL_SHARE_LIBARIES  := \
libcutils \
libdl

LOCAL_LDLIBS          += -landroid -llog -lc

include $(BUILD_STATIC_LIBRARY)


# #########################
# # Build the curl binary

# include $(CLEAR_VARS)
# include $(PROJECT_PATH)/src/Makefile.inc
# LOCAL_SRC_FILES := $(addprefix $(PROJECT_PATH)/src/,$(CURL_CFILES))

# LOCAL_MODULE := curl
# LOCAL_MODULE_TAGS := optional
# LOCAL_STATIC_LIBRARIES := libcurl
# LOCAL_SYSTEM_SHARED_LIBRARIES := libc

# LOCAL_C_INCLUDES += $(PROJECT_PATH)/include $(PROJECT_PATH)/lib

# # This may also need to include $(CURLX_CFILES) in order to correctly link
# # if libcurl is changed to be built as a dynamic library
# LOCAL_CFLAGS += $(common_CFLAGS)

# LOCAL_SHARE_LIBARIES  := \
# libcutils \
# libdl

# LOCAL_LDLIBS          += -landroid -llog -lc

# include $(BUILD_SHARED_LIBRARY)

