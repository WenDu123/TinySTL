#pragma once
#ifndef _CONSTRUCT_
#define _CONSTRUCT_

#include<new>
#include "Iterator.h"

#include "TypeTraits.h"
#include <cstddef>
namespace MyTinySTL {


	template<class T>
	inline void destory(T* ptr) {
		ptr-> ~T();
	}

	template<class T1,class T2>
	inline void construct(T1* p, const T2& value) {
		new(p)T2(value);
	}



	inline void destory(char*, char*) {}
	inline void destory(wchar_t*, wchar_t*) {}



	template<class ForwardIterator>
	inline void __destory_aux(ForwardIterator first, ForwardIterator last,__true_type) {}

	template<class ForwardIterator>
	inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type) {
		for (; first != last; ++first) {
			destory(&*first);
		}
	}

	template<class ForwardIterator, class T>
	inline void __destory(ForwardIterator first, ForwardIterator last, T*) {
		
		typedef typename __type_traits<T>::has_trivial_destructor  trivial_destructor;
		__destory_aux(first, last, trivial_destructor());
	}



	template<typename ForwardIterator>
	inline void destory(ForwardIterator first, ForwardIterator last) {
		

		__destory(first, last, value_type(first));
	}



}

#endif