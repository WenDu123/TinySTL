#ifndef _ALLOC_
#define _ALLOC_

#include <cstdlib>

namespace MyTinySTL {

	class alloc {

	private:	//�������
		static const int ALIGN = 8; //�����ϵ��߽�
		static const int MAXBYTES = 128; //��������
		static const int NFFEELISTS = MAXBYTES / ALIGN; //free_list�ĸ���
		static const int NOBJS = 20;//ÿ�������и��������,δ���㹻20

	private: //free_list���
		struct obj{
			obj* next;
		};
		static obj* free_list[NFFEELISTS];  //�������������

	private:  //ս����pool���
		static char* start_free;
		static char* end_free;
		static size_t heap_size;

	private:
		static size_t ROUND_UP(size_t bytes) {   //�ϵ������С����֤Ϊ�˵ı���
			return ((bytes)+ALIGN - 1) & ~(ALIGN - 1);
		}
		static size_t NFFEELIST_INDEX(size_t bytes) {  //�����ҵ�bytes��������ȷ��λ��
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