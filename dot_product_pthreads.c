// Bradley Stephen | 20207842
// Macnine Problem 1 - Part 1 - a
// April 4th, 2025
// dot_product_pthreads.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// will adjust these in other cases
#define VECTOR_SIZE 1000   
#define NUM_THREADS 8

double *A, *B;             
double dot_product = 0.0;  
pthread_mutex_t mutex;      

typedef struct {
    int start;
    int end;
} ThreadData;

void* dot_product_thread(void* arg) {
    ThreadData *data = (ThreadData*) arg;
    double partial_sum = 0.0;
    for (int i = data->start; i < data->end; i++) {
        partial_sum += A[i] * B[i];
    }
    // Update the shared dot_product variable safely
    pthread_mutex_lock(&mutex);
    dot_product += partial_sum;
    pthread_mutex_unlock(&mutex);
    free(data);
    return NULL;
}

int main() {
    // Allocate memory for vectors A and B
    A = (double*) malloc(VECTOR_SIZE * sizeof(double));
    B = (double*) malloc(VECTOR_SIZE * sizeof(double));
    if (!A || !B) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize vectors (here, every element is 1.0)
    for (int i = 0; i < VECTOR_SIZE; i++) {
        A[i] = 1.0;
        B[i] = 1.0;
    }

    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    int chunk_size = VECTOR_SIZE / NUM_THREADS;
    for (int t = 0; t < NUM_THREADS; t++) {
        ThreadData *data = (ThreadData*) malloc(sizeof(ThreadData));
        data->start = t * chunk_size;
        data->end = (t == NUM_THREADS - 1) ? VECTOR_SIZE : (t + 1) * chunk_size;
        pthread_create(&threads[t], NULL, dot_product_thread, data);
    }
    
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&mutex);

    // Compute sequential dot product for verification
    double seq_dot = 0.0;
    for (int i = 0; i < VECTOR_SIZE; i++) {
        seq_dot += A[i] * B[i];
    }

    printf("Pthreads Dot Product: %f\n", dot_product);
    printf("Sequential Dot Product: %f\n", seq_dot);

    free(A);
    free(B);
    return 0;
}
