#include <stdio.h>
#include "../include/timer.h"

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int comm_sz, my_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    Timer timer;
    timer_start(&timer);

    // !TODO: Add task 1 functionality here

    timer_stop(&timer);
    double elapsed = time_elapsed(&timer);


    MPI_Finalize();
    return 0;
}