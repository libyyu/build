#pragma once
#include <Windows.h>

#ifdef XIAMI_DLL_EXPORT
#define XIAMI_DLL_API __declspec(dllexport)
#else
#define XIAMI_DLL_API __declspec(dllimport)
#pragma comment(lib,"dsp.lib")
#endif

#ifdef __cplusplus
extern "C"{
#endif

XIAMI_DLL_API bool LoadDspPlugin(const wchar_t* wsPluginName,HWND hParentWnd);

XIAMI_DLL_API void UnLoadALLDspPlugin();

XIAMI_DLL_API void UnLoadSpecDspPlugin(const wchar_t* wsPluginName);

XIAMI_DLL_API bool IsCurrentDspOk();

XIAMI_DLL_API int ModifySamples(
	short int *pSamples, 
	int nNumsamples, 
	int nBitPerSample, 
	int nChannel, 
	int nSampleRate);


#ifdef __cplusplus
}
#endif
