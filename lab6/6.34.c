#include <stdio.h>
#include <pthread.h>

#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;
pthread_mutex_t resource_mutex;
pthread_cond_t resource_cond;

/* decrease available resources by count resources */
void decrease_count(int count)
{
    pthread_mutex_lock(&resource_mutex);

    while (available_resources < count)
    {

        pthread_cond_wait(&resource_cond, &resource_mutex);
    }

    available_resources -= count;
    pthread_mutex_unlock(&resource_mutex);
}

/* increase available resources by count */
void increase_count(int count)
{
    pthread_mutex_lock(&resource_mutex);
    available_resources += count;

    pthread_cond_signal(&resource_cond);

    pthread_mutex_unlock(&resource_mutex);
}

int main()
{
    pthread_mutex_init(&resource_mutex, NULL);
    pthread_cond_init(&resource_cond, NULL);

    decrease_count(3);
    printf("Successfully obtained 3 resources\n");

    increase_count(2);
    printf("Returned 2 resources\n");

    pthread_mutex_destroy(&resource_mutex);
    pthread_cond_destroy(&resource_cond);
    return 0;
}
