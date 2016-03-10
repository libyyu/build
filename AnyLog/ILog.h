/********************************************************************
FileName :  FType.h
Version  :  0.10
Date     :	2010-2-1 19:41:30
Author   :  Feng(libyyu@qq.com)
Comment  :

*********************************************************************/
#ifndef __ILOG_H__
#define __ILOG_H__

#include "FType.h"

enum LOG_TYPE
{
	LOG_ERROR = 0,
	LOG_ASSERT = 1,
	LOG_WARNING = 2,
	LOG_INFO = 3,
	LOG_EXCEPTION = 4,
};
typedef void (STD_CALL *PLogFunc) (int logType, const char* message);

namespace AnyLog
{
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

	typedef ILog *PLOG;
}

class FLog : public AnyLog::ILog
{
private:
	PLogFunc _log_message;
public:
	FLog() : _log_message(NULL) {}
	void SetLogCall(PLogFunc pFunc)
	{
		assert(pFunc && "AnyLog::PLogFunc must not be null.");
		_log_message = pFunc;
	}
	static AnyLog::PLOG CreateILog(void* pfunc)
	{
		AnyLog::PLOG pLog = new FLog();
		pLog->SetLogCall((PLogFunc)pfunc);
		pLog->Log("AnyLog Established!");
		return pLog;
	}
	static void DestroyILog(AnyLog::PLOG pLog)
	{
		if(NULL != pLog)
		{
			delete pLog;
			pLog = NULL;
		}
	}
protected:
	void LogImpl(LOG_TYPE logType, const char* message)
	{
		if (_log_message != NULL)
		{
			char buff[1025] = { 0 };
			snprintf(buff, 1024, "[FLua]%s", message);
			_log_message((int)logType, buff);
		}
	}
};


#endif//__ILOG_H__