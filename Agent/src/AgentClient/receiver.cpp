#include "receiver.h"
#include <tchar.h>
#include <time.h>

CReceiver::CReceiver(uint32 maxBufferSize /* = 4096 */):_maxBufferSize(maxBufferSize)
{

}

CReceiver::~CReceiver()
{

}

bool CReceiver::FixBuffer(spBuffer pbuffer)
{
	assert(pbuffer);
	if(!pbuffer)
		return false;

	if(pbuffer->empty())
		pbuffer->clear();

	const static size_t MinSize = 1024;

	int i = 3;
	while(pbuffer->nextwriteblocksize() < MinSize)
	{
		pbuffer->resize();
		--i;
		if(0 >= i)
			break;
	}

	return pbuffer->nextwriteblocksize() >= MinSize;
}
bool CReceiver::run(CSockAddr& svcAddr,HSOCK hsession,spDispatcher theDispatcher)
{
	assert(hsession);
	assert(theDispatcher);
	if(!hsession || !theDispatcher)
		return false;

	Header hdr = {0};
	hdr.index  = PI_GetOp;
	hdr.len    = HeaderSize;


	spBuffer theBuffer(new CBuffer());
	bool bRet = false;

#define MAXERRORCNT   (30)
	int nMaxError = MAXERRORCNT;
	while(nMaxError--)
	{
		if(!hsession->WaitWriteable())
			continue;
		if(!hsession->SendTo((char*)&hdr,HeaderSize,&svcAddr))
			continue;

		while(hsession->WaitReadable(1))
		{
			nMaxError = MAXERRORCNT;
			sLog(_T("Readable\n"));
			theBuffer->clear();
			char* buf  = (char*)((*theBuffer)[theBuffer->wpos()]);
			size_t len = theBuffer->nextwriteblocksize();

			CSockAddr addr;
			int n = hsession->RecvFrom(buf,len,&addr);
			if(SOCKET_ERROR == n)
			{
				sLog(_T("recvfrom error code:%d\n"),::WSAGetLastError());
				continue;;
			}
			sLog(_T("recv suss bytes:%d\n"),n);

			{
				Header hdr;
				hsession->SendTo((char*)&hdr,HeaderSize,&addr);
			}


			theBuffer->wpos(theBuffer->wpos() + n);
			while(HeaderSize <= theBuffer->cnt())
			{
				const size_t oldpos = theBuffer->rpos();
				Header hdr = {0};
				theBuffer->Read((uint8*)&hdr,HeaderSize);

				if(hdr.len > _maxBufferSize)
					break;

				if(theBuffer->cnt() < (hdr.len - HeaderSize))
					break;

				bRet &= theDispatcher->Dispatch(hsession,addr,hdr,theBuffer);
				theBuffer->rpos(theBuffer->rpos() + hdr.len);
			}
		}
	}

	return bRet;
}