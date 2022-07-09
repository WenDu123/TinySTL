#pragma once
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_
#include"Vector.h"
#include "functional.h"
#include "Hash_fun.h"

namespace MyTinySTL {
	template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	class hashtable;

	template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	struct __hashtable_iterator;

	template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	struct __hashtable_const_iterator;

	template<class Value>
	struct __hashtable_node {
		__hashtable_node* next;
		Value val;
	};

	//Value 节点实值型别,Key 节点键值型别,HashFcn hash function型别,
	//ExtractKey 从节点取出键值的方法（仿函数）,EqualKey 判断键值相同与否的方法（仿函数）,Alloc 空间配置器
	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey,class Alloc>
	struct __hashtable_iterator {
		typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>  hashtable;
		typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
		typedef __hashtable_const_iterator<Value,Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
		typedef __hashtable_node<Value> node;

		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef Value& reference;
		typedef Value* pointer;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		node* cur;//迭代器目前所指向的节点
		hashtable* ht;

		__hashtable_iterator(){}
		__hashtable_iterator(node* n,hashtable* htb):cur(n),ht(htb){}

		reference operator*() const { return cur->val; }
		pointer operator->() const { return &(operator*()); }

		iterator& operator++() {  //++i
			const node* old = cur;
			cur = cur->next;
			if (!cur) {
				size_type bucket = ht->bkt_num(old->val);
				while (!cur && ++bucket < ht->buckets.size()) {
					cur = ht->buckets[bucket];
				}
			}
			return *this;
		}

		iterator operator++(int) {
			iterator tmp = *this;
			++*this;
			return tmp;
		}

		bool operator==(const iterator& iter) const { return cur == iter.cur; }
		bool operator!=(const iterator& iter) const { return cur != iter.cur; }

	};

	template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	struct __hashtable_const_iterator {
		typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>  hashtable;
		typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
		typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
		typedef __hashtable_node<Value> node;

		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef const Value& reference;
		typedef const Value* pointer;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		const node* cur;//迭代器目前所指向的节点
		const hashtable* ht;

		__hashtable_const_iterator() {}
		__hashtable_const_iterator(const node* n, const hashtable* htb) : cur(n), ht(htb) {}
		__hashtable_const_iterator(const iterator& iter) : cur(iter.cur), ht(iter.ht) {}
					
		reference operator*() const { return cur->val; }
		pointer operator->() const { return &(operator*()); }

		const_iterator& operator++() {  //++i
			const node* old = cur;
			cur = cur->next;
			if (!cur) {
				size_type bucket = ht->bkt_num(old->val);
				while (!cur && ++bucket < ht->buckets.size()) {
					cur = ht->buckets[bucket];
				}
			}
			return *this;
		}

		const_iterator operator++(int) {
			const_iterator tmp = *this;
			++*this;
			return tmp;
		}

		bool operator==(const const_iterator& iter) const { return cur == iter.cur; }
		bool operator!=(const const_iterator& iter) const { return cur != iter.cur; }

	};

	static const int __stl_num_primes = 28;
	static const unsigned long __stl_primes_list[__stl_num_primes] = {
		53ul,         97ul,         193ul,       389ul,       769ul,
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 4294967291ul
	};
	inline unsigned long __stl_next_primes(unsigned long n) {
		unsigned long first = 0;
		unsigned long last = __stl_num_primes;
		if (n <= __stl_primes_list[first]) {
			return __stl_primes_list[first];
		}
		++first;
		for (; first != last; ++first) {
			if (n > __stl_primes_list[first-1] && n <= __stl_primes_list[first]) {
				return __stl_primes_list[first];
			}
		}
		return __stl_primes_list[last - 1];
	}


