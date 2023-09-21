#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler(int signum)
{
    //printf("Bye, World !\n");
    exit(0);
}

int main()
{
    pid_t pid;
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) // 子进程
    {
        signal(SIGUSR1, handler); // 注册信号处理函数

        while (1)
        {
            printf("I am Child Process, alive !\n");
            sleep(2);
        }
    }
    else // 父进程
    {
        char answer;
        do
        {
            printf("To terminate Child Process. Yes or No? ");
            scanf(" %c", &answer);
            if (answer == 'N' || answer == 'n')
            {
                sleep(2);
            }
            else if (answer == 'Y' || answer == 'y')
            {
                kill(pid, SIGUSR1);
                wait(NULL);
                exit(0);
            }
        } while (answer != 'Y' && answer != 'y');
    }

    return 0;
}
