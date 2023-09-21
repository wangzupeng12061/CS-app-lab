#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // child process
        printf("Child process: pid=%d, ppid=%d\n", getpid(), getppid());
        sleep(5);
        exit(123);
    } else {
        // parent process
        printf("Parent process: pid=%d, ppid=%d\n", getpid(), getppid());
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Child process exit with status %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}

