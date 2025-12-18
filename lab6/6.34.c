#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;
pthread_mutex_t resource_mutex;
pthread_cond_t resource_cond;

void decrease_count(int count, int thread_id)
{
    pthread_mutex_lock(&resource_mutex);

    printf("[Thread %d] wants %d resources. (Available: %d)\n", thread_id, count, available_resources);

    while (available_resources < count)
    {
        printf("[Thread %d] NOT ENOUGH resources. Going to sleep/waiting...\n", thread_id);

        pthread_cond_wait(&resource_cond, &resource_mutex);

        printf("[Thread %d] Woke up! Checking resources again...\n", thread_id);
    }

    available_resources -= count;
    printf("[Thread %d] SUCCESS: Took %d resources. (Remaining: %d)\n", thread_id, count, available_resources);

    pthread_mutex_unlock(&resource_mutex);
}

void increase_count(int count, int thread_id)
{
    pthread_mutex_lock(&resource_mutex);

    available_resources += count;
    printf("[Thread %d] RETURNED %d resources. (Now Available: %d)\n", thread_id, count, available_resources);

    pthread_cond_broadcast(&resource_cond);

    pthread_mutex_unlock(&resource_mutex);
}

void *consumer_thread(void *arg)
{
    int id = *(int *)arg;
    decrease_count(4, id);
    return NULL;
}

void *provider_thread(void *arg)
{
    int id = *(int *)arg;
    sleep(3);
    increase_count(5, id);
    return NULL;
}

int main()
{
    pthread_t t1, t2, t3;
    int id1 = 1, id2 = 2, id3 = 3;

    pthread_mutex_init(&resource_mutex, NULL);
    pthread_cond_init(&resource_cond, NULL);

    printf("--- System Start: %d Resources available ---\n", MAX_RESOURCES);

    pthread_create(&t1, NULL, consumer_thread, &id1);
    pthread_create(&t2, NULL, consumer_thread, &id2);

    pthread_create(&t3, NULL, provider_thread, &id3);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&resource_mutex);
    pthread_cond_destroy(&resource_cond);

    printf("--- All tasks finished. Final resources: %d ---\n", available_resources);
    return 0;
}