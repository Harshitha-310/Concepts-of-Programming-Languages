#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define MAX_MEMORY 1024  // Total memory size
#define MIN_BLOCK_SIZE 16 // Smallest allocatable block size

typedef struct Block {
    size_t size;    
    int free;       // Free (1) or Allocated (0)
    struct Block *next; 
} Block;

Block *freeList[11]; // Free lists for sizes 2^4 (16B) to 2^10 (1024B)


void initialize_memory() {
    for (int i = 0; i < 11; i++)
        freeList[i] = NULL;

    Block *initialBlock = (Block *)malloc(sizeof(Block));
    if (!initialBlock) {
        printf("Memory initialization failed!\n");
        exit(1);
    }

    initialBlock->size = MAX_MEMORY;
    initialBlock->free = 1;
    initialBlock->next = NULL;
    freeList[10] = initialBlock; 
}


size_t nextPowerOf2(size_t n) {
    if (n < MIN_BLOCK_SIZE) return MIN_BLOCK_SIZE;
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
}


void split_block(int index) {
    if (!freeList[index]) return;

    Block *block = freeList[index];
    freeList[index] = block->next;

    size_t newSize = block->size / 2;

    Block *buddy = (Block *)malloc(sizeof(Block));
    if (!buddy) {
        printf("Memory allocation failed during splitting.\n");
        return;
    }

    buddy->size = newSize;
    buddy->free = 1;
    buddy->next = freeList[index - 1];

    block->size = newSize;
    block->free = 1;

    freeList[index - 1] = block;
    freeList[index - 1]->next = buddy;
}


void *allocate(size_t size) {
    if (size <= 0 || size > MAX_MEMORY) return NULL;

    size = nextPowerOf2(size);
    int index = log2(size) - log2(MIN_BLOCK_SIZE);

    int i = index;
    while (i < 11 && freeList[i] == NULL)
        i++;

    if (i == 11) {
        printf("No sufficient memory available.\n");
        return NULL;
    }

    while (i > index) {
        split_block(i);
        i--;
    }

    Block *allocatedBlock = freeList[index];
    freeList[index] = allocatedBlock->next;
    allocatedBlock->free = 0;

    printf("Allocated %zu bytes at address %p\n", allocatedBlock->size, (void *)allocatedBlock);
    return (void *)allocatedBlock;
}


void merge_buddies(Block *block, int index) {
    Block *prev = NULL, *curr = freeList[index];

    while (curr) {
        if (curr->size == block->size && curr->free) {
            if ((uintptr_t)curr == (uintptr_t)block + block->size ||
                (uintptr_t)block == (uintptr_t)curr + curr->size) {
                
                if (prev) prev->next = curr->next;
                else freeList[index] = curr->next;

                block->size *= 2;
                merge_buddies(block, index + 1);
                return;
            }
        }
        prev = curr;
        curr = curr->next;
    }

    block->next = freeList[index];
    freeList[index] = block;
}


void free_memory(void *ptr) {
    if (!ptr) return;

    Block *block = (Block *)ptr;
    block->free = 1;

    int index = log2(block->size) - log2(MIN_BLOCK_SIZE);
    merge_buddies(block, index);

    printf("Freed memory at address %p\n", ptr);
}


int main() {
    initialize_memory();

    void *allocatedBlocks[100] = {NULL}; // Initialize all to NULL
    int count = 0;
    int choice;
    size_t size;
    void *ptr;

    while (1) {
        printf("\nBuddy System Memory Management\n");
        printf("1. Allocate Memory\n");
        printf("2. Free Memory\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the size of memory to allocate: ");
                scanf("%zu", &size);
                ptr = allocate(size);
                if (ptr) {
                    allocatedBlocks[count++] = ptr;
                }
                break;

            case 2:
                printf("Allocated addresses:\n");
                for (int i = 0; i < count; i++) {
                    if (allocatedBlocks[i] != NULL) {
                        printf("%d. %p\n", i + 1, allocatedBlocks[i]);
                    }
                }
                printf("Enter index of address to free: ");
                int index;
                scanf("%d", &index);
                if (index > 0 && index <= count && allocatedBlocks[index - 1] != NULL) {
                    free_memory(allocatedBlocks[index - 1]);
                    allocatedBlocks[index - 1] = NULL;
                } else {
                    printf("Invalid index.\n");
                }
                break;

            case 3:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice, try again.\n");
        }
    }

    return 0;
}
