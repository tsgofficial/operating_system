#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <num_points>\n", argv[0]);
        return 1;
    }
    long total_points = atol(argv[1]);
    long inside = 0;

#pragma omp parallel
    {
        unsigned int seed = time(NULL) ^ omp_get_thread_num();
        long local_inside = 0;

#pragma omp for
        for (long i = 0; i < total_points; i++)
        {
            double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            if (x * x + y * y <= 1.0)
                local_inside++;
        }

#pragma omp atomic
        inside += local_inside;
    }

    double pi = 4.0 * inside / total_points;
    printf("Estimated π = %f\n", pi);
    return 0;
}
