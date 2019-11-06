#include "bigint.h"
#include <cmath>
BigInt::~BigInt()
{
    delete[] uints;
}

BigInt::BigInt(int64_t value)
{
    uints=new uint8_t[8];
    isNegative=( value < 0 );
    uints_size=8;
    int64_t val=std::abs(value);
    for ( size_t i=0; i < 8; ++i ) {
        uints[i]=val & max_digit;
        val >>= 8;
    }
}

BigInt::BigInt(const BigInt& val)
{
    uints = new uint8_t[val.uints_size];
    uints_size=val.uints_size;
    std::copy(val.uints, val.uints + uints_size, uints);
    isNegative=val.isNegative;
}


BigInt& BigInt::operator=(const BigInt& copied)
{
    if ( this != &copied ) {
        if ( uints_size >= copied.uints_size ) {
            // если места достаточно, чтобы разместить копию, то делать присваивание
            for ( size_t i=0; i < uints_size; ++i ) {
                if ( i < copied.uints_size ) {
                    uints[i]=copied.uints[i];
                }
                else {
                    uints[i]=0;
                }
            }
        }
        else {
            // когда нет места выделяется новый участок памяти под копию
            uint8_t* ptr=new uint8_t[copied.uints_size];
            delete[] uints;
            uints=ptr;
            uints_size=copied.uints_size;
            std::copy(copied.uints, copied.uints + uints_size, uints);
        }
        isNegative=copied.isNegative;
    }
    return *this;
}



// 1
std::ostream& operator<<(std::ostream& os, const BigInt& val)
{
    //int old_flags=os.flags();
    std::ios_base::fmtflags old_flags=os.flags();
    if ( val.isNegative ) {
        os.put('-');
    }
    size_t first_uint=val.uints_size;
    if ( first_uint > 0 ) {
        size_t c_count=2 * val.sz_digit;

        while ( val.uints[first_uint - 1] == 0 && first_uint != 1 ) --first_uint;

        if ( first_uint == 0 )
            os.put('0');
        else {
            os << std::hex << int(val.uints[--first_uint]);
            while ( first_uint-- != 0 ) {
                os << std::setw(c_count) << std::setfill('0') << std::hex << int(val.uints[first_uint]);
            };
        }
    }
    os.setf(old_flags);
    return os;
}

// 2
BigInt& BigInt::operator+=(const BigInt& other)
{
    if ( isNegative == other.isNegative ) {
        add_abs(other);
    }
    else {
        bool old_is_negative=isNegative;
        dif_abs(other);
        if ( old_is_negative ) isNegative=!isNegative;
    }
    return *this;
}

BigInt BigInt::operator+(const BigInt& other) const
{
    BigInt tmp(*this);
    tmp += other;
    return tmp;
}




// 3
BigInt& BigInt::operator-=(const BigInt& other)
{
    bool old_is_negative=isNegative;
    if ( isNegative != other.isNegative ) {
        add_abs(other);
        isNegative=old_is_negative;
    }
    else {
        dif_abs(other);

        if ( old_is_negative ) isNegative=!isNegative;

    }
    return *this;
}

BigInt BigInt::operator-(const BigInt& other) const
{
    BigInt tmp(*this);
    tmp -= other;

    return tmp;
}


// 4 унарный минус
BigInt BigInt::operator-() const
{
    BigInt tmp(*this);
    tmp.isNegative=!isNegative;

    return tmp;
}


//5
bool BigInt::operator<(const BigInt& other) const
{
    if ( this == &other )
        return false;
    if ( isNegative && !other.isNegative ) return true;
    if ( !isNegative && other.isNegative ) return false;

    int k=abs_compare_less_than(*this, other);

    return ( isNegative && ( k < 0 ) ) || ( !isNegative && ( k > 0 ) );

}

