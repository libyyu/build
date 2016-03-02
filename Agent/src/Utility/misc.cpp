#define _USE_32BIT_TIME_T
#include "misc.h"
#include <cassert>
#include <sys/stat.h>
#include <stdio.h>
#include <windows.h>
#include <direct.h>
#include <time.h>
#include <atlbase.h>
#include <boost/algorithm/string/replace.hpp>
//#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/convenience.hpp>
#include <sstream>
#include <vector>

using namespace std;
#pragma comment ( lib, "Version.lib" )

namespace em_utility
{

namespace misc
{
	
CString GetSystemName()
{
	SYSTEM_INFO info;                                   //用SYSTEM_INFO结构判断64位AMD处理器 
    GetSystemInfo(&info);                               //调用GetSystemInfo函数填充结构 
    OSVERSIONINFOEX os; 
    os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);  /*在调用函数前必须用sizeof(OSVERSIONINFOEXA)填充dwOSVersionInfoSize结构成员*/ 
    if(GetVersionEx((OSVERSIONINFO *)&os))                  /*调用GetVersionEx函数OSVERSIONINFOEX结构必须将指针类型强制转换*/
    { 
        CString vname;

        //下面根据版本信息判断操作系统名称

        switch(os.dwMajorVersion){                        //判断主版本号 
            case 4: 
                switch(os.dwMinorVersion){                //判断次版本号 
                    case 0: 
                        if(os.dwPlatformId==VER_PLATFORM_WIN32_NT) 
                            vname=_T("Microsoft Windows NT 4.0");                //1996年7月发布 
                        else if(os.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS) 
                            vname=_T("Microsoft Windows 95"); 
                        break; 
                    case 10: 
                        vname=_T("Microsoft Windows 98"); 
                        break; 
                    case 90: 
                        vname=_T("Microsoft Windows Me"); 
                        break; 
                } 
                break; 
            case 5: 
                switch(os.dwMinorVersion){               //再比较dwMinorVersion的值 
                    case 0: 
                        vname=_T("Microsoft Windows 2000");                    //1999年12月发布 
                        break; 
                    case 1: 
                        vname=_T("Microsoft Windows XP");                    //2001年8月发布 
                        break; 
                    case 2: 
                        if(os.wProductType==VER_NT_WORKSTATION &&
                           info.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)

                            vname=_T("Microsoft Windows XP Professional x64 Edition"); 
                        else if(GetSystemMetrics(SM_SERVERR2)==0) 
                            vname=_T("Microsoft Windows Server 2003");        //2003年3月发布 
                        else if(GetSystemMetrics(SM_SERVERR2)!=0) 
                            vname=_T("Microsoft Windows Server 2003 R2"); 
                        break; 
                } 
                break; 
            case 6: 
                switch(os.dwMinorVersion){ 
                    case 0: 
                        if(os.wProductType==VER_NT_WORKSTATION)/*

                        VER_NT_WORKSTATION是桌面系统 */
                            vname=_T("Microsoft Windows Vista"); 
                        else 
                            vname=_T("Microsoft Windows Server 2008");          //服务器版本 
                        break; 
                    case 1: 
                        if(os.wProductType==VER_NT_WORKSTATION) 
                            vname=_T("Microsoft Windows 7"); 
                        else 
                            vname=_T("Microsoft Windows Server 2008 R2"); 
                        break; 
                } 
                break; 
            default: 
                vname=_T("未知操作系统"); 
        } 
        return vname; 
    } 
    else 
		return _T("");
}

CString GetSystemVersionMark()
{
	 OSVERSIONINFOEX os; 
    os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX); 
    if(GetVersionEx((OSVERSIONINFO *)&os)){ 
        CString vmark;

		printf("osMajorVer:%d,dwMinorVer:%d\n",os.dwMajorVersion,os.dwMinorVersion);
        switch(os.dwMajorVersion){                //先判断操作系统版本 
            case 5: 
                switch(os.dwMinorVersion){ 
                    case 0:                  //Windows 2000 
                        if(os.wSuiteMask==VER_SUITE_ENTERPRISE) 
                            vmark=_T("Advanced Server"); 
                        break; 
                    case 1:                  //Windows XP 
                        if(os.wSuiteMask==VER_SUITE_EMBEDDEDNT) 
                            vmark=_T("Embedded"); 
                        else if(os.wSuiteMask==VER_SUITE_PERSONAL) 
                            vmark=_T("Home Edition"); 
                        else 
                            vmark=_T("Professional"); 
                        break; 
                    case 2: 
                        if(GetSystemMetrics(SM_SERVERR2)==0 &&
                        os.wSuiteMask==VER_SUITE_BLADE)  //Windows Server 2003 
                            vmark=_T("Web Edition"); 
                        else if(GetSystemMetrics(SM_SERVERR2)==0 &&
                        os.wSuiteMask==VER_SUITE_COMPUTE_SERVER) 
                            vmark=_T("Compute Cluster Edition"); 
                        else if(GetSystemMetrics(SM_SERVERR2)==0 &&
                        os.wSuiteMask==VER_SUITE_STORAGE_SERVER) 
                            vmark=_T("Storage Server"); 
                        else if(GetSystemMetrics(SM_SERVERR2)==0 &&
                        os.wSuiteMask==VER_SUITE_DATACENTER) 
                            vmark=_T("Datacenter Edition"); 
                        else if(GetSystemMetrics(SM_SERVERR2)==0 &&
                        os.wSuiteMask==VER_SUITE_ENTERPRISE) 
                            vmark=_T("Enterprise Edition"); 
                        else if(GetSystemMetrics(SM_SERVERR2)!=0 &&
                           os.wSuiteMask==VER_SUITE_STORAGE_SERVER)  /*

                           Windows Server 2003 R2*/

                            vmark=_T("Storage Server"); 
                        break; 
                } 
                break; 
            case 6: 
                switch(os.dwMinorVersion){ 
                    case 0: 
                        if(os.wProductType!=VER_NT_WORKSTATION &&
                        os.wSuiteMask==VER_SUITE_DATACENTER)  /*

                        Windows Server 2008*/ 
                            vmark=_T("Datacenter Server"); 
                        else if(os.wProductType!=VER_NT_WORKSTATION &&
                        os.wSuiteMask==VER_SUITE_ENTERPRISE) 
                            vmark=_T("Enterprise"); 
                        else if(os.wProductType==VER_NT_WORKSTATION &&
                           os.wSuiteMask==VER_SUITE_PERSONAL)  //Windows Vista

                            vmark=_T("Home"); 
                        break;
                } 
                break; 
            default: 
                vmark=_T(""); 
        } 
        return vmark; 
    } 
    else 
        return _T("");
}


