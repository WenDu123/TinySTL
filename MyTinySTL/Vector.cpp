

namespace MyTinySTL{


    /*
    template<class T,class Alloc>
    void vector<T,Alloc>::insert_aux(iterator position,const T& val){
        if(finish != end_of_storage){
            construct(finish,*(finish - 1));//finish实际上指向的是vector最后一个元素的下一个
            ++finish;
            T val_copy = val;
            copy_backward(position,finish -2,finish - 1);
            *position = val_copy;
        }
        else{
            const size_type old_size = size();
            const size_type len = old_size != 0 ? 2*old_size : 1;

            iterator new_start = date_allocator::allocate(len);
            iterator new_finish = new_start;
            try{
            new_finish = uninitialized_copy(start,position,new_start);
            construct(new_finish,val);
            ++new_finish;
            new_finish = uninitialized_copy(position,finish,new_finish);
            }
            catch(...){
                destory(new_start,new_finish);
                deallocator();
                throw;
            }


            destory(start,finish);
            deallocator();

            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }

    }

    */


}