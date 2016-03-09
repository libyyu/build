

#ifndef __FASSIST_H__
#define __FASSIST_H__

#include "FType.h"
#include "ILog.h"
#include "lua.h"

#define  log_info(fmt,...)    \
if(g_theLog != NULL) \
{	\
	g_theLog->Log(fmt,##__VA_ARGS__);  \
}
#define  log_warning(fmt,...)  \
if(g_theLog != NULL) \
{	\
	g_theLog->LogWarning(fmt,##__VA_ARGS__);  \
}
#define  log_error(fmt,...)   \
if(g_theLog != NULL) \
{	\
	g_theLog->LogError(fmt,##__VA_ARGS__);  \
}
#define  log_exception(fmt,...)   \
if(g_theLog != NULL) \
{	\
	g_theLog->LogException(fmt,##__VA_ARGS__);  \
}

extern AnyLog::ILog* g_theLog;
extern lua_State* g_luaState;

_FCFunBegin

	F_DLL_API void L_CleanupLuaState();
	F_DLL_API void L_SetupLuaState(lua_State* l);
	F_DLL_API void L_EstablishAnyLog(void* pfunc);

_FCFunEnd


#endif//__FASSIST_H__

