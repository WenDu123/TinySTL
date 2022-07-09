#pragma once
#ifndef _UNORDERED_MAP_H_
#define _UNORDERED_MAP_H_
#include"HashTable.h"

namespace MyTinySTL {

	template<class Key,class T,class HashFcn = hash<Key>, class EqualKey = equal_to<Key>, class Alloc = alloc>
	class unordered_map {
	private:
		typedef hashtable<pair<const Key,T>, Key, HashFcn,select1st<pair<const Key, T>>, EqualKey, Alloc>  ht;
		ht rep;
	public:
		typedef T data_type;
		typedef T mapped_type;
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


		typedef typename ht::iterator iterator;
		typedef typename ht::const_iterator const_iterator;

		hasher hash_funct() const { return rep.hash_funct(); }
		key_equal key_eq() const { return rep.key_eq(); }


	public:
		unordered_map() :rep(100, hasher(), key_equal()) {}
		explicit unordered_map(size_type n) : rep(n, hasher(), key_equal()) {}
		unordered_map(size_type n, const hasher& hf) : rep(n, hf, key_equal()) {}
		unordered_map(size_type n, const hasher& hf, const key_equal& keq) : rep(n, hf, keq) {}


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
		void swap(unordered_map& um) { rep.swap(um); }

		friend bool operator== (const unordered_map&, const unordered_map&);

		iterator begin() { return rep.begin(); }
		iterator end()  { return rep.end(); }

		const_iterator begin() const  { return rep.begin(); }
		const_iterator end() const { return rep.end(); }

	public:
		pair<iterator, bool> insert(const value_type& val) {
			return rep.insert_unique(val);
		}

		pair<iterator, bool> insert_node(const value_type& val) {
			
			return rep.insert_unique_node(val);
		}

		iterator find(const key_type& key){ return rep.find(key); }
		const_iterator find(const key_type& key) const { return rep.find(key); }
		size_type count(const key_type& key) const { return rep.count(key); }

		T& operator[](const key_type& key) {
			return rep.find_or_insert(value_type(key, T())).second;
		}

		void clear() { rep.clear(); }

		void resize(size_type hint) { rep.resize(hint); }

	};  
	template<class Key,class T,class HashFcn, class EqualKey, class Alloc>
	bool operator== (const unordered_map <Key,T, HashFcn, EqualKey, Alloc>& um1, const unordered_map<Key, T, HashFcn, EqualKey, Alloc>& um2) {
		return um1.rep == um2.rep;
	}




	template<class Key, class T, class HashFcn = hash<Key>, class EqualKey = equal_to<Key>, class Alloc = alloc>
	class unordered_multimap {
	private:
		typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st<pair<const Key, T>>, EqualKey, Alloc>  ht;
		ht rep;
	public:
		typedef T data_type;
		typedef T mapped_type;
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


		typedef typename ht::iterator iterator;
		typedef typename ht::const_iterator const_iterator;

		hasher hash_funct() const { return rep.hash_funct(); }
		key_equal key_eq() const { return rep.key_eq(); }


	public:
		unordered_multimap() :rep(100, hasher(), key_equal()) {}
		explicit unordered_multimap(size_type n) : rep(n, hasher(), key_equal()) {}
		unordered_multimap(size_type n, const hasher& hf) : rep(n, hf, key_equal()) {}
		unordered_multimap(size_type n, const hasher& hf, const key_equal& keq) : rep(n, hf, keq) {}


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
		void swap(unordered_multimap& um) { rep.swap(um); }

		friend bool operator== (const unordered_multimap&, const unordered_multimap&);

		iterator begin() { return rep.begin(); }
		iterator end() { return rep.end(); }

		const_iterator begin() const { return rep.begin(); }
		const_iterator end() const { return rep.end(); }

	public:
		iterator insert(const value_type& val) {
			return rep.insert_equal(val);
		}

		iterator insert_node(const value_type& val) {

			return rep.insert_equal_node(val);
		}

		iterator find(const key_type& key) { return rep.find(key); }
		const_iterator find(const key_type& key) const { return rep.find(key); }
		size_type count(const key_type& key) const { return rep.count(key); }

		T& operator[](const key_type& key) {
			return rep.find_or_insert(value_type(key, T())).second;
		}

		void clear() { rep.clear(); }

		void resize(size_type hint) { rep.resize(hint); }

	};
	template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
	bool operator== (const unordered_map <Key, T, HashFcn, EqualKey, Alloc>& um1, const unordered_map<Key, T, HashFcn, EqualKey, Alloc>& um2) {
		return um1.rep == um2.rep;
	}

}


#endif // !_UNORDERED_MAP_H_

