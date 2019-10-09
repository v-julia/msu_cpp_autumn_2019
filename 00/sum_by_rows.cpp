#include "sums.h"



int main()
{
    int A[matrix_dim][matrix_dim];
    int sum = 0;
    Timer t;
    for (int i = 0; i < matrix_dim; i++) {
        for (int j = 0; j < matrix_dim; j++) {
            A[i][j] = 1;
        };
    };

    for (int i = 0; i < matrix_dim; i++) {
        for (int j = 0; j < matrix_dim; j++) {
            sum += A[i][j];
        };
    };

    return 0;
}

