#pragma once
#ifndef _ITERT_H_
#define _ITERT_H_


//�˴�ԭ����_ITERATOR_������������ʾvalue_type()�Ҳ�����ʶ�����ʶ�ɾ��������ĸȴ�ɹ��ˣ�֤��ͷ�ļ��ظ������ִ���
//��ͬͷ�ļ��еĺ�����ײ������#ifndef ���������������������ʱ�ͻᵼ���㿴��ͷ�ļ��������ڣ���������ȴӲ˵�Ҳ���������״��

#include <cstddef>



namespace MyTinySTL {


	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag :public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};




	template<class T, class Distance>
	struct input_iterator {
		typedef  input_iterator_tag		iterator_category;
		typedef  T						value_type;
		typedef  Distance				difference_type;
		typedef  T* pointer;
		typedef  T& reference;

	};

	struct output_iterator {
		typedef  output_iterator_tag		iterator_category;
		typedef  void						value_type;
		typedef  void						difference_type;
		typedef  void						pointer;
		typedef  void						reference;

	};

	template<class T, class Distance>
	struct forward_iterator {
		typedef  forward_iterator_tag		iterator_category;
		typedef  T							value_type;
		typedef  Distance					difference_type;
		typedef  T* pointer;
		typedef  T& reference;

	};

	template<class T, class Distance>
	struct bidirectional_iterator {
		typedef  bidirectional_iterator_tag		iterator_category;
		typedef  T						value_type;
		typedef  Distance				difference_type;
		typedef  T* pointer;
		typedef  T& reference;

	};


	template<class T, class Distance>
	struct random_access_iterator {
		typedef  random_access_iterator_tag		iterator_category;
		typedef  T						value_type;
		typedef  Distance				difference_type;
		typedef  T* pointer;
		typedef  T& reference;

	};

	template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator {
		typedef  Category			iterator_category;
		typedef  T					value_type;
		typedef  Distance			difference_type;
		typedef  Pointer			pointer;
		typedef  Reference			reference;
	};

	template<typename Iterator>
	struct iterator_traits {
		typedef typename Iterator::itrator_category		iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;

	};


	template<class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag				iterator_category;
		typedef T										value_type;
		typedef ptrdiff_t								difference_type;
		typedef T* pointer;
		typedef T& reference;

	};

	template<class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag				iterator_category;
		typedef T										value_type;
		typedef ptrdiff_t								difference_type;
		typedef const T* pointer;
		typedef const T& reference;

	};



	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category __iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}
	
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator& i) {
		return __iterator_category(i);
	}
	/*
	template<class T,class Distance>
	inline  random_access_iterator_tag iterator_category(const random_access_iterator<T,Distance>&) {
		return random_access_iterator_tag();
	}

	template<class T, class Distance>
	inline  random_access_iterator_tag iterator_category(const T*) {
		return random_access_iterator_tag();
	}
	*/






	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}


	template<typename Iterator>
	typename iterator_traits<Iterator>::value_type* value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}



	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		_distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type	n = 0;
		while (first != last) {
			first++;
			n++;
		}
		return n;
	}
	template<class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		_distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
		return last - first;
	}


	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last) {
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return _distance(first, last, category());
	}





	template<class InputIterator, class Distance>
	inline void _distance(InputIterator first, InputIterator last, Distance& n, input_iterator_tag) {
		while (first != last) {
			first++;
			n++;
		}
	
	}
	template<class RandomAccessIterator, class Distance>
	inline void _distance(RandomAccessIterator first, RandomAccessIterator last, Distance& n,random_access_iterator_tag) {
		n += last - first;
	}


	template<class InputIterator, class Distance>
	inline void distance(InputIterator first, InputIterator last, Distance& n) {
		_distance(first, last, n, iterator_category(first));
	}





	template<class InputIterator, class Distance>
	inline void _advance(InputIterator& i, Distance n, input_iterator_tag)
	{
		while (n--) ++i;
	}

	template<class BidirectionalIterator, class Distance>
	inline void _advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
	{
		if (n >= 0)
			while (n--) ++i;
		else
			while (n++) --i;
	}

	template<class RandomAccessIterator, class Distance>
	inline void _advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag)
	{
		i += n;
	}

	template<class InputIterator, class Distance>
	inline void advance(InputIterator& i, Distance n)
	{
		_advance(i, n, iterator_category(i));
	}





}

#endif

