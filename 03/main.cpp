
#include <iostream>
#include "Matrix.h"

int main()
{
    size_t rows=2;
    size_t cols=3;
    Matrix mt(rows, cols);
    Matrix mt_eq(rows, cols);
    Matrix mt_ne(rows, cols);

    size_t grows=mt.getRows();
    size_t gcols=mt.getCols();
    std::cout << "mt.getRows=" << grows << "; mt.getCols=" << gcols << ";\n";

    int k=1;
    for ( size_t i=1; i <= rows; ++i ) {
        for ( size_t j=1; j <= cols; ++j ) {
            mt[i][j]=k;
            k+=1;
        }
    }

    std::cout << "\n";
    std::cout << "Matrix mt:\n";
    for ( size_t i=1; i <= rows; ++i ) {
        for ( size_t j=1; j <= cols; ++j ) {
            int u=mt[i][j];
            std::cout << "mt[" << i << "][" << j <<"]=" << u << " ";
        }
        std::cout << "\n";

    }
    std::cout << "\n";

    int c=10;
    for ( size_t i=1; i <= rows; ++i ) {
        for ( size_t j=1; j <= cols; ++j ) {
            mt[i][j]*=c;
        }
    }


    std::cout << "\n";
    std::cout << "Matrix mt*(C="<< c << "):\n";
    for ( size_t i=1; i <= rows; ++i ) {
        for ( size_t j=1; j <= cols; ++j ) {
            int u=mt[i][j];
            std::cout << "mt[" << i << "][" << j << "]=" << u << " ";
        }
        std::cout << "\n";

    }
    std::cout << "\n";

    for ( size_t i=1; i <= rows; ++i ) {
        for ( size_t j=1; j <= cols; ++j ) {
            mt_eq[i][j]= mt[i][j];
            mt_ne[i][j] = -5;
        }
    }
    std::cout << "\n";
    std::cout << "Matrix equal to mt, mt_eq:\n";
    for ( size_t i=1; i <= rows; ++i ) {
        for ( size_t j=1; j <= cols; ++j ) {
            int u=mt_eq[i][j];
            std::cout << "mt_eq[" << i << "][" << j << "]=" << u << " ";
        }
        std::cout << "\n";

    }
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "Matrix not equal to mt, mt_ne:\n";
    for ( size_t i=1; i <= rows; ++i ) {
        for ( size_t j=1; j <= cols; ++j ) {
            int u=mt_ne[i][j];
            std::cout << "mt_ne[" << i << "][" << j << "]=" << u << " ";
        }
        std::cout << "\n";

    }
    std::cout << "\n";
    
    bool test_eq_mt_vs_mt_eq = ( mt == mt_eq );
    bool test_ne_mt_vs_mt_eq = ( mt != mt_eq );
    bool test_eq_mt_vs_mt_ne = ( mt == mt_ne );
    bool test_ne_mt_vs_mt_ne = ( mt != mt_ne );

    std::cout << "(mt == mt_eq) = " << ( test_eq_mt_vs_mt_eq ? "true" : "false" ) << "\n";
    std::cout << "(mt != mt_eq) = " << ( test_ne_mt_vs_mt_eq ? "true" : "false" ) << "\n";
    std::cout << "(mt == mt_ne) = " << ( test_eq_mt_vs_mt_ne ? "true" : "false" ) << "\n";
    std::cout << "(mt != mt_ne) = " << ( test_ne_mt_vs_mt_ne ? "true" : "false" ) << "\n";
    std::cout << "\n";


}

