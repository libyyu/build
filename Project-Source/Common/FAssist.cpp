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
lua_State* g_GetLuaState()
{
	return g_luaState;
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


_FCFunBegin

F_LIB_API void L_CleanupLuaState()
{
	if (g_luaState != NULL)
	{
		g_luaState = NULL;
		log_info("Cleanup luaState.");
	}
}
F_LIB_API void L_SetupLuaState(lua_State* l)
{
	g_luaState = l;
	log_info("SetupLuaState.");
}
F_LIB_API void L_EstablishAnyLog(void* pfunc)
{
	FLog::DestroyILog(g_theLog);
	g_theLog = FLog::CreateILog(pfunc);
}

F_LIB_API void L_UnEstablishAnyLog()
{
	log_info("UnEstablishAnyLog.");
	FLog::DestroyILog(g_theLog);
	g_theLog = NULL;
}

F_LIB_API void L_Exit()
{
	log_info("Exit.");
	L_CleanupLuaState();
	L_UnEstablishAnyLog();
}



_FCFunEnd