	//Value 节点实值型别,Key 节点键值型别,HashFcn hash function型别,
	//ExtractKey 从节点取出键值的方法（仿函数）,EqualKey 判断键值相同与否的方法（仿函数）,Alloc 空间配置器
	template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	class hashtable {
	public:
		typedef Value value_type;
		typedef Key key_type;
		typedef HashFcn hasher;
		typedef EqualKey key_equal;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;



		typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
		typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;

		friend struct __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
		friend struct __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;

		hasher hash_funct() const { return hash; }
		key_equal key_eq() const { return equals; }

	private:
		hasher hash; //HashFcn  hash function型别,
		key_equal equals;//判断键值相同与否的方法（仿函数）
		ExtractKey get_key;//ExtractKey 从节点取出键值的方法（仿函数）


		typedef __hashtable_node<Value> node;
		typedef allocator<node, Alloc> node_alloctor;
		vector<node*, Alloc> buckets;
		size_type num_elements;
	
	protected://内存管理
		node* new_node(const value_type& val) {
			node* n = node_alloctor::allocate(1);
			n->next = 0;
			construct(&n->val, val);
			return n;
		}

		void delete_node(node* n) {
			destory(&n->val);
			node_alloctor::deallocate(n, 1);
		}
		size_type next_node(size_type n) const {
			return __stl_next_primes(n);
		}

		void initialize_buckets(size_type n) {
			const size_type num_buckets = next_node(n);
			buckets.reserve(num_buckets);
			//此时的vector是空的，尾端就是开始
			buckets.insert(buckets.end(), num_buckets, (node*)0);
			num_elements = 0;
		}


	public:
		hashtable(size_type n, const HashFcn& hf, const EqualKey& eql)
			:hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
			initialize_buckets(n);
		}
		~hashtable() {
			clear();
			//destory(&buckets);
			//此处不用你调用vector析构函数，类的生命结束的最后，会自动调用vector析构函数
			//如果调用了，则会调用两次，第一次是你调用的成功析构vector，第二次自动调用传进去空指针个，程序崩溃
		}
		
	
	public:

		size_type bkt_num_Key(const key_type& key, size_type n) const {
			return hash(key) % n;
		}

		size_type bkt_num_key(const key_type& key) const {
			return bkt_num_Key(key, buckets.size());
		}

		size_type bkt_num(const Value& obj) const {
			return bkt_num_key(get_key(obj));
		}
		size_type bkt_num(const Value& obj, size_type n) const {
			return bkt_num_Key(get_key(obj), buckets.size());
		}

		size_type bucket_count() const { return buckets.size(); } //桶个数即为vector大小
		size_type max_bucket_count() const { return __stl_primes_list[__stl_num_primes - 1]; }
		size_type size() const { return num_elements; }
		size_type max_size() const { return size_type(-1); }
		bool empty() const { return size() == 0; }

		void swap(hashtable& hs) {
			MyTinySTL::swap(hash, hs.hash);
			MyTinySTL::swap(equals, hs.equals);
			MyTinySTL::swap(get_key, hs.gey_key);
			buckets.swap(hs.buckets);
			MyTinySTL::swap(num_elements, hs.num_elements);
		}

		iterator begin() {
			for (size_type n = 0; n < buckets.size(); ++n) {
				if (buckets[n]) {
					return iterator(buckets[n], this);
				}
			}
			return end();
		}
		iterator end() {
			return iterator(0, this);
		}

		//此处函数名的const不能少，否则不能重载（只有返回值不同不能重载）
		const_iterator end() const {
			return const_iterator(0, this);
		}

		const_iterator begin() const  {
			for (size_type n = 0; n < buckets.size(); ++n) {
				if (buckets[n]) {
					return const_iterator(buckets[n], this);
				}
			}
			return end();
		}


		



		void resize(size_type num_elements_hint) { //判断是否需要重建表格，需要就重建，不需要就返回
			const size_type old_n = buckets.size();
			if (num_elements_hint > old_n) {  //总元数个数大于buckets vector个数，则需要重建
				const size_type n = next_node(num_elements_hint);
				if (n > old_n) {
					vector<node*, Alloc>tmp(n, (node*)0);
					for (size_type bucket = 0; bucket < old_n; ++bucket) {
						node* first = buckets[bucket];
						while (first) {
							size_type new_bucket = bkt_num(first->val, n);
							buckets[bucket] = first->next;
							first->next = tmp[new_bucket];
							tmp[new_bucket] = first;
							first = buckets[bucket];
						}
					}
					buckets.swap(tmp);
				}

			}

		 }

