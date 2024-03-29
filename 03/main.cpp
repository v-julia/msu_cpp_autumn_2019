﻿
#include "Matrix.h"

#include <iostream>
#include <iomanip>


int main()
{
    size_t rows=13;
    size_t cols=6;
    Matrix mt(rows, cols);
    Matrix mt_eq(rows, cols);
    Matrix mt_ne(rows, cols);

    size_t grows=mt.getRows();
    size_t gcols=mt.getCols();
    std::cout << "mt.getRows=" << grows << "; mt.getCols=" << gcols << ";\n";

    //  здесь работает версия оператора [][]   c_row& operator[](size_t i)

    int k=0;
    for ( size_t i=0; i < rows; ++i ) {
        for ( size_t j=0; j < cols; ++j ) {
            mt[i][j]=k;
            k+=1;
        }
    }


    std::cout << "\n";
    std::cout << "Matrix mt:\n";
    for ( size_t i=0; i < rows; ++i ) {
        for ( size_t j=0; j < cols; ++j ) {
            int u=mt[i][j];
            std::cout << std::setw(6) << "mt[" << std::setw(2) << i << "][" << std::setw(2) << j << "]=" << std::setw(3) << u << " ";
        }
        std::cout << "\n";

    }
    std::cout << "\n";

    int g=3;
    mt+=g;

    std::cout << "\n";
    std::cout << "Matrix mt+=(C=" << g << "):\n";
    for ( size_t i=0; i < rows; ++i ) {
        for ( size_t j=0; j < cols; ++j ) {
            int u=mt[i][j];
            std::cout << std::setw(6) << "mt[" << std::setw(2) << i << "][" << std::setw(2) << j << "]=" << std::setw(3) << u << " ";
        }
        std::cout << "\n";

    }
    std::cout << "\n";


    int c=10;
    mt*=c;

    std::cout << "\n";
    std::cout << "Matrix mt*=(C=" << c << "):\n";
    for ( size_t i=0; i < rows; ++i ) {
        for ( size_t j=0; j < cols; ++j ) {
            int u=mt[i][j];
            std::cout << std::setw(6) << "mt[" << std::setw(2) << i << "][" << std::setw(2) << j << "]=" << std::setw(3) << u << " ";
        }
        std::cout << "\n";

    }
    std::cout << "\n";

    for ( size_t i=0; i < rows; ++i ) {
        for ( size_t j=0; j < cols; ++j ) {
            mt_eq.set(i, j, mt[i][j]);
            mt_ne.set(i, j, -5);
        }
    }
    std::cout << "\n";
    std::cout << "Matrix mt_eq:\n";
    for ( size_t i=0; i < rows; ++i ) {
        for ( size_t j=0; j < cols; ++j ) {
            int u=mt_eq[i][j];
            std::cout << std::setw(6) << "mt_eq[" << std::setw(2) << i << "][" << std::setw(2) << j << "]=" << std::setw(3) << u << " ";
        }
        std::cout << "\n";

    }
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "Matrix mt_ne:\n";
    for ( size_t i=0; i < rows; ++i ) {
        for ( size_t j=0; j < cols; ++j ) {
            int u=mt_ne[i][j];
            std::cout << std::setw(6) << "mt_ne[" << std::setw(2) << i << "][" << std::setw(2) << j << "]=" << std::setw(3) << u << " ";
        }
        std::cout << "\n";

    }
    std::cout << "\n";

    // Здесь сработает оператор  [][] const c_row& operator[](size_t i) const

    bool test_eq_mt_vs_mt_eq = ( mt == mt_eq );
    bool test_ne_mt_vs_mt_eq = ( mt != mt_eq );
    bool test_eq_mt_vs_mt_ne = ( mt == mt_ne );
    bool test_ne_mt_vs_mt_ne = ( mt != mt_ne );

    std::cout.setf(std::ios::boolalpha);
    std::cout << "(mt == mt_eq) = " << test_eq_mt_vs_mt_eq << "\n";
    std::cout << "(mt != mt_eq) = " << test_ne_mt_vs_mt_eq << "\n";
    std::cout << "(mt == mt_ne) = " << test_eq_mt_vs_mt_ne << "\n";
    std::cout << "(mt != mt_ne) = " << test_ne_mt_vs_mt_ne << "\n";
    std::cout << "\n";

    // тест на исключения
    try {
        mt.set(20, 1, 0);
    }
    catch ( std::out_of_range ) {
        std::cout << "exception for mt.set(20, 1, 0); catched!\n";
    }
    try {
        mt.set(-20, -1, 0);
    }
    catch ( std::out_of_range ) {
        std::cout << "exception for mt.set(-20, -1, 0); catched!\n";
    }
    try {
        int k=mt[40][5];
    }
    catch ( std::out_of_range ) {
        std::cout << "exception for k=mt[40][5]; catched!\n";
    }

    try {
        int k=mt[-1][2];
    }
    catch ( std::out_of_range ) {
        std::cout << "exception for k=mt[-1][2]; catched!\n";
    }
}

