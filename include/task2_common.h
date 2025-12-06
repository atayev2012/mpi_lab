#ifndef TASK2_COMMON_H_
#define TASK2_COMMON_H_

#define RANDOM_NUMBER_LIMITER 10
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>


void calc_sizes(int comm_sz, int rows, int cols, int* obj_sizes);
void calc_displs(int comm_sz, int* obj_displs, int* obj_sizes);
void fill_matrix(int my_rank, int rows, int cols, int* matrix, int* matrix_sizes, int* matrix_displs);
void fill_vector(int comm_sz, int vector_size, int* vector);

#endif