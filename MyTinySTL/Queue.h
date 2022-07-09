#pragma once

#ifndef _QUEUE_H__
#define _QUEUE_H__

#include"Deque.h"
namespace MyTinySTL {
	template<class T, class Sequence = deque<T>>
	class queue {
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type  size_type;
		typedef typename Sequence::reference  reference;
		typedef typename Sequence::const_reference  const_reference;
	protected:
		Sequence c;
	public:
		queue() :c() {}
		explicit queue(const Sequence& s) : c(s) {}
		void push(const value_type& val) {
			c.push_back(val);
		}
		void pop() { c.pop_front(); }
		reference front() { return c.front(); }
		const_reference front() const { return c.front(); }
		reference back() { return c.back(); }
		const_reference back() const { return c.back(); }
		bool empty() { return c.empty(); }
		size_type size() const { return c.size(); };

		friend bool operator== (const queue& x, const queue& y);
		friend bool operator!= (const queue& x, const queue& y);
		friend bool operator< (const  queue& x, const queue& y);
	};
	template<class T, class Sequence>
	bool operator== (const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
		return x.c == y.c;
	}

	template<class T, class Sequence>
	bool operator!= (const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
		return !(x == y);
	}

	template<class T, class Sequence>
	bool operator< (const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
		return x.c < y.c;
	}



}

#endif // !_QUEUE_H__


