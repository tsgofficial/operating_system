#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_CYLINDERS 5000
#define NUM_REQUESTS 1000

// Function to generate random cylinder requests
void generateRequests(int *requests, int num_requests)
{
    for (int i = 0; i < num_requests; i++)
    {
        requests[i] = rand() % NUM_CYLINDERS;
    }
}

// Comparison function for qsort (used to sort the requests in ascending order)
int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

// FCFS Disk Scheduling Algorithm
int fcfs(int *requests, int num_requests, int start_position)
{
    int total_movement = 0;
    int current_position = start_position;

    for (int i = 0; i < num_requests; i++)
    {
        total_movement += abs(requests[i] - current_position);
        current_position = requests[i];
    }

    return total_movement;
}

// SCAN Disk Scheduling Algorithm
int scan(int *requests, int num_requests, int start_position)
{
    int total_movement = 0;
    int current_position = start_position;

    qsort(requests, num_requests, sizeof(int), compare);

    int index = 0;
    while (index < num_requests && requests[index] < start_position)
    {
        index++;
    }

    for (int i = index; i < num_requests; i++)
    {
        total_movement += abs(requests[i] - current_position);
        current_position = requests[i];
    }

    total_movement += abs((NUM_CYLINDERS - 1) - current_position);
    current_position = NUM_CYLINDERS - 1;

    for (int i = index - 1; i >= 0; i--)
    {
        total_movement += abs(requests[i] - current_position);
        current_position = requests[i];
    }

    return total_movement;
}

// C-SCAN Disk Scheduling Algorithm
int cscan(int *requests, int num_requests, int start_position)
{
    int total_movement = 0;
    int current_position = start_position;

    qsort(requests, num_requests, sizeof(int), compare);

    int right = -1;
    for (int i = 0; i < num_requests; i++)
    {
        if (requests[i] >= current_position)
        {
            right = i;
            break;
        }
    }

    if (right != -1)
    {
        total_movement += abs(requests[right] - current_position);
        current_position = requests[right];

        for (int i = right + 1; i < num_requests; i++)
        {
            total_movement += abs(requests[i] - current_position);
            current_position = requests[i];
        }
    }

    total_movement += abs(NUM_CYLINDERS - 1 - current_position);
    current_position = 0;

    for (int i = 0; i < right; i++)
    {
        total_movement += abs(requests[i] - current_position);
        current_position = requests[i];
    }

    return total_movement;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <initial_position>\n", argv[0]);
        return 1;
    }

    int start_position = atoi(argv[1]);

    if (start_position < 0 || start_position >= NUM_CYLINDERS)
    {
        printf("Error: The initial position must be between 0 and 4999.\n");
        return 1;
    }

    srand(time(NULL));

    int requests[NUM_REQUESTS];
    generateRequests(requests, NUM_REQUESTS);

    // Apply FCFS algorithm
    int fcfs_faults = fcfs(requests, NUM_REQUESTS, start_position);
    printf("FCFS Total Head Movement: %d\n", fcfs_faults);

    // Apply SCAN algorithm
    int scan_faults = scan(requests, NUM_REQUESTS, start_position);
    printf("SCAN Total Head Movement: %d\n", scan_faults);

    // Apply C-SCAN algorithm
    int cscan_faults = cscan(requests, NUM_REQUESTS, start_position);
    printf("C-SCAN Total Head Movement: %d\n", cscan_faults);

    return 0;
}
