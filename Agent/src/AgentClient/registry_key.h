// (C) Copyright 2010 Jean Davy ( jean dot davy dot 06 at gmail dot com )
// Distributed under the Code Project Open License (CPOL) 1.02 (See accompanying
// file CPOL.html or copy at http://www.codeproject.com/info/cpol10.aspx.)

// ----------  Warning ----------
// In order to use "boost serialization to registry" you have to
//		"#include <boost/archive/basic_archive.hpp>"
// before including this file "registry_key.h"


// ----------  Warning ----------
// In order to use "MFC serialization to registry" you have to
//		"#include <afx.h>"
// before including this file "registry_key.h"

#if !defined(_MSC_VER) || (_MSC_VER < 1600)
#error Visual Studio 2010 required.
#endif

#pragma once

#pragma warning( push )
#pragma warning( disable : 4290 4800 4244 4310 4100 4189  ) 

#include <exception>
#include <string>
#include <vector>

#ifdef BOOST_ARCHIVE_BASIC_ARCHIVE_HPP

// BOOST_ARCHIVE_BASIC_ARCHIVE_HPP is defined in <boost/archive/basic_archive.hpp> file
// You have to #include <boost/archive/basic_archive.hpp> BEFORE this file in order
// to access the boost serialize functionalities

// See http://www.boost.org/libs/serialization for documentation.

/* Protect against #define of new */
#ifdef new
	#define _NEW_PUSHED_REGISTRY_KEY_H_
	#pragma push_macro("new")
	#undef  new
#endif

#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#ifdef _NEW_PUSHED_REGISTRY_KEY_H_
	#undef _NEW_PUSHED_REGISTRY_KEY_H_
	#pragma pop_macro("new")
#endif

#endif // #ifdef BOOST_ARCHIVE_BASIC_ARCHIVE_HPP

// CRegKey definition is in atlbase.h
#include <atlbase.h>

// win32_exception is generic, sould be defined in win32_utilty
class win32_exception : public std::exception
{
public:
	LONG WinErr; // Error code as defined WinError.h
	win32_exception()
		:WinErr( ERROR_SUCCESS )
	{}
	win32_exception( LONG Err )
		:WinErr( Err )
	{}
    win32_exception(const win32_exception& right)
		:std::exception( right )
		,WinErr( right.WinErr )
	{}
    win32_exception& operator=(const win32_exception& right)
	{
		dynamic_cast< std::exception& >( *this ) = right;
		WinErr = right.WinErr ;
		return *this;
	}
    virtual const char * what() const
	{
		LPVOID MsgBuf = NULL;
		DWORD Count = FormatMessageA( // ANSI version
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			WinErr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast< LPSTR >( &MsgBuf ),
			0, NULL );
		ATLASSERT( Count > 0 && MsgBuf );

		// Require base class to store this string as this is done in the constructor
		// I don't do it in constructor as we need it only if what() is called
		// Why move constructor is not implemented in std::exception ?
		const std::exception& My = *this;
		const_cast< std::exception& >( My ) = 
			std::exception( reinterpret_cast< const char* >( MsgBuf ) );

		LocalFree( MsgBuf );

		return  __super::what();
	}
};

#define _REGKEY_BEGIN namespace registry_key {
#define _REGKEY_END };

_REGKEY_BEGIN

typedef win32_exception regkey_exception ;

enum RegKeyType
{
	_RegKeyDword,
	_RegKeyRawBinary
};

// This templated class is empty as it only serves to define 
// the second template parameter with a default value.
// Then I partially specialize this template with the two possible values
// of second parameter.
template< class _Type, 
	RegKeyType _How = sizeof( _Type ) <= sizeof( DWORD ) ? _RegKeyDword : _RegKeyRawBinary >
class _TRegKey
{
	_Type Read( const TCHAR* Key );
	void Write( const TCHAR* Key, const _Type& );
};

// used when sizeof( _Type ) is less or equal to sizeof( DWORD )
template< class _Type > 
class _TRegKey< _Type, _RegKeyDword >
{
	public:
	CRegKey& m_RegKey;

