#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

// Define the total heap size you want to request from the OS (e.g., 1MB)
#define HEAP_SIZE (1024 * 1024)  
#define ALIGNMENT 4  // Ensure 4-byte alignment

// Block header structure for each memory block.
typedef struct Block {
    size_t size;          // Total size of this block (including header)
    int is_free;          // 1 if free, 0 if allocated
    struct Block* next;   // Pointer to the next block in the free list
    // You might add more fields (e.g., pointers to previous block) as needed.
} Block;

// Global pointer to the start of our mmap'ed heap and the free list head.
static void* heap_base = NULL;
static Block* free_list = NULL;

// Function prototypes
void init_allocator();
void* tmalloc(size_t size);
void tfree(void* ptr);
Block* find_free_block(size_t size);
void split_block(Block* block, size_t size);
void coalesce_free_blocks();

// Example main function to test the allocator.
int main() {
    // Initialize the allocator (mmap, create initial free block, etc.)
    init_allocator();

    // Example usage:
    void* ptr1 = tmalloc(100);
    void* ptr2 = tmalloc(200);

    // Free the allocated memory.
    tfree(ptr1);
    tfree(ptr2);

    // You could add more tests here (e.g., allocation patterns, measuring stats)

    return 0;
}

// Initialize the allocator by calling mmap and setting up the initial free block.
void init_allocator() {
    size_t pagesize = sysconf(_SC_PAGESIZE);
    // Optionally, adjust HEAP_SIZE to be a multiple of pagesize.

    // Request a big chunk of memory from the OS.
    heap_base = mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (heap_base == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    // Create the initial free block covering the entire heap.
    free_list = (Block*)heap_base;
    free_list->size = HEAP_SIZE - sizeof(Block);
    free_list->is_free = 1;
    free_list->next = NULL;
}

// tmalloc: Allocate memory from our heap.
void* tmalloc(size_t size) {
    // 1. Adjust the requested size for alignment and include header overhead.
    size_t total_size = size;  // TODO: Adjust total_size to include metadata and padding.

    // 2. Search the free list for a suitable free block (using first fit, best fit, etc.).
    Block* block = find_free_block(total_size);
    if (!block) {
        // Optionally, request more memory with mmap() if no block is found.
        return NULL;
    }

    // 3. If the block is larger than needed, split it.
    if (block->size > total_size + sizeof(Block)) {
        split_block(block, total_size);
    }

    // 4. Mark the block as allocated.
    block->is_free = 0;
    // TODO: Remove block from free list if necessary.

    // 5. Return a pointer to the usable memory (after the metadata header).
    return (void*)((char*)block + sizeof(Block));
}

// tfree: Free a previously allocated block.
void tfree(void* ptr) {
    if (ptr == NULL) return;

    // Convert the pointer back to a Block pointer (by subtracting header size).
    Block* block = (Block*)((char*)ptr - sizeof(Block));

    // Validate: ensure block is not already free.
    if (block->is_free) {
        // TODO: Handle double free error.
        return;
    }

    // Mark the block as free.
    block->is_free = 1;

    // Insert the block back into the free list.
    // TODO: Update the free list with this block.

    // Attempt to coalesce with adjacent free blocks to reduce fragmentation.
    coalesce_free_blocks();
}

// Helper: Find a free block that fits the requested size.
// Use allocation strategy (first fit, best fit, worst fit).
Block* find_free_block(size_t size) {
    // TODO: Implement your allocation strategy here.
    Block* current = free_list;
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Helper: Split a free block into two parts (allocated and remaining free block).
void split_block(Block* block, size_t size) {
    // TODO: Create a new Block header in the remaining space.
    // Update the size of 'block' and the new block.
    // Link the new block into the free list.
}

// Helper: Coalesce adjacent free blocks to reduce fragmentation.
void coalesce_free_blocks() {
    // TODO: Traverse your memory blocks and merge adjacent free blocks.
    // This might involve updating sizes and adjusting the free list pointers.
}
