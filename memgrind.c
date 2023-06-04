#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "mymalloc.h"

// Test A: malloc() and immediately free() a 1-byte chunk, 120 times
void testA()
{
    for (int i = 0; i < 120; i++) {
        char* temp = (char*)malloc(sizeof(char));
        free(temp);
    }
}

/*
Test B: Use malloc() to get 120 1-byte chunks, storing the pointers in an array,
then use free() to deallocate the chunks
*/
void testB()
{
    char* temp[120];
    for (int i = 0; i < 120; i++) {
        temp[i] = (char*)malloc(sizeof(char));
    }
    for (int i = 0; i < 120; i++) {
        free(temp[i]);
    }
}

/*
Test C: Randomly choose between 1) allocating a 1-byte chunk and storing the
pointer in an array, and 2) deallocating one of the chunks in the array (if any).
Repeat until you have called malloc() 120 times, then free all remaining allocated
chunks
*/
void testC()
{
    int mallocCount, freeCount, mallocTotal = 0;
    char* temp[120];
    int test;   // 0 = malloc, 1 = free

    while (mallocTotal < 120) {
        test = rand() % 2;

        // Run malloc()
        if (mallocCount == 0) {
            temp[mallocCount] = (char*)malloc(sizeof(char));
            mallocCount++;
            mallocTotal++;
        } else if (test == 0) {
            temp[mallocCount] = (char*)malloc(sizeof(char));
            mallocCount++;
            mallocTotal++;
        } else if (test == 1) {
            mallocCount--;
            free(temp[mallocCount]);
            freeCount++;
        } else {
            continue;
        }
    }
    while(freeCount < 120) {
        mallocCount--;
        free(temp[mallocCount]);
        freeCount++;
    }
}

/*
Test D: Attempt to allocate and then free randomly-sized chunks in
range 0 - 4096, 120 times.
*/ 
void testD()
{
    srand(time(NULL));
    for (int i = 0; i < 120; i++) {
        size_t size = (rand() % 4097);
        char* p = (char*)malloc(sizeof(char) * size);
        free(p);
    }
}

/*
Test E: malloc an array, free even-numbered blocks
and then malloc single-byte blocks using freed space
*/
void testE()
{
    char* temp[120];
    for (int i = 0; i < 120; i++) {
        temp[i] = (char*)malloc(sizeof(char));
    }
    for (int i = 1; i < 120; i++) {
        if(i % 2 == 0) {
            free(temp[i]);
        } else if (i > 1) {
            free(temp[i]);
        }
    }
    for(int i = 2; i < 120; i++) {
        temp[i] = (char*)malloc(sizeof(char));
    }
}

/*
Run each task 50 times, summing each for total amount of time per
task. Then report the average for each task.
*/
int main()
{
    clock_t start, end;
    double  timeA,
            timeB,
            timeC,
            timeD,
            timeE;

    
    for (int i = 0; i < 50; i++) {
        start = clock();
        testA();
        end = clock();
        timeA += (((double) (end - start)) / CLOCKS_PER_SEC);

        start = clock();
        testB();
        end = clock();
        timeB += (((double) (end - start)) / CLOCKS_PER_SEC);

        start = clock();
        testC();
        end = clock();
        timeC += (((double) (end - start)) / CLOCKS_PER_SEC);

        start = clock();
        testD();
        end = clock();
        timeD += (((double) (end - start)) / CLOCKS_PER_SEC);

        start = clock();
        testE();
        end = clock();
        timeE += (((double) (end - start)) / CLOCKS_PER_SEC);
    }

    timeA /= 50;
    timeB /= 50;
    timeC /= 50;
    timeD /= 50;
    timeE /= 50;
    
    printf("Average execution time for task A : %lf seconds\n", timeA);
    printf("Average execution time for task B : %lf seconds\n", timeB);
    printf("Average execution time for task C : %lf seconds\n", timeC);
    printf("Average execution time for task D : %lf seconds\n", timeD);
    printf("Average execution time for task E : %lf seconds\n", timeE);

    return 0;
}