int abs_compare_less_than(const BigInt& lhs, const BigInt& rhs)
{
    size_t lhs_dig_cnt=lhs.digits_count();
    size_t rhs_dig_cnt=rhs.digits_count();

    if ( lhs_dig_cnt < rhs_dig_cnt ) return 1;
    if ( lhs_dig_cnt > rhs_dig_cnt ) return -1;

    while ( lhs_dig_cnt-- != 0 ) {
        if ( lhs.uints[lhs_dig_cnt] < rhs.uints[lhs_dig_cnt] ) return 1;
        if ( lhs.uints[lhs_dig_cnt] > rhs.uints[lhs_dig_cnt] ) return -1;
    }
    return 0;

}

bool BigInt::operator==(const BigInt& other) const
{
    if ( this == &other )
        return true;
    if ( isNegative ^ other.isNegative ) return false; // разные знаки

    size_t dig_cnt=digits_count();
    size_t dig_cnt_other=other.digits_count();
    if ( dig_cnt != dig_cnt_other ) return false;
    while ( dig_cnt-- != 0 ) {
        if ( uints[dig_cnt] != other.uints[dig_cnt] ) return false;
    }

    return true;
}



size_t BigInt::digits_count() const
{
    size_t dig_cnt=0;
    for ( size_t i=0; i < uints_size; ++i ) {
        if ( uints[i] != 0 ) dig_cnt=i + 1;
    }
    return dig_cnt;
}

uint8_t BigInt::sum_digits(uint8_t a1, uint8_t a2, uint8_t& rem)
{
    // сложение цифр: 
    //   a1+a2<10 (нет переноса разряда) выполнено условие a2< (a1-9-1) или a2<=a1-9
    //   при a1+a2>=10 есть перенос разряда (то есть, (10-1)-a1>=a2, следовательно
    //   a2>=9+1-a1  или  a2>9-a1, тогда можно так вычислить 
    //   res=a1+a2-10=a1+a2-9+1=(a2-(9-a1))-1 и указать перенос разряда rem=1  
    uint8_t dopoln, res;
    if ( rem > 0 ) {
        int dopoln=max_digit - a1;
        if ( dopoln >= rem ) {
            a1=a1 + rem;
            rem=0;
        }
        else {
            a1=( rem - dopoln ) - 1;
            rem=1;
        }
    }
    dopoln=max_digit - a1;
    if ( a2 <= dopoln ) {
        res=a1 + a2;
    }
    else {
        res=( a2 - dopoln ) - 1;
        rem=1;
    }
    return res;
}

uint8_t BigInt::dif_digits(uint8_t a1, uint8_t a2, uint8_t& rem)
{
    // вычитание цифр: 
    //   если a1-a2>=0 (нет заимствования разряда) значит условие для цифр a1>=a2
    //   иначе a1<a2 (при этом a2!=0) и приходится занимать разряд a1-a2+10=a1-a2+9+1=a1+((9-a2)+1) и rem=1
    uint8_t res=0;
    if ( rem > 0 ) {
        if ( a1 > 0 ) {
            --a1;
            rem=0;
        }
        else {
            a1=max_digit;
            rem=1;
        }
    }
    res=a1;
    if ( a1 >= a2 ) {
        res-=a2;
        rem=0;
    }
    else {
        res=( max_digit - a2 ) + a1 + 1;
        rem=1;
    }
    return res;
}

void BigInt::add_abs(const BigInt& other)
{
    size_t dig_cnt=digits_count();
    size_t dig_cnt_other=other.digits_count();
    size_t dig_cnt_max=( dig_cnt > dig_cnt_other ) ? dig_cnt + 1 : dig_cnt_other + 1;

    // если места может не хватить для размещения суммы в this, то увеличивается количество нулевых старших элементов
    if ( uints_size < dig_cnt_max ) {
        uint8_t* ptr=new uint8_t[dig_cnt_max];
        std::copy(uints, uints + uints_size, ptr);
        delete[] uints;
        uints=ptr;
        for ( size_t i=uints_size; i < dig_cnt_max; ++i )
            ptr[i]=uint8_t(0);
        uints_size=dig_cnt_max;
    }
    uint8_t rem=0;
    for ( size_t i=0; i < dig_cnt_max; ++i ) {
        if ( i < dig_cnt_other ) {
            uint8_t res=sum_digits(uints[i], other.uints[i], rem);
            uints[i]=res;
        }
        else {
            uint8_t res=sum_digits(uints[i], uint8_t(0), rem);
            uints[i]=res;
        }
    }
}