DWORD GetDllVersion(LPCTSTR lpszDllName)  
{  
    HINSTANCE hinstDll;  
    DWORD dwVersion = 0;  
  
    /* For security purposes, LoadLibrary should be provided with a fully-qualified  
       path to the DLL. The lpszDllName variable should be tested to ensure that it  
       is a fully qualified path before it is used. */  
    hinstDll = LoadLibrary(lpszDllName);  
      
    if(hinstDll)  
    {  
        DLLGETVERSIONPROC pDllGetVersion;  
        pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");  
  
        /* Because some DLLs might not implement this function, you must test for  
           it explicitly. Depending on the particular DLL, the lack of a DllGetVersion  
           function can be a useful indicator of the version. */  
  
        if(pDllGetVersion)  
        {  
            DLLVERSIONINFO2 dvi;  
            HRESULT hr;  
  
            ZeroMemory(&dvi, sizeof(dvi));  
            dvi.info1.cbSize = sizeof(dvi);  
  
            hr = (*pDllGetVersion)(&dvi.info1);  
  
            if(SUCCEEDED(hr))  
            {  
               dwVersion = MAKELONG(dvi.info1.dwMinorVersion,dvi.info1.dwMajorVersion);  
            }  
        }  
        FreeLibrary(hinstDll);  
    }  
    return dwVersion;  
}  


BOOL IsAdmin()
{
	BOOL bIsElevated = FALSE;
	HANDLE hToken = NULL;
	UINT16 uWinVer = LOWORD(GetVersion());
	uWinVer = MAKEWORD(HIBYTE(uWinVer),LOBYTE(uWinVer));

	if(uWinVer < 0x0600) 
		return FALSE;

	if(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken))
	{
		struct {
			DWORD TokenIsElevated;
		}te;
		DWORD dwReturnLength = 0;

		if (GetTokenInformation(hToken,/*TokenElevation*/(_TOKEN_INFORMATION_CLASS)20,&te,sizeof(te),&dwReturnLength)) {  
			if (dwReturnLength == sizeof(te))  
				bIsElevated = te.TokenIsElevated;  
		}  
		CloseHandle( hToken );
	}
	return bIsElevated;
}


CString Htmlspecialchars(LPCTSTR pBuf)
{
	CString sResult;

	LPCTSTR pHeader = pBuf;
	while(_T('\0') != *pHeader)
	{
		switch(*pHeader)
		{
		case _T('&'):
			sResult += _T("&amp;");
			break;
		case _T('"'):
			sResult += _T("\\&quot;");
			break;
		case _T('\''):
			sResult += _T("\\\'");
			break;
		case _T('<'):
			sResult += _T("&lt;");
			break;
		case _T('>'):
			sResult += _T("&gt;");
			break;
		default:
			sResult += *pHeader;
		}

		++pHeader;
	}

	return sResult;
}

BOOL IsValidEmailChar(TCHAR ch)
{
	//26个小写字母
	if( (ch>=97) && (ch<=122) ) 
		return TRUE;

	//26个大写字母
	if( (ch>=65) && (ch<=90) )
		return TRUE;

	//0~9
	if((ch>=48) && (ch<=57)) 
		return TRUE;

	//_-.@
	if( ch==95 || ch==45 || ch==46 || ch==64 ) 
		return TRUE;
	return FALSE;
}
BOOL IsValidEmail(CString strEmail)
{
	 //a@b.c
	if(strEmail.GetLength()<5)
		return FALSE;
	//1. 首字符必须用字母
	TCHAR ch = strEmail[0];
	if( ((ch>=97) && (ch<=122)) || ((ch>=65) && (ch<=90)) )
	{
		int atCount =0;
		int atPos = 0;
		int dotCount = 0;
		//0已经判断过了，从1开始
		for(int i=1;i<strEmail.GetLength();i++) 
		{
			ch = strEmail[i];
			if(IsValidEmailChar(ch))
			{
				//"@"
				if(ch==64)
				{
					atCount ++;
					atPos = i;
				}//@符号后的"."号
				else if( (atCount>0) && (ch==46) )
					dotCount ++;
			}else
				return FALSE;
		}
		//6. 结尾不得是字符“@”或者“.”
		if( ch == 46 ) return FALSE;
		//2. 必须包含一个并且只有一个符号“@”
		//3. @后必须包含至少一个至多三个符号“.”
		if( (atCount!=1) || (dotCount<1) || (dotCount>3) )
			return FALSE;
		//5. 不允许出现“@.”或者.@
		if( strEmail.Find(_T("@."))>0 || strEmail.Find(_T(".@"))>0 )
			return FALSE;
		return TRUE;
	}
	return FALSE;
}

CString GetModulePath()
{
	TCHAR s_buf[MAX_PATH];
	s_buf[0] = 0x0;
	DWORD n_result = ::GetModuleFileName(NULL, s_buf, sizeof(TCHAR) * MAX_PATH);
	TCHAR	s_drive[MAX_PATH];
	s_drive[0] = 0x0;
	TCHAR s_dir[MAX_PATH];
	s_dir[0] = 0x0;
	/*errno_t n_err_no =*/ 
	_tsplitpath(s_buf, s_drive, s_dir, NULL, NULL);
	//assert(n_err_no == 0);
	_tcscpy_s(s_buf, s_drive);
	_tcscat_s(s_buf, s_dir);
	return CString(s_buf);
}

void  GetModulePath(CString  &Path)
{

	TCHAR s_buf[MAX_PATH];
	s_buf[0] = 0x0;
	DWORD n_result = ::GetModuleFileName(NULL, s_buf, sizeof(TCHAR) * MAX_PATH);
	TCHAR	s_drive[MAX_PATH];
	s_drive[0] = 0x0;
	TCHAR s_dir[MAX_PATH];
	s_dir[0] = 0x0;
	/*errno_t n_err_no =*/ 
	_tsplitpath(s_buf, s_drive, s_dir, NULL, NULL);
	//assert(n_err_no == 0);
	_tcscpy_s(s_buf, s_drive);
	_tcscat_s(s_buf, s_dir);
	Path = s_buf;
}

std::string get_module_path()
{
	CString s_result = GetModulePath();
	ATL::CW2A s_temp(s_result);
	return std::string(s_temp);
}

