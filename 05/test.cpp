#include <iostream>
#include <sstream>
#include "serializer.h"
#include <cassert>

int main()
{
    Data x{ 1, true, 2 };

    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    std::cout << stream.str() << "\n";

    Data y{ 0, false, 0 };

    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);

    assert(err == Error::NoError);

    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);

    std::cout << "\ny=x. OK!\n";

    return 0;
}

