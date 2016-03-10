#define _F_DLL_

#include "FAssist.h"

AnyLog::ILog* g_theLog = NULL;
lua_State* g_luaState = NULL;

void L_CleanupLuaState()
{
	if (g_luaState != NULL)
	{
		g_luaState = NULL;
		log_info("Cleanup luaState.");
	}
}
void L_SetupLuaState(lua_State* l)
{
	g_luaState = l;
	log_info("SetupLuaState.");
}
void L_EstablishAnyLog(void* pfunc)
{
	g_theLog = FLog::CreateILog(pfunc);
}

void L_UnEstablishAnyLog()
{
	log_info("UnEstablishAnyLog.");
	FLog::DestroyILog(g_theLog);
}
