#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(1);
    }

    struct timeval *start;
    start = mmap(NULL, sizeof(struct timeval),
                 PROT_READ | PROT_WRITE,
                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    pid_t pid = fork();
    if (pid == 0)
    {
        gettimeofday(start, NULL);
        execvp(argv[1], &argv[1]);
        perror("exec failed");
        exit(1);
    }
    else
    {
        wait(NULL);
        struct timeval end;
        gettimeofday(&end, NULL);

        double elapsed = (end.tv_sec - start->tv_sec) +
                         (end.tv_usec - start->tv_usec) / 1e6;
        printf("Elapsed time: %f\n", elapsed);
    }

    munmap(start, sizeof(struct timeval));
    return 0;
}
