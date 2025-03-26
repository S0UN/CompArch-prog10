#include "tdmm.h"
size_t HEAP_SIZE = 4 * 1024 * 1024;
void* heap_base;
Block* free_list;
Block* last_block;

size_t HEAP_SIZE = 4 * 1024 * 1024;  // Set HEAP_SIZE to 4MB
void* heap_base;  // Define the pointer to hold the heap memory base
alloc_strat_e current_strategy;
size_t metadata = sizeof(Block);
typedef struct Block {
		size_t size;        // Size of the block (excluding metadata)
		int is_free;        // Whether the block is free (1 for free, 0 for allocated)
		struct Block* next; // Pointer to the next block in the list
} Block;

void t_init(alloc_strat_e strat) {
	//size_t page_size = sysconf(_SC_PAGESIZE); // Get system page size

	HEAP_SIZE = (HEAP_SIZE + 3) & ~3;  

	heap_base = mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE,
										MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (heap_base == MAP_FAILED) {
			perror("mmap failed");
			exit(EXIT_FAILURE); 
	}

	current_strategy = strat;

	

	free_list = (Block*)heap_base;  
	free_list->size = HEAP_SIZE - sizeof(Block); 
	free_list->is_free = 1;
	free_list->next = NULL;
	last_block = free_list;

}
			

void * t_malloc (size_t size)
{
	size = (size + 3) & ~3;
	if(current_strategy == FIRST_FIT) 
	{
		return first_fit(size);
	}else if(current_strategy == BEST_FIT){
		return best_fit(size);
	}else if(current_strategy == WORST_FIT){
		return worst_fit(size);
	}
	return NULL;
}

void* split_block(Block* current, size_t size) {
	size_t aligned_size = (size + 3) & ~3;
    if (current->size - aligned_size >= metadata) {
        Block* new_block = (Block*)((char*)current + aligned_size + metadata); 
        new_block->size = current->size - aligned_size - sizeof(Block);  
        new_block->is_free = 1; 
        new_block->next = current->next; 

        current->size = aligned_size + metadata; 
        current->next = new_block; 

        if(new_block->next == NULL){
					last_block = new_block;
				}
				return (char*)current + sizeof(Block);  
    }

    current->is_free = 0;  
    return (char*)current + sizeof(Block);  
}


void* first_fit(size_t size){
	Block* temp = free_list;
	while(temp->next != NULL){
		if(temp->is_free == 1 && temp->size > ((size + metadata + 3) & ~3)){
			return split_block(temp, size);
		}
		temp = temp->next;
	}
	more_memory(last_block, HEAP_SIZE);
}

void more_memory(Block* last_block, size_t size){
    size = (size + 3) & ~3;

    // Request more memory from the operating system using mmap
    void* new_heap = mmap(NULL, size, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (new_heap == MAP_FAILED) {
        perror("mmap failed to extend heap");
        exit(EXIT_FAILURE);
    }
		Block* new_block = (Block*)new_heap;
    new_block->size = size - sizeof(Block);  
    new_block->is_free = 1;  
    new_block->next = NULL;  

    if (last_block != NULL) {
        last_block->next = new_block;  
    }
    last_block = new_block;


}
void* best_fit(size_t size){
	
}

void* worst_fit(size_t size){
	
}



void t_free (void *ptr)
{
	// TODO: Implement this
}


void t_gcollect (void)
{
	// TODO: Implement this
}