void BigInt::dif_abs(const BigInt& other)
{
    size_t dig_cnt=digits_count();
    size_t dig_cnt_other=other.digits_count();
    size_t dig_cnt_max=( dig_cnt > dig_cnt_other ) ? dig_cnt : dig_cnt_other;
    size_t dig_cnt_min=( dig_cnt < dig_cnt_other ) ? dig_cnt : dig_cnt_other;

    // если места может не хватить для размещения разности в this, то увеличивается количество нулевых старших элементов
    if ( uints_size < dig_cnt_max ) {
        uint8_t* ptr=new uint8_t[dig_cnt_max];
        std::copy(uints, uints + uints_size, ptr);
        delete[] uints;
        uints=ptr;
        for ( size_t i=uints_size; i < dig_cnt_max; ++i )
            ptr[i]=uint8_t(0);
        uints_size=dig_cnt_max;
    }
    int k = abs_compare_less_than(other, *this);
    bool isOtherLessOrEq = ( k >= 0 );
    // из большего по модулю вычитается меньшее
    uint8_t rem=0;
    uint8_t res;
    for ( size_t i=0; i < dig_cnt_max; ++i ) {
        if ( isOtherLessOrEq ) {
            res=dif_digits(uints[i], other.uints[i], rem);
        }
        else {
            res=dif_digits(other.uints[i], uints[i], rem);
        }
        uints[i]=res;
    }
    isNegative=!isOtherLessOrEq; // знак тоже здесь устанавливается 
}





BigInt::BigInt(const std::string& hex_str)
{
    size_t len=hex_str.size();
    size_t lenx=0;
    size_t c_count=2 * sz_digit; // количество 16-х цифр (0, 1, 2,  E, F) для отображения большой цифры
    isNegative=false;
    // вначале поиск знака числа, он должен быть первым (не 16-м) символом
    bool sign_fnd=false;
    for ( int i=0; i < len; ++i ) {
        if ( ' ' == hex_str[i] ) continue;
        if ( !sign_fnd ) {
            sign_fnd=true;
            if ( '-' == hex_str[i] ) {
                isNegative=true;
            }
            else if ( !( 0 == std::isxdigit(static_cast<unsigned char>( hex_str[i] )) ) ) {
                ++lenx;
            }
        }
        else {
            if ( !( 0 == std::isxdigit(static_cast<unsigned char>( hex_str[i] )) ) ) ++lenx;
        }
    }
    // перекодировка 16-х символов в одну digit (неверные символы пропустить)
    if ( lenx > 0 ) {
        size_t ostatok=lenx % c_count;
        lenx/=c_count;
        if ( ostatok > 0 ) ++lenx;
        uints=new uint8_t[lenx];
        size_t cnt=0;
        size_t idx_x=lenx - 1; // самый последний индекс набора чисел my_uints
        std::string sx="";
        for ( auto c : hex_str ) {
            if ( 0 == std::isxdigit(static_cast<unsigned char>( c )) ) continue;
            sx+=c;
            ++cnt;
            if ( cnt == c_count ) {
                uints[idx_x]=uint8_t(std::stoul(sx, nullptr, 16));
                unsigned int test=uints[idx_x];
                sx.clear();
                --idx_x;
                cnt=0;
            }
        }
        if ( cnt > 0 ) {
            uints[idx_x]=uint8_t(std::stoul(sx, 0, 16));
        }
        uints_size=lenx;
    }
    else {
        // при неверных данных сделать нулем
        uints=new uint8_t[1];
        uints[0]=0;
        isNegative=false;
        uints_size=1;
    }
}


