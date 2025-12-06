#include "../include/task2_common.h"

// Calculate sizes for each process to handle
void calc_sizes(int comm_sz, int rows, int cols, int* obj_sizes) {
    for (int i = 0; i < comm_sz; i++) {
        obj_sizes[i] = (rows / comm_sz + (rows % comm_sz > i ? 1 : 0)) * cols;
    }
}

// Calculate displacements indicating starting point for each process
void calc_displs(int comm_sz, int* obj_displs, int* obj_sizes) {
    for (int i = 1; i < comm_sz; i++) {
        obj_displs[i] = obj_sizes[i - 1] + obj_displs[i - 1];
    }
}

// fill matrix and distribute between processes only their own portion of data
void fill_matrix(int my_rank, int rows, int cols, int* matrix, int* matrix_sizes, 
    int* matrix_displs) {
    int* numbers = NULL;
    if (my_rank == 0) {
        // Fill matrix with numbers from 0 to RANDOM_NUMBER_LIMITER-1
        int matrix_elem_qty = rows * cols;
        numbers = calloc(matrix_elem_qty, sizeof(int));
        for (int i = 0;  i < matrix_elem_qty; i++) {
            numbers[i] = rand() % RANDOM_NUMBER_LIMITER;
        }
    }

    // Scatter data between processes
    MPI_Scatterv(numbers, matrix_sizes, matrix_displs, MPI_INT, matrix, 
        matrix_sizes[my_rank], MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        free(numbers);
    }
}

// Fill vector and spread to other processes
void fill_vector(int my_rank, int vector_size, int* vector) {
    if (my_rank == 0) {
            // Fill the vector with numbers from 0 to RANDOM_NUMBER_LIMITER-1
            for (int i = 0; i < vector_size; i++) {
                vector[i] = rand() % RANDOM_NUMBER_LIMITER;
            }
    }
    // Spread to other processes
    MPI_Bcast(vector, vector_size, MPI_INT, 0, MPI_COMM_WORLD);
}