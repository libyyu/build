#pragma once
#include <boost/shared_ptr.hpp>
#include <map>
#include "sock.h"
#include "../Utility/xiamiProtocol.h"
#include "../Utility/Buffer.h"

typedef boost::shared_ptr<CBuffer> spBuffer;

class IProto
{
public:
	IProto(){}
	virtual ~IProto(){}

	virtual bool Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer) = 0;
};
typedef boost::shared_ptr<IProto> spIProto;

class CDispatcher
{
public:
	CDispatcher();
	virtual ~CDispatcher();
public:
	bool AddProto(uint32 index,spIProto theProto);
	void RemoveProto(uint32 index); 
	virtual bool Dispatch(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer);
protected:
	typedef std::map<uint32,spIProto> ProtoMapT;
	typedef ProtoMapT::iterator       ProtoMapIterT;


	ProtoMapT   _map;
};

typedef boost::shared_ptr<CDispatcher> spDispatcher;

