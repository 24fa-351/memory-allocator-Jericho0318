#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h>

typedef struct Block {
    size_t size;
    struct Block *next;
    int free;
} Block;

void *my_malloc(size_t size);

void my_free(void *ptr);

void *my_realloc(void *ptr, size_t size);

#endif
