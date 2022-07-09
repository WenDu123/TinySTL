
#include "Alloc.h"


namespace MyTinySTL {


//静态成员赋初值
	char* alloc::start_free = 0;
	char* alloc::end_free = 0;
	size_t alloc::heap_size = 0;
	alloc::obj* alloc::free_list[NFFEELISTS] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};  //储存区块的链表


	void* alloc::allocate(size_t bytes) {
		obj** my_free_list;
		obj* result;
		if (bytes > MAXBYTES) {
			return malloc(bytes);
		}
		
		my_free_list = free_list + NFFEELIST_INDEX(bytes);
		result = *my_free_list;
		if (0 == result) {
			return refill(ROUND_UP(bytes));
		}
		else {
			*my_free_list = result->next;
			return result;
		}
	}
	void  alloc::deallocate(void* ptr, size_t bytes) {
		
		obj** my_free_list;
		obj* q = (obj*)ptr;

		if (bytes > MAXBYTES) {
			return free(ptr);
		}
		my_free_list = free_list + NFFEELIST_INDEX(bytes);
		q->next = *my_free_list;
		*my_free_list = q;

	}

	void* alloc::reallocate(void* ptr, size_t bytes) {
		deallocate(ptr,bytes);
		ptr = allocate(bytes);
		return ptr;
	}

	void* alloc::refill(size_t bytes) {
		size_t nobjs = NOBJS;
		char* chunk = chunk_malloc(bytes,nobjs);
		obj** my_free_list = 0;
		obj* result = 0;
		obj* current_obj = 0;
		obj* next_obj = 0;
		int i;

		if (1 == nobjs) {
			return(chunk);
		}
		my_free_list = free_list + NFFEELIST_INDEX(bytes);
		result = (obj*)chunk;
		*my_free_list = next_obj = (obj*)(chunk + bytes);
		for (i = 1;; ++i){
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + bytes);
			if (nobjs - 1 == i){
				current_obj->next = 0;
				break;
			}
			else {
				current_obj->next = next_obj;
			}
		}
		return result;

	}

	char* alloc::chunk_malloc(size_t size,size_t& nobjs) {
	
		char* result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else if( bytes_left >= size){
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else {
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 16);
			if (bytes_left > 0) {
				obj** my_free_list;
				my_free_list = free_list + NFFEELIST_INDEX(total_bytes);
				((obj*)start_free)->next = *my_free_list;
				*my_free_list = ((obj*)start_free);
			}

			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free) {
				obj** my_free_list;
				obj* ptr;
				for (int i = size + ALIGN;i <= MAXBYTES; i += ALIGN) {
					my_free_list = free_list + NFFEELIST_INDEX(i);
					ptr = *my_free_list;
					if (0 != my_free_list) {
						*my_free_list = ptr->next;
						start_free = (char*)ptr;
						end_free = start_free + i;
					}
				}
				end_free = 0;

			}

			heap_size += total_bytes;
			end_free = start_free + total_bytes;

			return chunk_malloc(size,nobjs);

		}	
	
	}

	/*
			static size_t ROUND_UP(size_t bytes) {   //上调申请大小，保证为八的倍数
			return ((bytes)+ALIGN - 1) & ~(ALIGN - 1);
		}
		static size_t NFFEELIST_INDEX(size_t bytes) {  //帮助找到bytes在链表正确的位置
			return (((bytes)+ALIGN - 1) / ALIGN - 1);

		}
	
	
	*/



//函数定义
}