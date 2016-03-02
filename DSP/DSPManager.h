#pragma once
#include <atlstr.h>
#include <map>
#include "DSP.H"

class CDSPManager
{
public:
	CDSPManager(void);
	~CDSPManager(void);

public:
	bool LoadDspPlugin(const CString& sDspPluginName,HWND hParentWnd); 
	void UnLoadALLDspPlugin();
	void UnLoadSpecDspPlugin(const CString& sDspPluginName);
	bool IsCurrentDspOk();

	int ModifySamples(
	short int *pSamples, 
	int nNumsamples, 
	int nBitPerSample, 
	int nChannel, 
	int nSampleRate);

private:
	bool InitDspEnv();
	static LRESULT CALLBACK DSPWndProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);
	static DWORD WINAPI MainThreadProc(LPVOID lpParam);
	void SetDspState(bool bState);

private:
	typedef std::map<CString,winampDSPModule*> vDspModT;
	vDspModT          m_DspMolMap;
	winampDSPHeader   *m_pDSPHeader;

	HWND    m_hParentWindow;
	HMODULE m_hDspDllPlugin;
	bool    m_bDspIsOk;
	bool    m_bHaveInit;

};

