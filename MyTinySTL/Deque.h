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

		T* first;//��������һ��Ԫ��
		T* last;//���������һ��Ԫ�ص���һ��λ��
		T* cur;
		map_pointer node;


		static size_t buffer_size() { return __deque_buffer_size(BufSiz, sizeof(T)); }

		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
			//difference_type();����ת������ʱ����
		}

		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }
		difference_type operator-(const self& x) const {
			return difference_type(buffer_size()) * (node - x.node - 1) + ((cur - first) + (x.last - x.cur));
			//����������ָ��cur�ľ���
			//node - x.node - 1���������������п����ľ��룬-1���������������
			//cur - first��ʾcur�ڻ������ڵľ���

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
				//Ϊʲô�˴�����cur = last - 1;�أ���Ϊif������������Ȼ��Ҫcur--
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
				new_distance = n + (cur - first)�������Ǵӻ�����first��ʼ���
				���new_distance > 0������new_distance > distance_type(buffer_size())
				�Ѿ��߹�����Ļ����������Թʶ�ֱ�� node + ���� ���ɣ�
				���new_distance < 0,
				��first��ʼ��һ�̾�����ǰһ���������ˣ���ʱnode-1
				��Ϊ����һ���������ƶ�ʱ��
				difference_type(-new_distance / difference_type(buffer_size())) < 1 = 0����ȡ��
				��Ҫ��֤node���ƶ�����һ��������
				����� -1 ��������
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
		//������һ��ָ�룬ָ�������ռ䣬�ռ���ÿһ��Ԫ�ض���ָ�룬ָ��buffer������Ҫ��˫��ָ��
		size_type map_size;

	protected:
		typedef  allocator<value_type, Alloc>	data_allocator;
		typedef  allocator<pointer, Alloc>	map_allocator;

	public://deque�Ĺ�����ڴ����
		void create_map_and_nodes(size_type n) { //Ϊmap�ͻ���������ռ�
			size_type num_nodes = n / buffer_size() + 1; //+1��ֹ������
			
			map_size = max(initial_map_size(), num_nodes + 2);//+2,ǰ�������һ�������������
			map = map_allocator::allocate(map_size);

			map_pointer nstart = map + (map_size - num_nodes)/2;
			//map_size - num_nodes ���Ϊmap�пհ׽ڵ��������ͷ+β�����ƶ�һ�루/2��,���ܵ�������ڵ��һ����
			map_pointer nfinish = nstart + num_nodes - 1;
			//num_nodes - 1,-1��start�Ѿ�ָ���һ���ˣ��ƶ�֮�󵽴�����ڵ����һ��
			map_pointer cur;

			try {
				for (cur = nstart; cur <= nfinish; ++cur) {
					*cur = allocate_node();
				}

			}
			catch(...) {
				//�������ȫ���ɹ�����һ������
				for (map_pointer p = nstart; p < cur; ++p)
					data_allocator::deallocate(*p);
				map_allocator::deallocate(map, map_size);
				throw;
			}
			start.set_node(nstart);
			finish.set_node(nfinish);
			start.cur = start.first;
			finish.cur = finish.first + n % buffer_size();
			//���n / buffer_size()Ϊ������������һ��������
			//��ʱ��finish.curӦ��ָ���������������������
		}
		void fill_initialized(size_type n, const value_type& val) { //����Ԫ�س�ֵ
			create_map_and_nodes(n);
			map_pointer cur;
			try {
				for (cur = start.node; cur < finish.node; ++cur) {
					uninitialized_fill(*cur, *cur + buffer_size(), val);
				}
				uninitialized_fill(finish.first,finish.cur, val);
				//β�˿����б��ÿռ䣬���踳��ֵ

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

		static size_type initial_map_size() { return 8; } //һ��map���ٹ���˸��ڵ�
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

			if (map_size > 2 * new_nums_nodes) { //�б��ÿռ䣬������һ������
				new_nstart = map + (map_size - new_nums_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
				if (new_nstart < start.node) {//β�����ˣ��ڵ�ռ�ǰ�ƣ���ǰ�濪ʼ���ƣ���copy
					copy(start.node, finish.node + 1, new_nstart);
				}
				else {//ǰ�����ˣ��ڵ�ռ����,�Ӻ��濪ʼ���ƣ���copy_backward
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
			//old_nums_nodes - 1,-1��start�Ѿ�ָ���һ���ˣ��ƶ�֮�󵽴�����ڵ����һ��

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
			if (start.cur != start.last - 1) {  //��������Ԫ��
				destory(start.cur);//start.curָ��ǰ��һ��Ԫ��
				++start.cur;
			}
			else {  //ֻ��һ��Ԫ��
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
			if (finish.cur != finish.first) {  //һ������Ԫ��
				//destory(finish.cur - 1);����
				--finish.cur; //finish.curָ�����һ��Ԫ����һ��λ��(����λ��)
				destory(finish.cur);
			}
			else {//��Ԫ��
				pop_back_aux();

			}
		}

		void clear() { //��Ҫ����һ��������
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
		��Ϊsize_t��unsigned��

		���Խ�-1ǿ��ת��Ϊunsigned��õ�������͵������

		Ҳ����˵....�ҵ�allocator��Ҫallocate���ٸ�������...
		*/
		bool empty() const { return finish == start; }



	};




}







#endif
