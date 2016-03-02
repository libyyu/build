#include "process.h"
#include "../Utility/json/include/json.h"
#include <atlapp.h>
#include <ShlObj.h>
#pragma comment(lib, "shell32.lib")
#define CRegKeyEx CRegKey

CString utf8_2_CString(const char* str)
{
	CString rs;
	if (str && ::strlen(str) > 0)
	{
		CA2W input(str, CP_UTF8);
		rs = input;
	}
	return rs;
}

template<typename T = Json::Value>
class CDataParser
{
public:
	typedef Json::Value          value_type;

	template<typename O>
	static void toValue(value_type& v,O& o);

	template<typename O>
	static void  getValue(value_type& v,const char* key,O& o)
	{
		assert(key);
		if(!v.isNull())
		{
			value_type theValue = v[key];
			if(!theValue.isNull())
				return toValue(theValue,o);
		}

		o = O();
	}

	template<typename O>
	static void getValue(value_type& v,const char* key,O& o,O def_o)
	{
		assert(key);
		if(!v.isNull())
		{
			value_type theValue = v[key];
			if(!theValue.isNull())
				return toValue(theValue,o);
		}

		o = def_o;
	}

	template<typename O>
	static O getValue(value_type& v,const char* key)
	{
		assert(key);
		O o = O();
		if(!v.isNull())
		{
			value_type theValue = v[key];
			if(!theValue.isNull())
				toValue(theValue,o);
		}
		return o;
	}

	// 		template<typename O>
	// 		static O getValue(value_type& v,const char* key,O def_o)
	// 		{
	// 			assert(key);
	// 			O o = def_o;
	// 			if(!v.isNull())
	// 			{
	// 				value_type theValue = v[key];
	// 				if(!theValue.isNull())
	// 					toValue(theValue,o);
	// 			}
	// 			return o;
	// 		}

	template<>
	static void toValue(value_type& v,CString& o)
	{
		o = utf8_2_CString(v.asCString());
	}

	template<>
	static void toValue(value_type& v,std::string& o)
	{
		o = v.asCString();
	}

	template<>
	static void toValue(value_type& v,int& o)
	{
		o = v.isInt() ? v.asInt() : atoi(v.asCString());
	}

	template<>
	static void toValue(value_type& v,unsigned& o)
	{
		o = v.isIntegral() ?  v.asUInt() : atoi(v.asCString());
	}

	template<>
	static void toValue(value_type& v,long& o)
	{
		o = v.isIntegral() ?  v.asInt(): atol(v.asCString());
	}

	template<>
	static void toValue(value_type& v,bool& o)
	{
		o = v.isIntegral() ? v.asBool() :  ("true" == v.asString());
	}
};

typedef CDataParser<Json::Value>   JsonParser;
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CRegisterFileExtHandler::Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer)
{
	std::string buf;
	(*buffer)>>buf;

	Json::Reader reader;
	Json::Value value;

	if (!reader.parse(buf, value) || value.isNull()) 
	{
		assert(false);
		return false;
	}

	CString sAppName;
	CString sExtension;
	CString sRunKey;
	CString sDefaultIcon;

	JsonParser::getValue(value,"appname",sAppName);
	JsonParser::getValue(value,"extension",sExtension);
	JsonParser::getValue(value,"runkey",sRunKey);
	JsonParser::getValue(value,"defaultIcon",sDefaultIcon);

	this->RegisterFileRelation(sAppName,sExtension,sRunKey,sDefaultIcon);

	Json::Value rightMenuValue;
	rightMenuValue = value["rightmenu"];
	for (Json::ValueIterator it = rightMenuValue.begin();it != rightMenuValue.end();++it)
	{
		CString sKey;
		CString sName;
		CString sCommand;
		JsonParser::getValue(*it,"key",sKey);
		JsonParser::getValue(*it,"value",sName);
		JsonParser::getValue(*it,"command",sCommand);

		this->AddRightMenu(sRunKey,sKey,sName,sCommand);
	}

	Header hd = {0};
	hd.len    = HeaderSize + sizeof(int);
	hd.index  = PI_RegExt;
	
	uint32 nResult = 1;

	CBuffer sendBuffer;
	sendBuffer.Write((uint8*)&hd,HeaderSize);
	sendBuffer<<nResult;

	hsession->SendTo((char*)sendBuffer[0],sendBuffer.cnt(),&addr);
	return true;
}

