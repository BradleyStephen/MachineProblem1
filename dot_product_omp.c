// Bradley Stephen | 20207842 | 19bbs2
// Machine Problem 1 - Part 1 - b
// April 4th 2025
// dot_product_omp.c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define VECTOR_SIZE 1000
#define NUM_THREADS 8

int main() {
    double *A = (double*) malloc(VECTOR_SIZE * sizeof(double));
    double *B = (double*) malloc(VECTOR_SIZE * sizeof(double));
    double dot_product = 0.0;

    if (!A || !B) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize vectors (all elements set to 1.0)
    for (int i = 0; i < VECTOR_SIZE; i++) {
        A[i] = 1.0;
        B[i] = 1.0;
    }

    omp_set_num_threads(NUM_THREADS);
#pragma omp parallel for reduction(+:dot_product)
    for (int i = 0; i < VECTOR_SIZE; i++) {
        dot_product += A[i] * B[i];
    }

    // Verify against sequential computation
    double seq_dot = 0.0;
    for (int i = 0; i < VECTOR_SIZE; i++) {
        seq_dot += A[i] * B[i];
    }

    printf("OpenMP Dot Product: %f\n", dot_product);
    printf("Sequential Dot Product: %f\n", seq_dot);

    free(A);
    free(B);
    return 0;
}
