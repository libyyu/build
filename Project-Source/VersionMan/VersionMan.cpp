#define _F_DLL_
#include "FAssist.h"

#include "VersionMan.hpp"


bool VersionMan::LoadVersions(const char* szFileName,std::string& err_msg)
{
	if(!szFileName || strlen(szFileName)==0)
	{
		err_msg = "path invalid";
		return false;
	}
	FILE* fp = fopen(szFileName,"r");
	if(!fp)
	{
		err_msg = "Failed to open version file";
		return false;
	}

	err_msg = "success";
	return true;
}

int VersionMan::Add(int a,int b)
{
	return a+b;
}

_FCFunBegin

	F_LIB_API bool LoadVersions(const char* szFileName,char** err_msg,int* len)
	{
		std::string msg;
		VersionMan v;
 		bool ret = v.LoadVersions(szFileName,msg);

 		char* p_msg = new char[msg.size()];
 		memset(p_msg,0,msg.size());
 		memcpy(p_msg,msg.c_str(),msg.size());
 		*err_msg = p_msg;
 		*len = msg.size();
 		return ret;
	}

	F_LIB_API void TestStringOut(const char* szInput,char** szOut,int* len)
	{
		int sz = strlen(szInput) + 1;
		char* p_msg = new char[sz];
		memset(p_msg,0,sz);
 		memcpy(p_msg,szInput,sz);
 		*szOut = p_msg;
 		*len = sz;
	}

_FCFunEnd



#ifdef TEST

//////////////////////////////////////////////////////////////////////
 int main()
 {
 	printf("%s\n", "Hello World");
 	VersionMan v;
 	std::string err_msg;
 	v.LoadVersions("version.xml",err_msg);
 	printf("err_msg=%s\n", err_msg.c_str());
 	
 	int a;
 	scanf("%d",&a);
 	return 0;
}

#endif