BOOL CRegisterFileExtHandler::AddRightMenu(
	CString& sRunKey, 
	CString& sKey,
	CString& sName,
	CString& sCommand )
{
	struct _rootKey_
	{
		HKEY    rootKey;
		LPCTSTR subkey;
	};


	_rootKey_ hRootKeys[] =
	{
			{HKEY_CLASSES_ROOT,NULL},
		//	{HKEY_LOCAL_MACHINE,_T("Software\\Classes\\")},
		//{HKEY_CURRENT_USER,_T("Software\\Classes\\")}
	};

	for (int i=0;i<sizeof(hRootKeys)/sizeof(hRootKeys[0]);++i)
	{
		CRegKeyEx hRootKey;

		if(ERROR_SUCCESS != hRootKey.Open(hRootKeys[i].rootKey,hRootKeys[i].subkey))
		{
			assert(FALSE);
			continue;
		}

		CString subKey;
		subKey.Format(_T("%s\\shell\\%s\\"),sRunKey,sKey);
		CRegKeyEx reg;
		if(ERROR_SUCCESS != reg.Open(hRootKey,subKey))
		{
			if(ERROR_SUCCESS != reg.Create(hRootKey,subKey))
			{
				assert(FALSE);
				continue;
			}
		}

		reg.SetStringValue(NULL,sName);
		
		CRegKeyEx commandReg;
		if(ERROR_SUCCESS != commandReg.Open(reg,_T("Command")))
		{
			if(ERROR_SUCCESS != commandReg.Create(reg,_T("Command")))
			{
				assert(FALSE);
				continue;
			}
		}

		commandReg.SetStringValue(NULL,sCommand);
	}

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL); 
	return TRUE;
}

