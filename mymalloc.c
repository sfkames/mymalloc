#include "mymalloc.h"

// Functions to be implemented
header* findNext(header* req);
header* findPrev(header* req);
void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);


// Global heap array
static char memory[MEMSIZE];

// Function returns first occurrence of next free block
header* findNext(header* req)
{
    char* p = (char*) req;
    p = p + req->size + HEADER_SIZE;
    return (header*) p;
}

// Function returns header for previous block
header* findPrev(header* req)
{
    header* temp = (header*)memory;
    header* p;
    while (temp != req) {
        p = temp;
        temp = findNext(temp);
    }
    return p;
}

// Returns a pointer to an object upon success, otherwise returns error
void *mymalloc(size_t size, char *file, int line)
{
    // 8-byte aligned request
    size = ALIGN(size);

    // 8-byte aligned request with header
    size_t block_size = ALIGN(size + HEADER_SIZE);

    // Pointer to beginning of memory
    header* head = (header*) memory;

    if(size == 0) {
        fprintf(stderr, "Error: Insufficient memory request (0 bytes) on LINE: '%d', in FILE: '%s'\n", line, file);
        return NULL;
    }

    // Heap not yet initialized, create root header
    if (memory[0] == 0) {
        head->flag = '0';
        head->size = MEMSIZE-HEADER_SIZE;
    }

    header *req = head;
    
    while ((char*)req < &memory[MEMSIZE]) {
        
        // Case: current block has valid space and is free
        if(req->size >= size && req->flag == '0'){

            size_t remainder = req->size - size;
            char* p = (char*)req + block_size;
            req->size = size;
            req->flag = '1';

            // Initialize next instance of header
            if ((p + HEADER_SIZE) < &memory[MEMSIZE]) {

                header* temp = (header*)p;
                temp->size = remainder - HEADER_SIZE;
                temp->flag = '0';
            } else {
                req->size += remainder;
            }
            p = (char*)req + HEADER_SIZE;
            // printf("%lu\n\n", (unsigned long)p);
            return (void*)p;
        } else {
            req = findNext(req);
        }
    }
    fprintf(stderr, "Error: Insufficient memory request (%ld bytes) on LINE: '%d', in FILE: '%s'\n", size, line, file);
    return NULL;
}

// Deallocates memory previously allocated by mymalloc
void myfree(void *ptr, char *file, int line)
{
    if (ptr == NULL) {
        fprintf(stderr, "Error: Null pointer, failure to free on LINE: '%d', in FILE: '%s'\n", line, file);
        return;
    }

    char* temp = (char*)ptr - HEADER_SIZE;
    header* req = (header*)temp;

    // Check if request has previously been allocated
    if (req->flag == '0') {
        fprintf(stderr, "Error: Unallocated memory, failure to free on LINE: '%d', in FILE: '%s'\n", line, file);
        return;
    }
    // Check if request is at the start of a chunk
    else if (req->flag != '1') {
        fprintf(stderr, "Error: Pointer not at beginning of valid memory block, failure to free on LINE: '%d', in FILE: '%s'\n", line, file);
        return;
    }
    // Check if request is within range of heap
    if ((char*) req < memory || (char*)req > (memory + MEMSIZE)) {
        fprintf(stderr, "Error: Pointer out-of-bounds, failure to free on LINE: '%d', in FILE: '%s'\n", line, file);
        return;
    } else if (req->flag == '1') {

        header* prev;
        header* next;
        
        // printf("req1 = %lu\n", (unsigned long)req);
        // printf("mem = %lu\n", (unsigned long)memory);

        // Case: deallocating first block, do not check for previous coalescing
        if ((char*)req == memory) {

            next = findNext(req);
            
            if (next->flag == '0') {
                req->size += next->size + HEADER_SIZE;
                req->flag = '0';
            } else {
                req->flag = '0';
            }
            return;
        }
        // Case: deallocating last block, do not check for next coalescing
        else if ((char*)req + req->size + HEADER_SIZE == &memory[MEMSIZE]) {
            prev = findPrev(req);
            if (prev->flag == '0') {
                prev->size += req->size + HEADER_SIZE;
            } else {
                req->flag = '0';
            }
            return;
        } 
        
        // Case: deallocating from middle of heap, check both prev and next for coalescing
        else {
            prev = findPrev(req);
            next = findNext(req);
            if (prev->flag == '0') {
                prev->size += req->size + HEADER_SIZE;
                if (next->flag == '0') {
                    prev->size += next->size + HEADER_SIZE;
                } else {
                    return;
                }
            } else if (next->flag == '0') {
                req->size += next->size + HEADER_SIZE;
                req->flag = '0';
                return;
            } else {
                req->flag = '0';
                return;
            }
        }
    }
}