#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGE_SIZE 10  // 页大小
#define FRAME_NUM 3   // 页框数
#define ARRAY_SIZE 2400 // 大数组大小
#define ACCESS_NUM 24000 // 总的访问次数

int a[ARRAY_SIZE]; // 大数组

int page_table[ARRAY_SIZE / PAGE_SIZE]; // 页表

int frames[FRAME_NUM][PAGE_SIZE]; // 页框

int counter[FRAME_NUM]; // 页框的计数器

int find_lru_frame() {
    int lru_frame = 0;
    for (int i = 1; i < FRAME_NUM; i++) {
        if (counter[i] < counter[lru_frame]) {
            lru_frame = i;
        }
    }
    return lru_frame;
}

int main() {
    srand(time(NULL)); // 初始化随机数生成器

    // 初始化大数组
    for (int i = 0; i < ARRAY_SIZE; i++) {
        a[i] = rand() % 100;
    }

    // 初始化页表
    for (int i = 0; i < ARRAY_SIZE / PAGE_SIZE; i++) {
        page_table[i] = -1;
    }

    int miss_count = 0; // 缺页计数器
    int access_count = 0; // 访问计数器

    // 随机访问大数组
    while (access_count < ACCESS_NUM) {
        int page_num = rand() % (ARRAY_SIZE / PAGE_SIZE); // 产生页号
        int frame_num = page_table[page_num]; // 获取页框号

        if (frame_num == -1) { // 页面不在页框中
            miss_count++; // 缺页计数器加1

            if (frame_num == -1) { // 找到一个空闲的页框
                for (int i = 0; i < FRAME_NUM; i++) {
                    if (page_table[i] == -1) {
                        frame_num = i;
                        break;
                    }
                }
            }

            if (frame_num == -1) { // 没有空闲页框，需要进行淘汰
                frame_num = find_lru_frame(); // 找到最近最少使用的页框
                page_table[frames[frame_num][0]] = -1; // 清除页表中的映射
            }

            // 将页面装入页框中
            for (int i = 0; i < PAGE_SIZE; i++) {
                frames[frame_num][i] = a[page_num * PAGE_SIZE + i];
            }
            page_table[page_num] = frame_num; // 更新页表中的映射
        }

        // 记录访问信息，计数器加1
        counter[frame_num] = access_count;
        access_count++;

        // 打印访问的值
        printf("%d ", frames[frame_num][rand() % PAGE_SIZE]);
    }

    printf("\n缺页次数：%d\n", miss_count);

    return 0;
}

