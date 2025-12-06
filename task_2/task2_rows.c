#include "../include/timer.h"
#include "../include/task2_common.h"
#define DEFAULT_VECTOR_COL_SIZE 1

void multiply_by_row(int local_rows, int cols, int* matrix, int *vector,
    int* local_result_vector, int* result_vector, int* vector_sizes,
    int* vector_displs, int my_rank);

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "ERROR! Not enough arguments\n");
        fprintf(stderr, "Usage: mpiexec -n [PROCESS QTY] task2_rows [ROWS] [COLUMNS]\n");
        return -1;
    }

    int rows, columns;
    int comm_sz, my_rank;

    rows = atoi(argv[1]);
    columns = atoi(argv[2]);

    // Initialized MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); // process count
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // current process

    // Assigning memory for size per each process
    int* matrix_sizes = calloc(comm_sz, sizeof(int));
    int* vector_sizes = calloc(comm_sz, sizeof(int));

    // Assigning memory for displacements for each process
    int* matrix_displs = calloc(comm_sz, sizeof(int));
    int* vector_displs = calloc(comm_sz, sizeof(int)); 

    calc_sizes(comm_sz, rows, columns, matrix_sizes);
    calc_sizes(comm_sz, rows, DEFAULT_VECTOR_COL_SIZE, vector_sizes);

    calc_displs(comm_sz, matrix_displs, matrix_sizes);
    calc_displs(comm_sz, vector_displs, vector_sizes);

    // Assigning memory to matrix and vector based on process distribution
    int* matrix = calloc(matrix_sizes[my_rank], sizeof(int));
    int* vector = calloc(columns, sizeof(int));

    srand(time(NULL)*13 + my_rank % 10);

    // Fill with numbers
    fill_matrix(my_rank, rows, columns, matrix, matrix_sizes, matrix_displs);
    fill_vector(my_rank, columns, vector);
    
    int local_rows = matrix_sizes[my_rank] / columns;
    int* local_result_vector = calloc(local_rows, sizeof(int));
    int* result_vector = calloc(rows, sizeof(int));

    Timer timer;
    timer_start(&timer);

    multiply_by_row(local_rows, columns, matrix, vector, local_result_vector, 
        result_vector, vector_sizes, vector_displs, my_rank);

    // wait for all processes to finish calculations
    MPI_Barrier(MPI_COMM_WORLD);
    timer_stop(&timer);
    double elapsed = time_elapsed(&timer);

    if (my_rank == 0) {
        // time elapsed, rows, columns, process qty
        printf("%lf, %d, %d, %d\n", elapsed, rows, columns, comm_sz);
    }

    // free dynamic memory
    free(matrix_sizes);
    free(vector_sizes);
    free(matrix_displs);
    free(vector_displs);
    free(local_result_vector);
    free(result_vector);

    MPI_Finalize();
    return 0;
}

// Multiply 
void multiply_by_row(int local_rows, int cols, int *matrix, int *vector, 
    int *local_result_vector, int* result_vector, int* vector_sizes, 
    int* vector_displs, int my_rank) {
    
    for (int i = 0; i < local_rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            local_result_vector[i] += matrix[i * cols + j] * vector[j];
        }
    }

    MPI_Gatherv(local_result_vector, local_rows, MPI_INT, result_vector, 
        vector_sizes, vector_displs, MPI_INT, 0, MPI_COMM_WORLD);
}
