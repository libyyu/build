#include "dispatcher.h"

CDispatcher::CDispatcher()
{

}

CDispatcher::~CDispatcher()
{

}

bool CDispatcher::AddProto(uint32 index,spIProto theProto)
{
	if(_map.end() != _map.find(index))
		return false;
	_map.insert(ProtoMapT::value_type(index,theProto));
	return true;
}
void CDispatcher::RemoveProto(uint32 index)
{
	_map.erase(index);
}
bool CDispatcher::Dispatch(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer)
{
	printf("buffer:%s\n",(char*)(*buffer)[buffer->rpos()]);
	assert(hsession);
	assert(buffer);
	assert(hdr.len);

	ProtoMapIterT it = _map.find(hdr.index);
	assert(_map.end() != it);
	if(_map.end() == it)
		return false;
	
	assert(it->second);
	if(!it->second) 
		return false;
	bool bRet = it->second->Handle(hsession,addr,hdr,buffer);
	assert(bRet);
	return bRet;
}