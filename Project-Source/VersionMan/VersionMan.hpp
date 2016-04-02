#ifndef _VERSIONMAN_H
#define _VERSIONMAN_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string>

typedef struct L_VERSION
{
	union 
	{
		struct  
		{
			int iVer0;
			int iVer1;
			int iVer2;
		};

		int aVerNum[3];
	};
}L_VERSION,*PVERSION;


inline L_VERSION MakeVersion(int def,int ver1,int ver2) {
	L_VERSION v;
	v.iVer0 = def;
	v.iVer1 = ver1;
	v.iVer2 = ver2;
	return v;
}

inline int CompareVersion(L_VERSION& ver1,L_VERSION& ver2)
{
	if (ver1.iVer0 == ver2.iVer0 && ver1.iVer1 == ver2.iVer1 && ver1.iVer2 == ver2.iVer2)
		return 0;
	else if ((ver1.iVer0 < ver2.iVer0) || (ver1.iVer0 == ver2.iVer0 && ver1.iVer1 < ver2.iVer1) || (ver1.iVer0 == ver2.iVer0 && ver1.iVer1 == ver2.iVer1 && ver1.iVer2 < ver2.iVer2))
		return -1;
	else
		return 1;
}

inline L_VERSION VersionMinus(L_VERSION& ver1,L_VERSION& ver2)
{
	return MakeVersion(ver1.iVer0-ver2.iVer0,ver1.iVer1-ver2.iVer1,ver1.iVer2-ver2.iVer2);
}

class VersionMan
{
public:
	VersionMan() {}
	int Add(int a,int b);
	bool LoadVersions(const char* szFileName,std::string& err_msg);
};



#endif//_VERSIONMAN_H