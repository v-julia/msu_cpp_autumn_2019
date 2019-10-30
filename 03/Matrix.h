#pragma once

#include <stdexcept>


class Matrix
{
private:
    class c_row
    {
    public:
        c_row(size_t cols)
        {
            if ( cols < 1 ) throw std::out_of_range("");
            row=static_cast<int*>( std::malloc(cols * sizeof(int)) );
            col_count=cols;

        };
        ~c_row()
        {
            free(row);
        };
        int& operator[](size_t i)
        {
            if ( i<1 || i> col_count ) throw std::out_of_range("");
            return *( row + i - 1 );
        }
        size_t getCols() { return col_count; }

    private:
        int* row;
        int col_count;
    };
public:
    Matrix(size_t rows, size_t cols)
    {
        if ( rows < 1 || cols < 1 ) throw std::out_of_range("");

        matr=static_cast<c_row**>( std::malloc(rows * sizeof(c_row*)) );
        row_count=rows;
        for ( int i=0; i < row_count; ++i ) {
            *( matr + i )=new c_row(cols);
        }

    }
    ~Matrix()
    {
        for ( int i=0; i < row_count; ++i ) {
            free(*( matr + i ));
        }

    }

    size_t getRows() const { return row_count; }
    size_t getCols() const { return ( *matr )->getCols(); }

    c_row& operator[](size_t i)
    {
        if ( i<1 || i> row_count ) throw std::out_of_range("");
        return **( matr + i - 1 );
    }
    
    Matrix& operator*=(const int& c)
    {
        size_t cols=getCols();
        for ( int j=1; j <= row_count; ++j ) {
            for ( int i=1; i <= cols; ++i ) {
                ( *this )[i][j]*=c;
            }
        }
    }
    Matrix& operator+=(const int& c)
    {
        size_t cols=getCols();
        for ( int j=1; j <= row_count; ++j ) {
            for ( int i=1; i <= cols; ++i ) {
                ( *this )[i][j]+=c;
            }
        }
    }
    bool operator!=( Matrix& otherM) 
    {
        if ( this == &otherM ) return true;
        if ( row_count != otherM.getRows() ) return true;
        size_t cols=getCols();
        if( cols!= otherM.getCols() ) return true;
        for ( int i=1; i <= row_count; ++i ) {
            for ( int j=1; j <= cols; ++j ) {
                if( ( *this )[i][j] != otherM[i][j]) return true;
            }
        }
        return false;
    }
    bool operator==(Matrix& otherM)
    {
        return !( ( *this ) != otherM );
    }

private:
    c_row** matr;
    int row_count;

};

