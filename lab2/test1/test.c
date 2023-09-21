#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
int count = 1000;

void* threadA(void* arg)
{
    while (count >= 1)
    {
        pthread_mutex_lock(&mutex);
        printf("A:%04d\n", count--);
        pthread_mutex_unlock(&mutex);
        usleep(200000); // 200ms
    }
    pthread_exit(NULL);
}

void* threadB(void* arg)
{
    int i = 1;
    while (i <= 1000)
    {
        pthread_mutex_lock(&mutex);
        printf("B:%04d\n", i++);
        pthread_mutex_unlock(&mutex);
        usleep(200000); // 200ms
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t tidA, tidB;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&tidA, NULL, threadA, NULL);
    pthread_create(&tidB, NULL, threadB, NULL);

    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}

