#include "DSPManager.h"
#include <assert.h>


CDSPManager::CDSPManager(void)
{
	m_hParentWindow = NULL;
	m_hDspDllPlugin = NULL;

	m_pDSPHeader    = NULL;

	m_bDspIsOk      = false;
	m_bHaveInit     = false;
}


CDSPManager::~CDSPManager(void)
{
}

bool CDSPManager::InitDspEnv()
{
	if (!::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) MainThreadProc,this, 0, NULL)) 
	{
		m_bHaveInit = false;
		return false;
	}
	
	m_bHaveInit = true;
	return true;
}

int CDSPManager::ModifySamples(
	short int *pSamples, 
	int nNumsamples, 
	int nBitPerSample, 
	int nChannel, 
	int nSampleRate)
{
	if(m_DspMolMap.size() > 0)
	{
		for (auto iter = m_DspMolMap.begin(); iter != m_DspMolMap.end(); iter++)
		{
			winampDSPModule *pDspMod = iter->second;
			pDspMod->ModifySamples(pDspMod,pSamples,nNumsamples,nBitPerSample,nChannel,nSampleRate);
		}
	}
	return 0;
}

void CDSPManager::UnLoadSpecDspPlugin(const CString& sDspPluginName)
{
	vDspModT::iterator iter = m_DspMolMap.find(sDspPluginName);
	if (iter != m_DspMolMap.end())
	{
		winampDSPModule *pDspMod =  iter->second;
		pDspMod->Quit(pDspMod);
		m_DspMolMap.erase(iter);
	}

	m_DspMolMap.size() > 0 ? SetDspState(true) : SetDspState(false);

}

bool CDSPManager::LoadDspPlugin(const CString& sDspPluginName,HWND hParentWnd)
{
	if (sDspPluginName.IsEmpty() || !hParentWnd)
		return false;

	m_hDspDllPlugin = ::LoadLibrary(sDspPluginName);

	assert(m_hDspDllPlugin);
	if(!m_hDspDllPlugin)
		return false;

	typedef void * (*GET_DSP_PROC)();
	GET_DSP_PROC proc = (GET_DSP_PROC)::GetProcAddress(m_hDspDllPlugin, "winampDSPGetHeader2");
	m_pDSPHeader = (winampDSPHeader *)proc();

	assert(m_pDSPHeader);
	if(!m_pDSPHeader)
	{
		::FreeLibrary(m_hDspDllPlugin);
		return false;
	}

	winampDSPModule* pDspMod = m_pDSPHeader->getModule(0);
	assert(pDspMod);
	if (!pDspMod)
	{
		::FreeLibrary(m_hDspDllPlugin);
		return false;
	}

	pDspMod->hDllInstance = m_hDspDllPlugin;
	pDspMod->hwndParent = hParentWnd;

	if (pDspMod->Init(pDspMod) != 0 )
	{
		::FreeLibrary(m_hDspDllPlugin);
		return false;
	}
	pDspMod->Config(pDspMod);
	SetDspState(true);
	m_DspMolMap.insert(std::make_pair(sDspPluginName,pDspMod));

	return true;
}

void CDSPManager::UnLoadALLDspPlugin()
{

	if (m_hParentWindow) 
		::SendMessage(m_hParentWindow, WM_QUIT, 0, 0);
	SetDspState(false);

	if(m_DspMolMap.size() > 0)
	{
		for (auto iter = m_DspMolMap.begin(); iter != m_DspMolMap.end(); iter++)
		{
			winampDSPModule *pDspMod = iter->second;
			pDspMod->Quit(pDspMod);
			::FreeLibrary(iter->second->hDllInstance);
		}
	}

	m_DspMolMap.clear();
		
}

LRESULT CALLBACK  CDSPManager::DSPWndProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	switch (message)
	{
	case WM_QUIT:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hDlg, message, wParam, lParam);
	}
	return 0;
}

DWORD WINAPI CDSPManager::MainThreadProc(LPVOID lpParam)
{
	CDSPManager* pDspManager = static_cast<CDSPManager*>(lpParam);
	assert(pDspManager);
	if (!pDspManager)
		return -1;

	WNDCLASS   wc;
	MSG        msg;

	memset(&wc, 0, sizeof(wc));
	wc.style         = CS_DBLCLKS;
	wc.lpfnWndProc   = DSPWndProc;
	wc.hInstance     = GetModuleHandle(NULL);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = _T("DspClass");

	if (!RegisterClass(&wc))
	{
		assert(false);
		printf("Unable to register window class\n");
		return -1;
	}

	pDspManager->m_hParentWindow = ::CreateWindow(_T("DspClass"),_T("DSPWindow"), WS_DISABLED, 0, 0, 0, 0, HWND_DESKTOP, NULL, wc.hInstance, NULL);
	assert(pDspManager->m_hParentWindow);
	if (!pDspManager->m_hParentWindow)
	{
		printf("Unable to create window\n");
		::UnregisterClass(_T("DspClass"), 0);
		return -1;
	}

	::ShowWindow(pDspManager->m_hParentWindow, SW_HIDE);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	pDspManager->m_hParentWindow = NULL;
	UnregisterClass(_T("DspClass"),0);

	return 0;
}

bool CDSPManager::IsCurrentDspOk()
{
	return m_bDspIsOk;
}

void  CDSPManager::SetDspState(bool bState)
{
	m_bDspIsOk = bState;
}
