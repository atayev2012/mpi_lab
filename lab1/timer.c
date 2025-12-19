#include "include/timer.h"

void timer_start(Timer* timer) {
    timer->start_time = MPI_Wtime();
}

void timer_stop(Timer* timer) {
    timer->end_time = MPI_Wtime();
}

double time_elapsed(const Timer* timer) {
    return timer->end_time - timer->start_time;
}