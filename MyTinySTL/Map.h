#pragma once
#pragma once
#ifndef __MAP_H_
#define __MAP_H_

#include"RBtree.h"

namespace MyTinySTL {

	//ȱʡ��Ĭ�ϣ������ʹ�õ�������
	//map�ļ�ֵ��ʵֵ��һ������ֵ�����Ըı䣬ʵֵ���Ըı�
   //������ͨ���������ı��ֵ������ı�ʵֵ
	//KeyΪ��ֵ�ͱ�TΪʵֵ�ͱ�
	template<class Key,class T,class Compare = less<Key>, class Alloc = alloc>
	class map {
	public:
		typedef Key key_type;
		typedef T data_type;
		typedef T mapped_type;
		typedef pair<const Key, T> value_type;//Ԫ���ͱ𣨼�ֵ/ʵֵ��
		typedef Compare key_compare;//��ֵ�ȽϺ���

		class value_compare
				:public binary_function<value_type, value_type, bool> {
			friend map<Key, T, Compare, Alloc>;
		protected:
			Compare comp;
			value_compare(Compare c):comp(c){}
		public:
			bool operator()(const value_type& x, const value_type& y) {
				return comp(x.first, y.first);
			}
		};


	private:
		typedef rb_tree< key_type,value_type, select1st<value_type>,key_compare, Alloc> rb_tree_type;
		rb_tree_type t;

	public:
		typedef typename rb_tree_type::const_pointer pointer;
		typedef typename rb_tree_type::const_pointer const_pointer;
		typedef typename rb_tree_type::const_reference reference;
		typedef typename rb_tree_type::const_reference const_reference;
		typedef typename rb_tree_type::iterator iterator;
		//��set��ȫ��const��һ����map�����޸�ֵ���������޸ļ�ֵ
		typedef typename rb_tree_type::const_iterator const_iterator;
		typedef typename rb_tree_type::size_type size_type;
		typedef typename rb_tree_type::difference_type difference_type;

	public:
		map() :t(Compare()) {}
		explicit map(const Compare& comp) :t(comp) {}

		template<class InputIterator>
		map(InputIterator first, InputIterator last) : t(Compare()) { t.insert_unique(first, last); }

		template<class InputIterator>
		map(InputIterator first, InputIterator last, const Compare& comp) : t(comp) { t.insert_unique(first, last); }

		map(const map& m) : t(m.t) {}

		map& operator=(const map& x) {
			t = x.t;
			return *this;
		}

		key_compare key_comp() const { return t.key_comp(); }
		value_compare value_comp() const { return value_compare(t.key_comp()); }
		iterator begin(){ return t.begin(); }
		const_iterator begin() const { return t.begin(); }
		iterator end() { return t.end(); }
		const_iterator end() const { return t.end(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }

		T& operator[](const key_type& k) {
			return  (*((insert(value_type(k, T()))).first)).second;
		}
		//��ʱ���������
		/*
		1. value_type(k, T())   ����pair��ʱ����
		2. insert(value_type(k, T())  ������ʱ���� pair<iterator, bool>
		3. ((insert(value_type(k, T()))).first) ȡ��iterator
		4. (*((insert(value_type(k, T()))).first))   *iterator ȡ��value_type�ͱ��value_field
		5. (*((insert(value_type(k, T()))).first)).second  value_typeΪpair���ͣ�ȡ���ĵڶ�������ʵֵ

		typedef rb_tree< key_type,value_type, select1st<value_type>,key_compare, Alloc> rb_tree_type;
		�ڶ�ģ�������Ϊ�ڵ���ͱ�value_type

		*/


		pair<iterator, bool> insert(const value_type& x) {
			
			return t.insert_unique(x);
		}

		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			t.insert_unique(first, last);
		}

		void erase(iterator position) {
			t.erase(position);
		}

		void clear() {
			t.clear();
		}

		iterator find(const key_type& x){ return t.find(x); }
		const_iterator find(const key_type& x) const { return t.find(x); }



		friend bool operator==(const  map&, const   map&);
		friend bool operator<(const  map&, const  map&);

	};
	template<class Key,class T,class Compare, class Alloc>
	inline bool operator==(const map<Key,T,Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
		return x.t == y.t;
	}
	template<class Key,class T,class Compare, class Alloc>
	inline bool operator<(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
		return x.t < y.t;
	}


}


#endif // !__MAP_H_