int file_last_modified(const char* p_file_name)
{
	if (p_file_name == NULL || strlen(p_file_name) == 0)
		return 0;
	assert(sizeof(time_t) == sizeof(unsigned int));
	struct stat file_info;
	if (stat(p_file_name, &file_info) == 0)
	{
		return file_info.st_mtime;
	}
	else
		return 0;
}
int file_last_modified(const wchar_t * p_file_name)
{
	if (p_file_name == NULL || wcslen(p_file_name) == 0)
		return 0;
	//assert(sizeof(time_t) == sizeof(unsigned int));
	struct _stat32 file_info;
	if (_wstat32(p_file_name, &file_info) == 0)
	{
		return file_info.st_mtime;
	}
	else
		return 0;
}

bool file_exist(const char * p_file_name)
{
	if (p_file_name != NULL && strlen(p_file_name) > 0)
	{
		struct stat stat_info;
		return stat(p_file_name, &stat_info) == 0;
	}
	else
		return false;
}

bool file_exist(const wchar_t* p_file_name)
{
	if (p_file_name != NULL && wcslen(p_file_name) > 0)
	{
		struct _stat32 stat_info;
		return _wstat32(p_file_name, &stat_info) == 0;
	}
	else
		return false;
}

bool is_dir_exist(const char* p_dir_name)
{
	if (p_dir_name == NULL || strlen(p_dir_name) == 0)
		return false;
	bool b_result = false;
	WIN32_FIND_DATAA wfd;
	memset(&wfd, 0x0, sizeof(wfd));
	HANDLE hFind = FindFirstFileA(p_dir_name, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		b_result = true;
	}
	if (hFind != INVALID_HANDLE_VALUE)
		FindClose(hFind);
	return b_result;
}

bool is_dir_exist(const wchar_t* p_dir_name)
{
	bool b_result = false;
	/*
	if (p_dir_name != NULL && wcslen(p_dir_name) > 0)
	{
		namespace fs = boost::filesystem;
		//路径的可移植
		fs::wpath full_path(fs::initial_path<fs::wpath>());
		full_path = fs::system_complete(fs::wpath(p_dir_name, fs::native));
		b_result = fs::exists(full_path);
	}
	return b_result;
	*/
	WIN32_FIND_DATA wfd;
	memset(&wfd, 0x0, sizeof(wfd));
	HANDLE hFind = FindFirstFile(p_dir_name, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		b_result = true;   
	}
	if (hFind != INVALID_HANDLE_VALUE)
		FindClose(hFind);
	return b_result;
}

bool delete_file(const char* p_file_name)
{
	assert(p_file_name != NULL);
	return ::remove(p_file_name) == 0;
}
bool delete_file(const wchar_t* p_file_name)
{
	assert(p_file_name != NULL);
	return ::_wremove(p_file_name) == 0;
}

bool delete_file_undo(const wchar_t* p_file_name, bool b_file_only)
{
	bool b_result = false;
	int n_result = 0;
	TCHAR s_buf[MAX_PATH + 2];
	memset(s_buf, 0x0, sizeof(TCHAR) * (MAX_PATH + 2));
	if (p_file_name == NULL || _tcslen(p_file_name) > MAX_PATH)
		return false;
	_tcscpy_s(s_buf, MAX_PATH, p_file_name);
	s_buf[_tcslen(p_file_name)] = NULL;
	s_buf[_tcslen(p_file_name) + 1] = NULL;
	SHFILEOPSTRUCT FileOp = { 0 };
	//FileOp.hwnd = NULL;
	FileOp.wFunc = FO_DELETE;
	FileOp.pFrom = s_buf;
	//FileOp.pTo = _T("");
	FileOp.fFlags = FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION;
	if (b_file_only)
		FileOp.fFlags |= FOF_FILESONLY;
	//FileOp.hNameMappings = NULL;
	//FileOp.lpszProgressTitle = _T("");
	n_result = ::SHFileOperation(&FileOp);
	if (n_result != 0)
	{
		DWORD n_err = ::GetLastError();
		b_result = false;
	}
	b_result = n_result == 0;
	return b_result;
}

bool delete_dir(const wchar_t* p_dir_name)
{
	assert(p_dir_name != NULL);
	bool b_result = false;
	unsigned int n_len = _tcslen(p_dir_name);
	TCHAR* p_buf = new TCHAR[n_len + 2];
	memset(p_buf, 0x0, sizeof(TCHAR) * (n_len + 2));
	SHFILEOPSTRUCT file_op;
	ZeroMemory((void*)&file_op, sizeof(SHFILEOPSTRUCT));
	_tcscpy_s(p_buf, n_len + 1, p_dir_name);

	file_op.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	file_op.hNameMappings = NULL;
	file_op.hwnd = NULL;
	file_op.lpszProgressTitle = NULL;
	file_op.pFrom = p_buf;
	file_op.pTo = NULL;
	file_op.wFunc = FO_DELETE;
	b_result = SHFileOperation(&file_op) == 0;
	delete[] p_buf;
	return b_result; 
}

bool is_directory(const char* p_file_name)
{
	assert(p_file_name != NULL);
	struct stat stat_info;
	int n_result = stat(p_file_name, &stat_info);
	if (n_result == -1)
		return false;
	return (stat_info.st_mode & S_IFDIR) == S_IFDIR;
}
bool is_directory(const wchar_t* p_file_name)
{
	assert(p_file_name != NULL);
	struct _stat32 stat_info;
	int n_result = _wstat32(p_file_name, &stat_info);
	if (n_result == -1)
		return false;
	return (stat_info.st_mode & S_IFDIR) == S_IFDIR;
}

CString rip_file_name(const CString& s_path_file, bool b_suffix)
{
	CString s_file_name;
	int n_pos = s_path_file.ReverseFind(_T('\\'));
	if (n_pos > 0)
	{
		s_file_name = s_path_file.Right(s_path_file.GetLength() - n_pos - 1);
		if (!b_suffix)
		{
			n_pos = s_file_name.ReverseFind(_T('.'));
			if (n_pos > 0)
				s_file_name = s_file_name.Left(n_pos);
		}
	}
	return s_file_name;
}

CString rip_file_ext(const CString& s_path_file, bool b_lower)
{
	CString s_ext;
	if (!s_path_file.IsEmpty())
	{
		TCHAR s_buf[MAX_PATH];
		s_buf[0] = 0x0;
		_wsplitpath(s_path_file.GetString(), NULL, NULL, NULL, s_buf);
		if (_tcslen(s_buf) > 0)
		{
			s_ext = s_buf;
			if (b_lower)
				s_ext.MakeLower();
			else
				s_ext.MakeUpper();
		}
	}
	return s_ext;
}

