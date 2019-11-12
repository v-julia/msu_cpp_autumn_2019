
#include "bigint.h"

#include <iostream>
#include <iomanip>
#include <cmath>


int main()
{
    // тестирование на числах int64_t (не более 0xffffffffffffff)
    int64_t ai= 0xf0f1f2f3f4f5f6;
    int64_t bi=-0x75da61fc20a68d;
    int64_t ci= 0x32536fda54cb46;
    int64_t di=-0x56745f1a3bb278;
    int64_t ei= 0x1000209f436a0b;
    int64_t fi=-0xcfda671baf702d;
    int64_t gi=-0xcfda671baf702d;

    BigInt a(ai);
    BigInt b(bi);
    BigInt c(ci);
    BigInt d(di);
    BigInt e(ei);
    BigInt f(fi);
    BigInt g(gi);

    std::cout.setf(std::cout.flags() | std::ios::boolalpha);

    // проверка операций сравнения и вывода в поток
   
    std::cout << std::endl;
    std::cout << "  test comparision:" << std::endl;
    std::cout << std::endl;
    std::cout << "a =" << a << "; b =" << b << std::endl;
    std::cout << "ai=" << ( ai < 0 ? "-" : "" ) << std::hex << std::abs(ai) << "; bi=" << ( bi < 0 ? "-" : "" ) << std::hex << std::abs(bi) << std::endl;
    std::cout << std::endl;
    std::cout << "(a <b )=" << ( a < b ) << "; (a <=b )=" << ( a <= b ) << "; (a > b )=" << ( a > b ) << "; (a >=b )=" << ( a >= b ) << "; (a == b )=" << ( a == b ) << "; (a != b )=" << ( a != b ) << std::endl;
    std::cout << "(ai<bi)=" << ( ai< bi) << "; (ai<=bi)=" << (ai <= bi) << "; (ai> bi)=" << ( ai> bi) << "; (ai>=bi)=" << ( ai>= bi) << "; (ai== bi)=" << ( ai ==bi) << "; (ai!= bi)=" << ( ai!= bi) << std::endl;
    std::cout << std::endl;
    std::cout << "f =" << f << "; g =" << g << std::endl;
    std::cout << "fi=" << ( fi < 0 ? "-" : "" ) << std::hex << std::abs(fi) << "; gi=" << ( gi < 0 ? "-" : "" ) << std::hex << std::abs(gi) << std::endl;
    std::cout << std::endl;
    std::cout << "(f <g )=" << ( f < g ) << "; (f <=g )=" << ( f <= g ) << "; (f > g )=" << ( f > g ) << "; (f >=g )=" << ( f >= g ) << "; (f == g )=" << ( f == g ) << "; (f != g )=" << ( f != g ) << std::endl;
    std::cout << "(fi<gi)=" << ( fi < gi ) << "; (fi<=gi)=" << ( fi <= gi ) << "; (fi> gi)=" << ( fi > gi ) << "; (fi>=gi)=" << ( fi >= gi ) << "; (fi== gi)=" << ( fi == gi ) << "; (fi!= gi)=" << ( fi != gi ) << std::endl;
   

    // проверка операций сложения вычитания, унарного минуса  и вывода в поток
   
    std::cout << std::endl << "  test addition/subtraction/un-:" << std::endl;

    BigInt h=a + b + c + d + f + g;
    BigInt i=a - b + c - d + f - g;
    BigInt j=-a + b - c - d + f + g;
    BigInt k(gi); ( k+=e )-=d;
    BigInt m=-g; m=-(( m-=f )+=c);

    int64_t hi=ai + bi + ci + di + fi + gi;
    int64_t ii=ai - bi + ci - di + fi - gi;
    int64_t ji=-ai + bi - ci - di + fi + gi;
    int64_t ki=gi; ( ki+=ei )-=di;
    int64_t mi=-gi; mi=-(( mi-=fi )+=ci);

    std::cout << "h:" << std::endl;
    std::cout << "    " << "a + b + c + d + f + g = " << h << std::endl;
    std::cout << "    " << "ai+ bi+ ci+ di+ fi+ gi= " << ( hi < 0 ? "-" : "" ) << std::hex << std::abs(hi) << std::endl;

    std::cout << "i:" << std::endl;
    std::cout << "    " << "a - b + c - d + f - g = " << i << std::endl;
    std::cout << "    " << "ai- bi+ ci- di+ fi- gi= " << ( ii < 0 ? "-" : "" ) << std::hex << std::abs(ii) << std::endl;

    std::cout << "j:" << std::endl;
    std::cout << "    " << "-a + b - c - d + f + g = " << j << std::endl;
    std::cout << "    " << "-ai+ bi- ci- di+ fi+ gi= " << ( ji < 0 ? "-" : "" ) << std::hex << std::abs(ji) << std::endl;

    std::cout << "k:" << std::endl;
    std::cout << "    " << "k(gi); ( k +=e )-=d ;  k = " << k << std::endl;
    std::cout << "    " << "ki=gi; ( ki+=ei)-=di;  ki= " << ( ki < 0 ? "-" : "" ) << std::hex << std::abs(ki) << std::endl;

    std::cout << "m:" << std::endl;
    std::cout << "    " << "m =-g ; m =-(( m -=f )+=c ); m = " << m << std::endl;
    std::cout << "    " << "mi=-gi; mi=-(( mi-=fi)+=ci); mi= " << ( mi < 0 ? "-" : "" ) << std::hex << std::abs(mi) << std::endl;

}


