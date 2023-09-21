#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

pthread_mutex_t mutex;
pthread_cond_t cond[N];
int state[N];

void test(int i) {
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        printf("哲学家 %d 拿到了左右两只筷子，开始进餐\n", i);
        pthread_cond_signal(&cond[i]);
    }
}

void pickup(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    printf("哲学家 %d 感到饥饿，想要进餐\n", i);
    test(i);
    while (state[i] != EATING) {
        pthread_cond_wait(&cond[i], &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void putdown(int i) {
    pthread_mutex_lock(&mutex);
    printf("哲学家 %d 放下了左右两只筷子，停止进餐\n", i);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    pthread_mutex_unlock(&mutex);
}

void *philosopher(void *arg) {
    int i = *(int *)arg;
    srand(time(NULL));
    while (true) {
        printf("哲学家 %d 开始思考\n", i);
        usleep((rand() % 401 + 100) * 1000);
        pickup(i);
        printf("哲学家 %d 开始进餐\n", i);
        usleep((rand() % 401 + 100) * 1000);
        putdown(i);
    }
}

int main() {
    pthread_t tid[N];
    int id[N];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < N; i++) {
        pthread_cond_init(&cond[i], NULL);
        state[i] = THINKING;
        id[i] = i;
    }
    for (int i = 0; i < N; i++) {
        pthread_create(&tid[i], NULL, philosopher, &id[i]);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
