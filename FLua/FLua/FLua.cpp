#include "FLua.h"

extern "C"
{
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
	// Windows ͷ�ļ�: 
#include <windows.h>
	BOOL WINAPI DllMain(HANDLE hModule, DWORD  dwReason, LPVOID lpReserved)
	{
		printf("FLua.dll Attached\n");
		return true;
	}
#endif // _WIN32
}