BOOL CRegisterFileExtHandler::RegisterFileRelation(
	CString& sApplicationName,
	CString& sExtension, 
	CString& sRunKey,
	CString& sDefaultIcon)
{
	CString bakKeyName;
	bakKeyName.Format(_T("bak_%s"),sRunKey);

	struct _rootKey_
	{
		HKEY    rootKey;
		LPCTSTR subkey;
	};


	_rootKey_ hRootKeys[] =
	{
		{HKEY_CLASSES_ROOT,NULL},
	//	{HKEY_LOCAL_MACHINE,_T("Software\\Classes\\")},
	//	{HKEY_CURRENT_USER,_T("Software\\Classes\\")}
	};

	for (int i=0;i<sizeof(hRootKeys)/sizeof(hRootKeys[0]);++i)
	{
		CRegKeyEx hRootKey;

		if(ERROR_SUCCESS != hRootKey.Open(hRootKeys[i].rootKey,hRootKeys[i].subkey))
		{
			assert(FALSE);
			continue;
		}

		{
			CRegKeyEx reg;

			if(ERROR_SUCCESS != reg.Open(hRootKey,sExtension) )
			{
				if(ERROR_SUCCESS != reg.Create(hRootKey,sExtension))
				{
					assert(FALSE);
					continue;
				}
			}

			TCHAR lpa[_MAX_DIR] = {0};
			DWORD dwAidSize = MAX_PATH;
			if(ERROR_SUCCESS == reg.QueryStringValue(NULL,lpa,&dwAidSize))
			{
				if(0 != sRunKey.CompareNoCase(lpa))
				{
					reg.SetStringValue(bakKeyName,lpa);
				}
			}

			if(ERROR_SUCCESS != reg.SetStringValue(NULL,sRunKey))
			{
				assert(FALSE);
				continue;
			}
		}

		{
			CRegKeyEx reg;
			if(ERROR_SUCCESS != reg.Open(hRootKey,sRunKey))
			{
				if(ERROR_SUCCESS != reg.Create(hRootKey,sRunKey))
				{
					assert(FALSE);
					continue;
				}
			}

			CRegKeyEx defaultIconReg;
			if(ERROR_SUCCESS != defaultIconReg.Open(reg,_T("DefaultIcon")))
			{
				if(ERROR_SUCCESS != defaultIconReg.Create(reg,_T("DefaultIcon")))
				{
					assert(FALSE);
					continue;
				}
			}

			defaultIconReg.SetStringValue(NULL,sDefaultIcon);

			CRegKeyEx shellReg;
			if(ERROR_SUCCESS != shellReg.Open(reg,_T("Shell")))
			{
				if(ERROR_SUCCESS != shellReg.Create(reg,_T("Shell")))
				{
					assert(FALSE);
					continue;
				}
			}

			shellReg.SetStringValue(NULL,_T("Open"));

			CRegKeyEx commandReg;
			if(ERROR_SUCCESS != commandReg.Open(reg,_T("Shell\\open\\command")))
			{
				if(ERROR_SUCCESS != commandReg.Create(reg,_T("Shell\\open\\command")))
				{
					assert(FALSE);
					continue;
				}
			}
			if(ERROR_SUCCESS !=commandReg.SetStringValue(NULL,sApplicationName))
			{
				assert(FALSE);
				continue;
			}
		}
	}

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL); 

	CRegKeyEx hRootReg;
	if(ERROR_SUCCESS != hRootReg.Open(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\")))
	{
		assert(FALSE);
		return FALSE;
	}

	CRegKeyEx reg;
	if(ERROR_SUCCESS != reg.Open(hRootReg,sExtension))
	{
		assert(FALSE);
		return FALSE;
	}

	CRegKeyEx userChoiceReg;
	if(ERROR_SUCCESS != userChoiceReg.Open(reg,_T("UserChoice")))
	{
		if(ERROR_SUCCESS != userChoiceReg.Create(reg,_T("UserChoice")))
		{
			assert(FALSE);
			return FALSE;
		}
	}
	else
	{
		TCHAR lpa[_MAX_DIR] = {0};
		DWORD dwAidSize = MAX_PATH;
		if(ERROR_SUCCESS == userChoiceReg.QueryStringValue(_T("Progid"),lpa,&dwAidSize))
		{
			if(0 != sRunKey.CompareNoCase(lpa))
				userChoiceReg.SetStringValue(bakKeyName,lpa);
		}
	}

	userChoiceReg.SetStringValue(_T("Progid"),sRunKey);
	userChoiceReg.SetStringValue(_T("Application"),sRunKey);

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL); 
	return TRUE;
}

