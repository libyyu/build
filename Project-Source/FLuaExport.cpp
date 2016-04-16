
#define _F_DLL_
#define LUA_LIB

#include "FAssist.h"

_FCFunBegin

F_LIB_API void L_CleanupLuaState()
{
	if (g_GetLuaState() != NULL)
	{
		g_SetLuaState( NULL );
		log_info("Cleanup luaState.");
	}
}
F_LIB_API void L_SetupLuaState(lua_State* l)
{
	g_SetLuaState(l);
	log_info("SetupLuaState.");
}
F_LIB_API void L_EstablishAnyLog(void* pfunc)
{
	g_SetAnyLog(FLog::CreateILog(pfunc));
}

F_LIB_API void L_UnEstablishAnyLog()
{
	log_info("UnEstablishAnyLog.");
	g_SetAnyLog(NULL);
}

F_LIB_API void L_Exit()
{
	log_info("Exit.");
	L_CleanupLuaState();
	L_UnEstablishAnyLog();
}

_FCFunEnd
