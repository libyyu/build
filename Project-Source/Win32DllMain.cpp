// dllmain : Defines the entry point for the DLL application.
#define _F_DLL_
#define LUA_LIB

#include "FAssist.h"

#include <Windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
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
LUALIB_API void luaL_openlibs(lua_State *L);
LUA_API void luaS_openextlibs(lua_State *L);

LUAOPEN_MODULE(MODULE_NAME)
{
	luaL_openlibs(L);
	luaS_openextlibs(L);

	printf("FengEngine attach.\n");

	return 0;
}

_FCFunEnd