BOOL CRegisterFileExtHandler::UnRegisterFileRelation( CString& sExtension, CString& sRunKey )
{
	struct _rootKey_
	{
		HKEY    rootKey;
		LPCTSTR subkey;
	};

	_rootKey_ hRootKeys[] =
	{
		{HKEY_CLASSES_ROOT,NULL},
	//	{HKEY_LOCAL_MACHINE,_T("Software\\Classes\\")},
	//	{HKEY_CURRENT_USER,_T("Software\\Classes\\")}
	};

	for (int i=0;i<sizeof(hRootKeys)/sizeof(hRootKeys[0]);++i)
	{
		CRegKeyEx hRootKey;

		if(ERROR_SUCCESS != hRootKey.Open(hRootKeys[i].rootKey,hRootKeys[i].subkey))
		{
			sLog(_T("i:%d\n"),i);
			assert(FALSE);
			continue;
		}

		CRegKeyEx reg;
		if(ERROR_SUCCESS != reg.Open(hRootKey,sExtension))
		{
			assert(FALSE);
			continue;
		}

		CString bakKeyName;
		bakKeyName.Format(_T("bak_%s"),sRunKey);

		TCHAR lpa[_MAX_DIR];
		DWORD dwAidSize = MAX_PATH;
		if(ERROR_SUCCESS !=reg.QueryStringValue(bakKeyName,lpa,&dwAidSize))
		{
		//	assert(FALSE);
			continue;
		}

		if(ERROR_SUCCESS != reg.SetStringValue(NULL,lpa))
		{
			assert(FALSE);
			continue;
		}
	}

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL); 

	CRegKeyEx rootKey;
	if(ERROR_SUCCESS == rootKey.Open(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\")))
	{
		CRegKeyEx reg;
		if(ERROR_SUCCESS == reg.Open(rootKey,sExtension))
		{
			CRegKeyEx userChoiceReg;
			if(ERROR_SUCCESS == userChoiceReg.Open(reg,_T("UserChoice")))
			{
				CString bakKeyName;
				bakKeyName.Format(_T("bak_%s"),sRunKey);
				TCHAR lpa[_MAX_DIR] = {0};
				DWORD dwAidSize = MAX_PATH;

				if(ERROR_SUCCESS == userChoiceReg.QueryStringValue(bakKeyName,lpa,&dwAidSize))
				{
					userChoiceReg.SetStringValue(_T("Application"),lpa);
					userChoiceReg.SetStringValue(_T("Progid"),lpa);
					userChoiceReg.DeleteValue(bakKeyName);
					SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL); 
				}else
				{
					reg.DeleteSubKey(_T("UserChoice"));
				}
			}
		}
	}

	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CUnRegisterFileExtHandler::Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer)
{
	std::string buf;
	(*buffer)>>buf;

	Json::Reader reader;
	Json::Value value;

	if (!reader.parse(buf, value) || value.isNull()) 
	{
		assert(FALSE);
		return false;
	}

	CString sExtension;
	CString sRunKey;
	JsonParser::getValue(value,"extension",sExtension);
	JsonParser::getValue(value,"runkey",sRunKey);

	this->UnRegisterFileRelation(sExtension,sRunKey);

	Header hd = {0};
	hd.len    = HeaderSize + sizeof(int);
	hd.index  = PI_UnRegExt;

	uint32 nResult = 1;

	CBuffer sendBuffer;
	sendBuffer.Write((uint8*)&hd,HeaderSize);
	sendBuffer<<nResult;

	hsession->SendTo((char*)sendBuffer[0],sendBuffer.cnt(),&addr);
	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

bool CRegisterRunHandler::Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer)
{
	std::string buf;
	(*buffer)>>buf;

	Json::Reader reader;
	Json::Value value;

	if (!reader.parse(buf, value) || value.isNull()) 
	{
		assert(FALSE);
		return false;
	}

	CString keyName;
	CString strAppName;
	JsonParser::getValue(value,"keyname",keyName);
	JsonParser::getValue(value,"appname",strAppName);

	this->RegisterAutoRun(keyName,strAppName);

	Header hd = {0};
	hd.len    = HeaderSize + sizeof(int);
	hd.index  = PI_Run;

	uint32 nResult = 1;

	CBuffer sendBuffer;
	sendBuffer.Write((uint8*)&hd,HeaderSize);
	sendBuffer<<nResult;

	hsession->SendTo((char*)sendBuffer[0],sendBuffer.cnt(),&addr);
	return true;
}

BOOL CRegisterRunHandler::RegisterAutoRun(const CString& keyName,const CString& strAppName)
{
	HKEY hRootKey = HKEY_CLASSES_ROOT;
	CString strRunKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	CRegKeyEx reg;
	if(ERROR_SUCCESS != reg.Open(HKEY_LOCAL_MACHINE,strRunKey))
	{
		assert(FALSE);
		return FALSE;
	}
	if(ERROR_SUCCESS !=reg.SetStringValue(keyName,strAppName))
	{
		assert(FALSE);
		return FALSE;
	}
	return TRUE;
}

BOOL CRegisterRunHandler::RegisterUnAutoRun(const CString& keyName)
{
	HKEY hRootKey = HKEY_CLASSES_ROOT;
	CString strRunKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	CRegKeyEx reg;
	if(ERROR_SUCCESS != reg.Open(HKEY_LOCAL_MACHINE,strRunKey))
	{
		assert(FALSE);
		return FALSE;
	}

	reg.DeleteValue(keyName);
	
	return TRUE;
}

