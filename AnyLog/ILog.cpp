#include "ILog.h"

namespace AnyLog
{
	void ILog:Log(char* format,...)
	{
		va_list va;
		va_start(va,format);
		LogFormatInner(LOG_INFO,format,va);
		va_end(va);
	}

	void ILog:LogWarning(char* format,...)
	{
		va_list va;
		va_start(va,format);
		LogFormatInner(LOG_WARNING,format,va);
		va_end(va);
	}

	void ILog:LogError(char* format,...)
	{
		va_list va;
		va_start(va,format);
		LogFormatInner(LOG_ERROR,format,va);
		va_end(va);
	}

	void ILog:LogException(char* format,...)
	{
		va_list va;
		va_start(va,format);
		LogFormatInner(LOG_EXCEPTION,format,va);
		va_end(va);
	}

	void ILog:LogFormatInner(LOG_TYPE logType,const char* format, va_list va)
	{
		if(format)
		{
			char buff[1025] = {0};
#ifdef _WIN32
			_vsnprintf(buff,1024,format,va);
#else
			vsnprintf(buff,1024,format,va);
#endif

			LogImpl(logType,buff);
		}
	}
}