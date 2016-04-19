// dllmain : Defines the entry point for the DLL application.
#define _F_DLL_
#define LUA_LIB

#include "FAssist.h"

#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

_FCFunBegin
LUA_API void luaS_openextlibs(lua_State *L);

LUAOPEN_MODULE(FengEngine)
{
	int nTop = lua_gettop(L);
	luaS_openextlibs(L);
	lua_settop(L, nTop);

	printf("%s %s attach.\n", MODULE_NAME, MODULE_VERSION);

	lua_newtable(L);
	///* make version string available to scripts */
	lua_pushstring(L, "_VERSION");
	lua_pushstring(L, MODULE_VERSION);
	lua_settable(L, -3);

	return 1;
}

//avoid msvc not export luaL_openlibs
LUA_API void luaL_openlibs(lua_State *L);
LUA_API void luaS_openlibs(lua_State* L)
{
	luaL_openlibs(L);
	luaS_openextlibs(L);
}

_FCFunEnd