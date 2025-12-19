#include <stdio.h>
#include <stdlib.h>
#include "../include/timer.h"
#include <time.h>
#include <math.h>


int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int comm_size, my_rank;

    long long total_points = 10000;
    if (argc > 1) {
        total_points = atoll(argv[1]);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    long long local_points = total_points / comm_size;
    
    Timer timer;
    srand(time(NULL));
    
    MPI_Barrier(MPI_COMM_WORLD);

    timer_start(&timer);

    long long local_inside = 0;
    long long total_inside = 0;

    for (long long i = 0; i < local_points; ++i)
    {
        long double x = (long double)rand() / RAND_MAX * 2.0 - 1.0;
        long double y = (long double)rand() / RAND_MAX * 2.0 - 1.0;
        
        if (x * x + y * y <= 1)
        {
            ++local_inside;
        }
    }
    
    MPI_Reduce(&local_inside, &total_inside, 1, MPI_LONG_LONG,
        MPI_SUM, 0, MPI_COMM_WORLD);

    timer_stop(&timer);
    double elapsed = time_elapsed(&timer);

    if (my_rank == 0) {
        long double pi_estimate = 4.0 * (double)total_inside / total_points;
        printf("============================================\n");
        printf("Общее количество точек: %lld\n", total_points);
        printf("Количество процессов: %d\n", comm_size);
        printf("Точек в круге: %lld\n", total_inside);
        printf("Вычисленное π = %.10Lf\n", pi_estimate);
        printf("Погрешность: %.10Lf\n", pi_estimate - M_PI);
        printf("Время выполнения: %.6f секунд\n", elapsed);
        printf("============================================\n");
    }

    MPI_Finalize();
    return 0;
}
