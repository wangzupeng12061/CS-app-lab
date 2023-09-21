#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#define PAGE_SIZE 4096
#define PAGE_SHIFT 12
#define PAGEMAP_LENGTH 8

uintptr_t get_physical_address(pid_t pid, uintptr_t virtual_address) {
    char pagemap_path[50];
    sprintf(pagemap_path, "/proc/%d/pagemap", pid);
    FILE *pagemap_file = fopen(pagemap_path, "rb");

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
        page_frame_number = pagemap_entry & ((1ULL << 55) - 1);
    } else {
        fprintf(stderr, "Page not present\n");
        exit(EXIT_FAILURE);
    }

    fclose(pagemap_file);
    return (page_frame_number << PAGE_SHIFT) + page_offset;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pid> <virtual_address>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    uintptr_t virtual_address = strtol(argv[2], NULL, 16);

    uintptr_t physical_address = get_physical_address(pid, virtual_address);
    printf("Virtual address: %p\n", (void*)virtual_address);
    printf("Physical address: %p\n", (void*)physical_address);

    return 0;
}

