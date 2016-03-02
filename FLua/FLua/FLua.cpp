//#define LUA_LIB

extern "C"
{
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
	// Windows 头文件: 
#include <windows.h>
	BOOL WINAPI DllMain(HANDLE hModule, DWORD  dwReason, LPVOID lpReserved)
	{
		return true;
	}
#endif // _WIN32
}