	_TRegKey( CRegKey& RegKey )
		:m_RegKey( RegKey )
	{}

	_Type Read( const TCHAR* Key ) throw( regkey_exception )
	{
		static_assert( sizeof( _Type ) <= sizeof( DWORD ), 
			"sizeof( _Type ) must be less or equal to sizeof( DWORD )" );

		DWORD Value;
		LONG Err = m_RegKey.QueryDWORDValue( Key, Value );
		if( ERROR_SUCCESS != Err )
			throw regkey_exception( Err );
		_Type rValue = static_cast< _Type >( Value );
		return rValue ;
	}
	void Write( const TCHAR* Key, const _Type& Value )  throw( regkey_exception )
	{
		static_assert( sizeof( _Type ) <= sizeof( DWORD ), 
			"sizeof( _Type ) must be less or equal to sizeof( DWORD )" );

		DWORD dwValue = Value;
		LONG Err = m_RegKey.SetDWORDValue( Key, dwValue );
		if( ERROR_SUCCESS != Err )
			throw regkey_exception( Err );
	}
};

// used when sizeof( _Type ) is higher than sizeof( DWORD )
template< class _Type > 
class _TRegKey< _Type, _RegKeyRawBinary >
{
	public:
	CRegKey& m_RegKey;

	_TRegKey( CRegKey& RegKey )
		:m_RegKey( RegKey )
	{}

	_Type Read( const TCHAR* Key )  throw( regkey_exception )
	{
		ULONG Size = sizeof( _Type );
		_Type Value;
		LONG Err = m_RegKey.QueryBinaryValue( Key, &Value, &Size );
		if( ERROR_SUCCESS != Err )
			throw regkey_exception( Err );
		return Value ;
	}
	void Write( const TCHAR* Key, const _Type& Value ) throw( regkey_exception )
	{
		LONG Err = m_RegKey.SetBinaryValue( Key, &Value, sizeof( _Type ) );
		if( ERROR_SUCCESS != Err )
			throw regkey_exception( Err );
	}
};

// any kind of string (C, stl, wide or not) will be converted to CString 
// in order to use this _TRegKey specialization
template<> 
class _TRegKey< CString >
{
	public:
	CRegKey& m_RegKey;

	_TRegKey( CRegKey& RegKey )
		:m_RegKey( RegKey )
	{}

	CString Read( const TCHAR* Key )  throw( regkey_exception )
	{
		ULONG Size = 0;
		LONG Err = m_RegKey.QueryStringValue( Key, NULL, &Size );
		if( ERROR_SUCCESS != Err )
			throw regkey_exception( Err );
		CString Value;
		Err = m_RegKey.QueryStringValue( Key, Value.GetBufferSetLength( Size ), &Size );
		if( ERROR_SUCCESS != Err )
			throw regkey_exception( Err );
		Value.ReleaseBuffer();
		return Value ;
	}
	void Write( const TCHAR* Key, const CString& Value ) throw( regkey_exception )
	{
		LONG Err = m_RegKey.SetStringValue( Key, Value );
		if( ERROR_SUCCESS != Err )
			throw regkey_exception( Err );
	}
};


#ifdef __AFX_H__

template< class _Type >
void RegKeyMFCArchiveWrite( CRegKey& RegKey, const TCHAR* Key, 	const _Type& Value 
	) throw( ...  )// regkey_exception and CArchiveException
{
	CMemFile File;
	{
		CArchive ar( &File, CArchive::store );
		ar << Value ;
	}
	ULONGLONG Size =  File.GetLength( );
	BYTE* Ptr = File.Detach();

	LONG Err = RegKey.SetBinaryValue( Key, Ptr, Size );
	free( Ptr );

	if( ERROR_SUCCESS != Err )
		throw regkey_exception( Err );
}

