#define _F_DLL_

#include "FAssist.h"
#include "lua.h"

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

F_DLL_API void WINAPI L_CleanupLuaState()
{
	if (g_luaState != NULL)
	{
		g_luaState = NULL;
		log_info("Cleanup luaState.");
	}
}
F_DLL_API void WINAPI L_SetupLuaState(lua_State* l)
{
	g_luaState = l;
	log_info("SetupLuaState.");
}
F_DLL_API void WINAPI L_EstablishAnyLog(void* pfunc)
{
	g_theLog = FLog::CreateILog(pfunc);
}

F_DLL_API void WINAPI L_UnEstablishAnyLog()
{
	log_info("UnEstablishAnyLog.");
	FLog::DestroyILog(g_theLog);
}

_FCFunEnd
