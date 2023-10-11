#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "my_malloc.h"

/**
 * ATTENTION:
 *   - I don't really understand when to step using offsets from MY_HEAP
 *     and when I can just add 1 from the current block.
 *
 *   - How do I test this ?
 *
 *   - Where should I define my constants ? Should I redifine them at the start of each function ?
 */

/**
 * This heap is build as a doubly linked, explicit list.
 *
 * Explicit list, meaning each free block points towards the next free block.
 * Unlike implicit list, where every block points to the next, wether it is free, or not.
 *
 * Doubly linked, meaning each block starts and ends with a metadata word. Allowing traversing both ways.
 * Free block contain on top of that, two extra words, one containing a pointer to the next free block, and
 * another containing a pointer towards the previous free block.
 * --
 * Our heap size is 64 [kb], and we chose 2 [byte] length words.
 * --
 * The heap starts with a word that points towards the first free block.
 */

uint8_t MY_HEAP[64000];

void my_init()
{
    const uint16_t METADATA_SIZE = 2;
    const uint16_t HEAP_SIZE = 64000;

    // Initialize start block to point to 1st free block
    uint16_t *start = (uint16_t *)MY_HEAP;
    uint16_t *block = (uint16_t *)(MY_HEAP + 1 * METADATA_SIZE);

    *start = METADATA_SIZE;

    uint16_t block_size = HEAP_SIZE - 1 * METADATA_SIZE;
    *(block + 0) = block_size;                    // block size (in bytes)
    *(block + 1) = 0;                             // ptr to next free block (here: null)
    *(block + 2) = 0;                             // ptr to previous free block
    *(block + (block_size / 2) - 1) = block_size; // last: = 1st
}

void *my_malloc(size_t size)
{
    const uint16_t WORD = 2;
    const uint16_t MIN_BLOCK_SIZE = 4 * WORD;
    size += 2 * WORD; // Additional words for metadata

    uint16_t *start = (uint16_t *)MY_HEAP;
    uint16_t *current = (uint16_t *)(MY_HEAP + *start);

    while (current != NULL)
    {
        uint16_t block_size = *current;
        if (block_size >= size)
        {
            uint16_t remaining_size = block_size - size;
            uint16_t *new_block;

            if (remaining_size >= MIN_BLOCK_SIZE)
            {
                // Split block
                new_block = current + size;
                *new_block = remaining_size;
                *(new_block + 1 * WORD) = *(current + 1 * WORD);
                *(new_block + 2 * WORD) = *(current + 2 * WORD);
                *(new_block + *new_block - WORD) = *new_block;
            }

            // Update current block
            *current = size;
            *(current + size - WORD) = size;

            // Update previous block's next pointer
            // TODO

            // Update next block's previous pointer
            // TODO

            return (void *)(current + 1 * WORD);
        }

        current = *(current + 1 * WORD);
    }

    return NULL;
}


void my_free(void *pointer) {
    // Find the first previous free space
        // if previous is adjacent => merge
            // Change size of previous: previous += current
            // Wait to point to next with the 'next pointer' of the previous => current becomes previous
        // else
            // Point to the head of previous
            // Change 'next pointer' of previous to current

    // Find the next free space
        // if next is adjacent => merge
            // Change size of current: current += next
            // Change 'next pointer' of current with the value form 'next pointer' of next
        // else
            // Point the 'next pointer' to the head of next
            // Change 'previous pointer' of next to the head of current/previous of current
}