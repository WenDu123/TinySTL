#ifndef  _MYTINYSTL_LIST_
#define  _MYTINYSTL_LIST_

#include "Allocator.h"

#include "Iterator.h"

#include "Construct.h"

#include "Algorithm.h"

namespace MyTinySTL {


	//list的节点
	 
	template<class T>
	struct __list_node {
		__list_node<T>* prev;//
		__list_node<T>* next;
		T data;
		__list_node(__list_node* x){
			data = x->data;
			prev = x->prev;
			next = x->next;
		
		}

	};
	//list的迭代器，在list内部重定义，然后外部使用list时，可以用迭代器指向list节点
	template<class T, class Ref, class Ptr>
	struct __list_iterator {
		typedef	__list_iterator<T, T&, T*>	iterator;
		typedef __list_iterator<T, Ref, Ptr>	self;

		typedef ptrdiff_t	difference_type;

		typedef  bidirectional_iterator_tag		iterator_category;
		typedef T	value_type;
		typedef	T& reference;
		typedef T* pointer;
		typedef __list_node<T>* link_type;
		typedef size_t  size_type;
		


		link_type node;

		__list_iterator(link_type x) :node(x) {}
		__list_iterator() {}
		__list_iterator(const __list_iterator& x) : node(x.node) {}

		bool operator == (const self& x) const { return node == x.node; }
		bool operator != (const self& x) const { return !(node == x.node); }
		
		reference operator* () const { return (*node).data; }
		pointer operator->() { return &(operator*()); }

		self& operator++ () {
			node = (link_type)(node->next);
			return *this;
		}


		self operator++ (int) {
			self tmp = *this;
			++*this;//
			return tmp;
		}


		self& operator-- () {
			node = (*node).prev;
			return *this;
		}

		self operator-- (int) {
			self tmp = *this;
			--* this;
			return tmp;
		}


	};



	template<class T, class Alloc = alloc>
	class list {
	protected:
		typedef	__list_node<T>	list_node;
		typedef allocator<list_node, Alloc> list_node_allocate;//
	public:

		typedef T	value_type;
		typedef	value_type& reference;
		typedef	const value_type& const_reference;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;

		typedef size_t  size_type;
		typedef ptrdiff_t	difference_type;

		typedef __list_iterator<T, T&, T*>	iterator;
		typedef __list_iterator<T, const T&, const T*>	const_iterator;

	protected:
		typedef __list_node<T>* link_type;
		link_type node;
		
		link_type get_node() { return list_node_allocate::allocate(); }
		void put_node(link_type p) { list_node_allocate::deallocate(p); }

		link_type create_node(const value_type& val) {
			link_type p = get_node();
			construct(&p->data, val);//	
			return p;
		}

		void destroy_node(link_type p) {
			//p->next = p->prev = nullptr;
			destory(&p->data);
			put_node(p);
		}

		void empty_initialize() {
					//link_type node = get_node();//error
			node = get_node();
			node->next = node;
			node->prev = node;
		}

	public:
		list() { empty_initialize(); }
		list(size_type n, const T& x) {
			empty_initialize();
			while(n--)
			push_back(x);
		}
		list(const list& x){
			empty_initialize();
			insert(this->begin(), x.begin(), x.end());
		}

		void clear() {
			link_type cur = (link_type)(node->next);
			while (cur != node) {
				link_type  tmp = cur;
				destroy_node(tmp);
				cur++;
			}
			node->next = node;
			node->prev = node;
		}
		~list() {
			clear();
			destroy_node(node);
		}

		iterator begin() { return (link_type)((*node).next); }
		iterator end() { return node; }
		const_iterator begin() const { return (link_type)((*node).next); }
		const_iterator end() const { return node; }
		bool empty() { return node->next == node; }
		size_type size() { //可以改一下试试看
			size_type result = 0;
			iterator cur = begin();
			while (cur != end()) {
				cur++;
				result++;
			}
			return result;
		}
		reference front() { return *begin(); }
		reference back() { return *(--end()); }

