#include "FAssist.h"

_FCFunBegin

#ifdef _WIN32
	BOOL WINAPI DllMain(HANDLE hModule, DWORD  dwReason, LPVOID lpReserved)
	{
		printf("slua.dll Attached\n");
		return true;
	}
#endif // _WIN32

_FCFunEnd