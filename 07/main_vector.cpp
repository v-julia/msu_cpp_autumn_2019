


#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

#include "Vector.h"





int main()
{

    Vector<int> v(2);
    std::cout << "class Vector:" << "\n";
    std::cout << "\n" << "constructor test: Vector<int> v(2);" << "\n";
    for ( auto& k : v ) {
        k = 5;
        std::cout  << "vk=  " << k << "\n";
    }

    std::cout << "\n" << "push_back test: v.push_back(123456);" << "\n";
    v.push_back(123456);
    for ( auto& k : v ) {
        std::cout << "vk=  " << k << "\n";
    }
    std::cout << "\n" << "pop_back test: v.pop_back();" << "\n";
    v.pop_back();
    for ( auto& k : v ) {
        std::cout << "vk=  " << k << "\n";
    }

    std::cout << "\n" << "clear test: v.clear();" << "\n";
    v.clear();
    std::cout << "v.size =  " << v.size() << "\n";

    std::cout << "\n" << "resize test: v.resize(9);" << "\n";
    v.resize(9);
    std::cout << "v.resize(9); v.size =  " << v.size() << "\n";
    
    
    std::cout  << "\n" << "sorting test" << "\n";
    std::cout << "initial:" << "\n";
    int val = 9;
    for ( auto it = v.begin(); it != v.end(); ++it ) {
        *it = val--;
    }
    std::cout << "vk= ";
    for ( auto& k : v ) {
        std::cout << k << " ";
    }
    std::cout << "\n";

    std::cout << "sorted:" << "\n";
    std::cout << "vk= ";
    std::sort(v.begin(), v.end());
    for ( auto& k : v ) {
        std::cout << k << " ";
    }
    std::cout << "\n";

    std::cout << "\n" << "Vector<double> test" << "\n";

    Vector<double> w(3);
    for ( auto& k : w ) {
        k = 1.1111111;
        std::cout << "wk=  " << k << "\n";
    }

    std::cout << "\n" << "Vector<std::string> test" << "\n";
    Vector<std::string> s(3);
    for ( auto& k : s ) {
        k = "this is string";
        std::cout << "sk=  " << k << "\n";
    }

    return 0;


}

