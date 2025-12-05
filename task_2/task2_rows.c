#include <stdio.h>
#include <stdlib.h>
#include "../include/timer.h"

void calc_sizes(int comm_sz, long rows, long cols, long* obj_sizes);
void calc_displs(int comm_sz, long* obj_displs, long* obj_sizes);

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "ERROR! Not enough arguments\nUsage: mpiexec -n [PROCESS QTY] task2_rows [ROWS] [COLUMNS]\n");
        return 1;
    }

    long rows, columns;
    int comm_sz, my_rank;

    rows = atol(argv[1]);
    columns = atol(argv[2]);

    // Initialized MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); // process count
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // current process

    // size per each process
    long* matrix_sizes = calloc(comm_sz, sizeof(long));
    long* vector_sizes = calloc(comm_sz, sizeof(long));

    // displacements for each process
    long* matrix_displs = calloc(comm_sz, sizeof(long));
    long* vector_displs = calloc(comm_sz, sizeof(long)); 

    calc_sizes(comm_sz, rows, columns, matrix_sizes);
    calc_sizes(comm_sz, rows, 1, vector_sizes);

    calc_displs(comm_sz, matrix_displs, matrix_sizes);
    calc_displs(comm_sz, vector_displs, vector_sizes);

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

// calculate sizes for each process to handle
void calc_sizes(int comm_sz, long rows, long cols, long* obj_sizes) {
    for (int i = 0; i < comm_sz; i++) {
        obj_sizes[i] = (rows / comm_sz + (rows % comm_sz > i ? 1 : 0)) * cols;
    }
}

// calculate displacements indicating starting point for each process
void calc_displs(int comm_sz, long* obj_displs, long* obj_sizes) {
    for (int i = 1; i < comm_sz; i++) {
        obj_displs[i] = obj_sizes[i - 1] + obj_displs[i - 1];
    }
}