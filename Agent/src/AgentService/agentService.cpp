#include "agentService.h"
#include <string>
#include <atlstr.h>
#include "../Utility/misc.h"
CAgentService::spAgentSvcT CAgentService::Instance()
{
	static spAgentSvcT theSvc(new CAgentService());
	return theSvc;
}

CAgentService::CAgentService(
	UINT nPort /* = 6789 */,
	UINT nPacketTimeOut /* = 10 */,
	UINT nMaxPacketCnt /* = 100 */, 
	UINT nMaxPacketSize /* = 1024*4 */)
	:_nTimeOut(nPacketTimeOut),
	_nMaxPacketCnt(std::max<UINT>(1,nMaxPacketCnt)),
	_nMaxPacketSize(std::max<UINT>(1024,nMaxPacketSize)),
	_isRunning(true),
	_s(new CSock)
{
	assert(_s);
	int n = 0;
	while(1)
	{	
		if(!_s->Create(SOCK_DGRAM))
			continue;
		CSockAddr addr(nPort + n++,NULL);
		if(_s->Bind(&addr))
			break;
	}

	boost::thread t(boost::bind(&CAgentService::OnThreadPro,this));
}

CAgentService::~CAgentService()
{
	if(_s)
	{
		_s->Close();
		_s = HSOCK();
	}

	while(_isRunning)
		boost::this_thread::yield();
}

bool CAgentService::AsynSendPacket(unsigned int nIndex,const void* buf,size_t cnt)
{
	assert(buf);
	assert(cnt);
	assert(cnt <= _nMaxPacketSize);
	assert(_isRunning);
	if(!buf || !cnt || cnt > _nMaxPacketSize) return false;
	if(!_isRunning) return false;

	if(!OpenAgentClinet())
		return false;

	spBufferT packet(new CBuffer(std::max<size_t>(cnt,4096)));
	assert(packet);
	if(!packet) return false;

	Header hdr = {0};
	hdr.index  = nIndex;
	hdr.len    = HeaderSize + cnt;

	packet->Write((const uint8*)&hdr,HeaderSize);
	packet->Write((const uint8*)buf,cnt);

	ScopedLockT lock(_mu);
	_packets.push(packet);
	while(_packets.size() > _nMaxPacketCnt)
		_packets.pop();
	return true;
}


bool CAgentService::OpenAgentClinet()
{
	{
 		boost::shared_ptr<void> theMutex(::CreateMutex(NULL, TRUE, _T("xiami_OrioleAgent")),::CloseHandle);
		if(ERROR_ALREADY_EXISTS == ::GetLastError())
		{
			return true;
		}
	}

	CSockAddr addr;
	int addrLen = sizeof(addr);
	::getsockname(*_s,(sockaddr	*)&addr,&addrLen);
	int nPort = ::ntohs(addr.sin_port);
	

	TCHAR s_exeFileName[MAX_PATH] = {0};
	DWORD n_result = ::GetModuleFileName(NULL, s_exeFileName, sizeof(TCHAR) * MAX_PATH);

	CString parame;
	parame.Format(_T("-agent:%d"),nPort);

	SYSTEM_INFO info;                                   //用SYSTEM_INFO结构判断64位AMD处理器 
	GetSystemInfo(&info);                               //调用GetSystemInfo函数填充结构 
	OSVERSIONINFOEX os; 
	os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);  /*在调用函数前必须用sizeof(OSVERSIONINFOEXA)填充dwOSVersionInfoSize结构成员*/ 
	bool bVasta = (GetVersionEx((OSVERSIONINFO *)&os) && os.dwMajorVersion>=6);  

	int nShow = SW_HIDE;
	
#ifdef _DEBUG
	nShow = SW_SHOWNORMAL;
#endif // _DEBUG
	
	SHELLEXECUTEINFO ShellInfo = {0};
	ShellInfo.cbSize = sizeof(ShellInfo);
	ShellInfo.hwnd   = NULL;
	ShellInfo.lpVerb = (bVasta ? _T("runas") : _T("open"));
	ShellInfo.lpFile = s_exeFileName;
	ShellInfo.nShow  = nShow;
	ShellInfo.fMask  = SEE_MASK_NOCLOSEPROCESS;
	ShellInfo.lpParameters = parame.GetBuffer();

	BOOL bRet= ShellExecuteEx(&ShellInfo);
	assert(bRet);

	if(bRet)
	{
		Sleep(1000*1);
	}

	return !!bRet;
}

void CAgentService::OnThreadPro()
{
	assert(_s);
	spBufferT recvBuffer(new CBuffer(_nMaxPacketSize));
	while(1)
	{
		HSOCK theSock(_s);
		if(!theSock)
			break;

		if(!theSock->WaitReadable())
			continue;

		recvBuffer->clear();
		char* buf  = (char*)((*recvBuffer)[recvBuffer->wpos()]);
		size_t len = recvBuffer->nextwriteblocksize();

		CSockAddr addr;
		int n = theSock->RecvFrom(buf,len,&addr);
		if(SOCKET_ERROR == n)
		{
			sLog(_T("recvfrom error code:%d\n"),::WSAGetLastError());
			continue;;
		}

		sLog(_T("recv suss bytes:%d\n"),n);
		recvBuffer->wpos(recvBuffer->wpos() + n);


		while(HeaderSize <= recvBuffer->cnt())
		{
			const size_t oldpos = recvBuffer->rpos();
			Header hdr = {0};
			recvBuffer->Read((uint8*)&hdr,HeaderSize);

			if(hdr.len > _nMaxPacketSize)
				break;

			if(recvBuffer->cnt() < (hdr.len - HeaderSize))
				break;

			bool bRet = this->Handle(theSock,addr,hdr,recvBuffer);
			recvBuffer->rpos(recvBuffer->rpos() + hdr.len - HeaderSize);
		}
	}

	_isRunning = false;
}

bool CAgentService::DoOnceSend(HSOCK hsession,CSockAddr& addr)
{
	assert(hsession);
	if(!hsession) 
		return false;

	spBufferT thePacket;
	{
		ScopedLockT lock(_mu);
		if(!_packets.empty())
		{
			thePacket = _packets.front();
			_packets.pop();
		}
	}

	if(!thePacket)
		return false;

	spBufferT recvBuffer(new CBuffer);

	int nMaxError = 3;
	while(nMaxError--)
	{
		if(!hsession->WaitWriteable())
			continue;

		char* buf  = (char*)(*thePacket)[thePacket->rpos()];
		size_t cnt = thePacket->cnt();
		if(cnt != hsession->SendTo(buf,cnt,&addr))
			continue;
		 
		if(!hsession->WaitReadable())
			continue;
		recvBuffer->clear();
		CSockAddr recvAdd(addr);
		int n = hsession->RecvFrom((char*)(*recvBuffer)[0],recvBuffer->nextwriteblocksize(),&recvAdd);
		if(SOCKET_ERROR == n)
			continue;
		recvBuffer->wpos(recvBuffer->wpos()+n,true);
		if(recvBuffer->cnt() != HeaderSize)
			continue;
		return true;
	}
	return false;
}


bool CAgentService::Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBufferT buffer)
{
	if(PI_GetOp == hdr.index)
	{
		while(DoOnceSend(hsession,addr));
	}
	return true;
}