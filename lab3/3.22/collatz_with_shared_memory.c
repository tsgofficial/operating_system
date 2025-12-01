#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <positive_integer>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    if (n <= 0)
    {
        fprintf(stderr, "Argument must be positive.\n");
        return 1;
    }

    int *shared = mmap(NULL, 1024 * sizeof(int),
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    pid_t pid = fork();
    if (pid == 0)
    {
        int i = 0;
        while (n != 1)
        {
            shared[i++] = n;
            if (n % 2 == 0)
                n /= 2;
            else
                n = 3 * n + 1;
        }
        shared[i++] = 1;
        shared[i] = -1; // terminator
        exit(0);
    }
    else
    {
        wait(NULL);
        int i = 0;
        while (shared[i] != -1)
        {
            printf("%d ", shared[i++]);
        }
        printf("\n");
    }
    return 0;
}
