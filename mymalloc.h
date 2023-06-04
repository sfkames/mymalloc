#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Constants and macros*/
#define MEMSIZE 4096    // array length
#define ALIGNMENT 8


#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)

// Round to the nearest multiple of 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
// Header size
#define HEADER_SIZE (ALIGN(sizeof(header)))
// Number of possible entries in heap
#define NUM_HEADER (MEMSIZE/HEADER_SIZE)
// Number of possible blocks in heap
#define NUM_BLOCKS (MEMSIZE/8)

// Metadata struct
typedef struct header {
    size_t size;    // block size in bytes
    char flag;       // '0' = free, '1' = allocated
} header;

// Functions in mymalloc.c
header* findNext(header* req);
header* findPrev(header* req);
void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);