#pragma once
#ifndef __FUNCTIONAL_H__
#define __FUNCTIONAL_H__
#include"pair.h"

namespace MyTinySTL {
	template<class Arg,class Result> //һԪ����
	struct unary_function{
		typedef Arg argument_type;
		typedef Result result_type;
	};
	template<class Arg1,class Arg2,class Result> //��Ԫ����
	struct binary_function {
		typedef Arg1 first_argument_type;
		typedef Arg2 seconde_argument_type;
		typedef Result result_type;
	};

	//������
	//�ӷ�
	template<class T>
	struct plus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x + y; }
	};
	//����
	template<class T> 
	struct minus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x - y; }
	};
	//�˷�
	template<class T> 
	struct multiplies : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x * y; }
	};
	//����
	template<class T> 
	struct divides : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x / y; }
	};
	//ȡģ
	template<class T> 
	struct modulus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x % y; }
	};
	//��
	template<class T>
	struct negate : public binary_function<T, T, T> {
		T operator()(const T& x) const { return -x; }
	};

	//��ϵ������

	//����
	template<class T> 
	struct equal_to : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x == y; }
	};
	//������
	template<class T> 
	struct not_equal_to : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x != y; }
	};
	//����
	template<class T> 
	struct greater : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x > y; }
	};

	//С��
	template<class T> //С�ڷ���1�����ڵ��ڷ���0
	struct less : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x < y; }
	};

	//���ڻ����
	template<class T> 
	struct greater_equal : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x >= y; }
	};
	//С�ڻ����
	template<class T> 
	struct less_equal : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x < y; }
	};

	//֤ͬ
	template<class T>
	struct identity : public unary_function<T, T> {
		const T& operator()(const T& x) const { return x ; }
	};

	//ѡ��
	template<class Pair>
	struct select1st :public unary_function<Pair, typename Pair::first_type> {
		const typename Pair::first_type operator()(const Pair& x) const {
			return x.first;
		}


	};


}




#endif // !__FUNCTIONAL_H__
