#ifndef _ALLOC_
#define _ALLOC_

#include <cstdlib>

namespace MyTinySTL {

	class alloc {

	private:	//区块相关
		static const int ALIGN = 8; //区块上调边界
		static const int MAXBYTES = 128; //区块上限
		static const int NFFEELISTS = MAXBYTES / ALIGN; //free_list的个数
		static const int NOBJS = 20;//每次申请切割的区块数,未必足够20

	private: //free_list相关
		struct obj{
			obj* next;
		};
		static obj* free_list[NFFEELISTS];  //储存区块的链表

	private:  //战备池pool相关
		static char* start_free;
		static char* end_free;
		static size_t heap_size;

	private:
		static size_t ROUND_UP(size_t bytes) {   //上调申请大小，保证为八的倍数
			return ((bytes)+ALIGN - 1) & ~(ALIGN - 1);
		}
		static size_t NFFEELIST_INDEX(size_t bytes) {  //帮助找到bytes在链表正确的位置
			return (((bytes)+ALIGN - 1) / ALIGN - 1);

		}

		static void* refill(size_t bytes );

		static char* chunk_malloc(size_t size, size_t& nobjs);
		
	public:
		static void* allocate(size_t bytes);
		static void  deallocate(void* ptr, size_t bytes);
		static void* reallocate(void* ptr, size_t bytes);


	};















}

#endif