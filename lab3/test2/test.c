#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#define PAGE_SIZE 4096
#define PAGE_SHIFT 12
#define PAGEMAP_LENGTH 8

int main() {
    pid_t pid = getpid();
    char pagemap_path[50];
    sprintf(pagemap_path, "/proc/%d/pagemap", pid);
    FILE *pagemap_file = fopen(pagemap_path, "rb");

    uintptr_t virtual_address = (uintptr_t)&main; // 以main函数为例
    uintptr_t page_frame_number = 0;
    int page_offset = virtual_address % PAGE_SIZE;

    off_t offset = (virtual_address / PAGE_SIZE) * PAGEMAP_LENGTH;
    if (fseek(pagemap_file, offset, SEEK_SET) != 0) {
        perror("fseek");
        exit(EXIT_FAILURE);
    }

    uint64_t pagemap_entry;
    if (fread(&pagemap_entry, 1, PAGEMAP_LENGTH, pagemap_file) != PAGEMAP_LENGTH) {
        perror("fread");
        exit(EXIT_FAILURE);
    }

    if (pagemap_entry & (1ULL << 63)) {
        printf("Page present\n");
        page_frame_number = pagemap_entry & ((1ULL << 55) - 1);
    } else {
        printf("Page not present\n");
    }


    uintptr_t physical_address = (page_frame_number << PAGE_SHIFT) + page_offset;
    printf("Virtual address: %p\n", (void*)virtual_address);
    printf("Physical address: %p\n", (void*)physical_address);
    printf("Page frame number: %lx\n", page_frame_number);
    printf("Page offset: %d\n", page_offset);

    fclose(pagemap_file);
    return 0;
}

