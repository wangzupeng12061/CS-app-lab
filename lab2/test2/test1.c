#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // child process
        printf("Child process: pid=%d, ppid=%d\n", getpid(), getppid());
        while(1);
        exit(0);
    } else {
        // parent process
        printf("Parent process: pid=%d, ppid=%d\n", getpid(), getppid());
        sleep(1); // let parent process exit first
        printf("Parent process exit\n");
    }
    return 0;
}

