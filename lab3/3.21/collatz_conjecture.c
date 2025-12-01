#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    pid_t pid = fork();
    if (pid == 0)
    {
        while (n != 1)
        {
            printf("%d ", n);
            if (n % 2 == 0)
                n /= 2;
            else
                n = 3 * n + 1;
        }
        printf("1\n");
        exit(0);
    }
    else
    {
        wait(NULL);
    }
    return 0;
}
