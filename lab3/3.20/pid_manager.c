#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_PID 300
#define MAX_PID 5000
#define SIZE (MAX_PID - MIN_PID + 1)

static int pid_map[SIZE];

int allocate_map(void)
{
    memset(pid_map, 0, sizeof(pid_map));
    return 1;
}

int allocate_pid(void)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (pid_map[i] == 0)
        {
            pid_map[i] = 1;
            return i + MIN_PID;
        }
    }
    return -1; // no pid available
}

void release_pid(int pid)
{
    if (pid >= MIN_PID && pid <= MAX_PID)
    {
        pid_map[pid - MIN_PID] = 0;
    }
}

int main()
{
    allocate_map();
    int pid1 = allocate_pid();
    int pid2 = allocate_pid();
    printf("Allocated PIDs: %d, %d\n", pid1, pid2);
    release_pid(pid1);
    int pid3 = allocate_pid();
    printf("Reallocated PID: %d\n", pid3);
    return 0;
}
