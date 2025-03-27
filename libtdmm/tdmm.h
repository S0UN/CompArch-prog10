#ifndef TDMM_H_
#define TDMM_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

typedef enum
{
  FIRST_FIT,
  BEST_FIT,
  WORST_FIT,
  BUDDY
} alloc_strat_e;

typedef struct Block {
    size_t size;        
    int is_free;     
    struct Block* next; 
    struct Block* prev; 
} Block;
/**
 * Initializes the memory allocator with the given strategy.
 * @param strat The strategy to use for memory allocation.
 */
void t_init (alloc_strat_e strat);

/**
 * Allocates a block of memory of the given size.
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block
 * fails.
 */
void *t_malloc (size_t size);

/**
 * Frees the given memory block.
 * @param ptr The pointer to the memory block to free. This must be a
 * pointer returned by t_malloc.
 */
void t_free (void *ptr);

/**
 * Performs basic garbage collection by scanning the stack and heap managed
 * by t_malloc and t_free.
 */
void t_gcollect (void);

void* split_block(Block* current, size_t size);
void* first_fit(size_t size);
void* worst_fit(size_t size);
void* best_fit(size_t size);
void more_memory(size_t size);



#endif // TDMM_H_
