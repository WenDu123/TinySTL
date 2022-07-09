#pragma once


#include"Allocator.h"
#include"Construct.h"
#include"Uninitialized.h"

namespace MyTinySTL {

	template<class T, class Alloc = alloc>
	class vector {
	public:
		
		typedef T						value_type;
		typedef value_type*				pointer;
		typedef value_type*				iterator;
		typedef const value_type*		const_iterator;
		typedef value_type&				reference;
		typedef const value_type&		const_reference;
		//typedef const value_type*		const_reference;
		//注意一个符号的错误，现在因为没用到所以没事，一旦很久之后用到，难以找出，要细心，也要会注释错误语句，层层剖析找错误
		typedef size_t					size_type;
		typedef ptrdiff_t				difference_type;
	protected:
		iterator start;
		iterator finish;
		iterator end_of_storage;

		typedef allocator<T,Alloc>date_allocator;
	public:

		vector() : start(0), finish(0), end_of_storage(0) { }
		explicit vector(size_type n) { fill_initialize(n, value_type()); };
		vector(size_type n, const value_type& value) { fill_initialize(n, value); };
		//vector(const vector& v){};
		vector& operator = (const vector& v);
		~vector(){
			destory(start, finish);
			deallocator();
		};

		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		
		iterator end() { return finish; }
		const_iterator end() const { return finish; }
		


		size_type size() const { return size_type(end() - begin()); }
		size_type  capacity() const { return size_type(end_of_storage - begin()); }
		bool empty() const { return start == finish; }
		void resize(size_type new_size, value_type val = 0){
			if (new_size < size()) {
				erase(begin() + new_size, end());
			}
			else {
				insert(end(), new_size - size(), val);
			}
		};
		void reserve(size_type n) {
			if (capacity() < n) {
				const size_type old_size = size();
				iterator tmp = allocator_and_copy(n, start, finish);
				destory(start, finish);
				start = tmp;
				finish = tmp + old_size;
				end_of_storage = start + n;
			}
		}

		reference operator[] (size_type n) { return *(begin() + n); }
		const_reference operator[](size_type n) const { return *(begin() + n); }
		reference front() { return *(begin()); }
		reference back() { return *(end() - 1); }

		void push_back(const value_type& x){
			if(finish != end_of_storage){
				construct(finish,x);
				++finish;
			}
			else{
				insert_aux(end(),x);
			}
		};
		void pop_back(){
			--finish;
			destory(finish);
		};
		
		iterator insert(iterator position, const value_type& val);
		void insert(iterator position, size_type n, const value_type& val) {
			if (n != 0) {
				if (size_type(end_of_storage - finish) >= n) {   //有备用空间
					T val_copy = val;
					const size_type elems_after = finish - position;
					iterator old_finish = finish;
					if (elems_after > n) {  //插入的元素少于原插入点后元素
						uninitialized_copy(finish - n, finish, finish);
						finish += n;
						copy_backward(position, old_finish - n, old_finish);
						fill(position, position + n, val_copy);
					}
					else {  //插入的元素多于原插入点后元素
						uninitialized_fill_n(finish, n - elems_after, val_copy);
						finish += n - elems_after;
						uninitialized_copy(position, old_finish, finish);
						finish += elems_after;
						fill(position, old_finish, val_copy);
					}
					
				}
				else {
					const size_type old_size = size();
					const size_type len = old_size + max(old_size, n);

					iterator new_start = date_allocator::allocate(len);
					iterator new_finish = new_start;
					try {
						new_finish = uninitialized_copy(start, position, new_start);
						new_finish = uninitialized_fill_n(new_finish, n, val);
						new_finish = uninitialized_copy(position,finish, new_finish);
					}
					catch (...) {
						destory(new_start, new_finish);
						deallocator();
						throw;
					}


					destory(start, finish);
					deallocator();

					start = new_start;
					finish = new_finish;
					end_of_storage = new_start + len;

				}

			}


		}
		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);

		iterator erase(iterator position){
			if(position + 1 != end()){
				iterator i = copy(position + 1,finish,position);
				--finish;
				destory(finish);
				return position;
			}else{
				pop_back();
			}
		};
		iterator erase(iterator first, iterator last){
			iterator i = copy(last,finish,first);
			destory(i,finish);
			finish = finish - (last - first);
			return first;

		};


		void clear();
		void swap(vector& v) {
			MyTinySTL::swap(start, v.start);
			MyTinySTL::swap(finish, v.finish);
			MyTinySTL::swap(end_of_storage, v.end_of_storage);
		}

		void insert_aux(iterator position, const T& val) {
			if (finish != end_of_storage) {
				construct(finish, *(finish - 1));//finish实际上指向的是vector最后一个元素的下一个
				++finish;
				T val_copy = val;
				copy_backward(position, finish - 2, finish - 1);
				*position = val_copy;
			}
			else {
				const size_type old_size = size();
				const size_type len = old_size != 0 ? 2 * old_size : 1;

				iterator new_start = date_allocator::allocate(len);
				iterator new_finish = new_start;
				try {
					new_finish = uninitialized_copy(start, position, new_start);
					construct(new_finish, val);
					++new_finish;
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch (...) {
					destory(new_start, new_finish);
					deallocator();
					throw;
				}


				destory(start, finish);
				deallocator();

				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}

		Alloc get_allocator() { 
			return date_allocator(); //要加(),生成临时对象才能返回
			}

	protected:
		/*
		void destroy_and_allocator();
		void allocator_and_fill(size_type n, const value_type& val);
		template<class InputIterator>
		void allocator_and_copy(InputIterator first, InputIterator last);
		*/

		void fill_initialize(size_type n, const value_type& val) {
			start = allocator_and_fill(n, val);
			finish = start + n;
			end_of_storage = finish;
		}

		iterator allocator_and_fill(size_type n, const value_type& val) {
			iterator result = date_allocator::allocate(n);
			uninitialized_fill_n(result, n, val);
			return result;

		}
		template<class InputIterator>
		iterator allocator_and_copy(size_type n, InputIterator first, InputIterator last) {
			iterator result = date_allocator::allocate(n);
			uninitialized_copy(first, last, result);

			return result;

		}

		iterator allocator_and_copy(size_type n, const_iterator first,const_iterator last) {
			iterator result = date_allocator::allocate(n);
			uninitialized_copy(first,last,result);

			return result;

		}

		void deallocator() {
			if (start) {
				date_allocator::deallocate(start, end_of_storage - start);
			}
		};

		
	};



}





