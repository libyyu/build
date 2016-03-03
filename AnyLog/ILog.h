#ifndef _ilog_h_
#define _ilog_h_

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>

#ifdef _WIN32 
	#define STD_LOG_CALL __stdcall
#else
	#define STD_LOG_CALL
#endif

namespace AnyLog
{
	enum LOG_TYPE
	{
		LOG_ERROR = 0,
		LOG_ASSERT = 1,
		LOG_WARNING = 2,
		LOG_INFO = 3,
		LOG_EXCEPTION = 4,
	};

	typedef void (STD_LOG_CALL *PLogFunc) (int logType,const char* message);

	class ILog
	{
	public:
		void Log(char* format,...);
		void LogWarning(char* format,...);
		void LogError(char* format,...);
		void LogException(char* format,...);
		void SetLogCall(PLogFunc pFunc) = 0;
	protected:
		virtual void LogImpl(LOG_TYPE logType,const char* message) = 0;
	private:
		void LogFormatInner(LOG_TYPE logType,const char* format, va_list va);
	};
}

#endif//_ilog_h_