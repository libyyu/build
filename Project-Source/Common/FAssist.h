

#ifndef __FASSIST_H__
#define __FASSIST_H__

#include "FType.h"
#include "ILog.h"

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


struct lua_State;

AnyLog::ILog* g_GetAnyLog();
lua_State* g_GetLuaState();


#endif//__FASSIST_H__

