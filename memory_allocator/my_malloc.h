#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h>

// Initialize the memory allocator
void init();

// Allocate a block of memory of the given size
void *my_malloc(size_t size);

// Free a previously allocated block of memory
void my_free(void *ptr);

#endif // MY_MALLOC_H