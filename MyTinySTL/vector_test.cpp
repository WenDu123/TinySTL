#include <iostream>
#include "Vector.h"

int main()
{
    std::cout << "Hello World!\n";
    MyTinySTL::vector<int>v(10,5);
    std::cout << v.begin() <<std::endl;
    //for(int i = 0;i<11;i++)   {}
    return 0;
}