bool CRegisterUnRunHandler::Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer)
{
	std::string buf;
	(*buffer)>>buf;

	Json::Reader reader;
	Json::Value value;

	if (!reader.parse(buf, value) || value.isNull()) 
	{
		assert(FALSE);
		return false;
	}

	CString keyName;
	JsonParser::getValue(value,"keyname",keyName);

	this->RegisterUnAutoRun(keyName);

	Header hd = {0};
	hd.len    = HeaderSize + sizeof(int);
	hd.index  = PI_UnRun;

	uint32 nResult = 1;

	CBuffer sendBuffer;
	sendBuffer.Write((uint8*)&hd,HeaderSize);
	sendBuffer<<nResult;

	hsession->SendTo((char*)sendBuffer[0],sendBuffer.cnt(),&addr);
	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CRegisterUrlHandler::Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer)
{
	std::string buf;
	(*buffer)>>buf;

	Json::Reader reader;
	Json::Value value;

	if (!reader.parse(buf, value) || value.isNull()) 
	{
		assert(FALSE);
		return false;
	}

	CString keyName;
	CString strAppName;
	JsonParser::getValue(value,"keyname",keyName);
	JsonParser::getValue(value,"appname",strAppName);

	this->RegisterUrl(keyName,strAppName);

	Header hd = {0};
	hd.len    = HeaderSize + sizeof(int);
	hd.index  = PI_RegUrlProto;

	uint32 nResult = 1;

	CBuffer sendBuffer;
	sendBuffer.Write((uint8*)&hd,HeaderSize);
	sendBuffer<<nResult;

	hsession->SendTo((char*)sendBuffer[0],sendBuffer.cnt(),&addr);
	return true;
}

BOOL CRegisterUrlHandler::RegisterUrl(const CString& keyName,const CString& strAppName)
{
	HKEY hRootKey = HKEY_CLASSES_ROOT;
	CRegKeyEx reg;
	if(ERROR_SUCCESS != reg.Open(hRootKey,keyName) )
	{
		if(ERROR_SUCCESS != reg.Create(hRootKey,keyName))
		{
			assert(FALSE);
			return FALSE;
		}
	}
	reg.SetStringValue(_T("URL Protocol"),strAppName);
	CRegKeyEx commondReg;
	if(ERROR_SUCCESS != commondReg.Open(reg,_T("shell\\open\\command")))
	{
		if(ERROR_SUCCESS != commondReg.Create(reg,_T("shell\\open\\command")))
		{
			assert(FALSE);
			return FALSE;
		}
	}

	return ERROR_SUCCESS != commondReg.SetStringValue(NULL,strAppName);
}

BOOL CRegisterUrlHandler::UnRegisterUrl(const CString& keyName)
{
	HKEY hRootKey = HKEY_CLASSES_ROOT;
	CRegKeyEx reg(hRootKey);
	reg.DeleteValue(keyName);

	return TRUE;
}

bool CUnRegisterUrlHandler::Handle(HSOCK hsession,CSockAddr& addr,const Header& hdr,spBuffer buffer)
{
	std::string buf;
	(*buffer)>>buf;

	Json::Reader reader;
	Json::Value value;

	if (!reader.parse(buf, value) || value.isNull()) 
	{
		assert(FALSE);
		return false;
	}

	CString keyName;
	CString strAppName;
	JsonParser::getValue(value,"keyname",keyName);
	JsonParser::getValue(value,"appname",strAppName);

	this->UnRegisterUrl(keyName);

	Header hd = {0};
	hd.len    = HeaderSize + sizeof(int);
	hd.index  = PI_UnRegUrlProto;

	uint32 nResult = 1;

	CBuffer sendBuffer;
	sendBuffer.Write((uint8*)&hd,HeaderSize);
	sendBuffer<<nResult;

	hsession->SendTo((char*)sendBuffer[0],sendBuffer.cnt(),&addr);
	return true;
}