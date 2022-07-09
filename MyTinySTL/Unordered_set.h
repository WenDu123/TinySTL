#pragma once
#ifndef _UNORDERED_SET_H_
#define _UNORDERED_SET_H_
#include"HashTable.h"

namespace MyTinySTL {

	template<class Value,class HashFcn = hash<Value>,class EqualKey = equal_to<Value>, class Alloc = alloc>
	class unordered_set {
	private:
		typedef hashtable<Value,Value, HashFcn,identity<Value>, EqualKey, Alloc>  ht;
		ht rep;
	public:
		typedef typename ht::value_type value_type;
		typedef typename ht::key_type key_type;
		typedef typename ht::hasher hasher;
		typedef typename ht::key_equal key_equal;

		typedef typename ht::size_type size_type;
		typedef typename ht::difference_type difference_type;
		typedef typename ht::const_reference reference;
		typedef typename ht::const_reference const_reference;
		typedef typename ht::const_pointer pointer;
		typedef typename ht::const_pointer const_pointer;


		typedef typename ht::const_iterator iterator;
		typedef typename ht::const_iterator const_iterator;

		hasher hash_funct() const { return rep.hash_funct(); }
		key_equal key_eq() const { return rep.key_eq(); }
	

	public:
		unordered_set():rep(100,hasher(),key_equal()){}
		explicit unordered_set(size_type n) : rep(n, hasher(), key_equal()){}
		unordered_set(size_type n,const hasher& hf) : rep(n, hf, key_equal()) {}
		unordered_set(size_type n, const hasher& hf, const key_equal& keq) : rep(n, hf, keq) {}


		/*
		template<class InputIterator>
		unordered_set(InputIterator first, InputIterator last) : rep(100, hasher(), key_equal()) { rep.insert_unique(first, last,iterator_category(first)); }
		*/

	public:
		size_type size() const { return rep.size(); }
		size_type max_size() const { return rep.max_size(); }
		size_type bucket_count() const { return rep.bucket_count(); } //桶个数即为vector大小
		size_type max_bucket_count() const { return rep.max_bucket_count(); }
		bool empty() const { return rep.empty(); }
		void swap(unordered_set& us) { rep.swap(us); }

		friend bool operator== (const unordered_set&, const unordered_set&);

		iterator begin() const { return rep.begin(); }
		iterator end() const { return rep.end(); }

	public:
		pair<iterator, bool> insert(const value_type& val) {
			pair<typename ht::iterator, bool> p = rep.insert_unique(val);
			return pair<iterator, bool>(p.first, p.second);
		}

		pair<iterator, bool> insert_node(const value_type& val) {
			pair<typename ht::iterator, bool> p = rep.insert_unique_node(val);
			return pair<iterator, bool>(p.first, p.second);
		}

		iterator find(const key_type& key) const { return rep.find(key); }
		size_type count(const key_type& key) const { return rep.count(key); }

		void clear() { rep.clear(); }

		void resize(size_type hint) { rep.resize(hint); }

	};
	template<class Value, class HashFcn, class EqualKey, class Alloc>
	bool operator== (const unordered_set<Value, HashFcn, EqualKey, Alloc>& us1, const unordered_set<Value, HashFcn, EqualKey, Alloc>& us2) {
		return us1.rep == us2.rep;
	}





	template<class Value, class HashFcn = hash<Value>, class EqualKey = equal_to<Value>, class Alloc = alloc>
	class unordered_multiset {
	private:
		typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc>  ht;
		ht rep;
	public:
		typedef typename ht::value_type value_type;
		typedef typename ht::key_type key_type;
		typedef typename ht::hasher hasher;
		typedef typename ht::key_equal key_equal;

		typedef typename ht::size_type size_type;
		typedef typename ht::difference_type difference_type;
		typedef typename ht::const_reference reference;
		typedef typename ht::const_reference const_reference;
		typedef typename ht::const_pointer pointer;
		typedef typename ht::const_pointer const_pointer;


		typedef typename ht::const_iterator iterator;
		typedef typename ht::const_iterator const_iterator;

		hasher hash_funct() const { return rep.hash_funct(); }
		key_equal key_eq() const { return rep.key_eq(); }


	public:
		unordered_multiset() :rep(100, hasher(), key_equal()) {}
		explicit unordered_multiset(size_type n) : rep(n, hasher(), key_equal()) {}
		unordered_multiset(size_type n, const hasher& hf) : rep(n, hf, key_equal()) {}
		unordered_multiset(size_type n, const hasher& hf, const key_equal& keq) : rep(n, hf, keq) {}


		/*
		template<class InputIterator>
		unordered_set(InputIterator first, InputIterator last) : rep(100, hasher(), key_equal()) { rep.insert_unique(first, last,iterator_category(first)); }
		*/

	public:
		size_type size() const { return rep.size(); }
		size_type max_size() const { return rep.max_size(); }
		size_type bucket_count() const { return rep.bucket_count(); } //桶个数即为vector大小
		size_type max_bucket_count() const { return rep.max_bucket_count(); }
		bool empty() const { return rep.empty(); }
		void swap(unordered_multiset& us) { rep.swap(us); }

		friend bool operator== (const unordered_multiset&, const unordered_multiset&);

		iterator begin() const { return rep.begin(); }
		iterator end() const { return rep.end(); }

	public:
		iterator insert(const value_type& val) {
			return rep.insert_equal(val);
		}

		iterator insert_node(const value_type& val) {
			
			return rep.insert_equal_node(val);
		}

		iterator find(const key_type& key) const { return rep.find(key); }
		size_type count(const key_type& key) const { return rep.count(key); }

		void clear() { rep.clear(); }

		void resize(size_type hint) { rep.resize(hint); }

	};
	template<class Value, class HashFcn, class EqualKey, class Alloc>
	bool operator== (const unordered_multiset<Value, HashFcn, EqualKey, Alloc>& us1, const unordered_multiset<Value, HashFcn, EqualKey, Alloc>& us2) {
		return us1.rep == us2.rep;
	}







}


#endif // !_UNORDERED_SET_H_