std::vector<std::wstring> get_dir_stack(const std::wstring& s_file_name, bool b_reverse)
{
	std::vector<std::wstring> list;
	std::wstring s_item;
	wchar_t* w_buf = wcsdup(s_file_name.c_str());
	if (s_file_name.empty() || w_buf == NULL)
		return list;
	wchar_t* w_token = NULL;
	w_token = wcstok(w_buf, (L"\\"));
	while (w_token != NULL)
	{
		s_item = w_token;
		//boost::to_lower(s_item);
		if (!s_item.empty())
			list.push_back(s_item);
		w_token = wcstok(NULL, L"\\");
	}
	free(w_buf);
	if (!b_reverse)
		return list;
	std::vector<std::wstring> reverse_list;
	reverse_list.reserve(list.size());
	for (unsigned int i = 0; i < list.size(); ++i)
	{
		s_item = list[list.size() - i - 1];
		reverse_list.push_back(s_item);
	}
	return reverse_list;
}

bool is_file(const char* p_file_name)
{
	assert(p_file_name != NULL);
	struct stat stat_info;
	int n_result = stat(p_file_name, &stat_info);
	if (n_result == -1)
		return false;
	return !((stat_info.st_mode & S_IFDIR) == S_IFDIR);

}

bool move_file(const TCHAR* p_src_file, const TCHAR* p_dest_file)
{
	assert(p_src_file != NULL);
	assert(p_dest_file != NULL);
	return ::MoveFile(p_src_file, p_dest_file) == TRUE;
}

CString rip_path(const CString& s_file_name)
{
	CString s_path;
	int n_pos = s_file_name.ReverseFind('\\');
	if (n_pos >= 0)
		s_path = s_file_name.Left(n_pos);
	return s_path;
}

/*文件是否只读状态*/
bool is_read_only(const char* p_file_name)
{
	DWORD state =GetFileAttributesA(p_file_name);
	return (state &FILE_ATTRIBUTE_READONLY)?true:false;
}
bool is_read_only(const wchar_t* p_file_name)
{
	DWORD state =GetFileAttributesW(p_file_name);
	return (state &FILE_ATTRIBUTE_READONLY)?true:false;
}
/*去掉的文件的只读状态*/
bool remove_read_only(const char* p_file_name)
{
	DWORD Mask=(0xFFFFFFFF^FILE_ATTRIBUTE_READONLY);
	DWORD state =GetFileAttributesA(p_file_name);
	state =state&Mask;
	return SetFileAttributesA(p_file_name,state);	
}
bool remove_read_only(const wchar_t* p_file_name)
{
	DWORD Mask=(0xFFFFFFFF^FILE_ATTRIBUTE_READONLY);
	DWORD state =GetFileAttributesW(p_file_name);
	state =state&Mask;
	return SetFileAttributesW(p_file_name,state);	
}
bool copy_file(const TCHAR* p_source_file, const TCHAR* p_dest_file, bool b_force)
{
	return ::CopyFile(p_source_file, p_dest_file, b_force ? FALSE : TRUE) == TRUE;
}

bool create_file(const TCHAR* p_file_name, bool b_force, unsigned int n_len)
{
	bool b_result(false);
	DWORD n_value = 0;
	if (b_force)
		n_value = CREATE_ALWAYS;
	else
		n_value = CREATE_NEW;
	HANDLE h_file= ::CreateFile(p_file_name,GENERIC_WRITE, 	FILE_SHARE_READ, NULL, n_value, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h_file != INVALID_HANDLE_VALUE)
	{
		n_value = ::SetFilePointer(h_file, n_len, NULL, FILE_BEGIN);
		assert(n_value != INVALID_SET_FILE_POINTER);
		BOOL b_EOF_resutl = ::SetEndOfFile(h_file);
		//assert(b_EOF_resutl == TRUE);
		::CloseHandle(h_file);
		b_result = b_EOF_resutl == TRUE;
	}
	return b_result;
}

unsigned int get_file_size(const char* p_file_name)
{
	assert(p_file_name != NULL);
	struct stat stat_info;
	if (stat(p_file_name, &stat_info) == 0)
		return stat_info.st_size;
	return 0;	
}
unsigned int get_file_size(const wchar_t* p_file_name)
{
	assert(p_file_name != NULL);
	struct _stat32 stat_info;
	if (_wstat32(p_file_name, &stat_info) == 0)
		return stat_info.st_size;
	return 0;	
}

bool confirm_dir_2(const char* p_file_name, const char c_splitter)
{
	assert(p_file_name != NULL);
	std::string s_file = p_file_name;
	std::string::size_type n_pos = s_file.rfind(c_splitter);
	if (n_pos == std::string::npos)
		return false;
	std::string s_dir = s_file.substr(0, n_pos);
	return confirm_dir(s_dir.c_str(), c_splitter);
}

bool confirm_dir(const char* p_dir_name, const char c_splitter)
{
	assert(p_dir_name != NULL);
	std::string s_dir = p_dir_name;
	if (*s_dir.rbegin() == c_splitter)
		s_dir = s_dir.substr(0, s_dir.size() - 1);
	std::string::size_type n_pos = s_dir.find(c_splitter);
	if (n_pos == std::string::npos)
		return false;
	std::string s_confirm;
	while (true)
	{
		n_pos = s_dir.find(c_splitter, n_pos + 1);
		if (n_pos == std::string::npos)
		{
			s_confirm = s_dir;
			_mkdir(s_confirm.c_str());
			break;
		}
		s_confirm = s_dir.substr(0, n_pos);
		_mkdir(s_confirm.c_str());
		if (n_pos >= s_dir.size())
			break;
	}
	return is_dir_exist(p_dir_name);
}

bool confirm_dir_2(const wchar_t* p_file_name, const wchar_t c_splitter)
{
	assert(p_file_name != NULL);
	std::wstring s_file = p_file_name;
	std::wstring::size_type n_pos = s_file.rfind(c_splitter);
	if (n_pos == std::wstring::npos)
		return false;
	std::wstring s_dir = s_file.substr(0, n_pos);
	return confirm_dir(s_dir.c_str(), c_splitter);
}

bool confirm_dir(const wchar_t* p_dir_name, const wchar_t c_splitter)
{
	assert(p_dir_name != NULL);
	std::wstring s_dir = p_dir_name;
	if (*s_dir.rbegin() == c_splitter)
		s_dir = s_dir.substr(0, s_dir.size() - 1);
	std::wstring::size_type n_pos = s_dir.find(c_splitter);
	if (n_pos == std::wstring::npos)
		return false;
	std::wstring s_confirm;
	while (true)
	{
		n_pos = s_dir.find(c_splitter, n_pos + 1);
		if (n_pos == std::wstring::npos)
		{
			s_confirm = s_dir;
			_wmkdir(s_confirm.c_str());
			break;
		}
		s_confirm = s_dir.substr(0, n_pos);
		_wmkdir(s_confirm.c_str());
		if (n_pos >= s_dir.size())
			break;
	}
	return is_dir_exist(p_dir_name);
}

