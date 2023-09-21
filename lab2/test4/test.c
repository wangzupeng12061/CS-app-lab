#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;
pthread_mutex_t mutex;
sem_t empty, full;

void *producer1(void *arg)
{
    int data;
    while (1)
    {
        data = rand() % 1000 + 1000;
        usleep(rand() % 900000 + 100000); // 随机间隔100ms-1s
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = data;
        printf("Producer 1 produces data %d\n", data);
        in = (in + 1) % BUFFER_SIZE;
        count++;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *producer2(void *arg)
{
    int data;
    while (1)
    {
        data = rand() % 1000 + 2000;
        usleep(rand() % 900000 + 100000); // 随机间隔100ms-1s
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = data;
        printf("Producer 2 produces data %d\n", data);
        in = (in + 1) % BUFFER_SIZE;
        count++;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer1(void *arg)
{
    int data;
    while (1)
    {
        usleep(rand() % 900000 + 100000); // 随机间隔100ms-1s
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        data = buffer[out];
        printf("Consumer 1 consumes data %d\n", data);
        out = (out + 1) % BUFFER_SIZE;
        count--;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

void *consumer2(void *arg)
{
    int data;
    while (1)
    {
        usleep(rand() % 900000 + 100000); // 随机间隔100ms-1s
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        data = buffer[out];
        printf("Consumer 2 consumes data %d\n", data);
        out = (out + 1) % BUFFER_SIZE;
        count--;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

void *consumer3(void *arg)
{
    int data;
    while (1)
    {
        usleep(rand() % 900000 + 100000); // 随机间隔100ms-1s
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        data = buffer[out];
        printf("Consumer 3 consumes data %d\n", data);
        out = (out + 1) % BUFFER_SIZE;
        count--;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main()
{
    pthread_t producer_thread1, producer_thread2, consumer_thread1, consumer_thread2, consumer_thread3;
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&producer_thread1, NULL, producer1, NULL);
    pthread_create(&producer_thread2, NULL, producer2, NULL);
    pthread_create(&consumer_thread1, NULL, consumer1, NULL);
    pthread_create(&consumer_thread2, NULL, consumer2, NULL);
    pthread_create(&consumer_thread3, NULL, consumer3, NULL);
    pthread_join(producer_thread1, NULL);
    pthread_join(producer_thread2, NULL);
    pthread_join(consumer_thread1, NULL);
    pthread_join(consumer_thread2, NULL);
    pthread_join(consumer_thread3, NULL);
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}
