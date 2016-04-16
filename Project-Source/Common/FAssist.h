

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

#define MODULE_NAME "FengEngine"
#define MODULE_VERSION "1.0.1"

AnyLog::ILog* g_GetAnyLog();
void g_SetAnyLog(AnyLog::ILog*);
lua_State* g_GetLuaState();
void g_SetLuaState(lua_State*);

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



#define  log_info(fmt,...)    \
if(g_GetAnyLog() != NULL) \
{	\
	g_GetAnyLog()->Log(fmt,##__VA_ARGS__);  \
}
#define  log_warning(fmt,...)  \
if(g_GetAnyLog() != NULL) \
{	\
	g_GetAnyLog()->LogWarning(fmt,##__VA_ARGS__);  \
}
#define  log_error(fmt,...)   \
if(g_GetAnyLog() != NULL) \
{	\
	g_GetAnyLog()->LogError(fmt,##__VA_ARGS__);  \
}
#define  log_exception(fmt,...)   \
if(g_GetAnyLog() != NULL) \
{	\
	g_GetAnyLog()->LogException(fmt,##__VA_ARGS__);  \
}

#define LUAOPEN_MODULE(name)  LUALIB_API int luaopen_##name(lua_State* L)

//频率较高的工具函数
void MBS2WCS(const char* str, wchar_t* &out,int* len);
void WCS2MBS(const wchar_t* str, char* &out,int* len);


#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif
 
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p){delete[] (p);  (p)=NULL;} }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p){ (p)->Release();  (p)=NULL;} }
#endif








#endif//__FASSIST_H__

