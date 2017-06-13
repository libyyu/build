

#ifndef __FASSIST_H__
#define __FASSIST_H__

#include <stdarg.h>
//#include <string.h>
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

void formatString(Fstring& str, const Fchar* fmt, ...);

typedef enum ErrRet
{
	ERRRET_IGNORE = 0,
	ERRRET_CONTINUE,
	ERRRET_BREAKPOINT,
	ERRRET_ABORT
}ErrRet;

ErrRet NotifyAssert(const Fchar* condition, const Fchar* fileName, int lineNumber, const Fchar* formats, ...);

#if defined( _DEBUG ) || defined( DEBUG )
#if defined( _WIN32 ) || defined( WIN32 )
#define f_assertx(x,fmt,...) { \
		static bool _ignoreAssert = false; \
		if(!_ignoreAssert && !(x)) \
		{ \
			ErrRet _err_result = NotifyAssert(#x, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
			if (_err_result == ERRRET_IGNORE) \
			{ \
				_ignoreAssert = true; \
			} \
			else if (_err_result == ERRRET_BREAKPOINT) \
			{ \
				__debugbreak(); \
			} \
		}}
#else
#define f_assertx(x, fmt, ...) { \
		if (!(x)) \
		{ \
			Fstring comment = _T(""); \
			formatString(comment, fmt, ##__VA_ARGS__); \
			if(!comment.empty()){ comment = Fstring("Assert comment:") + comment + Fstring("\n"); } \
			Ffprintf (stderr, _T("%s"), comment.c_str()); \
			assert(x); \
			exit(-1);\
		}}
	#endif//_WIN32
#else
	#define f_assertx(x, fmt, ...)
#endif//_DEBUG

#if defined(_DEBUG) || defined(DEBUG)
#	define g_assert(x)		f_assertx(x, _T(""))
#else
#	define g_assert(x)
#endif





#endif//__FASSIST_H__