		//在position之前插入
		iterator insert(iterator position, const T& x) {

			link_type tmp = create_node(x);
			tmp->next = position.node;
			tmp->prev = position.node->prev;		
			(link_type(position.node->prev))->next = tmp;
			position.node->prev = tmp;		
			return tmp;
		}

		void insert(iterator position, iterator first, iterator last) {
			for (; first != last; ++first) {
				insert(position, *first);
			}
		}

		void insert(iterator position, const_iterator first, const_iterator last) {

			for (;first != last; ++first) {
				insert(position, *first);
			}

		}



		void push_front(const T& x) {
			insert(begin(), x);
		}
		void push_back(const T& x) {
			insert(end(), x);
		} 

		iterator erase(iterator position) {
			link_type next_node = position.node->next;
			link_type prev_node = position.node->prev;

			
			prev_node->next = position.node->next;
			next_node->prev = position.node->prev;

			destroy_node(position.node);
			return iterator(next_node);
		}

		void pop_front() {
			iterator tmp = begin();
			erase(tmp);
		}

		void pop_bcak() {
			iterator tmp = end();
			--tmp;
			erase(tmp);
		}

		void remove(const T& val) {
			iterator first = begin();
			iterator last = end();
			while (first != last) {
				iterator next = first;
				next++;
				if (*first == val) erase(first);
				first = next;
			}
		}

		void unique() {
			iterator first = begin();
			iterator last = end();

			if (first == last) { return; }
			iterator next = first;
			while (++next != last) {
				if (*first == next) {
					erase(next);
				}
				else {
					first = next;
				}
				next = first;
			}
		}

		void transfer(iterator position, iterator first, iterator last) {
			if (position != last) {
				(last.node->prev)->next = position.node;
				(first.node->prev)->next = last.node;
				(position.node->prev)->next = first.node;
				iterator tmp = position.node->prev;
				position.node->prev = last.node->prev;
				last.node->prev = first.node->prev;
				first.node->prev = tmp.node;
			}
		}

		void splice(iterator position, list& x) {
			if (!x.empty()) {
				iterator first = x.begin();
				iterator last = x.end();
				transfer(position, first, last);
			}
		}

		void splice(iterator position, list& x, iterator i) {
			iterator j = i;
			++j;
			if (j == position || i == position) return;
			transfer(position, i, j);
		}

		void splice(iterator position, iterator first, iterator last) {
			if (first != last) {
				transfer(position, first, last);
			}
		}

		void merge(list& x) {
			iterator first1 = begin();
			iterator last1 = end();
			iterator first2 = x.begin();
			iterator last2 = x.end();
			while (first1 != last1 && first2 != last2) {
				if (*first1 > *first2) {
					iterator next = first2;
					++next;
					transfer(first1, first2, next);
					first2 = next;
				}
				else {
					++first1;
				}

			}
			if (first2 != last2) {
				transfer(last1,first2, last2);
			}

		}

		void reverse() {
			if (node->next == node || node->next->next == node) {
				return;
			}
			iterator first = begin();
			++first;
			while (first != end()) {
				iterator tmp = first;
				++first;
				transfer(begin(), tmp, first);
			}
		}

		void list_swap(list& x) {
			/*list tmp(x);
			x.clear();
			transfer(x.node, begin(), end());
			clear();
			transfer(node, tmp.begin(), tmp.end());*/
			swap(node, x.node);
		}

		void sort() {
			if(node->next == node || (node->next)->next == node){
				return;
			}
			list carry; //中介区，未排序
			list counter[64]; //结果区，已排序
			int fill = 0;

			while (!empty()) {
				carry.splice(carry.begin(), *this, begin());//移动一个元素到carry
				int i = 0;
				//1.结束一次while大循环后把i置0，保证counter[i].empty()
				while (i < fill && !counter[i].empty()) {
					counter[i].merge(carry);
					carry.list_swap(counter[i++]);
				}
				carry.list_swap(counter[i]);
				if (i == fill) ++fill;  //
			}
			for (int i = 1; i < fill; ++i) {
				counter[i].merge(counter[i-1]);
			}
			list_swap(counter[fill - 1]);



		}

		



	};


}



#endif
