#define _F_DLL_

#include "FAssist.h"

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
