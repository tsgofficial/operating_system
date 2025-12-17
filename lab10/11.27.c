#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_CYLINDERS 5000 // Number of cylinders (0 to 4999)
#define NUM_REQUESTS 1000  // Number of cylinder requests

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

    // Sort requests in ascending order
    qsort(requests, num_requests, sizeof(int), compare);

    // Debug: Print the sorted requests for SCAN
    // printf("SCAN Sorted Requests: ");
    // for (int i = 0; i < num_requests; i++)
    // {
    //     printf("%d ", requests[i]);
    // }
    // printf("\n");

    // Find the closest request in the direction of movement (towards 4999)
    int right = -1;
    for (int i = 0; i < num_requests; i++)
    {
        if (requests[i] >= current_position)
        {
            right = i;
            break;
        }
    }

    // Calculate movement to the rightmost request
    if (right != -1)
    {
        total_movement += abs(requests[right] - current_position);
        current_position = requests[right];
    }

    // Reverse direction and service requests on the way back
    for (int i = right + 1; i < num_requests; i++)
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

    // Sort the requests in ascending order
    qsort(requests, num_requests, sizeof(int), compare);

    // Find the index of the first request that is greater than or equal to the current position
    int right = -1;
    for (int i = 0; i < num_requests; i++)
    {
        if (requests[i] >= current_position)
        {
            right = i;
            break;
        }
    }

    // Step 1: Service requests from the current position to the highest cylinder
    if (right != -1) // If there is a request greater than or equal to the current position
    {
        total_movement += abs(requests[right] - current_position); // Move to the first request in the right direction
        current_position = requests[right];                        // Update the head position

        // Service all requests towards the highest cylinder
        for (int i = right + 1; i < num_requests; i++)
        {
            total_movement += abs(requests[i] - current_position); // Move to each next request
            current_position = requests[i];                        // Update head position
        }
    }

    // Step 2: Jump back to cylinder 0 (the beginning of the disk)
    total_movement += abs(NUM_CYLINDERS - 1 - current_position); // Jump to cylinder 0
    current_position = 0;                                        // Update the head position to cylinder 0

    // Step 3: Service the remaining requests from cylinder 0 to the highest requested cylinder
    for (int i = 0; i < right; i++) // Service the remaining requests that are before the current head position (0)
    {
        total_movement += abs(requests[i] - current_position); // Move to each request
        current_position = requests[i];                        // Update head position
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

    // Generate a random page-reference string
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
