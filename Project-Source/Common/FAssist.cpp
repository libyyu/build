#define _F_DLL_
#include "FAssist.h"
#if !defined(WIN32) && !defined(_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

static AnyLog::ILog* g_theLog = NULL;
static lua_State* g_luaState = NULL;

AnyLog::ILog* g_GetAnyLog()
{
	return g_theLog;
}
void g_SetAnyLog(AnyLog::ILog* pLog)
{
	FLog::DestroyILog(g_theLog);
	g_theLog = pLog;
}
lua_State* g_GetLuaState()
{
	return g_luaState;
}
void g_SetLuaState(lua_State* L)
{
	g_luaState = L;
}

#ifdef _WIN32
#elif _ANDROID
	static JNIEnv* g_JniEnv = NULL;
	JNIEnv* g_GetJniEnv()
	{
		return g_JniEnv;
	}

	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
	{
		LOGD("hello in c native code.\n");
		jint result = -1;

		if (vm->GetEnv((void**) &g_JniEnv, JNI_VERSION_1_4) != JNI_OK) {
			return -1;
		}
		assert(g_JniEnv != NULL);

		/* success -- return valid version number */
		result = JNI_VERSION_1_4;

		return result;
	}
#else
#endif


//频率较高的工具函数
void MBS2WCS(const char* str, wchar_t* &p_out,int* len)
{
#   if defined(_WIN32) || defined(WIN32)
		int size = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
#	else
		size_t size = mbstowcs(NULL, str, 0);
#	endif

	p_out = new wchar_t[size];
	memset(p_out,0,size);
#   if defined(_WIN32) || defined(WIN32)
		MultiByteToWideChar(CP_ACP, 0, str, strlen(str), p_out, size);
#	else
		mbstowcs(p_out, str, size);
#	endif
	
	*len = size;
}
void WCS2MBS(const wchar_t* str, char* &p_out,int* len)
{
#   if defined(_WIN32) || defined(WIN32)
		int size = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
#	else
		size_t size = wcstombs(NULL, str, 0);
#	endif

	p_out = new char[size];
	memset(p_out,0,size);
#   if defined(_WIN32) || defined(WIN32)
		WideCharToMultiByte(CP_ACP, 0, str, -1, p_out, size, NULL, NULL);	
#	else
		wcstombs(p_out, str, ( size + 1 )*4);
#	endif
	
	*len = size;
}


_FCFunBegin

	F_LIB_API void SafeReleaseIntPtr(char* ptr)
	{
		SAFE_DELETE(ptr);
	} 

_FCFunEnd