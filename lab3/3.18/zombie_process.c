// zombie_demo.c
// Creates a child that becomes a zombie for at least 10 seconds.

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

int main(void)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        // Child: exit immediately -> becomes zombie after exit
        // because parent hasn't waited yet.
        _Exit(0); // _Exit to avoid flushing stdio twice
    }

    // Parent: hold off on waiting so the child stays zombie.
    printf("Parent PID: %d\n", getpid());
    printf("Child  PID: %d (will be a zombie now)\n", pid);
    printf("\nFor the next 15 seconds, run in another terminal:\n");
    printf("    ps -l | grep ' %d '\n", pid);
    printf("and look for S column = 'Z' and PPID = %d.\n", getpid());
    printf("\nTo forcibly remove the zombie early, kill the parent:\n");
    printf("    kill -9 %d\n\n", getpid());

    // Keep the child as a zombie for at least 15 seconds.
    sleep(15);

    // After the demo window, reap the child so we leave no mess.
    // Using waitpid with WNOHANG in case the child was already reaped by some policy.
    int status;
    pid_t w = waitpid(pid, &status, WNOHANG);
    if (w == 0)
    {
        // Child still a zombie; reap it now (blocking wait).
        waitpid(pid, &status, 0);
    }

    printf("Parent reaped child. Exiting cleanly.\n");
    return 0;
}
