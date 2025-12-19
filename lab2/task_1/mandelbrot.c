#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <omp.h>

#include "../include/timer.h"

static uint64_t g_npoints;
static uint64_t g_mandelbrot_npoints = 0;
static complex double* g_mandelbrot_points_arr;

int is_mandelbrotset(double complex c)
{
    double complex z = 0;

    for (uint16_t i = 0; i < 4000; i++)
    {
        z = z * z + c;
        if (cabs(z) >= 2.0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    double start, finish;

    if (argc != 3)
    {
        fprintf(stderr, "Usage:\n%s [nthreads] [npoints]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int thread_count = strtoll(argv[1], NULL, 10);
    g_npoints = strtoll(argv[2], NULL, 10);

    g_mandelbrot_points_arr =
        malloc(sizeof(complex double) * g_npoints);

    double x_start = -2.0;
    double x_end   =  2.0;
    double dx = 0.00015;
    double dy = 0.00015;

    omp_set_num_threads(thread_count);

    GET_TIME(start);
    int nx = (int)((x_end - x_start) / dx); 
#pragma omp parallel for schedule(static)
   
    for (int ix = 0; ix < nx; ix++)
    {
        double x = x_start + ix * dx;

        for (double y = -1.0; y < 1.0; y += dy)
        {
            if (g_mandelbrot_npoints >= g_npoints)
                continue;

            double complex c = x + y * I;

            if (is_mandelbrotset(c))
            {
#pragma omp critical
                {
                    if (g_mandelbrot_npoints < g_npoints)
                    {
                        g_mandelbrot_points_arr[g_mandelbrot_npoints] = c;
                        g_mandelbrot_npoints++;
                    }
                }
            }
        }
    }

    GET_TIME(finish);

    FILE *file = fopen("output.csv", "w");
    if (file)
    {
        for (uint64_t i = 0; i < g_mandelbrot_npoints; i++)
        {
            fprintf(file, "(%lf, %lf)\n",
                    creal(g_mandelbrot_points_arr[i]),
                    cimag(g_mandelbrot_points_arr[i]));
        }
        fclose(file);
    }
    else
    {
        fprintf(stderr, "Cannot open output.csv\n");
    }

    printf("Result in ./output.csv file\n");
    printf("Time elapsed: %lfs | %d threads | %llu points\n",
           (finish - start), thread_count, g_npoints);

    free(g_mandelbrot_points_arr);
    return EXIT_SUCCESS;
}
