#pragma once
#ifndef __FUNCTIONAL_H__
#define __FUNCTIONAL_H__
#include"pair.h"

namespace MyTinySTL {
	template<class Arg,class Result> //一元函数
	struct unary_function{
		typedef Arg argument_type;
		typedef Result result_type;
	};
	template<class Arg1,class Arg2,class Result> //二元函数
	struct binary_function {
		typedef Arg1 first_argument_type;
		typedef Arg2 seconde_argument_type;
		typedef Result result_type;
	};

	//算数类
	//加法
	template<class T>
	struct plus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x + y; }
	};
	//减法
	template<class T> 
	struct minus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x - y; }
	};
	//乘法
	template<class T> 
	struct multiplies : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x * y; }
	};
	//除法
	template<class T> 
	struct divides : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x / y; }
	};
	//取模
	template<class T> 
	struct modulus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x % y; }
	};
	//否定
	template<class T>
	struct negate : public binary_function<T, T, T> {
		T operator()(const T& x) const { return -x; }
	};

	//关系类运算

	//等于
	template<class T> 
	struct equal_to : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x == y; }
	};
	//不等于
	template<class T> 
	struct not_equal_to : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x != y; }
	};
	//大于
	template<class T> 
	struct greater : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x > y; }
	};

	//小于
	template<class T> //小于返回1，大于等于返回0
	struct less : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x < y; }
	};

	//大于或等于
	template<class T> 
	struct greater_equal : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x >= y; }
	};
	//小于或等于
	template<class T> 
	struct less_equal : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x < y; }
	};

	//证同
	template<class T>
	struct identity : public unary_function<T, T> {
		const T& operator()(const T& x) const { return x ; }
	};

	//选择
	template<class Pair>
	struct select1st :public unary_function<Pair, typename Pair::first_type> {
		const typename Pair::first_type operator()(const Pair& x) const {
			return x.first;
		}


	};


}




#endif // !__FUNCTIONAL_H__
