#pragma once

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstring>
#include <string>
#include <limits>

class BigInt
{
protected:
    // у большого числа в качестве цифр будем применять беззнаковые целые числа uint8_t
    // все цифры будут храниться в массиве my_uints, причем младшие разряды в начале массива
    // например, количество разных цифр будет ровно 256, от 0 до 255 - это наибольшая цифра
    
    uint8_t* uints=nullptr;
    
    size_t uints_size=0; // количество элементов uint8_t в массиве my_uints
    
    bool isNegative;     // знак числа false-неотрицательное, true - отрицательное
    
public:
    // нужно знать также размер цифры в байтах, чтобы правильно отображать каждую цифру
    // цифры можно записывать в 16-й системе - если sz_digit=1, то для каждой цифры потребуется 2 буквы: 0A
    
    const static size_t sz_digit=sizeof(uint8_t);

    // для uint запоминается максимальная цифра (для сложения-вычитания цифр) 
    
    const static unsigned int max_digit=std::numeric_limits<uint8_t>::max();


public:

    BigInt(int64_t value=0);
    BigInt(const BigInt& val);
    // перемещение
    BigInt(BigInt&& moved) noexcept;

    ~BigInt();


    // требуется: 1 оператор вывода, 2 сложение, 3 вычитание, 4 унарный минус, 5 все операции сравнения
    // делаем по примерам из лекции №5 

    // присваивания
    BigInt& operator=(const BigInt& copied);     // копирующий 
    // перемещение
    BigInt& operator=(BigInt&& moved) noexcept;

    // 1
    friend std::ostream& operator<<(std::ostream& os, const BigInt& val);

    // 2
    BigInt& operator+=(const BigInt& rhs);
    BigInt operator+(const BigInt& other) const;
                                                              
    // 3
    BigInt& operator-=(const BigInt& rhs);
    BigInt operator-(const BigInt& other) const;

    // 4 унарный минус
    BigInt operator-() const;


    // 5
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const { return !( *this == other ); };

    bool operator<(const BigInt& other) const;
    bool operator>=(const BigInt& other) const { return !( *this < other ); }
    bool operator>(const BigInt& other) const { return !(*this < other || *this == other ); }
    bool operator<=(const BigInt& other) const { return !( *this > other ); }


    friend int abs_compare_less_than(const BigInt& lhs, const BigInt& rhs);


public:
    
    size_t        digits_count() const;                  // количество значащих цифр (без нулей в старших разрядах)


private:
    // вычисление суммы-разности цифр с учетом переноса разряда
    uint8_t sum_digits(uint8_t a1, uint8_t a2, uint8_t& rem);
    uint8_t dif_digits(uint8_t a1, uint8_t a2, uint8_t& rem);

    // добавление и вычисление разности без учета знаков (применяется только для += и -=)
    void add_abs(const BigInt& other);
    void dif_abs(const BigInt& other);

public:
    BigInt(const std::string& hex_str);       // это для упрощения ввода чисел при отладке (число из 16-й строки)
};

std::ostream& operator<<(std::ostream& os, const BigInt& val);


int abs_compare_less_than(const BigInt& lhs, const BigInt& rhs);
