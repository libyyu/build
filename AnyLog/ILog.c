#include "ILog.h"

inline void LogImpl(PLogFunc _log_message,LOG_TYPE logType, const char* message)
{
	if (_log_message != NULL)
	{
		char buff[1025] = { 0 };
		snprintf(buff, 1024, "[FLua]%s", message);
		_log_message((int)logType, buff);
	}
}

inline void LogFormatInner(void* pLog, LOG_TYPE logType, const char* format, va_list va)
{
	assert(pLog && "pLog is null");
	if (format)
	{
		char buff[1025] = { 0 };
#ifdef _WIN32
		_vsnprintf(buff, 1024, format, va);
#else
		vsnprintf(buff, 1024, format, va);
#endif
		ILog* theLog = (ILog*)pLog;
		LogImpl(theLog->_log_message,logType, buff);
	}
}

static void Log(void* pLog, const char* format, ...)
{
	va_list va;
	va_start(va, format);
	LogFormatInner(pLog,LOG_INFO, format, va);
	va_end(va);
}

static void LogWarning(void* pLog, const char* format, ...)
{
	va_list va;
	va_start(va, format);
	LogFormatInner(pLog,LOG_WARNING, format, va);
	va_end(va);
}

static void LogError(void* pLog, const char* format, ...)
{
	va_list va;
	va_start(va, format);
	LogFormatInner(pLog,LOG_ERROR, format, va);
	va_end(va);
}

static void LogException(void* pLog, const char* format, ...)
{
	va_list va;
	va_start(va, format);
	LogFormatInner(pLog,LOG_EXCEPTION, format, va);
	va_end(va);
}

ILog* CreateILog(PLogFunc pFunc)
{
	ILog* pLog = new ILog();
	assert(pLog && "can not new ILog");
	pLog->_log_message = pFunc;
	pLog->Log = Log;
	pLog->LogWarning = LogWarning;
	pLog->LogError = LogError;
	pLog->LogException = LogException;

	return pLog;
}