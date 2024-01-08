#include <iostream>
#include <omp.h>
using namespace std;
int** initMatrix(int size);
void deleteMatrix(int** matrix, int size);

int main()
{
    const int matrix_size = 2000;
    int** a = initMatrix(matrix_size),
        ** b = initMatrix(matrix_size),
        ** res = initMatrix(matrix_size);

    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            res[i][j] = 0;
        }
    }
    cout << "Generatig started\n";
    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            a[i][j] = rand() % 10;
            b[i][j] = rand() % 10;
        }
    }
    cout << "Generating ended\n";
    const int threads = 6;
    omp_set_num_threads(threads);
    cout << "Multiplication started\n";
    double time = omp_get_wtime();
    int n, m, k;
#pragma omp parallel num_threads(threads) private(n,m,k)
    {
#pragma omp for 
        for (int n = 0; n < matrix_size; ++n) {
            for (int m = 0; m < matrix_size; ++m) {
                for (int k = 0; k < matrix_size; ++k) {
                    res[n][m] += a[n][k] * b[k][m];
                }
            }
        }
    }
    cout << "Multiplication ended.\nTime: " << omp_get_wtime() - time;

    deleteMatrix(a, matrix_size);
    deleteMatrix(b, matrix_size);
    deleteMatrix(res, matrix_size);
    return 0;
}

int** initMatrix(int size) {
    int** res = new int* [size];
    for (int i = 0; i < size; ++i) {
        res[i] = new int[size];
    }
    return res;
}

void deleteMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}