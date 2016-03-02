#ifndef XIAMI_DLL_EXPORT
#define XIAMI_DLL_EXPORT
#endif
#include "Agent.h"
#include "./AgentClient/receiver.h"
#include "./AgentClient/dispatcher.h"
#include "./AgentService/agentService.h"
#include "./AgentClient/process.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	static CWSAStartup wsa;
	return (TRUE);
}


#ifdef   __cplusplus
extern "C" {
#endif

	XIAMI_DLL_EXPORT bool  AsynSendPacket(unsigned int nIndex,const void* buf,size_t cnt)
	{
		return theAgentSvc->AsynSendPacket(nIndex,buf,cnt);
	}
	XIAMI_DLL_EXPORT bool  InitAgentClient(int nport)
	{
		HSOCK hsock(new CSock());
		if(!hsock) return false;

		if(!hsock->Create(SOCK_DGRAM)) return false;


		CReceiver    theReceiver;
		spDispatcher theDispatcher(new CDispatcher()); 
		theDispatcher->AddProto(PI_RegExt,spIProto(new CRegisterFileExtHandler));
		theDispatcher->AddProto(PI_UnRegExt,spIProto(new CUnRegisterFileExtHandler));
		theDispatcher->AddProto(PI_Run,spIProto(new CRegisterRunHandler));
		theDispatcher->AddProto(PI_UnRun,spIProto(new CRegisterUnRunHandler));
		theDispatcher->AddProto(PI_RegUrlProto,spIProto(new CRegisterUrlHandler));
		theDispatcher->AddProto(PI_UnRegUrlProto,spIProto(new CUnRegisterUrlHandler));

		sLog(_T("run...\n"));

		CSockAddr svcAddr(nport,"127.0.0.1");
		return theReceiver.run(svcAddr,hsock,theDispatcher);
	}

#ifdef   __cplusplus
}
#endif

