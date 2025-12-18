#include <stdio.h>
#include <pthread.h>

#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;
pthread_mutex_t resource_mutex;

int decrease_count(int count)
{
    pthread_mutex_lock(&resource_mutex);

    if (available_resources < count)
    {
        pthread_mutex_unlock(&resource_mutex);
        return -1;
    }
    else
    {
        available_resources -= count;
        pthread_mutex_unlock(&resource_mutex);
        return 0;
    }
}

int increase_count(int count)
{
    pthread_mutex_lock(&resource_mutex);
    available_resources += count;
    pthread_mutex_unlock(&resource_mutex);
    return 0;
}

int main()
{
    pthread_mutex_init(&resource_mutex, NULL);

    if (decrease_count(3) == 0)
    {
        printf("Successfully obtained 3 resources\n");
    }
    else
    {
        printf("Not enough resources\n");
    }

    increase_count(2);
    printf("Returned 2 resources\n");

    pthread_mutex_destroy(&resource_mutex);
    return 0;
}
