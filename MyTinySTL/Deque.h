#pragma once
#ifndef _DEQUE_H_
#define _DEQUE_H_

#include"Allocator.h"

#include"Uninitialized.h"



namespace MyTinySTL {


	inline size_t __deque_buffer_size(size_t n, size_t sz) {
		return n != 0 ? n : (sz < 512 ? 512 / sz : size_t(1));
	}
	struct random_access_iterator_tag;
	template<class T,class Ref,class Ptr, size_t BufSiz>
	struct __deque_iterator : public iterator<random_access_iterator_tag,T> {
		typedef __deque_iterator<T, T&, T*, BufSiz>	iterator;
		typedef __deque_iterator<T,const T&,const T*, BufSiz>	const_iterator;

		typedef random_access_iterator_tag	iterator_category;
		typedef T	        value_type;
		typedef Ptr		pointer;
		typedef Ref		    reference;
		typedef ptrdiff_t	difference_type;
		typedef T**			map_pointer;

		typedef __deque_iterator  self;

		T* first;//缓冲区第一个元素
		T* last;//缓冲区最后一个元素的下一个位子
		T* cur;
		map_pointer node;


		static size_t buffer_size() { return __deque_buffer_size(BufSiz, sizeof(T)); }

		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
			//difference_type();类型转换，临时对象
		}

		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }
		difference_type operator-(const self& x) const {
			return difference_type(buffer_size()) * (node - x.node - 1) + ((cur - first) + (x.last - x.cur));
			//两个缓冲区指针cur的距离
			//node - x.node - 1，两个缓冲区在中控器的距离，-1代表不包括相邻情况
			//cur - first表示cur在缓冲区内的距离

		}

		self operator++(){
			++cur;
			if (cur == last) {
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		self operator++(int){
			self tmp = *this;
			++*this;
			return tmp;
		
		}
		self operator--(){
			if (cur == first) {
				set_node(node - 1);
				cur = last;
				//为什么此处不是cur = last - 1;呢？因为if条件不成立仍然需要cur--
			}
			--cur;
			return *this;
		}
		self operator--(int){
			self tmp = *this;
			--*this;
			return tmp;	
		}


		self& operator+=(difference_type n) {
			difference_type new_distance = n + (cur - first); 
			if (new_distance < difference_type(buffer_size()) && new_distance >= 0) {
				cur = cur + n;
			}
			else {
				difference_type node_distance =
					new_distance > 0 ? new_distance / difference_type(buffer_size())
					: -difference_type(-new_distance / difference_type(buffer_size())) - 1; 
				/*
				new_distance = n + (cur - first)，代表是从缓冲区first开始算的
				如果new_distance > 0，代表new_distance > distance_type(buffer_size())
				已经走过本身的缓冲区，所以故而直接 node + 距离 即可；
				如果new_distance < 0,
				从first开始下一刻就跳到前一个缓冲区了，这时node-1
				因为在下一个缓冲区移动时，
				difference_type(-new_distance / difference_type(buffer_size())) < 1 = 0向下取整
				需要保证node先移动到下一个缓冲区
				这就是 -1 的由来；
				*/
				set_node(node + node_distance);
				cur = first + (new_distance - node_distance * difference_type(buffer_size()));
			}
			return *this;

		}

		self operator+(difference_type n){
			self tmp = *this;
			tmp += n;
			return tmp;
		}
		self& operator-=(difference_type n) { return *this += -n; }
		self operator-(difference_type n){
			self tmp = *this;
			tmp -= n;
			return tmp;
		}
		reference operator[](difference_type n) {
			return *(*this + n);
		}
		bool operator==(const self& x) const { return cur == x.cur; }
		bool operator!=(const self& x) const { return !(*this == x); }
		bool operator<(const self& x) const { return (node == x.node) ? (cur < x.cur) : (node < x.node); }
		bool operator>(const self& x) const { return !(*this < x); }
		
	};
	
	template<class T, class Ref, class Ptr, size_t BufSiz>  
	inline random_access_iterator_tag iterator_category(const __deque_iterator<T, Ref, Ptr, BufSiz>&) {
		return random_access_iterator_tag();
	}

	template<class T, class Ref, class Ptr, size_t BufSiz>
	inline T*
		value_type(const __deque_iterator<T, Ref, Ptr, BufSiz>&) {
		return 0;
	}

	template<class T, class Ref, class Ptr, size_t BufSiz>
	inline ptrdiff_t*
		distance_type(const __deque_iterator<T, Ref, Ptr, BufSiz>&) {
		return static_cast<ptrdiff_t*>(0);
	}





	template<class T,class Alloc = alloc,size_t BufSiz = 0>
	class deque {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef ptrdiff_t	difference_type;
		typedef size_t size_type;

		typedef const reference const_reference;

	public:
		typedef __deque_iterator<T, T&, T*, BufSiz>  iterator;

	protected:
		typedef pointer* map_pointer;
		static size_t buffer_size() { return __deque_buffer_size(BufSiz, sizeof(T)); }

		iterator start;
		iterator finish;

		map_pointer map; 
		//本身是一个指针，指向连续空间，空间里每一个元素都是指针，指向buffer，所以要用双重指针
		size_type map_size;

	protected:
		typedef  allocator<value_type, Alloc>	data_allocator;
		typedef  allocator<pointer, Alloc>	map_allocator;

	public://deque的构造和内存管理
		void create_map_and_nodes(size_type n) { //为map和缓冲区分配空间
			size_type num_nodes = n / buffer_size() + 1; //+1防止有余数
			
			map_size = max(initial_map_size(), num_nodes + 2);//+2,前后各多留一个，扩充可以用
			map = map_allocator::allocate(map_size);

			map_pointer nstart = map + (map_size - num_nodes)/2;
			//map_size - num_nodes 结果为map中空白节点的数量（头+尾），移动一半（/2）,就能到达所需节点第一个；
			map_pointer nfinish = nstart + num_nodes - 1;
			//num_nodes - 1,-1是start已经指向第一个了，移动之后到达所需节点最后一个
			map_pointer cur;

			try {
				for (cur = nstart; cur <= nfinish; ++cur) {
					*cur = allocate_node();
				}

			}
			catch(...) {
				//如果不是全部成功，则一个不留
				for (map_pointer p = nstart; p < cur; ++p)
					data_allocator::deallocate(*p);
				map_allocator::deallocate(map, map_size);
				throw;
			}
			start.set_node(nstart);
			finish.set_node(nfinish);
			start.cur = start.first;
			finish.cur = finish.first + n % buffer_size();
			//如果n / buffer_size()为整数，会多分配一个缓冲区
			//此时的finish.cur应该指向多分配的这个缓冲区的起点
		}
		void fill_initialized(size_type n, const value_type& val) { //设置元素初值
			create_map_and_nodes(n);
			map_pointer cur;
			try {
				for (cur = start.node; cur < finish.node; ++cur) {
					uninitialized_fill(*cur, *cur + buffer_size(), val);
				}
				uninitialized_fill(finish.first,finish.cur, val);
				//尾端可能有备用空间，无需赋初值

			}
			catch (...) {
				for (map_pointer p = start.node; p < cur; ++p) {
					destory(*p, *p + buffer_size());
				}
				//destory(start, finish);
				for (; start != finish; ++start) {
					destory(&start);
				}
			}
		}

		static size_type initial_map_size() { return 8; } //一个map至少管理八个节点
		pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
		void dealloacte_node(pointer p) {
			data_allocator::deallocate(p, buffer_size());
		}

		deque():start(), finish(), map(0), map_size(0) { create_map_and_nodes(0); }

		deque(int n, const value_type& val) :start(), finish(), map(0), map_size(0) {
			fill_initialized(n, val);
		}
		deque(const deque& d){
			create_map_and_nodes(d.size());
			uninitialized_copy(d.begin(), d.end(), start);
		}

		~deque() {
			for (map_pointer cur = start.node; cur <= finish.node; ++cur) {
				destory(*cur, *cur + buffer_size());
				dealloacte_node(*cur);
			}

		}

	public:	
		void reallocate_map(size_type nodes_to_add, bool add_at_front) {
			size_type old_nums_nodes = finish - start + 1;
			size_type new_nums_nodes = old_nums_nodes + nodes_to_add;


			map_pointer new_nstart;

			if (map_size > 2 * new_nums_nodes) { //有备用空间，但是有一端满了
				new_nstart = map + (map_size - new_nums_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
				if (new_nstart < start.node) {//尾端满了，节点空间前移，从前面开始复制，用copy
					copy(start.node, finish.node + 1, new_nstart);
				}
				else {//前端满了，节点空间后移,从后面开始复制，用copy_backward
					copy_backward(start.node, finish.node + 1, new_nstart + old_nums_nodes);
				}
			}
			else {
				size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;

				map_pointer new_map = map_allocator::allocate(new_map_size);
				new_nstart = map + (new_map_size - new_nums_nodes) / 2 + (add_at_front ? nodes_to_add : 0);

				copy(start.node, finish.node + 1, new_nstart);

				map_allocator::deallocate(map,map_size);

				map = new_map;
				map_size = new_map_size;
			}

			start.set_node(new_nstart);
			finish.set_node(new_nstart + old_nums_nodes - 1);
			//old_nums_nodes - 1,-1是start已经指向第一个了，移动之后到达所需节点最后一个

		}


		void reserve_map_back(size_type nodes_to_add = 1) {
			if (nodes_to_add + 1 > map_size - (finish.node - map)) {
				reallocate_map(nodes_to_add, false);
			}			
		}
		void reserve_map_front(size_type nodes_to_add = 1) {
			if (nodes_to_add > size_type(start.node - map)) {
				reallocate_map(nodes_to_add, true);
			}
		}





		void push_back_aux(const value_type& val) {
			value_type val_copy = val;

			reserve_map_back();

			*(finish.node + 1) = allocate_node();
			construct(finish.cur, val_copy);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
			
		}


		void push_back(const value_type& val) {
			if (finish.cur != finish.last - 1 ) {
				construct(finish.cur, val);
				++finish.cur;
			}
			else {
				push_back_aux(val);

			}
		}

		void push_front_aux(const value_type& val) {
			value_type val_copy = val;

			reserve_map_front();

			*(start.node - 1) = allocate_node();
			
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, val_copy);

		}


		void push_front(const value_type& val) {
			if (start.cur != start.first) {
				construct(start.cur - 1, val);
				--start.cur;
			}
			else {
				push_front_aux(val);

			}
		}

		void pop_front_aux() {
			destory(start.cur);
			dealloacte_node(start.first);
			start.set_node(start.node + 1);
			start.cur = start.first;

		}

		void pop_front() {
			if (start.cur != start.last - 1) {  //两个以上元素
				destory(start.cur);//start.cur指向当前第一个元素
				++start.cur;
			}
			else {  //只有一个元素
				pop_front_aux();

			}
		}

		void pop_back_aux() {	
			dealloacte_node(finish.first);
			finish.set_node(finish.node - 1);
			finish.cur = finish.last - 1;
			destory(finish.cur);

		}

		void pop_back() {
			if (finish.cur != finish.first) {  //一个以上元素
				//destory(finish.cur - 1);可行
				--finish.cur; //finish.cur指向最后一个元素下一个位置(空闲位子)
				destory(finish.cur);
			}
			else {//无元素
				pop_back_aux();

			}
		}

		void clear() { //需要保留一个缓冲区
			for (map_pointer cur = start.node + 1; cur < finish.node; ++cur) {
				destory(*cur, *cur + buffer_size());
				dealloacte_node(*cur);
			}

			if (start.node != finish.node) {
				destory(start.first, start.last);
				destory(finish.first, finish.cur);
				dealloacte_node(finish.first);
			}
			else {
				destory(start.cur, finish.cur);
			}
			finish = start;
		}

		iterator erase(iterator pos) {
			iterator pos_next = pos;
			++pos_next;

			difference_type  index = pos - start;  
			copy_backward(start, pos, pos_next);
			pop_front();
			/*
			if (size_type(index) < size() / 2) {
				copy_backward(start,pos,pos_next);
				pop_front();
			}
			else {
				//copy(pos_next,finish, pos);
				copy_backward(start, pos, pos_next);
				pop_back();
			}*/
			return start + index;
		}

		iterator insert_aux(iterator position, const value_type& val) {
			difference_type  index = position - start;
			value_type val_copy = val;
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			//position = start + index;
			copy_backward(position, back2, back1);

			*position = val_copy;
			return position;
		}
		iterator insert(iterator position, const value_type& val) {
			if (position.cur == start.cur) {
				push_front(val);
				return start;
			}
			else if(position.cur == finish.cur) {
				push_back(val);
				iterator tmp = finish;
				--tmp;
				return tmp;
			}
			else {
				return insert_aux(position, val);

			}



		}



	
	public:

		iterator begin() { return start; }
		iterator end() { return finish; }

		reference operator[](size_type n) {
			return start[difference_type(n)];
		}

		reference front() { return *start; }
		reference back() {
			iterator tmp = finish;
			--tmp;
			return *tmp;
		}
		size_type size() const { return finish - start; }
		size_type max_size() const { return size_type(-1); } //?
		/*
		因为size_t是unsigned的

		所以将-1强制转换为unsigned会得到这个类型的最大数

		也就是说....我的allocator你要allocate多少给多少呗...
		*/
		bool empty() const { return finish == start; }



	};




}







#endif
