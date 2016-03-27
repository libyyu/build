

#ifndef __FASSIST_H__
#define __FASSIST_H__

#include "FType.h"
#include "ILog.h"
#include "lua.hpp"

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


#define MODULE_NAME "slua"
#define MODULE_VERSION "1.0.1"

AnyLog::ILog* g_GetAnyLog();
lua_State* g_GetLuaState();


#endif//__FASSIST_H__

