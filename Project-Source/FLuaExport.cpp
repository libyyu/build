
#define _F_DLL_
#define LUA_LIB

#include "FAssist.h"

_FCFunBegin

F_LIB_API void L_CleanupLuaState()
{
	log_info("Cleanup luaState.");
	g_SetLuaState( NULL );
}
F_LIB_API void L_SetupLuaState(lua_State* l)
{
	g_SetLuaState(l);
	log_info("Setup luaState.");
}
F_LIB_API void L_EstablishAnyLog(void* pfunc)
{
	g_SetAnyLog(FLog::CreateILog(pfunc));
	log_info("AnyLog Established!");
}

F_LIB_API void L_UnEstablishAnyLog()
{
	log_info("AnyLog UnEstablish!");
	g_SetAnyLog(NULL);
}


_FCFunEnd
