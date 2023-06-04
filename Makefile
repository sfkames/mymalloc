CC = gcc
CCFLAGS = -Wall 

memgrind: memgrind.o mymalloc.o
	$(CC) $(CCFLAGS) memgrind.o mymalloc.o -omemgrind 

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CCFLAGS) -c memgrind.c

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CCFLAGS) -c mymalloc.c
	
clean:
	rm -f memgrind *.o