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
		void Log(const char* format,...);
		void LogWarning(const char* format,...);
		void LogError(const char* format,...);
		void LogException(const char* format,...);
		virtual void SetLogCall(PLogFunc pFunc) = 0;
	protected:
		virtual void LogImpl(LOG_TYPE logType,const char* message) = 0;
	private:
		void LogFormatInner(LOG_TYPE logType,const char* format, va_list va);
	};
}

#include <assert.h>
class FLog : public AnyLog::ILog
{
private:
	AnyLog::PLogFunc _log_message;
public:
	FLog() : _log_message(NULL) {}
	void SetLogCall(AnyLog::PLogFunc pFunc)
	{
		assert(pFunc && "AnyLog::PLogFunc must not be null.");
		_log_message = pFunc;
	}
protected:
	void LogImpl(AnyLog::LOG_TYPE logType, const char* message)
	{
		if (_log_message != NULL)
		{
			char buff[1025] = { 0 };
			snprintf(buff, 1024, "[FLua]%s", message);
			_log_message((int)logType, buff);
		}
	}
};

#endif//_ilog_h_