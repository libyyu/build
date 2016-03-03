

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
	#include <float.h>
	#define isnan		_isnan
	#define snprintf	_snprintf
	#define STD_CALL	__stdcall
	#define CALLBACK    __stdcall
	#define WINAPI      __stdcall
#else
	#define STD_CALL
	#define CALLBACK
	#define WINAPI
#endif

#if defined(LUA_BUILD_AS_DLL)
	#if defined(LUA_CORE) || defined(LUA_LIB)
		#define DLL_API __declspec(dllexport)
	#else
		#define DLL_API __declspec(dllimport)
	#endif
#else
	#define DLL_API		extern
#endif

#define  log_info(fmt,...)    \
if(g_theLog != NULL) \
{	\
	g_theLog->Log(fmt,##__VA_ARGS__);  \
}
#define  log_error(fmt,...)   \
if(g_theLog != NULL) \
{	\
	g_theLog->LogError(fmt,##__VA_ARGS__);  \
}
#define  log_warning(fmt,...)  \
if(g_theLog != NULL) \
{	\
	g_theLog->LogWarning(fmt,##__VA_ARGS__);  \
}

