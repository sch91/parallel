#include <mpi.h>
#include <iostream>
#include <vector>
#include <future>
using namespace std;
int** initMatrix(int size);
void deleteMatrix(int** matrix, int size);

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    double startTime;
    const int matrix_size = 2000;

    int** a = initMatrix(matrix_size),
        ** b = initMatrix(matrix_size),
        ** res = initMatrix(matrix_size);

    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            res[i][j] = 0;
        }
    }
    const int matrix_a_tag = 1, matrix_b_tag = 2, matrix_res_tag = 3;
    if (rank == 0) {
        cout << "Generatig started\n";
        for (int i = 0; i < matrix_size; ++i) {
            for (int j = 0; j < matrix_size; ++j) {
                a[i][j] = rand() % 10;
                b[i][j] = rand() % 10;
            }
        }
        cout << "Generating ended\n";
        cout << "Multiplication started\n";
        startTime = MPI_Wtime();

        for (int i = 1; i < size; ++i)
        {
            MPI_Send(&a, matrix_size * matrix_size, MPI_INT, i, matrix_a_tag, MPI_COMM_WORLD);
            MPI_Send(&b, matrix_size * matrix_size, MPI_INT, i, matrix_b_tag, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(&a, matrix_size * matrix_size, MPI_INT, 0, matrix_a_tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, matrix_size * matrix_size, MPI_INT, 0, matrix_b_tag, MPI_COMM_WORLD, &status);
    }
    for (int i = matrix_size / size * rank; i < matrix_size / size * (rank + 1); ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            res[i][j] = 0;
            for (int k = matrix_size / size * rank; k < matrix_size / size * (rank + 1); ++k) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    if (rank != 0) {
        MPI_Send(&res, matrix_size * matrix_size, MPI_INT, 0, matrix_res_tag, MPI_COMM_WORLD);
    }
    else {
        for (int i = 1; i < size; ++i) {
            MPI_Recv(&res, matrix_size * matrix_size, MPI_INT, i, matrix_res_tag, MPI_COMM_WORLD, &status);
        }
        cout << "Multiplication ended.\ntime spent: " << MPI_Wtime() - startTime << "\n";
    }
    deleteMatrix(a, matrix_size);
    deleteMatrix(b, matrix_size);
    deleteMatrix(res, matrix_size);
    MPI_Finalize();
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