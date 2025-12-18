#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct
{
    char *direction;
    int id;
} FarmerInfo;

pthread_mutex_t bridge_lock;

void *farmer(void *arg)
{
    FarmerInfo *info = (FarmerInfo *)arg;

    printf("Farmer [%s #%d] is waiting to cross.\n", info->direction, info->id);

    pthread_mutex_lock(&bridge_lock);

    printf(">>> Farmer [%s #%d] is ON the bridge.\n", info->direction, info->id);

    int travel_time = rand() % 3 + 1;
    sleep(travel_time);

    printf("<<< Farmer [%s #%d] has finished crossing.\n", info->direction, info->id);

    pthread_mutex_unlock(&bridge_lock);

    free(info);
    pthread_exit(NULL);
}

int main()
{
    int num_farmers = 6;
    pthread_t farmers[num_farmers];
    pthread_mutex_init(&bridge_lock, NULL);
    srand(time(NULL));

    char *north = "North";
    char *south = "South";

    for (int i = 0; i < 3; i++)
    {

        FarmerInfo *infoN = malloc(sizeof(FarmerInfo));
        infoN->direction = north;
        infoN->id = i + 1;
        pthread_create(&farmers[i], NULL, farmer, (void *)infoN);

        FarmerInfo *infoS = malloc(sizeof(FarmerInfo));
        infoS->direction = south;
        infoS->id = i + 1;
        pthread_create(&farmers[i + 3], NULL, farmer, (void *)infoS);
    }

    for (int i = 0; i < num_farmers; i++)
    {
        pthread_join(farmers[i], NULL);
    }

    pthread_mutex_destroy(&bridge_lock);
    printf("All farmers have crossed safely.\n");
    return 0;
}