#pragma once
#include <cstdlib>
#include <iostream>

class LinearAllocator
{
    public:
        LinearAllocator(std::size_t maxSize);
        char* alloc(std::size_t size);
        void reset();

        ~LinearAllocator();

private:
        char* pstart; // начало
        char* pend;   // окончание
        char* pcurr;  // начало свободного места
};

