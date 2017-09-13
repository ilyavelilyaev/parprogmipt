#include <omp.h>
#include <stdio.h>

double **allocate_matrix(int size) {
    double **matrix = (double **)malloc(size * sizeof(double *));
    for (int i = 0; i < size; i ++) {
        matrix[i] = (double *)malloc(size * sizeof(double));
    }
    return matrix;
}

double **clean_matrix(int size) {
    double **matrix = allocate_matrix(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }
    }

    return matrix;
}

double **random_matrix(int size) {
    double **matrix = allocate_matrix(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (double)(arc4random() % 100000);
        }
    }

    return matrix;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: $ ./a.out N n\nN - matrix size\nn - threads count\n");
        return 1;
    }

    int size;
    int n; //threads count

    sscanf(argv[1], "%d", &size);
    sscanf(argv[2], "%d", &n);


    double **A = random_matrix(size);
    double **B = random_matrix(size);

    double **C = clean_matrix(size);

    #pragma omp parallel num_threads(n) 
    {
        #pragma omp for
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                double sum = 0;
                for (int k = 0; k < size; k++) {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
    }

    return 0;
}
