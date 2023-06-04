CS214: Project 1

Name: Samantha Ames


Custom implementations of malloc() and free()


Test Plan:

    Library Properties for Correctness:

    malloc() Requirements:
        -   malloc() reserves unallocated memory
        -   malloc() returns 8-byte aligned pointer upon success
        -   malloc() returns a pointer that does not overlap with
            previously allocated object
        -   malloc() returns error for the following: attempting
            to allocate 0 bytes, attempting to allocate without
            sufficient free space in heap

    free() Requirements:
        -   free() deallocates memory
        -   free() allows for the coalescing of adjacent blocks
        -   free() returns error for the following: attempting to free
            an address not allocated by malloc(), attempting to free
            an address that does not begin block, attempting to free the
            same pointer twice

    Tests:
        -   Test A verifies that free() only deallocate memory previously
            allocated by malloc(), otherwise error
        -   Test B verifies that free() only deallocates memory given pointer
            beginning at block, otherwise error
        -   Test C verifies that malloc() only reserves unallocated memory,
            free only deallocates memory previously allocated by malloc, and
            free() will not deallocate the same pointer twice
        -   Test D verifies that malloc() returns an 8-byte aligned pointer
        -   Test E verifies that free() coalesces freed blocks with available
            adjacent blocks