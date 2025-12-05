#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/timer.h"
#define RANDOM_NUMBER_LIMITER 10
#define DEFAULT_VECTOR_COL_SIZE 1

void calc_sizes(int comm_sz, long rows, long cols, long* obj_sizes);
void calc_displs(int comm_sz, long* obj_displs, long* obj_sizes);
void fill_matrix(int my_rank, long rows, long cols, long* matrix, long* matrix_sizes, long* matrix_displs);
void fill_vector(int comm_sz, long vector_size, long* vector);

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "ERROR! Not enough arguments\n");
        fprintf(stderr, "Usage: mpiexec -n [PROCESS QTY] task2_rows [ROWS] [COLUMNS]\n");
        return -1;
    }

    long rows, columns;
    int comm_sz, my_rank;

    rows = atol(argv[1]);
    columns = atol(argv[2]);

    // Initialized MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); // process count
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // current process

    // Assigning memory for size per each process
    long* matrix_sizes = calloc(comm_sz, sizeof(long));
    long* vector_sizes = calloc(comm_sz, sizeof(long));

    // Assigning memory for displacements for each process
    long* matrix_displs = calloc(comm_sz, sizeof(long));
    long* vector_displs = calloc(comm_sz, sizeof(long)); 

    calc_sizes(comm_sz, rows, columns, matrix_sizes);
    calc_sizes(comm_sz, rows, DEFAULT_VECTOR_COL_SIZE, vector_sizes);

    calc_displs(comm_sz, matrix_displs, matrix_sizes);
    calc_displs(comm_sz, vector_displs, vector_sizes);

    // Assigning memory to matrix and vector based on process distribution
    long* matrix = calloc(matrix_sizes[my_rank], sizeof(long));
    long* vector = calloc(columns, sizeof(long));

    srand(time(NULL));

    // Fill with numbers
    fill_matrix(my_rank, rows, columns, matrix, matrix_sizes, matrix_displs);
    fill_vector(my_rank, columns, vector);

    
    // Timer timer;
    // timer_start(&timer);

    // MPI_Barrier(MPI_COMM_WORLD);

    // timer_stop(&timer);
    // double elapsed = time_elapsed(&timer);


    free(matrix_sizes);
    free(vector_sizes);
    free(matrix_displs);
    free(vector_displs);

    MPI_Finalize();
    return 0;
}

// Calculate sizes for each process to handle
void calc_sizes(int comm_sz, long rows, long cols, long* obj_sizes) {
    for (int i = 0; i < comm_sz; i++) {
        obj_sizes[i] = (rows / comm_sz + (rows % comm_sz > i ? 1 : 0)) * cols;
    }
}

// Calculate displacements indicating starting point for each process
void calc_displs(int comm_sz, long* obj_displs, long* obj_sizes) {
    for (int i = 1; i < comm_sz; i++) {
        obj_displs[i] = obj_sizes[i - 1] + obj_displs[i - 1];
    }
}

// fill matrix and distribute between processes only their own portion of data
void fill_matrix(
    int my_rank,
    long rows,
    long cols,
    long* matrix,
    long* matrix_sizes,
    long* matrix_displs
) {
    long* numbers = NULL;
    if (my_rank == 0) {
        // Fill matrix with numbers from 0 to RANDOM_NUMBER_LIMITER-1
        long matrix_elem_qty = rows * cols;
        numbers = calloc(matrix_elem_qty, sizeof(long));
        for (long i = 0;  i < matrix_elem_qty; i++) {
            numbers[i] = rand() % RANDOM_NUMBER_LIMITER;
        }
    }

    // Scatter data between processes
    MPI_Scatterv(
        numbers, 
        matrix_sizes, 
        matrix_displs, 
        MPI_LONG, 
        matrix, 
        matrix_sizes[my_rank], 
        MPI_LONG, 
        0, 
        MPI_COMM_WORLD
    );

    if (my_rank == 0) {
        free(numbers);
    }
}

// Fill vector and spread to other processes
void fill_vector(int my_rank, long vector_size, long* vector) {
    if (my_rank == 0) {
            // Fill the vector with numbers from 0 to RANDOM_NUMBER_LIMITER-1
            for (long i = 0; i < vector_size; i++) {
                vector[i] = rand() % RANDOM_NUMBER_LIMITER;
            }
    }
    // Spread to other processes
    MPI_Bcast(vector, vector_size, MPI_LONG, 0, MPI_COMM_WORLD);
}