template< class _Type >
_Type RegKeyMFCArchiveRead( CRegKey& RegKey, const TCHAR* Key, const _Type& 
	)  throw( ... ) // regkey_exception and CArchiveException
{
	DWORD BufferSize = 0;
	LONG Err = RegKey.QueryBinaryValue( Key, NULL, &BufferSize );
	if( ERROR_SUCCESS != Err )
		throw regkey_exception( Err );

	std::vector< BYTE > Vec( BufferSize );
	Err = RegKey.QueryBinaryValue( Key, &Vec[0], &BufferSize );
	if( ERROR_SUCCESS != Err )
		throw regkey_exception( Err );
	CMemFile File( &Vec[0], BufferSize );

	_Type Value;
	{
		CArchive ar( &File, CArchive::load );
		ar >> Value ;
	}

	return Value ;
}

#endif #ifdef __AFX_H__

_REGKEY_END

#ifdef BOOST_ARCHIVE_BASIC_ARCHIVE_HPP

#ifndef BOOST_IOSTREAMS_EXAMPLE_CONTAINTER_DEVICE_HPP_INCLUDED

// container_sink is copied from http://www.boost.org/libs/iostreams/example/container_device.hpp
//
// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2005-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)
//
// Model of Sink which appends to an STL-compatible sequence.
//

namespace boost { namespace iostreams {

template<typename Container>
class container_sink {
public:
    typedef typename Container::value_type  char_type;
    typedef sink_tag                        category;
    container_sink(Container& container) : container_(container) { }
    std::streamsize write(const char_type* s, std::streamsize n)
    {
        container_.insert(container_.end(), s, s + n);
        return n;
    }
    Container& container() { return container_; }
private:
    container_sink operator=(const container_sink&);
    Container& container_;
};

typedef stream_buffer< container_sink< std::vector< char > > > mem_stream;

}}

#endif // #ifndef BOOST_IOSTREAMS_EXAMPLE_CONTAINTER_DEVICE_HPP_INCLUDED

_REGKEY_BEGIN

template< class _Type >
void RegKeyArchiveWrite( CRegKey& RegKey, const TCHAR* Key, const _Type& Value 
	) throw( ... ) // regkey_exception and boost::archive::archive_exception
{
	typedef boost::iostreams::mem_stream mem_stream;

	std::vector< char > Vec;
	{
		mem_stream ms( Vec );
		boost::archive::binary_oarchive oa( ms );
		oa << Value;
	}

	LONG Err = RegKey.SetBinaryValue( Key, &Vec[0], Vec.size() * sizeof( Vec[0] ) );
	if( ERROR_SUCCESS != Err )
		throw regkey_exception( Err );
}

template< class _Type >
_Type RegKeyArchiveRead( CRegKey& RegKey, const TCHAR* Key, const _Type& 
	) throw( ... ) // regkey_exception and boost::archive::archive_exception
{
	DWORD BufferSize = 0;
	LONG Err = RegKey.QueryBinaryValue( Key, NULL, &BufferSize );
	if( ERROR_SUCCESS != Err )
		throw regkey_exception( Err );

	std::vector< char > Vec( BufferSize );
	Err = RegKey.QueryBinaryValue( Key, &Vec[0], &BufferSize );
	if( ERROR_SUCCESS != Err )
		throw regkey_exception( Err );

	_Type Value;
	{
		using namespace ::boost::iostreams ;
		array_source as( &Vec[0], BufferSize );
		stream_buffer< array_source > sb( as );
		boost::archive::binary_iarchive ia( sb );
		ia >> Value ;
	}

	return Value ;
}

_REGKEY_END

namespace boost {
namespace serialization {

// Sure this is not the more effective way, but the easiest, I leverage on well done
// stl::string archiving by copying to and from it
template<class Archive>
void save( Archive & ar, const CString& Obj, unsigned int version )
{
#ifdef _UNICODE
	std::wstring String = Obj;
#else
	std::string String = Obj;
#endif
    ar & String;
}

template<class Archive>
void load(Archive & ar, CString& Obj, unsigned int version)
{
#ifdef _UNICODE
	std::wstring String;
#else
	std::string String;
#endif
    ar & String;
	Obj = String.c_str() ;
}

} // namespace serialization
} // namespace boost