		pair<iterator, bool> insert_unique(const value_type& val) {

			resize(num_elements + 1);
			return insert_unique_node(val);
		}

		pair<iterator, bool> insert_unique_node(const value_type& val) {
			const size_type n = bkt_num(val);
			node* first = buckets[n];
			for (node* cur = first; cur; cur=cur->next) {
				if (equals(get_key(cur->val), get_key(val))) {
					return pair<iterator, bool>(iterator(cur,this), false);
				}
			}
			node* tmp = new_node(val);
			tmp->next = first;
			buckets[n] = tmp;
			++num_elements;
			return pair<iterator, bool>(iterator(tmp, this), true);
		}
		/*
		template<class FowardIterator>
		void insert_unique(FowardIterator first, FowardIterator last, forward_iterator_tag) {
			size_type n = 0;
			distance(first, last, n);
		}

		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last) {
			insert_unique(first, last, iterator_category(first));
		}*/

		iterator insert_equal(const value_type& val) {

			resize(num_elements + 1);
			return insert_equal_node(val);
		}

		iterator insert_equal_node(const value_type& val) {
			const size_type n = bkt_num(val);
			node* first = buckets[n];
			for (node* cur = first; cur; cur = cur->next) {
				if (equals(get_key(cur->val), get_key(val))) {
					node* tmp = new_node(val);
					tmp->next = cur->next;
					cur->next = tmp;
					++num_elements;
					return iterator(tmp, this);
				}
			}
			node* tmp = new_node(val);
			tmp->next = first;
			buckets[n] = tmp;
			++num_elements;
			return iterator(tmp, this);
		}

		void clear() {
			for (size_type i = 0; i < buckets.size(); ++i) {
				node* cur = buckets[i];
				while (cur != 0) {
					node* next = cur->next;
					delete_node(cur);
					cur = next;
				}
				buckets[i] = 0;
			}
			num_elements = 0;
		}//vector仍然未释放，只释放了list node

		void copy_from(const hashtable& ht) {
			buckets.clear();
			buckets.reserve(ht.buckets.size());
			buckets.insert(buckets.end(), buckets.size(), (node*)0);
			for (size_type i = 0; i < buckets.size(); ++i) {
				if (const node* cur = ht.buckets[i]) {
					node* copy = new_node(cur->val);
					buckets[i] = copy;

					for (node* next = cur->next; next; cur = next, next = cur->next) {
						copy->next = next;
						copy = copy->next;
					}
				}
				
			}
			num_elements = ht.num_elements;
		 }

		iterator find(const key_type& key) {  //搜寻键值为key的元素
			size_type n = bkt_num_key(key);
			node* first;
			for(first = buckets[n]; first&&!equals(get_key(first->val),key);first=first->next){}
			return iterator(first, this);
		}
		size_type count(const key_type& key) { //计算键值为key的个数
			size_type n = bkt_num_key(key);
			size_type result = 0;
			node* first;
			for (first = buckets[n]; first ; first = first->next) {
				if(equals(get_key(first->val), key))
					++result;
			}
			return result;
		}

		reference find_or_insert(const value_type& val) {
			resize(num_elements + 1);

			size_type n = bkt_num(val);
			node* first = buckets[n];

			for (node* cur = first; cur; cur = cur->next) {
				if (equals(get_key(cur->val), get_key(val))) {
					return cur->val;
				}
			}

			node* tmp = new_node(val);
			tmp->next = first;
			buckets[n] = tmp;
			++num_elements;
			return tmp->val;
		}
		
		};
		
	}










#endif // _HASHTABLE_H_
