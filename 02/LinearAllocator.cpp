#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(std::size_t maxSize)
{

    pstart = static_cast<char*>( std::malloc(maxSize) );
    if (pstart != nullptr) {
        pend = pstart+( maxSize - 1 );
    }
    else {
        pend = nullptr;
    }
    pcurr = pstart;
}
char* LinearAllocator::alloc(std::size_t size)
{
    if (pstart == nullptr) {
        return nullptr;
    }
    if ( pcurr + size - 1 > pend ) {
        return nullptr;
    }
    else {
        char* p = pcurr;
        pcurr += size;
        return p;
    }
}



void LinearAllocator::reset()
{
    pcurr=pstart;
}

LinearAllocator::~LinearAllocator()
{
    if ( pstart != nullptr ) {
        free(pstart); // очистить все
    }
}