void format_time_old(int n_gmt_time, const char* p_format, char* p_output)
{
	p_output[0] = 0x0;
	assert(sizeof(time_t) == sizeof(int));
	struct tm* p_time = localtime((const time_t*)&n_gmt_time);
	if (p_time != NULL)
	{
		assert(p_format != NULL);
		assert(strlen(p_format) <= MAX_TIME_BUF / 2);
		char s_format[MAX_TIME_BUF];
		strftime(s_format, MAX_TIME_BUF - 1, p_format, p_time);
		strcpy_s(p_output, MAX_TIME_BUF - 1, s_format);
	}
	return;
}

void format_time_old(int n_gmt_time, const wchar_t* p_format, wchar_t* p_output)
{
	assert(sizeof(time_t) == sizeof(int));
	struct tm* p_time = localtime((const time_t*)&n_gmt_time);
	assert(p_format != NULL);
	assert(wcslen(p_format) <= MAX_TIME_BUF / 2);
	wchar_t s_format[MAX_TIME_BUF];
	wcsftime(s_format, MAX_TIME_BUF - 1, p_format, p_time);
	wcscpy_s(p_output, MAX_TIME_BUF - 1, s_format);
	return;
}

void format_time_new(int n_gmt_time, const char* p_format, char* p_output)
{
	assert(p_output != NULL);
	//static const unsigned short MAX_TIME_BUF = 128;
	assert(p_format != NULL);
	assert(strlen(p_format) <= MAX_TIME_BUF / 2);
	assert(sizeof(time_t) == sizeof(int));
	struct tm gmt_time;
	errno_t n_result = gmtime_s(&gmt_time, (time_t*)&n_gmt_time);
	assert(n_result == 0);
	char s_format[MAX_TIME_BUF];
	strftime(s_format, MAX_TIME_BUF, p_format, &gmt_time);
	strcpy_s(p_output, MAX_TIME_BUF, s_format);
	return;
}

void format_time(int n_gmt_time, const char* p_format, char* p_output)
{
	return format_time_old(n_gmt_time, p_format, p_output);
	/*
	assert(p_output != NULL);
	//static const unsigned short MAX_TIME_BUF = 128;
	assert(p_format != NULL);
	assert(strlen(p_format) <= MAX_TIME_BUF / 2);
	assert(sizeof(time_t) == sizeof(int));
	struct tm gmt_time;
	errno_t n_result = gmtime_s(&gmt_time, (time_t*)&n_gmt_time);
	assert(n_result == 0);
	char s_format[MAX_TIME_BUF];
	strftime(s_format, MAX_TIME_BUF, p_format, &gmt_time);
	strcpy_s(p_output, MAX_TIME_BUF, s_format);
	return;
	*/
}

void format_time(int n_gmt_time, const wchar_t* p_format, wchar_t* p_output)
{
	return format_time_old(n_gmt_time, p_format, p_output);
	/*
	assert(p_output != NULL);
	//static const unsigned short MAX_TIME_BUF = 128;
	assert(p_format != NULL);
	assert(wcslen(p_format) <= MAX_TIME_BUF / 2);
	assert(sizeof(time_t) == sizeof(int));
	struct tm gmt_time;
	errno_t n_result = gmtime_s(&gmt_time, (time_t*)&n_gmt_time);
	assert(n_result == 0);
	wchar_t s_format[MAX_TIME_BUF];
	wcsftime(s_format, MAX_TIME_BUF, p_format, &gmt_time);
	wcscpy_s(p_output, MAX_TIME_BUF, s_format);
	return;
	*/
}

void init_time_zone()
{
	assert(sizeof(time_t) == sizeof(int));
	int n_gmt_now = now_time();
	char s_buf[256];
	memset(s_buf, 0x0, 256);
	long n_timezone = 0;
	_get_timezone(&n_timezone);
	format_time_old(n_gmt_now, "%Y.%m.%d - %H:%M:%S", s_buf);
	n_timezone = 0;
	_get_timezone(&n_timezone);
	return;
}

void init_io()
{
	//bf_io_base::SetBigFileName("D:\\mp3.dat");
	//bf_io_base::SetBigFileSize(1024*1024*100);
	//hf_io_base::set_root("D:\\hi");
}

int now_time(int n_local_time)
{
	assert(sizeof(time_t) == sizeof(int));
	long n_timezone = 0;
	_get_timezone(&n_timezone);
	//int n_timezone = timezone;
	int n_local = 0;
	if (n_local_time == 0)
		n_local = local_time();
	else
		n_local = n_local_time;
	return n_local + n_timezone;
	/*
	assert(sizeof(time_t) == sizeof(int));
	return (int)time(NULL);
	*/
}

int local_time(int n_gmt_time)
{
	/*
	assert(sizeof(time_t) == sizeof(int));
	int n_timezone = timezone;
	if (n_gmt_time == 0)
		return (int)(now_time() + n_timezone);
	else
		return n_gmt_time + n_timezone;
		*/
	assert(sizeof(time_t) == sizeof(int));
	long n_timezone = 0;
	_get_timezone(&n_timezone);
	//int n_timezone = timezone;
	if (n_gmt_time == 0)
		return (int)time(NULL);
	else
		return n_gmt_time - n_timezone;
}

int time_diff(int n_from, int n_to)
{
	assert(sizeof(time_t) == sizeof(int));
	if (n_from == 0)
		return 0;
	assert(n_from > 0);
	if (n_to == 0)
		n_to = now_time();
	double n_diff = difftime(n_to, n_from);
	return static_cast<int>(n_diff);
}

unsigned int check_char(const unsigned char* p_input, unsigned int n_len, const unsigned char c_find)
{
	assert(p_input != NULL && n_len > 0);
	unsigned int i = 0;
	for (; i < n_len; ++i)
	{
		if (*(p_input +i) == c_find)
			break;
	}
	return i;
}
unsigned int check_char(const unsigned wchar_t* p_input, unsigned int n_len, const unsigned wchar_t c_find)
{
	assert(p_input != NULL && n_len > 0);
	unsigned int i = 0;
	for (; i < n_len; ++i)
	{
		if (*(p_input +i) == c_find)
			break;
	}
	return i;
}