// this macro used at global level says to boost serialization that
// it has to use load and save functions defined for CString
// You can use this BOOST_SERIALIZATION_SPLIT_FREE macro for your own
BOOST_SERIALIZATION_SPLIT_FREE(CString)

// primitive_type assumes it is a fundamental type (as int, double ...)
// there is no runtime overhead associated reading/writing instances of this level
// as it is as bit to bit copy
// You can use this BOOST_CLASS_IMPLEMENTATION macro at global level in order to 
// say to boost serialization that your class/struct is a primitive_type
BOOST_CLASS_IMPLEMENTATION(POINT, primitive_type) 
BOOST_CLASS_IMPLEMENTATION(SIZE, primitive_type) 
BOOST_CLASS_IMPLEMENTATION(RECT, primitive_type) 
BOOST_CLASS_IMPLEMENTATION(POINTS, primitive_type) 

#endif // #ifdef BOOST_ARCHIVE_BASIC_ARCHIVE_HPP

_REGKEY_BEGIN

template< class _Type >
void RegKeyWrite( CRegKey& RegKey, const TCHAR* Key, const _Type& Value )
{
	_TRegKey< _Type >( RegKey ).Write( Key, Value );
}

// In C++ it is not possible to overload a function based on it's return value.
// This is why RegKeyRead have his last parameter without any value, 
// just a way to easily set the returned type.
template< class _Type >
_Type RegKeyRead( CRegKey& RegKey, const TCHAR* Key, const _Type& )
{
	return _TRegKey< _Type >( RegKey ).Read( Key );
}

inline void RegKeyWrite( CRegKey& RegKey, const TCHAR* Key, const CString& Value )
{
	_TRegKey< CString >( RegKey ).Write( Key, Value );
}

//------------------ String helper functions ------------------
// Strings stored in registry are always UNICODE, registry_key library does the
// conversion but it is the responability of the developer to check consistency. 
// In other words, you can write a UNICODE string and read back as ANSI, it works, no check is 
// done by registry_key library, it's up to you.

inline void RegKeyWrite( CRegKey& RegKey, const TCHAR* Key, const char* Value )
{
#ifdef _UNICODE
	CString String = CA2W( Value );
#else
	CString String = Value ;
#endif
	RegKeyWrite( RegKey, Key, String );
}

inline void RegKeyWrite( CRegKey& RegKey, const TCHAR* Key, const wchar_t* Value )
{
#ifdef _UNICODE
	CString String = Value;
#else
	CString String = CW2A( Value );
#endif
	RegKeyWrite( RegKey, Key, String );
}

inline void RegKeyWrite( CRegKey& RegKey, const TCHAR* Key, const std::wstring& Value )
{
	RegKeyWrite( RegKey, Key, Value.c_str() );
}

inline void RegKeyWrite( CRegKey& RegKey, const TCHAR* Key, const std::string& Value )
{
	RegKeyWrite( RegKey, Key, Value.c_str() );
}

inline CString RegKeyRead( CRegKey& RegKey, const TCHAR* Key, const CString& )
{
	return _TRegKey< CString >( RegKey ).Read( Key );
}

inline std::wstring RegKeyRead( CRegKey& RegKey, const TCHAR* Key, const std::wstring& ) 
{
	CString Value = RegKeyRead( RegKey, Key, Value );
#ifdef _UNICODE
	return std::wstring( Value );
#else
	return std::wstring( CA2W( Value ) );
#endif
}

inline std::string RegKeyRead( CRegKey& RegKey, const TCHAR* Key, const std::string& ) 
{
	CString Value = RegKeyRead( RegKey, Key, Value );
#ifdef _UNICODE
	return std::string( CW2A( Value ) );
#else
	return std::string( Value );
#endif
}

_REGKEY_END

#pragma warning( pop )
