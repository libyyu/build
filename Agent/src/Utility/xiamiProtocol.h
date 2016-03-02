#pragma once
#include "XiamiRuntime.h"
enum ProtocolIndex{
	PI_Empty = 1,
	PI_GetOp,
	PI_RegExt,
	PI_UnRegExt,
	PI_Run,
	PI_UnRun,
	PI_RegUrlProto,
	PI_UnRegUrlProto,
	PI_END
};


typedef struct _tagheader_
{
	uint32       len;
	uint32       index;
	uint32       flag;
}Header; 

const size_t HeaderSize = sizeof(Header);