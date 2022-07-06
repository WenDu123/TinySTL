#ifndef _ALLOCATOR_
#define _ALLOCATOR_

#include"alloc.h"

namespace MyTinySTL{

	template<class T,class Alloc>
	class allocator {
	
		static T* allocate(size_t n) {
			return 0 == n ? 0 : (T*)alloc::allocate(n * sizeof(T));
		}
		static T* allocate() {
			return (T*)alloc::allocate(sizeof(T));		
		}
		static void  deallocate(T* ptr, size_t n) {
			if (0 != n) {
				alloc::deallocate(ptr, n * sizeof(T));
			}
		}
		static void  deallocate(T* ptr) {
			alloc::deallocate(ptr, sizeof(T));
		}
};	

}



#endif