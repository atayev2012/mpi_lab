#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "../include/timer.h"

#define G 6.67430e-11
#define DT 0.01

typedef struct {
    double m;
    double x, y, z;
    double vx, vy, vz;
} Body;

int main(int argc, char *argv[])
{
    double start, finish;
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: %s [t_end] [input_file] [nthreads]\n", argv[0]);
        return 1;
    }

    int thread_count = 1;

    if (argc == 4) {
        thread_count = atoi(argv[3]);
    }

    double tend = atof(argv[1]);

    FILE *fin = fopen(argv[2], "r");
    if (!fin) {
        perror("Input file");
        return 1;
    }

    int n;
    fscanf(fin, "%d", &n);

    Body *bodies = malloc(n * sizeof(Body));
    for (int i = 0; i < n; i++) {
        fscanf(fin, "%lf %lf %lf %lf %lf %lf %lf",
               &bodies[i].m,
               &bodies[i].x, &bodies[i].y, &bodies[i].z,
               &bodies[i].vx, &bodies[i].vy, &bodies[i].vz);
    }
    fclose(fin);

    int steps = (int)(tend / DT) + 1;

    // Array of coordin
    double *traj = malloc((size_t)steps * n * 2 * sizeof(double));

    omp_set_num_threads(thread_count);
    GET_TIME(start);

    double t = 0.0;
    for (int step = 0; step < steps; step++) {

        // Saving coordinates
        for (int i = 0; i < n; i++) {
            traj[step * n * 2 + i * 2 + 0] = bodies[i].x;
            traj[step * n * 2 + i * 2 + 1] = bodies[i].y;
        }

        double *fx = calloc(n, sizeof(double));
        double *fy = calloc(n, sizeof(double));
        double *fz = calloc(n, sizeof(double));

#pragma omp parallel for schedule(static)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) continue;

                double dx = bodies[j].x - bodies[i].x;
                double dy = bodies[j].y - bodies[i].y;
                double dz = bodies[j].z - bodies[i].z;

                double dist = sqrt(dx*dx + dy*dy + dz*dz) + 1e-9;
                double coeff = G * bodies[i].m * bodies[j].m /
                               (dist * dist * dist);

                fx[i] += coeff * dx;
                fy[i] += coeff * dy;
                fz[i] += coeff * dz;
            }
        }

#pragma omp parallel for
        for (int i = 0; i < n; i++) {
            bodies[i].vx += fx[i] / bodies[i].m * DT;
            bodies[i].vy += fy[i] / bodies[i].m * DT;
            bodies[i].vz += fz[i] / bodies[i].m * DT;

            bodies[i].x += bodies[i].vx * DT;
            bodies[i].y += bodies[i].vy * DT;
            bodies[i].z += bodies[i].vz * DT;
        }

        free(fx);
        free(fy);
        free(fz);

        t += DT;
    }

    GET_TIME(finish);

    // Output results to CSV
    FILE *fout = fopen("output.csv", "w");
    for (int step = 0; step < steps; step++) {
        fprintf(fout, "%lf", step * DT);
        for (int i = 0; i < n; i++) {
            fprintf(fout, ",%lf,%lf",
                    traj[step * n * 2 + i * 2 + 0],
                    traj[step * n * 2 + i * 2 + 1]);
        }
        fprintf(fout, "\n");
    }
    fclose(fout);

    double elapsed = finish - start;
    printf("Elapsed time: %lfs | Bodies: %d | Threads: %d\n", elapsed, n, thread_count);

    free(traj);
    free(bodies);

    return 0;
}
