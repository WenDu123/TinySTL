
#pragma once
#ifndef  _ALGOBASE_
#define  _ALGOBASE_


#include "Iterator.h"

#include "TypeTraits.h"
#include <cstring>


namespace MyTinySTL {

	//fill
	template<class ForwardIterator,class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value) {
		for (; first != last; ++first) {
			*first = value;
		}
	}


	//fill_n
	 template<class OutputIterator,class Size,class T>
	 OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
		 for (; n > 0; --n, ++first) {
			 *first = value;
		 }
		 return first;
	 }

	//copy

	 template<class Random_Access_Iterator, class OutputIterator, class Distance>
	 inline OutputIterator __copy_d(Random_Access_Iterator first, Random_Access_Iterator last, OutputIterator result, Distance*) {

		 for (Distance n = last - first; n > 0; --n, ++result, ++first) {
			 *result = *first;  //assignmet operator
		 }
		 return result;
	 }
	 
	 template<class InputIterator, class OutputIterator>
	 inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
		 for (; first != last; ++result, ++first) {
			 *result = *first;
		 }
		 return result;
	 }
	
	 template<class Random_Access_Iterator, class OutputIterator>
	 inline OutputIterator __copy(Random_Access_Iterator first, Random_Access_Iterator last, OutputIterator result, random_access_iterator_tag) {
		 return __copy_d(first, last, result, distance_type(first));
	 }



	 template<class T>
	 inline T* __copy_t(T* first, T* last, T* result, __true_type) {
		 memmove(result, first, sizeof(T) * (last - first));
		 return result + (last - first);
	 }

	 template<class T>
	 inline T* __copy_t(T* first, T* last, T* result, __false_type) {

		 return __copy_d(first, last, result, (ptrdiff_t*)0);
	 }

	template<class InputIterator, class OutputIterator>
	 struct __copy_dispatch {
		 OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result){	
			 return __copy(first,last, result, iterator_category(first));
		 }
	 };

	 template<class T>
	 struct __copy_dispatch<T*,T*> {
		 T* operator() (T* first, T* last,T* result){
			 typedef typename __type_traits<T>::has_trivial_assignment_operator t;
			 return __copy_t(first, last, result, t());
		 }
	 };

	 template<class T>
	 struct __copy_dispatch<const T*, T*> {
		 T* operator() (const T* _first,const T* last,T* result){
			 typedef typename __type_traits<T>::has_trivial_assignment_operator t;
			 return __copy_t(_first,last,result,t());
		 }
	 };

	 template<class InputIterator,class OutputIterator>
	 inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
		 return __copy_dispatch<InputIterator, OutputIterator>() (first, last, result); //调用仿函数时，类名<T>() ( 参数，已重载（） )
	 }

	
	 inline char* copy(const char* first, const char* last, char* result) {
		 memmove(result, first, last - first);
		 return result + (last - first);
	 }




	 //copy_backward

	 template<class BidirectionalIterator1, class BidirectionalIterator2>
	 inline BidirectionalIterator2
		 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
		 while (first != last) *--result = *--last;
		 return result;

	 }

	 template<class T>
	 inline T* __copy_backward_t(const T* first, const T* last, T* result, __true_type) {
		 const ptrdiff_t N = last - first;
		 memmove(result - N, first, sizeof(T) * N);
		 return result - N;
	 }

	 template<class T>
	 inline T* __copy_backward_t(const T* first, const T* last, T* result, __false_type) {
		 return  __copy_backward(first, last, result);
	 }



	 template<class BidirectionalIterator1,class BidirectionalIterator2>
	 struct __copy_backward_dispatch{
		 BidirectionalIterator2 operator() (BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result){
			 return  __copy_backward(first,last,result);
		 }

	 };

	 template<class T>
	 struct __copy_backward_dispatch<T* ,T*>{
		 T* operator() (T* first,T* last,T* result){
 			 typedef typename  __type_traits<T>::has_trivial_assignment_operator t;
			 return  __copy_backward_t(first,last,result,t());			
		 }

	 };

	 template<class T>
	 struct __copy_backward_dispatch<const T*,T*>{	
		T* operator() (const T* first,const T* last,T* result){
			 typedef typename  __type_traits<T>::has_trivial_assignment_operator t;
			 return  __copy_backward_t(first,last,result,t());
		 }

	 };


	 template<class BidirectionalIterator1,class BidirectionalIterator2>
	 inline BidirectionalIterator2 
	 copy_backward(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result){
		return __copy_backward_dispatch<BidirectionalIterator1,BidirectionalIterator2>() (first,last,result);	 
	 }


	 //swap
	 template<class T>
	 void swap(T& a, T& b) {
		 T tmp = a;
		 a = b;
		 b = tmp;
	 }


	 //max
	 template<class T>
	 T max(const T& a,const T& b) {
		 return a > b ? a : b;
	 }

	 template<class InputIterator1,class InputIterator2>
	 inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
		 for (; first1 != last1; ++first1, ++first2) {
			 if (*first1 != *first2) {
				 return false;
			 }
		 }
		 return true;
	 }


	 template<class InputIterator1, class InputIterator2>
	 inline bool lexicographocal_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,InputIterator2 last2) {
		 for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			 if (*first1 < *first2) {
				 return true;
			 }
			 if (*first1 > *first2) {
				 return false;
			 }
		 }
		 return first1 == last1 && first2 != last2;
		 //先到达last1，未到达last2，返回true；先到达last2，未到达last1，返回false，同时到达，返回false
	 }
	 
	 //lower_bound


}





#endif
