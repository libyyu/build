#pragma once
#include "dispatcher.h"
#include <string>
#include <atlstr.h>

typedef std::wstring stringT;

class CRegisterFileExtHandler :
	public IProto
{
public:
	CRegisterFileExtHandler(){}
	~CRegisterFileExtHandler(){}

	bool Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer);
public:
	BOOL RegisterFileRelation(
		CString& sApplicationName,
		CString& sExtension,
		CString& sRunKey,
		CString& sDefaultIcon);
	BOOL UnRegisterFileRelation(
		CString& sExtension,
		CString& sRunKey
		);

	BOOL AddRightMenu(
		CString& sRunKey,
		CString& sKey,
		CString& sName,
		CString& sCommand
		);
};

class CUnRegisterFileExtHandler : 
	public CRegisterFileExtHandler
{
public:
	CUnRegisterFileExtHandler(){}
	~CUnRegisterFileExtHandler(){}

	bool Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer);
};

class CRegisterRunHandler :
	public IProto
{
public:
	bool Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer);
protected:
	BOOL RegisterAutoRun(const CString& keyName,const CString& strAppName);
	BOOL RegisterUnAutoRun(const CString& keyName);
};

class CRegisterUnRunHandler :
	public CRegisterRunHandler
{
public:
	bool Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer);
	
};

class CRegisterUrlHandler : 
	public IProto
{
public:
	bool Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer);
protected:
	BOOL RegisterUrl(const CString& keyName,const CString& strAppName);
	BOOL UnRegisterUrl(const CString& keyName);
};

class CUnRegisterUrlHandler : 
	public CRegisterUrlHandler
{
public:
	bool Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer);
};