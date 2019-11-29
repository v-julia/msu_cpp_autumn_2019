
#include <iostream>
#include <random>
#include <algorithm>


#include "Vector.h"



int main()
{

    Vector<int> v(2);
    std::cout << "class Vector:" << "\n";
    std::cout << "\n" << "constructor test: Vector<int> v(2);" << "\n";
    for ( auto& k : v ) {
        k = 5;
        std::cout << "vk=  " << k << "\n";
    }

    std::cout << "\n" << "push_back test: v.push_back(123456);" << "\n";
    v.push_back(123456);
    std::cout << "v= ";  for ( auto& k : v ) { std::cout << k << " "; }; std::cout << "\n" << "\n";


    std::cout << "\n" << "pop_back test: v.pop_back();" << "\n";
    v.pop_back();
    std::cout << "v= ";  for ( auto& k : v ) { std::cout << k << " "; }; std::cout << "\n" << "\n";


    std::cout << "\n" << "emplace_back test: v.emplace_back(123,456,789);" << "\n";
    v.emplace_back(123, 456, 789);
    std::cout << "v= ";  for ( auto& k : v ) { std::cout << k  << " "; }; std::cout << "\n" << "\n";






    std::cout << "\n" << "clear test: v.clear();" << "\n";
    v.clear();
    std::cout << "v.size =  " << v.size() << "    v.capacity =  " << v.capacity() << "\n";


    std::cout << "\n" << "resize test: v.resize(9);" << "\n";
    v.resize(9,1);
    std::cout << "v.size =  " << v.size() << "    v.capacity =  " << v.capacity() << "\n";

    std::cout << "\n" << "shrink test: v.shrink_to_fit();" << "\n";
    v.shrink_to_fit();
    std::cout << "v.size =  " << v.size() << "    v.capacity =  " << v.capacity() << "\n";





    std::cout << "\n" << "sorting test" << "\n";
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




    std::cout << "\n" << "reverse test" << "\n";
    std::cout << "vk= ";
    for ( auto it = v.rbegin(); it != v.rend(); ++it ) {
        std::cout << *it << " ";
    }
    std::cout << "\n";





    std::cout << "\n" << "Vector<double> test" << "\n";

    Vector<double> w={1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9};
    for ( auto& k : w ) {
        std::cout << "wk=  " << k << "\n";
    }



    std::cout << "\n" << "Vector<std::string> test" << "\n";
    Vector<std::string> s = { "one","two","three","four","five","six","seven","eigth","nine","ten" };
    std::cout << "s =  "; for ( auto& k : s ) { std::cout << k << " "; }std::cout << "\n";



    std::cout << "\n" << "random_shuffle test" << "\n";
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(s.begin(), s.end(), g);
    std::cout << "s =  "; for ( auto& k : s ) {        std::cout << k << " ";    }    std::cout << "\n";



    std::cout << "\n" << "sort by length strings test" << "\n";
    std::sort(s.begin(), s.end(), [](std::string& a, std::string& b) {return ( a.length() == b.length() ) ? ( a < b ) : ( a.length() < b.length() ); });
    std::cout << "s =  "; for ( auto& k : s ) { std::cout << k << " "; }    std::cout << "\n";



    std::cout << "\n" << "sort by alphabet strings test" << "\n";
    std::sort(s.begin(), s.end(), [](std::string& a, std::string& b) {return a < b; });
    std::cout << "s =  "; for ( auto& k : s ) { std::cout << k << " "; }    std::cout << "\n";


    return 0;
}

