#ifndef _TIMER_H_
#define _TIMER_H_

#include <mpi.h>

typedef struct {
    double start_time;
    double end_time;
} Timer;

void timer_start(Timer* timer);
void timer_stop(Timer* timer);
double time_elapsed(const Timer* timer);

#endif