#include "FDef.h"
#include "ILog.h"
#include "lua.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
	
	extern AnyLog::ILog* g_theLog;
	extern lua_State* g_luaState;

	DLL_API void FLua_CleanupLuaState();
	DLL_API void FLua_SetupLuaState(lua_State* l);
	DLL_API void FLua_EstablishAnyLog(void* pfunc);

#ifdef __cplusplus
}
#endif // __cplusplus


