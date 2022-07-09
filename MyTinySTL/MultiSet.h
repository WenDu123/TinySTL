#pragma once
#ifndef __MULTI_SET_H_
#define __MULTI_SET_H_

#include "RBtree.h"


namespace MyTinySTL {

	//缺省（默认）情况下使用递增排序
	//set的键值为实值
	//不允许通过迭代器对set在任意处写入
	template<class Key, class Compare = less<Key>, class Alloc = alloc>
	class multiset {
	public:
		typedef Key key_type;
		typedef Key value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
	private:
		typedef rb_tree< key_type, value_type, identity<value_type>, Compare, Alloc> rb_tree_type;
		rb_tree_type t;

	public:
		typedef typename rb_tree_type::const_pointer pointer;
		typedef typename rb_tree_type::const_pointer const_pointer;
		typedef typename rb_tree_type::const_reference reference;
		typedef typename rb_tree_type::const_reference const_reference;
		typedef typename rb_tree_type::const_iterator iterator;
		//不允许通过迭代器对set在任意处写入,因为set是已经排好序的
		typedef typename rb_tree_type::const_iterator const_iterator;
		typedef typename rb_tree_type::size_type size_type;
		typedef typename rb_tree_type::difference_type difference_type;

	public:
		multiset() :t(Compare()) {}
		explicit multiset(const Compare& comp) :t(comp) {}

		template<class InputIterator>
		multiset(InputIterator first, InputIterator last) : t(Compare()) { t.insert_equal(first, last); }

		template<class InputIterator>
		multiset(InputIterator first, InputIterator last, const Compare& comp) : t(comp) { t.insert_equal(first, last); }

		multiset(const multiset& s) : t(s.t) {}

		multiset& operator=(const multiset& x) {
			t = x.t;
			return *this;
		}

		key_compare key_comp() const { return t.key_comp(); }
		value_compare value_comp() const { return t.key_comp(); }
		iterator begin() const { return t.begin(); }
		iterator end() const { return t.end(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }

		iterator insert(const value_type& val) {
			return t.insert_equal(val);
		}

		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			t.insert_equal(first, last);
		}

		void erase(iterator position) {
			typedef typename rb_tree_type::iterator rb_iterator;
			t.erase((rb_iterator&)position);
		}

		void clear() {
			t.clear();
		}

		iterator find(const key_type& x) const { return t.find(x); }


		friend bool operator==(const multiset&, const multiset&);
		friend bool operator<(const multiset&, const vset&);

	};
	template<class Key, class Compare, class Alloc>
	inline bool operator==(const multiset<Key, Compare, Alloc>& x, const multiset<Key, Compare, Alloc>& y) {
		return x.t == y.t;
	}
	template<class Key, class Compare, class Alloc>
	inline bool operator<(const multiset<Key, Compare, Alloc>& x, const multiset<Key, Compare, Alloc>& y) {
		return x.t < y.t;
	}

}


#endif // !__MULTI_SET_H_