__int64 get_disk_space(const TCHAR* p_disk_name)
{
	__int64 i64_free_bytes_2_caller = 0;
	__int64 i64_total_bytes = 0;
	__int64 i64_free_bytes = 0;
	if (_tcscmp(p_disk_name, _T("A:")) == 0 || _tcscmp(p_disk_name, _T("B:")) == 0)
		return 0;
    
	    UINT nError   = ::SetErrorMode(SEM_FAILCRITICALERRORS); //如果磁盘未就绪或者不可读 
		//msdn The system does not display the critical-error-handler message box
		BOOL b_result = ::GetDiskFreeSpaceEx(p_disk_name,  (PULARGE_INTEGER)&i64_free_bytes_2_caller,
			 (PULARGE_INTEGER)&i64_total_bytes,  (PULARGE_INTEGER)&i64_free_bytes);
		SetErrorMode(nError);
	return i64_free_bytes_2_caller;
}

CString get_max_space_disk()
{
	DWORD n_drivers = ::GetLogicalDrives();
	static const unsigned short MAX_DISK = 26;
	CString s_driver;
	__int64 i64_size = 0;
	for (unsigned short i = 0; i < MAX_DISK; ++i)
	{
		//判断当前位是否有驱动器   
		if ((n_drivers & (1 << i)) != 0)   
		{
			CString s_name = (TCHAR)('A' + i);
			s_name += _T(":");
			UINT n_type = ::GetDriveType(s_name);
			if (n_type == DRIVE_FIXED/* || n_type == DRIVE_REMOVABLE*/)
			{
				if (s_driver.IsEmpty())
				{
					s_driver = s_name;
					assert(i64_size == 0);
					i64_size = get_disk_space(s_name);
				}
				else
				{
					__int64 i64_disk_size = get_disk_space(s_name);
					if (i64_disk_size > i64_size)
					{
						s_driver = s_name;
						i64_size = i64_disk_size;
					}
				}
			}
		}
	}
	return s_driver;
}

CString get_have_dir_disk(const TCHAR* p_dir)
{
	DWORD n_drivers = ::GetLogicalDrives();
	static const unsigned short MAX_DISK = 26;
	CString s_driver;
	vector<CString>	sub_dir_list;
	CString s_dir = p_dir;
	s_dir.Replace('\/','\\');
	int index = 0;
	CString s_sub_dir;
	for(index = 0; index <= s_dir.GetLength(); index++)
	{
		if( index == s_dir.GetLength())
		{
			if(s_sub_dir.GetLength() > 0)
			{
				sub_dir_list.push_back(s_sub_dir);
				s_sub_dir=_T("");
			}
		}
		else if(s_dir[index] == '\\')
		{
			if(s_sub_dir.GetLength() > 0)
			{
				sub_dir_list.push_back(s_sub_dir);
				s_sub_dir=_T("");
			}
		}
		else
		{
			s_sub_dir.AppendChar(s_dir[index]);
		}
	}
	int n_max_count=0;
	int n_max_time=0;

	__int64 i64_size = 0;
	for (unsigned short i = 0; i < MAX_DISK; ++i)
	{
		//判断当前位是否有驱动器   
		if ((n_drivers & (1 << i)) != 0)   
		{
			CString s_name = (TCHAR)('A' + i);
			s_name += _T(":");
			UINT n_type = ::GetDriveType(s_name);
			if (n_type == DRIVE_FIXED/* || n_type == DRIVE_REMOVABLE*/)
			{
				vector<CString>	::iterator iter ;
				int n_count = 0;
				CString s_path = s_name;
				int n_time = 0;
				for(iter =sub_dir_list.begin();iter !=  sub_dir_list.end();iter++ )
				{
					s_path += _T("\\");
					s_path +=(*iter) ;

					if(file_exist(s_path))
					{
						n_count ++;
						n_time = file_last_modified(s_path);
					}
				}

				if(n_count > n_max_count)
				{
					n_max_count = n_count;
					s_driver = s_name;
					n_max_time = n_time;
				}
				else if(n_count == n_max_count && (n_max_time < n_time) )
				{
					n_max_count = n_count;
					s_driver = s_name;
					n_max_time = n_time;
				}
			}
		}
	}
	return s_driver;
}

std::string binary_2_hex(const unsigned char* p_data, unsigned int n_len)
{
	assert(p_data != NULL && n_len > 0);

	char* p_output = new char[n_len * 2 + 1];
	ZeroMemory(p_output, n_len * 2 + 1);
	char* p_pos = p_output;
	for (unsigned int i = 0; i < n_len; ++i)
	{
		sprintf_s(p_pos, 3, "%02x", p_data[i]);
		p_pos += 2;
	}
	std::string s_result = p_output;
	delete[] p_output;
	return s_result;
}

CString _binary_2_hex(const unsigned char* p_data, unsigned int n_len)
{
	assert(p_data != NULL && n_len > 0);
	TCHAR* p_output = new TCHAR[n_len * 2 + 1];
	ZeroMemory(p_output, sizeof(TCHAR) * (n_len * 2 + 1));
	TCHAR* p_pos = p_output;
	for (unsigned int i = 0; i < n_len; ++i)
	{
		_stprintf_s(p_pos, 3, _T("%02x"), p_data[i]);
		p_pos += 2;
	}
	CString s_result = p_output;
	delete[] p_output;
	return s_result;
}

CString utf8_2_CString(const char* p_input)
{
	CString s_result;
	if (p_input != NULL && strlen(p_input) > 0)
	{
		CA2W input(p_input, CP_UTF8);
		s_result = input;
	}
	return s_result;
}

std::string unicode_2_utf8(const wchar_t* p_input)
{
	std::string s_result;
	if (p_input != NULL && wcslen(p_input) > 0)
	{
		CW2A input(p_input, CP_UTF8);
		s_result = input;
	}
	return s_result;
}

std::string unicode_2_ansi(const wchar_t* p_input)
{
	std::string s_result;
	if (p_input != NULL && wcslen(p_input) > 0)
	{
		CW2A input(p_input);
		s_result = input;
	}
	return s_result;
}

std::string utf8_2_ansi(const char* p_input)
{
	std::string s_result;
	CString s_uni = utf8_2_CString(p_input);
	s_result = unicode_2_ansi(s_uni);
	return s_result;
}

std::string ansi_2_utf8(const char* p_input)
{
	std::string s_result;
	if (p_input != NULL && strlen(p_input) > 0)
	{
		CA2W input(p_input);
		s_result = unicode_2_utf8(input);
	}
	return s_result;
}

CString ansi_2_CString(const char* p_input)
{
	CString s_result;
	if (p_input != NULL && strlen(p_input) > 0)
	{
		CA2W input(p_input, CP_ACP);
		s_result = input;
	}
	return s_result;
}

