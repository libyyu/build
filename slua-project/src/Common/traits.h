#ifndef traits_h
#define traits_h
#pragma once

namespace em_utility
{
namespace trait
{
	template<typename T>
	struct type_traits
	{
		typedef T                      value_type;
		typedef T*                     pointer;
		typedef T&                     reference;
		typedef const T*               const_pointer;
		typedef const T&               const_reference;
	};

	template<typename T>
	struct type_traits<T*>
	{
		typedef T                      value_type;
		typedef T*                     pointer;
		typedef T&                     reference;
		typedef const T*               const_pointer;
		typedef const T&               const_reference;
	};

	template<typename T>
	struct type_traits<const T*>
	{
		typedef T                      value_type;
		typedef T*                     pointer;
		typedef T&                     reference;
		typedef const T*               const_pointer;
		typedef const T&               const_reference;
	};

	template<typename T>
	struct type_traits<T&>
	{
		typedef T                      value_type;
		typedef T*                     pointer;
		typedef T&                     reference;
		typedef const T*               const_pointer;
		typedef const T&               const_reference;
	};

	template<>
	struct type_traits<void>
	{
		typedef void                   value_type;
		typedef void*                  pointer;
		typedef const void*            const_pointer;
	};
}//namespace traits

}//namespace em_utility

#endif//traits_h