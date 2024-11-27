#include <stdio.h>
#include <stdlib.h>

#define MALLOC_NUM_OF_ITER 10
#define REALLOC_NUM_OF_ITER 10

int main(int argc, char* argv[]) {
    if (argc != 3) {
        perror("Provide 3 arguments: ./<filename>, <size to malloc>, <size to realloc>\n");
        exit(1);
    }
    int size = atoi(argv[1]);
    int* ptr;
    printf("Allocated memory:\n");
    for(int ix = 0; ix < MALLOC_NUM_OF_ITER; ++ix) {
        ptr = malloc(size);
        if(ptr == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            exit(1);
        }
        printf("%d,\n", ptr);
    } 

    int realloc_size = atoi(argv[2]);
    printf("Reallocated memory:\n");
    for(int ix = 0; ix < REALLOC_NUM_OF_ITER; ++ix) {
        ptr = realloc(ptr, size);
        if(ptr == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            exit(1);
        }
        printf("%d,\n", ptr);
    }
    free(ptr);
    printf("Freed all allocated memory\n");
    return 0;
}
