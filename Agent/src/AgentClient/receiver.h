#pragma once
#include "dispatcher.h"


class CReceiver
{
public:
	CReceiver(uint32 maxBufferSize = 1024*100);
	virtual ~CReceiver();

	virtual bool run(CSockAddr& svcAddr,HSOCK hsession,spDispatcher theDispatcher);
protected:
	bool FixBuffer(spBuffer pbuffer);
protected:
	uint32      _maxBufferSize;
};
