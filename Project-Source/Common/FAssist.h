

#ifndef __FASSIST_H__
#define __FASSIST_H__

#include "FType.h"
#include "ILog.h"
#include "lua.hpp"
#ifdef _WIN32
#elif _ANDROID
	#include <jni.h>
	#include <android/log.h>
#else
#endif

#define  log_info(fmt,...)    \
if(g_theLog != NULL) \
{	\
	g_theLog->Log(fmt,##__VA_ARGS__);  \
}
#define  log_warning(fmt,...)  \
if(g_theLog != NULL) \
{	\
	g_theLog->LogWarning(fmt,##__VA_ARGS__);  \
}
#define  log_error(fmt,...)   \
if(g_theLog != NULL) \
{	\
	g_theLog->LogError(fmt,##__VA_ARGS__);  \
}
#define  log_exception(fmt,...)   \
if(g_theLog != NULL) \
{	\
	g_theLog->LogException(fmt,##__VA_ARGS__);  \
}


#define MODULE_NAME "slua"
#define MODULE_VERSION "1.0.1"

AnyLog::ILog* g_GetAnyLog();
lua_State* g_GetLuaState();

#ifdef _WIN32
#elif _ANDROID
	JNIEnv* g_GetJniEnv();
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, MODULE_NAME, __VA_ARGS__) 
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , MODULE_NAME, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , MODULE_NAME, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  , MODULE_NAME, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , MODULE_NAME, __VA_ARGS__)
#else
#endif

#endif//__FASSIST_H__

