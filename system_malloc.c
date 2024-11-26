#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        perror("Provide 3 arguments: ./<filename>, <size of memory>, <size of reallocated memory>\n");
        exit(1);
    }
    int size = atoi(argv[1]);
    int* ptr;

    for (int ix = 0; ix < size; ix++) {
        ptr = malloc(size * sizeof(int));
        
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

    for (int ix = 1; ix < realloc_size; ix++) {
        ptr = realloc(ptr, realloc_size * sizeof(int));
        if(ptr == NULL) {
            fprintf(stderr, "Failed to reallocate memory.\n");
            exit(1);
        }
        ptr[ix] = ix + 1;
        printf("Reallocated memory: %p, ptr[ix] = %d\n", ptr, ix);
    }
    free(ptr);
    printf("Freed all allocated memory\n");
    return 0;
}