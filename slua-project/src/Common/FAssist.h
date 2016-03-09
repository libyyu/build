

#ifndef __FASSIST_H__
#define __FASSIST_H__

#include "FType.h"
#include "ILog.h"
#include "lua.h"
#ifdef   F_CPLUSPLUA
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
#else
#define  log_info(fmt,...)    \
if(g_theLog != NULL) \
{	\
	g_theLog->Log(g_theLog,fmt,##__VA_ARGS__);  \
}
#define  log_warning(fmt,...)  \
if(g_theLog != NULL) \
{	\
	g_theLog->LogWarning(g_theLog,fmt,##__VA_ARGS__);  \
}
#define  log_error(fmt,...)   \
if(g_theLog != NULL) \
{	\
	g_theLog->LogError(g_theLog,fmt,##__VA_ARGS__);  \
}
#define  log_exception(fmt,...)   \
if(g_theLog != NULL) \
{	\
	g_theLog->LogException(g_theLog,fmt,##__VA_ARGS__);  \
}
#endif
#ifdef   F_CPLUSPLUA
extern AnyLog::ILog* g_theLog;
#else
extern ILog* g_theLog;
#endif
extern lua_State* g_luaState;

_FCFunBegin

	F_DLL_API void L_CleanupLuaState();
	F_DLL_API void L_SetupLuaState(lua_State* l);
	F_DLL_API void L_EstablishAnyLog(void* pfunc);
	F_DLL_API void L_UnEstablishAnyLog();

_FCFunEnd


#endif//__FASSIST_H__

