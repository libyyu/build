#define _F_DLL_
#include "FAssist.h"
#if !defined(WIN32) && !defined(_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

static AnyLog::ILog* g_theLog = NULL;
static lua_State* g_luaState = NULL;

AnyLog::ILog* g_GetAnyLog()
{
	return g_theLog;
}
void g_SetAnyLog(AnyLog::ILog* pLog)
{
	FLog::DestroyILog(g_theLog);
	g_theLog = pLog;
}
lua_State* g_GetLuaState()
{
	return g_luaState;
}
void g_SetLuaState(lua_State* L)
{
	g_luaState = L;
}

#ifdef _WIN32
#elif _ANDROID
	static JNIEnv* g_JniEnv = NULL;
	JNIEnv* g_GetJniEnv()
	{
		return g_JniEnv;
	}

	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
	{
		LOGD("hello in c native code.\n");
		jint result = -1;

		if (vm->GetEnv((void**) &g_JniEnv, JNI_VERSION_1_4) != JNI_OK) {
			return -1;
		}
		assert(g_JniEnv != NULL);

		/* success -- return valid version number */
		result = JNI_VERSION_1_4;

		return result;
	}
#else
#endif


//频率较高的工具函数
void MBS2WCS(const char* str, wchar_t* &p_out,int* len)
{
#   if defined(_WIN32) || defined(WIN32)
		int size = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
#	else
		size_t size = mbstowcs(NULL, str, 0);
#	endif

	p_out = new wchar_t[size];
	memset(p_out,0,size);
#   if defined(_WIN32) || defined(WIN32)
		MultiByteToWideChar(CP_ACP, 0, str, strlen(str), p_out, size);
#	else
		mbstowcs(p_out, str, size);
#	endif
	
	*len = size;
}
void WCS2MBS(const wchar_t* str, char* &p_out,int* len)
{
#   if defined(_WIN32) || defined(WIN32)
		int size = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
#	else
		size_t size = wcstombs(NULL, str, 0);
#	endif

	p_out = new char[size];
	memset(p_out,0,size);
#   if defined(_WIN32) || defined(WIN32)
		WideCharToMultiByte(CP_ACP, 0, str, -1, p_out, size, NULL, NULL);	
#	else
		wcstombs(p_out, str, ( size + 1 )*4);
#	endif
	
	*len = size;
}

void formatString(Fstring& str, const Fchar* fmt, ...)
{
	Fchar buff[2049] = { 0 };
	va_list va;
	va_start(va, fmt);
	Fvsnprintf(buff, 2048, fmt, va);
	va_end(va);

	str = buff;
}

ErrRet DisplayError(const Fchar* errorTitle,
	const Fchar* errorText,
	const Fchar* errorDescription,
	const Fchar* fileName,
	int lineNumber)
{
#if defined(_WIN32) || defined(WIN32)
	const int MODULE_NAME_SIZE = 255;
	Fchar moduleName[MODULE_NAME_SIZE];
	// attempt to get the module name
	if (!GetModuleFileName(NULL, moduleName, MODULE_NAME_SIZE))
	{
		const Fchar *msg = _T("<unknown application>");
		//_tcscpy_s(moduleName, _tcsclen(msg), msg);
		Fstrcpy(moduleName, msg);
	}

	// build a collosal string containing the entire asster message
	const	int		MAX_BUFFER_SIZE = 1024;
	Fchar	buffer[MAX_BUFFER_SIZE];

	int Size = Fsnprintf(buffer,
		MAX_BUFFER_SIZE,
		_T("%s\n\nProgram : %s\nFile : %s\nLine : %d\nError: %s\nComment: %s\n \
		Abort to exit (or debug), Retry to continue,\n \
		Ignore to disregard all occurances of this error\n"),
		errorTitle,
		moduleName,
		fileName,
		lineNumber,
		errorText,
		errorDescription);

	// place a copy of the message into the clipboard
	if (OpenClipboard(NULL))
	{
		size_t bufferLength = Fstrlen(buffer);
		HGLOBAL hMem = GlobalAlloc(GHND | GMEM_DDESHARE, bufferLength + 1);

		if (hMem)
		{
			BYTE* pMem = (BYTE*)GlobalLock(hMem);
			memcpy(pMem, buffer, bufferLength);
			GlobalUnlock(hMem);
			EmptyClipboard();
			SetClipboardData(CF_TEXT, hMem);
		}

		CloseClipboard();
	}

	// find the top most window of the current application
	HWND hWndParent = GetActiveWindow();
	if (NULL != hWndParent)
	{
		hWndParent = GetLastActivePopup(hWndParent);
	}
	Fstring sbuf;
	sbuf += buffer;
	// put up a message box with the error
	int iRet = MessageBox(hWndParent,
		sbuf.c_str(),
		_T("ERROR NOTIFICATION..."),
		MB_TASKMODAL | MB_SETFOREGROUND | MB_ABORTRETRYIGNORE | MB_ICONERROR);

	// Figure out what to do on the return.
	if (iRet == IDRETRY)
	{
		// ignore this error and continue
		return (ERRRET_CONTINUE);
	}
	if (iRet == IDIGNORE)
	{
		// ignore this error and continue,
		// plus never stop on this error again (handled by the caller)
		return (ERRRET_IGNORE);
	}

	// The return has to be IDABORT, but does the user want to enter the debugger
	// or just exit the application?
	iRet = MessageBox(hWndParent,
		_T("Do you wish to debug the last error?"),
		_T("DEBUG OR EXIT?"),
		MB_TASKMODAL | MB_SETFOREGROUND | MB_YESNO | MB_ICONQUESTION);

	if (iRet == IDYES)
	{
		// inform the caller to break on the current line of execution
		return (ERRRET_BREAKPOINT);
	}

	// must be a full-on termination of the app
	ExitProcess((UINT)-1);
	return (ERRRET_ABORT);
#else
	return (ERRRET_BREAKPOINT);
#endif
}


ErrRet NotifyAssert(const Fchar* condition, const Fchar* fileName, int lineNumber, const Fchar* formats, ...)
{
	Fchar szBuffer[4096];

	va_list args;
	va_start(args, formats);
	Fvsnprintf(szBuffer,4096, formats, args);
	va_end(args);

	Fstring filenameStr = fileName;

	// pass the data on to the message box
	ErrRet result = DisplayError(_T("Assert Failed!"),
		condition,
		szBuffer,
		filenameStr.c_str(),
		lineNumber);
	return result;
}

_FCFunBegin

	F_LIB_API void SafeReleaseIntPtr(char* ptr)
	{
		SAFE_DELETE(ptr);
	} 

_FCFunEnd