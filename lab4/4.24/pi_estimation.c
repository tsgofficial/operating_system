#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long total_points;
long inside_circle = 0;

void *monte_carlo(void *arg)
{
    unsigned int seed = (unsigned int)(time(NULL) ^ (uintptr_t)pthread_self());
    long count = 0;
    for (long i = 0; i < total_points; i++)
    {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0)
            count++;
    }
    inside_circle = count;
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <num_points>\n", argv[0]);
        return 1;
    }
    total_points = atol(argv[1]);

    pthread_t tid;
    pthread_create(&tid, NULL, monte_carlo, NULL);
    pthread_join(tid, NULL);

    double pi = 4.0 * inside_circle / total_points;
    printf("Estimated π = %f\n", pi);

    return 0;
}
