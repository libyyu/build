#define LUA_LIB

#include "FLua.h"

AnyLog::ILog* g_theLog = NULL;
lua_State* g_luaState = NULL;

void FLua_CleanupLuaState()
{
	if (g_luaState != NULL)
	{
		g_luaState = NULL;
		log_info("Cleanup luaState.");
	}
}
void FLua_SetupLuaState(lua_State* l)
{
	g_luaState = l;
	log_info("SetupLuaState.");
}
void FLua_EstablishAnyLog(void* pfunc)
{
	if (g_theLog != NULL)
	{
		delete g_theLog;
	}
	g_theLog = new FLog();
	g_theLog->SetLogCall((AnyLog::PLogFunc)pfunc);
	g_theLog->Log("AnyLog Established!");
}
