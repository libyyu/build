#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <queue>
#include "../AgentClient/sock.h"
#include "../Utility/Buffer.h"
#include "../Utility/xiamiProtocol.h"
#include <Windows.h>
#include <tchar.h>

class CAgentService
{
public:
	CAgentService(
		UINT nPort = 6789,
		UINT nPacketTimeOut = 10,
		UINT nMaxPacketCnt = 100,
		UINT nMaxPacketSize = 1024*4);
	~CAgentService();
public:
	typedef boost::shared_ptr<CBuffer> spBufferT;
	typedef boost::shared_ptr<CAgentService> spAgentSvcT;
	static spAgentSvcT Instance();
	bool AsynSendPacket(unsigned int nIndex,const void* buf,size_t cnt);
protected:
	bool OpenAgentClinet();
	void OnThreadPro();
	bool DoOnceSend(HSOCK hsession,CSockAddr& addr);
	bool Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBufferT buffer);
protected:
	typedef boost::mutex               LockT;
	typedef LockT::scoped_lock         ScopedLockT;

	UINT                               _nTimeOut;
	UINT                               _nMaxPacketCnt;
	UINT                               _nMaxPacketSize;

	HSOCK                              _s;
	bool                               _isRunning;
	LockT                              _mu;
	boost::condition_variable          _con;
	std::queue<spBufferT>              _packets;
};

#define theAgentSvc   (CAgentService::Instance())