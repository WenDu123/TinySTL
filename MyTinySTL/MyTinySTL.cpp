// MyTinySTL.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//#include "RBtree.h"
//#include "functional.h"
//#include"Vector.h"
#include <iostream>
#include "Unordered_map.h"
#include<cstring>

using namespace MyTinySTL;
int main()
{
    
   
    
    struct eqstr {
        bool operator()(const char* s1, const char* s2) const {

            return strcmp(s1, s2) == 0;
        }
    };
    unordered_map<const char*,int,hash<const char*>, eqstr> name_age;

    std::cout << "size = " << name_age.size() << std::endl;
    std::cout << "buckets size = " << name_age.bucket_count() << std::endl;
    name_age["LCW"] = 21;
    name_age["PHY"] = 21;
    name_age["DD"] = 1;
    name_age["ZZ"] = 0;

    unordered_map<const char*,int,hash<const char*>, eqstr>::iterator iter1 = name_age.begin();
    unordered_map<const char*,int,hash<const char*>, eqstr>::iterator iter2 = name_age.end();
    for (; iter1 != iter2; ++iter1) {
        std::cout << iter1->first << std::endl;

    }

    std::cout<< "LCW:" << name_age["LCW"] << std::endl;
    std::cout << "PHY:" << name_age["PHY"] << std::endl;

    


    /*
    hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc> hs(53, hash<int>(), equal_to<int>());

    std::cout << "size = " << hs.size() << std::endl;
    std::cout << "buckets size = " << hs.bucket_count() << std::endl;
    hs.insert_unique(10);
    hs.insert_unique(4);
    hs.insert_unique(33);
    hs.insert_unique(34);
    hs.insert_unique(13);
    hs.insert_unique(17);
    hs.insert_unique(990);
    hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc>::iterator iter = hs.begin();
    for (int i = 0; i < hs.size(); ++i, iter++) {
        std::cout << *iter << std::endl;

    }
    std::cout << std::endl;
    hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc>::iterator iter1 = hs.find(4);
    std::cout << *iter1 << std::endl;

    std::cout << std::endl;
    hs.insert_equal(4);
    std::cout << hs.count(4) << std::endl;

    */

    /*
    vector<int>v(50);
    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    v.reserve(100);
    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    */
    /*
    multimap<int,int>s;
    s[0] = 7;
    s[1] = 9;
    s[2] = 3;
    s[3] = 5;
    s[4] = 0;
    pair<int, int>a(4,1);
    s.insert(a);


    std::cout << "size = " << s.size() << std::endl;
    multimap<int,int>::iterator iter1 = s.begin();
    multimap<int,int>::iterator iter2 = s.end();

    for (; iter1 != iter2; ++iter1) {
        std::cout << "s" << "[" << iter1->first << "]" 
            << iter1->second
            << std::endl;
    }

   
   */




    /*
    
    rb_tree<int, int, identity<int>, less<int>> itree;
    std::cout << "RBtree size = " << itree.size() << std::endl;

    itree.insert_unique(10);
    itree.insert_unique(7);
    itree.insert_unique(8);

    itree.insert_unique(15);
    itree.insert_unique(5);
    itree.insert_unique(6);

    itree.insert_unique(11);

    itree.insert_unique(13);
    itree.insert_unique(12);

    std::cout << "RBtree size = " << itree.size() << std::endl;
    rb_tree<int, int, identity<int>, less<int>>::iterator iter1 = itree.begin();
    rb_tree<int, int, identity<int>, less<int>>::iterator iter2 = itree.end();
    __rb_tree_iterator_base iter6;
    for(;iter1 != iter2; ++iter1){
        iter6 = __rb_tree_iterator_base(iter1);
        std::cout <<*iter1<<"("<< iter6.node->color << ")" << std::endl;
    }

    rb_tree<int, int, identity<int>, less<int>>::iterator iter8 = itree.begin();
    iter8 = itree.find(8);
    std::cout << *itree.find(8) << "(" << iter8.node->color << ")" << std::endl;
    itree.erase(iter8);
    //itree.clear();

    std::cout << "RBtree size = " << itree.size() << std::endl;
    rb_tree<int, int, identity<int>, less<int>>::iterator iter3 = itree.begin();
    rb_tree<int, int, identity<int>, less<int>>::iterator iter4 = itree.end();
    __rb_tree_iterator_base iter5;
    for (; iter3 != iter4; ++iter3) {
        iter5 = __rb_tree_iterator_base(iter3);
        std::cout << *iter3 << "(" << iter3.node->color << ")" << std::endl;
    }

    */


    /*
    queue<int>s;
    s.push(1);
    s.push(2);
    s.push(3);
    std::cout << s.front() << std::endl;
    std::cout << std::endl;
    s.pop();
    std::cout << s.front() << std::endl;
    std::cout << std::endl;
    int i = s.size();
    std::cout << "size:" << i << std::endl;
    std::cout << std::endl;
    std::cout << s.back() << std::endl;
    std::cout << std::endl;
    */
    /*
    //d.clear();
   // std::cout << d[0] << std::endl;
    
    //std::cout << *iter << std::endl;
    //std::cout << *(iter+1) << std::endl;
    d.push_front(1);
    d.push_front(2);
    d.push_front(3);
    d.push_front(4);
    d.push_front(5);
    d.push_back(11);
    d.push_back(12);
    deque<int>::iterator iter = d.begin();
    for (; iter != d.end(); iter++) {
      std::cout << *iter << std::endl;
    
    }

    deque<int>::iterator iter1 = d.begin() + 1;
    d.erase(iter1);
    std::cout << std::endl;

    deque<int>::iterator iter2 = d.begin();
    for (; iter2 != d.end(); iter2++) {
        std::cout << *iter2 << std::endl;

    }

    deque<int>::iterator iter3 = d.begin() + 3;
    d.erase(iter3);
    std::cout << std::endl;

    deque<int>::iterator iter4 = d.end() - 2;
    d.erase(iter4);
    std::cout << std::endl;



    deque<int>::iterator iter5 = d.begin();
    for (; iter5 != d.end(); iter5++) {
        std::cout << *iter5 << std::endl;

    }

    deque<int>::iterator iter6 = d.begin() + 2;
    d.insert(iter6, 0);
    std::cout << std::endl;


    deque<int>::iterator iter7 = d.begin();
    for (; iter7 != d.end(); iter7++) {
        std::cout << *iter7 << std::endl;

    }

    */
    //deque<int>::iterator iter1 = d.begin()+2;
   // d.erase(iter);


    /*
    std::cout << "Hello World!\n";
    MyTinySTL::vector<int>v(10,5);
    std::cout << *v.begin()<<std::endl;
    std::cout << "vector容量：" << v.capacity() << std::endl;
    std::cout << "vector元素个数：" << v.size() << std::endl;
    v.push_back(8);
    std::cout << "vector容量：" << v.capacity() << std::endl;
    std::cout << "vector元素个数：" << v.size() << std::endl;
    v.pop_back();
    std::cout << "vector容量：" << v.capacity() << std::endl;
    std::cout << "vector元素个数：" << v.size() << std::endl;
    v.push_back(6);
    v.push_back(7);
    v.push_back(8);
    std::cout << "vector容量：" << v.capacity() << std::endl;
    std::cout << "vector元素个数：" << v.size() << std::endl;
    for (MyTinySTL::vector<int>::iterator iter = v.begin(); iter != v.cend();iter++) {
        std::cout << *iter<<std::endl; 
    }
    
    MyTinySTL::list<int>l(10,7);
    MyTinySTL::list<int>x(10,3);
    //l.insert(l.end(),5);
    l.push_back(5);
    l.pop_bcak();
    l.push_front(10);
    l.pop_front();
    std::cout << "list size：" << l.size() << std::endl;
    MyTinySTL::list<int>::iterator iter0 = l.begin();
    iter0++;
    l.splice(iter0, x);
    for (MyTinySTL::list<int>::iterator iter = l.begin(); iter != l.end(); iter++) {
        std::cout << *iter << std::endl;
    }
    l.push_front(10);
    l.push_front(9);
    l.push_back(1);
    l.push_back(0);
    std::cout << "list size：" << l.size() << std::endl;
    l.sort();
    for (MyTinySTL::list<int>::iterator iter = l.begin(); iter != l.end(); iter++) {
        std::cout << *iter << std::endl;
    }
    //l.swap(x);
    MyTinySTL::list<int>h(3,0);
    MyTinySTL::list<int>y(h);
    std::cout << "list size：" << y.size() << std::endl;
    for (MyTinySTL::list<int>::iterator iter = y.begin(); iter != y.end(); iter++) {
        std::cout << *iter << std::endl;
    }

    MyTinySTL::list<int>l(1, 10);
    l.push_back(9);
    l.push_back(8);
    l.push_back(7);
    l.push_back(6);
    l.push_back(5);
    l.push_back(4);
    l.push_back(3);
    l.push_back(2);
    l.push_back(1);
    l.push_back(0);
    std::cout << "list size：" << l.size() << std::endl;
    for (MyTinySTL::list<int>::iterator iter = l.begin(); iter != l.end(); iter++) {
        std::cout << *iter << std::endl;
    }
    l.sort();
    for (MyTinySTL::list<int>::iterator iter = l.begin(); iter != l.end(); iter++) {
        std::cout << *iter << std::endl;
    }
    
    */
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
