#include "tdmm.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define METADATA sizeof(Block)
#define MIN_BLOCK_SIZE 4   

// Global variables
void* heap_base;
Block* free_list;
Block* last_block;
size_t HEAP_SIZE = 4096 * 4;  
alloc_strat_e current_strategy;

// t_init: initializes the heap with mmap and sets up the free list.
void t_init(alloc_strat_e strat) {
    HEAP_SIZE = (HEAP_SIZE + 3) & ~3;

    heap_base = mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (heap_base == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    current_strategy = strat;

    // Initialize the free list: the entire heap becomes one free block.
    free_list = (Block*)heap_base;
    free_list->size = HEAP_SIZE - METADATA; 
    free_list->is_free = 1;
    free_list->next = NULL;
    free_list->prev = NULL;
    last_block = free_list;
}

// t_malloc: allocates a block using the chosen strategy.
void* t_malloc(size_t size) {
    size = (size + 3) & ~3;

    if (current_strategy == FIRST_FIT) {
        return first_fit(size);
    } else if (current_strategy == BEST_FIT) {
        return best_fit(size);
    } else  {
        return worst_fit(size);
    }
    return NULL; 
}

void* split_block(Block* current, size_t size) {
    size = (size + 3) & ~3;  

    if (current == NULL) {
        return NULL;
    }
    if (current->size >= size + METADATA + MIN_BLOCK_SIZE) {
        Block* new_block = (Block*)((char*)current + METADATA + size);
        new_block->size = current->size - size - METADATA;
        new_block->is_free = 1;
        new_block->next = current->next;
        new_block->prev = current;
        current->next = new_block;
        if (new_block->next != NULL) {
            new_block->next->prev = new_block; 
        } else {
            last_block = new_block;
        }
        current->size = size;
    }
    current->is_free = 0;
    return (char*)current + METADATA;
}
// first_fit: finds the first free block that fits the requested size.
void* first_fit(size_t size) {
    Block* temp = free_list;
    while (temp != NULL) {
        if (temp->is_free && temp->size >= size) {
            return split_block(temp, size);
        }
        temp = temp->next;
    }
    // If no suitable block, extend the heap.
    size_t extend_size = (HEAP_SIZE > (size + METADATA + MIN_BLOCK_SIZE)) ?
                           HEAP_SIZE : (size + METADATA + MIN_BLOCK_SIZE);
    more_memory(extend_size);
    return split_block(last_block, size);
}

// best_fit: finds the free block with the smallest leftover space.
void* best_fit(size_t size) {
    Block* temp = free_list;
    Block* bestBlock = NULL;
    while (temp != NULL) {
        if (temp->is_free && temp->size >= size) {
            if (bestBlock == NULL || temp->size < bestBlock->size) {
                bestBlock = temp;
            }
        }
        temp = temp->next;
    }
    if (bestBlock == NULL) {
        size_t extend_size = (HEAP_SIZE > (size + METADATA + MIN_BLOCK_SIZE)) ?
                               HEAP_SIZE : (size + METADATA + MIN_BLOCK_SIZE);
        more_memory(extend_size);
        return split_block(last_block, size);
    }
    return split_block(bestBlock, size);
}

// worst_fit: finds the free block with the largest leftover space.
void* worst_fit(size_t size) {
    Block* temp = free_list;
    Block* worstBlock = NULL;
    while (temp != NULL) {
        if (temp->is_free && temp->size >= size) {
            if (worstBlock == NULL || temp->size > worstBlock->size) {
                worstBlock = temp;
            }
        }
        temp = temp->next;
    }
    if (worstBlock == NULL) {
        size_t extend_size = (HEAP_SIZE > (size + METADATA + MIN_BLOCK_SIZE)) ?
                               HEAP_SIZE : (size + METADATA + MIN_BLOCK_SIZE);
        more_memory(extend_size);
        return split_block(last_block, size);
    }
    return split_block(worstBlock, size);
}

// more_memory: requests additional memory from the OS and appends it to the free list.
void more_memory(size_t size) {
    size = (size + 3) & ~3;
    void* new_heap = mmap(NULL, size, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_heap == MAP_FAILED) {
        perror("mmap failed to extend heap");
        exit(EXIT_FAILURE);
    }
    Block* new_block = (Block*)new_heap;
    new_block->size = size - METADATA;
    new_block->is_free = 1;
    new_block->next = NULL;
    new_block->prev = NULL;

    if (last_block != NULL) {
        last_block->next = new_block;
        new_block->prev = last_block;
    }
    last_block = new_block;
}

// t_free: frees a previously allocated block, coalescing adjacent free blocks.
void t_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    // Get the block header by subtracting METADATA.
    Block* currBlock = (Block*)((char*)ptr - METADATA);
    currBlock->is_free = 1;

    // Coalesce with next block if possible and adjacent in memory.
    if (currBlock->next != NULL && currBlock->next->is_free == 1 &&
        (char*)currBlock + METADATA + currBlock->size == (char*)currBlock->next) {
        Block* nextBlock = currBlock->next;
        currBlock->size += nextBlock->size + METADATA;
        currBlock->next = nextBlock->next;
        if (nextBlock->next != NULL) {
            nextBlock->next->prev = currBlock;
        } else {
            last_block = currBlock;
        }
    }
    // Coalesce with previous block if possible and adjacent in memory.
    if (currBlock->prev != NULL && currBlock->prev->is_free == 1 &&
        (char*)currBlock->prev + METADATA + currBlock->prev->size == (char*)currBlock) {
        Block* prevBlock = currBlock->prev;
        prevBlock->size += currBlock->size + METADATA;
        prevBlock->next = currBlock->next;
        if (currBlock->next != NULL) {
            currBlock->next->prev = prevBlock;
        } else {
            last_block = prevBlock;
        }
    }
}

// t_gcollect: Basic garbage collection (not implemented yet)
void t_gcollect(void) {
    // TODO: Implement garbage collection if needed.
}