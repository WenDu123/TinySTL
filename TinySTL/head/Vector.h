#ifndef _VECTOR_
#define _VECTOR_


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
		typedef const value_type*		const_reference;
		typedef size_t					size_type;
		typedef ptrdiff_t				difference_type;
	private:
		iterator start;
		iterator finish;
		iterator end_of_storage;

		typedef allocator<value_type, Alloc>date_allocator;
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
		const_iterator cbegin() const { return start; }
		iterator end() { return finish; }
		const_iterator end() const { return finish; }
		const_iterator cend() const { return finish; }


		size_type size() const { return size_type(end() - start()); }
		size_type  capacity() const { return size_type(end_of_storage - start()); }
		bool empty() const { return start == finish; }
		void resize(size_type new_size, value_type val = 0){
			if (new_size < size()) {
				erase(begin() + new_size, end());
			}
			else {
				insert(end(), new_size - size(), val);
			}
		};
		void reserve(size_type n);

		reference operator[] (const size_type n) { return *(begin() + n); }
		const_reference operator[](const size_type n) const { return *(cbegin() + n); }
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
		void insert(iterator position, size_type n, const value_type& val);
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
		void swap(vector& v);

		void insert_aux(iterator position, const T& val);

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
		void deallocator() {
			if (start) {
				date_allocator::deallocate(start, end_of_storage - start);
			}
		};

		
	};



}




#endif
