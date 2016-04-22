/********************************************************************
    FileName :  FType.h   
    Version  :  0.10
    Date     :	2010-2-1 19:41:30
    Author   :  Feng(libyyu@qq.com)
    Comment  : 

*********************************************************************/
#ifndef __FTYPE_H__
#define __FTYPE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>

#if defined(WIN32) || defined(_WIN32)
  #define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
  #include <windows.h>
  #include <float.h>
  #include <io.h>
  #include <stdlib.h>

  #define STD_CALL  __stdcall
  #define CALLBACK  __stdcall
  #define WINAPI    __stdcall
  #ifdef _T
  #undef _T
  #endif  

  #if defined( UNICODE ) || defined( _UNICODE )
	#define  _T(type)  L##type
	#define  _W(fun)   w##fun
  
	#define Fchar               wchar_t
	#define Fstring				std::wstring
	#define Fstrftime           wcsftime
	#define Fstrcat             wcscat      
	#define Ffopen              _wfopen

	#define Ffprintf            fwprintf
	#define Fstrrchr            wcsrchr
	#define Fstrncpy            wcsncpy   
	#define Fstrlen             wcslen
	#define Fstrcmp             wcscmp
	#define Fstrcpy             lstrcpy
	#define Fvfprintf           vfwprintf
	#define Fvsnprintf		    _vsnwprintf_s 
	#define Fsprintf	        swprintf
	#define Fsnprintf          _snwprintf
  #else
     #define _T(type)            type
     #define _W(fun)             fun
     #define Fchar               char
	 #define Fstring			 std::string
     #define Fstrftime           strftime
     #define Fstrcat             strcat
     #define Ffopen              fopen

     #define Ffprintf            fprintf
     #define Fstrrchr            strrchr
     #define Fstrncpy            strncpy
     #define Fstrlen             strlen
     #define Fstrcmp             strcmp
     #define Fstrcpy             strcpy
     #define Fvfprintf           vfprintf
	 #define Fvsnprintf			 vsnprintf 
	 #define Fsprintf	         sprintf
	 #define Fsnprintf           snprintf
  #endif //UNICODE  
  #ifdef _STL
      #define  Fcout            _W(cout)
      #define  Fcin             _W(cin)
      #define  Fcerr            _W(cerr)
      #define  Fclog            _W(clog)
      //#define  Fstring          _W(string)
      #define  Ffstream         _W(fstream)
      #define  Fifstream        _W(ifstream)
      #define  Fofstream        _W(ofstream)
      #define  Fstringstream    _W(stringstream)
      //#define  Fsprintf         _W(sprintf) 
      #define  Fprintf          _W(printf)
  #endif//_STL
#elif defined ( ANDROID ) || defined( _ANDROID ) || defined( IOS ) || defined( _IOS )
	#define STD_CALL
	#define CALLBACK
	#define WINAPI
	#ifdef _T
	#undef _T
	#endif  
	#if defined( UNICODE ) || defined( _UNICODE )
		#define  _T(type)  L##type
		#define  _W(fun)   w##fun

		#define Fchar               wchar_t
		#define Fstring				std::wstring
		#define Fstrftime           wcsftime
		#define Fstrcat             wcscat      
		#define Ffopen              _wfopen

		#define Ffprintf            fwprintf
		#define Fstrrchr            wcsrchr
		#define Fstrncpy            wcsncpy   
		#define Fstrlen             wcslen
		#define Fstrcmp             wcscmp
		#define Fstrcpy             lstrcpy
		#define Fvfprintf           vfwprintf
		#define Fvsnprintf		    _vsnwprintf_s 
		#define Fsprintf	        swprintf
		#define Fsnprintf          _snwprintf
	#else
		#define _T(type)            type
		#define _W(fun)             fun
		#define Fchar               char
		#define Fstring			    std::string
		#define Fstrftime           strftime
		#define Fstrcat             strcat
		#define Ffopen              fopen

		#define Ffprintf            fprintf
		#define Fstrrchr            strrchr
		#define Fstrncpy            strncpy
		#define Fstrlen             strlen
		#define Fstrcmp             strcmp
		#define Fstrcpy             strcpy
		#define Fvfprintf           vfprintf
		#define Fvsnprintf			vsnprintf 
		#define Fsprintf	        sprintf
		#define Fsnprintf           snprintf
	#endif
#endif //WIN32 

#ifdef  _DEBUG
     #define  FDeBugOut(type)   cout(type)
#else
     #define  FDeBugOut(type)   //cout(type)
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef   __cplusplus
    #define  _FCFunBegin   extern "C" {
    #define  _FCFunEnd     }
#else
	#define  _FCFunBegin
	#define  _FCFunEnd
#endif//__cplusplus



#define  _FNameSpaceBegin(name)   namespace name {
#define  _FNameSpaceEnd           }
#define  _FUsing(name)            using namespace name;

#define _FStdBegin  _FNameSpaceBegin(FStd)
#define _FStdEnd    _FNameSpaceEnd


#if defined(API_BUILD_AS_DLL)
  #if defined(_F_DLL_)
    #define F_DLL_API __declspec(dllexport)
  #else
    #define F_DLL_API __declspec(dllimport)
  #endif
#else
  #define F_DLL_API   extern
#endif


#define F_LIB_API  F_DLL_API
//////////////////////////////////////////////////////////////////////////



/********************************************************************************
 
 Base integer types for all target OS's and CPU's
 
 UInt8            8-bit unsigned integer 
 SInt8            8-bit signed integer
 UInt16          16-bit unsigned integer 
 SInt16          16-bit signed integer           
 UInt32          32-bit unsigned integer 
 SInt32          32-bit signed integer   
 UInt64          64-bit unsigned integer 
 SInt64          64-bit signed integer   
 
 *********************************************************************************/
typedef unsigned char                   uchar;
typedef unsigned short                  ushort;
typedef unsigned int                    uint;
typedef unsigned long                   ulong;

typedef signed char                     int8;
typedef uchar                           uint8;
typedef signed short                    int16;
typedef ushort                          uint16;


#ifdef __LP64__
typedef signed int                      int32;
typedef uint                            uint32;
#else
typedef signed long                     int32;
typedef unsigned long                   uint32;
#endif

#if defined(_MSC_VER) && !defined(__MWERKS__) && defined(_M_IX86)
typedef   signed __int64                int64;
typedef unsigned __int64                uint64;
#else
typedef   signed long long              int64;
typedef unsigned long long              uint64;
#endif


#endif//__FTYPE_H__




