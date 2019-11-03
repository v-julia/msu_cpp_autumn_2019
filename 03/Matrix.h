#pragma once

#include <stdexcept>


class Matrix
{
private:
    // класс для одиночной строки
    class c_row
    {
    public:
        c_row(size_t cols)
        {
            row=new int[cols];
            col_count=cols;
        };
        ~c_row()
        {
           delete[] row;
        };
        int& operator[](size_t i) const
        {
            if ( i < 0 || i >= col_count ) throw std::out_of_range("");
            return row[i];
        }
        size_t getCols() { return col_count; }

    private:
        int* row;
        size_t col_count;
    };

        // класс для одиночной строки
private:
    // данные класса Matrix
    c_row* matr;      // указатель на массив из строк
    size_t row_count; // количество строк

public:
    Matrix(size_t rows, size_t cols)
    {
        // применяется placement new
        matr=static_cast<c_row*>( operator new[](rows * sizeof(c_row)) );
        for ( size_t i = 0; i < rows; ++i ) {
            new ( matr + i ) c_row(cols);
        }
        row_count=rows;
    }
    ~Matrix()
    {
        // деструктор: освобождение памяти
        for ( size_t i = 0; i < row_count; ++i ) {
            matr[i].~c_row(); // вначале освободить память строк
        }
        operator delete[](matr); // последний - освободить указатели на строки

    }

    size_t getRows() const { return row_count; }
    size_t getCols() const
    {
        if ( row_count > 0 ) {
            return matr[0].getCols();
        }
        else {
            return 0;
        }
    }
    c_row& operator[](size_t i) const
    {
        if (i<0 || i >= row_count ) throw std::out_of_range("");
        return matr[i];
    }

    Matrix& operator*=(const int& c)
    {
        size_t cols=getCols();
        for ( size_t j=0; j < row_count; ++j ) {
            for ( size_t i=0; i < cols; ++i ) {
                matr[i][j]*=c;
            }
        }
    }
    Matrix& operator+=(const int& c)
    {
        size_t cols=getCols();
        for ( size_t j=0; j < row_count; ++j ) {
            for ( size_t i=0; i < cols; ++i ) {
                matr[i][j]*=c;
            }
        }
    }
    bool operator!=(const Matrix& otherM) const
    {
        if ( this == &otherM ) return true;
        if ( row_count != otherM.getRows() ) return true;
        size_t cols=getCols();
        if ( cols != otherM.getCols() ) return true;
        for ( size_t i=0; i < row_count; ++i ) {
            for ( size_t j=0; j < cols; ++j ) {
                if ( matr[i][j] != otherM[i][j] ) return true;
            }
        }
        return false;
    }
    bool operator==(const Matrix& otherM) const
    {
        return !( ( *this ) != otherM );
    }

};

