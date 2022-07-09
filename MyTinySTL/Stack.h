#pragma once

#ifndef _STACK_H__
#define _STACK_H__

#include"Deque.h"
namespace MyTinySTL{
template<class T,class Sequence = deque<T>>
class stack {
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type  size_type;
	typedef typename Sequence::reference  reference;
	typedef typename Sequence::const_reference  const_reference;
protected:
	Sequence c;
public:
	stack():c(){}
	explicit stack(const Sequence& s): c(s){}
	void push(const value_type& val) {
		c.push_back(val);
	}
	void pop() { c.pop_back(); }
	reference top() { return c.back(); }
	const_reference top() const { return c.back(); }
	bool empty() {return c.empty(); }
	size_type size() const { return c.size(); };

	friend bool operator== (const stack& x, const stack& y);
	friend bool operator!= (const stack& x, const stack& y);
	friend bool operator< (const stack& x, const stack& y);
};
template<class T, class Sequence>
bool operator== (const stack<T,Sequence>& x, const stack<T, Sequence>& y) {
	return x.c == y.c;
}

template<class T, class Sequence>
bool operator!= (const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
	return !(x == y);
}

template<class T, class Sequence>
bool operator< (const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
	return x.c < y.c;
}



}

#endif // !_STACK_H__

