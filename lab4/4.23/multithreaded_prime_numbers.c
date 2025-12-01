#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

int limit;

bool is_prime(int n)
{
    if (n < 2)
        return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;
    return true;
}

void *print_primes(void *arg)
{
    for (int i = 2; i <= limit; i++)
        if (is_prime(i))
            printf("%d ", i);
    printf("\n");
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }
    limit = atoi(argv[1]);

    pthread_t tid;
    pthread_create(&tid, NULL, print_primes, NULL);
    pthread_join(tid, NULL);

    return 0;
}