void confirm_splash(CString& s_input, bool b_tail)
{
	int n_size = s_input.GetLength();
	if (n_size == 0)
	{
		s_input += _T('\\');
	}
	else
	{
		if (b_tail)		//确保尾部有
		{
			if (n_size > 0 && s_input[n_size - 1] != _T('\\') && s_input[n_size - 1] != _T('/'))
			{
				s_input += _T('\\');
			}
		}
		else			//确保头部有
		{
			if (n_size > 0 && s_input[0] != _T('\\') && s_input[0] != _T('/'))
			{
				s_input = _T('\\') + s_input;
			}
		}
	}
	return;
}



void confirm_n_splash(CString& s_input, bool b_tail)
{
	int n_size = s_input.GetLength();
	if (b_tail)				//确保尾部没有
	{
		if (n_size > 0 && (s_input[n_size - 1] == _T('\\') || s_input[n_size - 1] == _T('/')))
		{
			s_input = s_input.Left(n_size - 1);
		}
	}
	else						//确保头部没有
	{
		if (n_size > 0 && (s_input[0] == _T('\\') || s_input[0] == _T('/')))	
		{
			s_input = s_input.Right(n_size - 1);
		}
	}
	return;
}

CString link_dir_splash(const CString& s_base, const CString& s_sub)
{
	CString s_result = s_base;
	confirm_splash(s_result);
	CString s_link = s_sub;
	confirm_n_splash(s_link, false);
	s_result += s_link;
	return s_result;
}

CString fs_replace(const CString& s_input, const TCHAR c_new)
{
	const TCHAR REPLACE_CHARS[] = _T("\/\\\:\*\"\?\>\<\|");

	CString s_output = s_input;
	unsigned int n_count = sizeof(REPLACE_CHARS) / sizeof(REPLACE_CHARS[0]);
	for (unsigned int i = 0; i < n_count; ++i)
	{
		const TCHAR c_old = REPLACE_CHARS[i];
		s_output.Replace(c_old, c_new);
	}
	int n_len = s_output.GetLength();

	//对非可视ASCII字符的处理
	static const unsigned short MIN_ASCII_VALID = 30;
	for (int i = 0; i < n_len; ++i)
	{
		TCHAR s_value = s_output[i];
		unsigned short n_value = s_value;
		if (n_value < MIN_ASCII_VALID)
		{
			s_output.SetAt(i, c_new);
		}
	}

	if (n_len > 0)														//对点号的特殊处理
	{
		if (s_output[0] == _T('.'))									//起首点号
		{
			s_output.SetAt(0, c_new);
		}
		if (s_output[n_len - 1] == _T('.'))						//结尾点号
		{		
			s_output.SetAt(n_len - 1, c_new);
		}
	}
	return s_output;
}

void shift_html_item(std::wstring& s_data)
{
	static const std::wstring FIND_LIST[] = { (L"&amp;"), (L"&lt;"), (L"&gt;"), (L"&quot;"), (L"&apos;"), (L"&nbsp;")};
	static const std::wstring REPLACE_LIST[] = { (L"&"), (L"<"), (L">"), (L"\""), (L"\'"), (L" ")};

	unsigned int n_count = sizeof(FIND_LIST) / sizeof(FIND_LIST[0]);
	for (unsigned int i = 0; i < n_count; ++i)
	{
		boost::algorithm::replace_all(s_data, FIND_LIST[i], REPLACE_LIST[i]);
	}

	return;
}

void erase_html_item(std::wstring& s_data)
{
	static const wchar_t LEFT_ARROW = L'<';
	static const wchar_t RIGHT_ARROW = L'>';
	//std::wstring s_src = s_data;
	std::wstring::size_type n_pos = 0;
	int n_count = 0;
	while (true)
	{
		n_count = 0;
		n_pos = s_data.find(LEFT_ARROW, 0);		//查找第一个<
		if (n_pos == std::wstring::npos)
			break;
		std::wstring::size_type n_left_pos = n_pos;
		++n_count;
		++n_pos;
		while (n_pos < s_data.size())
		{
			if (s_data[n_pos] == LEFT_ARROW)
				++n_count;
			else if (s_data[n_pos] == RIGHT_ARROW)
				--n_count;
			if (n_count == 0)		//一段结束
				break;
			++n_pos;
		}
		if (n_pos >= s_data.size())
			break;
		assert(n_pos > n_left_pos);
		s_data.erase(n_left_pos, n_pos - n_left_pos + 1);
	}
	return;
}

ver_info get_version_info(const char* p_app_name)
{
	ver_info info;
	info.n_major_1 = info.n_major_2 = info.n_minor_1 = info.n_minor_2 = 0;
	if (p_app_name == NULL || strlen(p_app_name) == 0)
		return info;
	unsigned int n_info_size = sizeof(VS_FIXEDFILEINFO);
	int n_ver_size = ::GetFileVersionInfoSizeA(p_app_name, NULL);
	if (n_ver_size != 0)
	{
		char* p_buf = new char[n_ver_size];
		if (::GetFileVersionInfoA(p_app_name, 0, n_ver_size, p_buf))
		{
			VS_FIXEDFILEINFO* p_vs_info = NULL;
			if (::VerQueryValueA(p_buf, "\\", (void**)&p_vs_info, &n_info_size))
			{
				assert(p_vs_info != NULL);
				info.n_major_1 = HIWORD(p_vs_info->dwFileVersionMS);
				info.n_major_2 = LOWORD(p_vs_info->dwFileVersionMS);
				info.n_minor_1 = HIWORD(p_vs_info->dwFileVersionLS);
				info.n_minor_2 = LOWORD(p_vs_info->dwFileVersionLS);

				char s_buf[32];
				s_buf[0] = 0x0;
				sprintf_s(s_buf, 32, "%u.%u.%u.%u", info.n_major_1, info.n_major_2, info.n_minor_1, info.n_minor_2);
				info.version = s_buf;
			}
		}
		delete[] p_buf;
	}
	return info;
}

