#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "my_malloc.h"
#define ALIGN(size) ((size + (sizeof(int) - 1)) / sizeof(int) * sizeof(int))

Block *head = NULL;
void* heap_start = NULL;

void split_block(Block *block, size_t size) {
    if (block->size <= size + sizeof(Block)) {
        return;
    }

    Block *new_block = (Block *)((char *)block + sizeof(Block) + size);
    new_block->size = block->size - size - sizeof(Block);
    new_block->free = 0;
    new_block->next = block->next;
    
    block->size = size;
    block->next = new_block;
}

void* get_blocks(size_t size) {
    void* ptr = sbrk(size);
    if (ptr == (void*)-1) {
        fprintf(stderr, "Failed to allocate memory using sbrk\n");
        exit(1);
    }
    return ptr;
}

void* my_malloc(size_t size) {
    size = ALIGN(size);

    Block* current = head;
    Block* prev = NULL;

    while (current) {
        if (current->free && current->size >= size) {
            split_block(current, size);
            current->free = 1;
            return (void*)(current + 1);
        }
        prev = current;
        current = current->next;
    }

    if (heap_start == NULL) {
        heap_start = get_blocks(size);
    }

    Block* new_block = (Block*)heap_start;
    new_block->size = size;
    new_block->free = 0;
    new_block->next = NULL;

    if (head == NULL) {
        head = new_block;
    } else {
        prev->next = new_block;
    }

    heap_start = (char*)heap_start + sizeof(Block) + size;

    return (void*)(new_block + 1);
}



void my_free(void *ptr) {
    if (ptr == NULL) {
        exit(1);
    }
    
    Block *block = (Block *)ptr - 1;
    block->free = 0;
    
    Block *current = head;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += sizeof(Block) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

void *my_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return my_malloc(size);
    }
    
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }
    
    Block *block = (Block *)ptr - 1;
    
    if (block->size >= size) {
        return ptr;
    }
    
    void *new_ptr = my_malloc(size);
    if (new_ptr == NULL) {
        return NULL;
    }
    
    memcpy(new_ptr, ptr, block->size);
    my_free(ptr);
    
    return new_ptr;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        perror("Provide 3 arguments: ./<filename>, <size of memory>, <size of reallocated memory>\n");
        exit(1);
    }
    int size = atoi(argv[1]);
    int* ptr;

    for (int ix = 0; ix < size; ix++) {
        ptr = my_malloc(size);
        if(ptr == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            exit(1);
        }
        ptr[ix] = ix + 1;
        printf("Allocated memory: %p, ptr[ix] = %d\n", ptr, ix); 
    }
    printf("\n\n");

    int realloc_size = atoi(argv[2]);

    if (ptr == NULL) {
        fprintf(stderr,"Memory reallocation failed!\n");
        exit(1);
    }

    for (int ix = 0; ix < realloc_size; ix++) {
        ptr = my_realloc(ptr, realloc_size * sizeof(int));
        if(ptr == NULL) {
            fprintf(stderr, "Failed to reallocate memory.\n");
            exit(1);
        }
        ptr[ix] = ix + 1;
        printf("Reallocated memory: %p, ptr[ix] = %d\n", ptr, ix);
    }
    my_free(ptr);
    printf("Freed all allocated memory\n");
    return 0;
}