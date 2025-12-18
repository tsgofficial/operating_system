#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

int main(void)
{
    pid_t child_pid = fork();
    if (child_pid == 0)
    {
        _Exit(0);
    }

    // Parent process
    printf("Parent PID: %d\n", getpid());
    printf("Child PID: %d (this child will become a zombie)\n", child_pid);
    printf("\nFor the next 15 seconds, run in another terminal:\n");
    printf("    ps -l | grep ' %d '\n", child_pid);
    printf("and check for the 'Z' state in the S column and PPID = %d.\n", getpid());
    printf("\nTo forcefully remove the zombie early, kill the parent process:\n");
    printf("    kill -9 %d\n\n", getpid());

    sleep(15); // Allow time for the child to become a zombie

    int exit_status;
    pid_t reaped_pid = waitpid(child_pid, &exit_status, WNOHANG);
    if (reaped_pid == 0)
    {
        waitpid(child_pid, &exit_status, 0); // Block until the child is reaped
    }

    printf("Parent has reaped the child process. Exiting cleanly.\n");
    return 0;
}
