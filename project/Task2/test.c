#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void intensive_task(const char* username, int iterations)
{
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    printf("User: %s\n", username);

    // 模拟计算密集型任务
    for (int i = 0; i < iterations; ++i) {
        for (int j = 0; j < 10000000; ++j) {
            // 更复杂的计算操作
            int result = 0;
            for (int k = 0; k < 100; ++k) {
                result += j * k;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    long elapsed_ns = (end_time.tv_sec - start_time.tv_sec) * 1000000000 + (end_time.tv_nsec - start_time.tv_nsec);
    double elapsed_ms = elapsed_ns / 1e6;
    printf("%s: Elapsed time: %.2f milliseconds\n", username, elapsed_ms);
}


int main()
{
    uid_t uid = getuid();

    if (uid == 0) {
        printf("Current user is the root user\n");
    } else if (uid == 1001) {
        printf("Current user is wzp\n");
        intensive_task("wzp", 5);
    } else {
        printf("Current user is a different user\n");
        intensive_task("other", 5);
    }

    return 0;
}

