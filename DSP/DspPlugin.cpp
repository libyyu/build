#include"DspPlugin.h"
#include "DSPManager.h"

static CDSPManager g_DspMgr;

#ifdef __cplusplus
extern "C"{
#endif

XIAMI_DLL_API bool LoadDspPlugin(const wchar_t* wsPluginName,HWND hParentWnd)
{
	return g_DspMgr.LoadDspPlugin(wsPluginName,hParentWnd);
}
XIAMI_DLL_API void UnLoadALLDspPlugin()
{
	g_DspMgr.UnLoadALLDspPlugin();
}
XIAMI_DLL_API void UnLoadSpecDspPlugin(const wchar_t* wsPluginName)
{
	g_DspMgr.UnLoadSpecDspPlugin(wsPluginName);
}
XIAMI_DLL_API bool IsCurrentDspOk()
{
	return g_DspMgr.IsCurrentDspOk();
}

XIAMI_DLL_API int ModifySamples(
	short int *pSamples, 
	int nNumsamples, 
	int nBitPerSample, 
	int nChannel, 
	int nSampleRate)
{
	return g_DspMgr.ModifySamples(pSamples,nNumsamples,nBitPerSample,nChannel,nSampleRate);
}


#ifdef   __cplusplus
}
#endif