#include "LinearAllocator.h"
#include <iostream>


int main()
{
     // проверка для LinearAllocator
    LinearAllocator allocator(10);

    char* a=allocator.alloc(5); // память выделена
    for ( int i=0; i < 5; ++i ) {
        *( a + i )='A';
    }
    if (a != nullptr){
    std::cout << "maxsize=10, size=5; passed" << std::endl;
    }
    else std::cout << "maxsize=10, size=5; not passed" << std::endl;
    
    
    char* b=allocator.alloc(4); // память выделена
    for ( int i=0; i < 4; ++i ) {
        *( b + i )='B';
    }
    
    if (b != nullptr){
    std::cout << "maxsize=10, size=4 after 5; passed" << std::endl;
    }
    else std::cout << "maxsize=10, size=4 after 5; not passed" << std::endl;
    
    char* c=allocator.alloc(4); // память не выделена
    
    if (c == nullptr){
    std::cout << "maxsize=10, size=4 after 9; passed" << std::endl;
    }
    else std::cout << "maxsize=10, size=4 after 9; not passed" << std::endl;

    allocator.reset(); // сброс

    char* d=allocator.alloc(3); // память выделена
    for ( int i=0; i < 3; ++i ) {
        *( d + i )='D';
    }
    
    
    if (d != nullptr){
    std::cout << "maxsize=10, size=3 after reset; passed" << std::endl;
    }
    else std::cout << "maxsize=10, size=4 after reset; not passed" << std::endl;

    // при выходе - уничтожается переменная allocator и метод ~LinearAllocator() освобождает занятую память
}
