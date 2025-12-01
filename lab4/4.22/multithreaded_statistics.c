#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *numbers;
int count;
double average;
int minimum;
int maximum;

void *calc_average(void *arg)
{
    int sum = 0;
    for (int i = 0; i < count; i++)
        sum += numbers[i];
    average = (double)sum / count;
    pthread_exit(0);
}

void *calc_min(void *arg)
{
    minimum = numbers[0];
    for (int i = 1; i < count; i++)
        if (numbers[i] < minimum)
            minimum = numbers[i];
    pthread_exit(0);
}

void *calc_max(void *arg)
{
    maximum = numbers[0];
    for (int i = 1; i < count; i++)
        if (numbers[i] > maximum)
            maximum = numbers[i];
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <numbers>\n", argv[0]);
        return 1;
    }
    count = argc - 1;
    numbers = malloc(count * sizeof(int));
    for (int i = 0; i < count; i++)
        numbers[i] = atoi(argv[i + 1]);

    pthread_t tid[3];
    pthread_create(&tid[0], NULL, calc_average, NULL);
    pthread_create(&tid[1], NULL, calc_min, NULL);
    pthread_create(&tid[2], NULL, calc_max, NULL);

    for (int i = 0; i < 3; i++)
        pthread_join(tid[i], NULL);

    printf("The average value is %.0f\n", average);
    printf("The minimum value is %d\n", minimum);
    printf("The maximum value is %d\n", maximum);

    free(numbers);
    return 0;
}