ver_info get_version_info(const wchar_t* p_module_name)
{
	ver_info info;
	info.n_major_1 = info.n_major_2 = info.n_minor_1 = info.n_minor_2 = 0;
	if (p_module_name == NULL || _tcslen(p_module_name) == 0)
		return info;
	unsigned int n_info_size = sizeof(VS_FIXEDFILEINFO);
	int n_ver_size = ::GetFileVersionInfoSize(p_module_name, NULL);
	if (n_ver_size != 0)
	{
		char* p_buf = new char[n_ver_size];
		if (::GetFileVersionInfo(p_module_name, 0, n_ver_size, p_buf))
		{
			VS_FIXEDFILEINFO* p_vs_info = NULL;
			if (::VerQueryValue(p_buf, _T("\\"), (void**)&p_vs_info, &n_info_size))
			{
				assert(p_vs_info != NULL);
				info.n_major_1 = HIWORD(p_vs_info->dwFileVersionMS);
				info.n_major_2 = LOWORD(p_vs_info->dwFileVersionMS);
				info.n_minor_1 = HIWORD(p_vs_info->dwFileVersionLS);
				info.n_minor_2 = LOWORD(p_vs_info->dwFileVersionLS);

				char s_buf[32];
				s_buf[0] = 0x0;
				sprintf_s(s_buf, 32, "%u.%u.%u.%u", info.n_major_1, info.n_major_2, info.n_minor_1, info.n_minor_2);
				info.version = s_buf;
			}
		}
		delete[] p_buf;
	}
	return info;
}
void get_file_name(CString &cs_path)
{	
	int n_file_begin=cs_path.ReverseFind(_T('\\'));
	if (n_file_begin<0)
	{
		return;
	}
	cs_path=cs_path.Right(cs_path.GetLength()-n_file_begin-1);
	int n_file_end=cs_path.ReverseFind(_T('.'));
	cs_path=cs_path.Left(n_file_end);   
}

int  compare_version(const ver_info& ver1,const ver_info& ver2)
{
	if (ver1.n_major_1>ver2.n_major_1)
	{
		return 1;
	}
	if (ver1.n_major_1<ver2.n_major_1)
	{
		return -1;
	}
	if (ver1.n_major_2>ver2.n_major_2)
	{
		return 1;
	}
	if (ver1.n_major_2<ver2.n_major_2)
	{
		return -1;
	}
	if (ver1.n_minor_1>ver2.n_minor_1)
	{
		return 1;
	}
	if (ver1.n_minor_1<ver2.n_minor_1)
	{
		return -1;
	}
	if (ver1.n_minor_2>ver2.n_minor_2)
	{
		return 1;
	}
	if (ver1.n_minor_2<ver2.n_minor_2)
	{
		return -1;
	}
	return 0;
}

time_counter::time_counter(unsigned int n_mill_seconds)
{
	mn_mill_seconds = n_mill_seconds;
}

void time_counter::begin()
{
	m_tick_list.clear();
	tick_it();
}

bool time_counter::end()
{
	tick_it();
	bool b_time_out = timeout() >= mn_mill_seconds;
	return  b_time_out;
}

unsigned int time_counter::timeout() const
{
	unsigned int n_mill_seconds = 0;
	if (m_tick_list.size() > 1)
	{
		DWORD n_begin = *m_tick_list.begin();
		DWORD n_end = *m_tick_list.rbegin();
		n_mill_seconds = n_end - n_begin;
	}
	return n_mill_seconds;
}

void time_counter::tick_it()
{
	m_tick_list.push_back(::GetTickCount());
}

std::string time_counter::print_tick_list() const
{
	std::stringstream info;
	int n_diff = 0;
	DWORD n_begin = 0;
	for (c_tick_iter i = m_tick_list.begin(); i != m_tick_list.end(); ++i)
	{
		if (n_begin == 0)
			n_begin = *i;
		else
		{
			n_diff = *i - n_begin;
			if (i + 1 == m_tick_list.end())					//最后一个
				info << i - m_tick_list.begin() << "=" << n_diff << "(end). ";
			else
				info << i - m_tick_list.begin() << "=" << n_diff << ", ";
			n_begin = *i;
		}
	}
	return info.str();
}

handle_dir::handle_dir()
{
	mp_handle = NULL;
}

void handle_dir::run(PHANDLEFILE p_handler, const CString& s_root)
{
	mp_handle = p_handler;
	if (s_root.IsEmpty())
		return;
	ms_root = s_root;
	check_dir(ms_root);
	return;
}

void handle_dir::check_dir(const CString& s_cur_dir)
{
	if (s_cur_dir.IsEmpty())
		return;
	
	WIN32_FIND_DATA find_data;
	HANDLE h_find = INVALID_HANDLE_VALUE;
	CString s_search_dir = s_cur_dir;
	if (s_search_dir[s_search_dir.GetLength() - 1] != '\\')
		s_search_dir += _T('\\');
	s_search_dir += _T("*.*");
	CString s_suffix;
	h_find = ::FindFirstFile(s_search_dir, &find_data);
	if (h_find == INVALID_HANDLE_VALUE)
		return;
	if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)        //目录
	{
		if (!(_tcscmp(find_data.cFileName, _T(".")) ==0 || _tcscmp(find_data.cFileName, _T("..")) == 0))	//非上层和自身目录
		{
			s_search_dir = s_cur_dir + _T('\\');
			s_search_dir += find_data.cFileName;
			check_dir(s_search_dir);
		}
	}
	else						//文件
	{
		s_search_dir = s_cur_dir + _T('\\');
		s_search_dir += find_data.cFileName;
		//s_search_dir.MakeLower();
		if (mp_handle != NULL)
			mp_handle(s_search_dir);
	}
	while (::FindNextFile(h_find, &find_data))
	{
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)        //目录
		{
			if (!(_tcscmp(find_data.cFileName, _T(".")) ==0 || _tcscmp(find_data.cFileName, _T("..")) == 0))
			{
				s_search_dir = s_cur_dir + _T('\\');
				s_search_dir += find_data.cFileName;
				check_dir(s_search_dir);
			}
		}
		else					//文件
		{
			s_search_dir = s_cur_dir + _T('\\');
			s_search_dir += find_data.cFileName;
			//s_suffix = s_search_dir.Right(4);
			//s_suffix.MakeLower();
			if (mp_handle != NULL)
				mp_handle(s_search_dir);
		}
	}
	::FindClose(h_find);
	return;
}

std::string url_encoder::url_encoding(const std::string& s_input)
{
	std::string s_output;
	for (int i = 0; i < s_input.size(); ++i)
	{
		unsigned char buf[4];
		memset(buf, 0x0, 4);
		if (isalnum((unsigned char)s_input[i]))
		{
			buf[0] = s_input[i];
		}
		else if (isspace((unsigned char)s_input[i]))
		{
			buf[0] = '+';
		}
		else
		{
			buf[0] = '%';
			buf[1] = conv_2_hex((unsigned char)s_input[i] >> 4);
			buf[2] = conv_2_hex((unsigned char)s_input[i] % 16);
		}
		s_output += (char*)buf;
	}
	return s_output;	
}

}		//end namespace misc

}		//end namespace em_utility