#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include "malloc.h"
#include <string.h>

    static struct s_block   *ptr_stc_lcl_static_base_memory_page = NULL;

size_t Fszt__align16(size_t number)
{
    return (number + 16 - number % 16);
}

void    *Fptr_void__get_base_or_extend_base(void *ptr_pssd_base)
{
    struct s_block          *ptr_lcl_browse_blocks;

    ptr_lcl_browse_blocks    = NULL;
    if (NULL == ptr_pssd_base)
    {
        /**
         *  NULL to point to a "fake" address
         *  getpagesize() to use a multiple of a system page
         *  PROT_READ | PROT_WRITE to read & write 
         *  MAP_SHARED to share modifications to all processes & sub-processes
         *  MAP_ANONYMOUS to get a fresh memory area that shares no pages with other mappings (obtained memory is initialized to ZERO)
         */
        ptr_lcl_browse_blocks = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
        printf("%p is received address\n", ptr_lcl_browse_blocks);
        if (ptr_lcl_browse_blocks == NULL)
        {
            return (NULL);
        }
        ptr_lcl_browse_blocks->u64_size  = getpagesize();
        ptr_lcl_browse_blocks->u8_status = ZERO;
        ptr_lcl_browse_blocks->next      = NULL;
    }
    else
    {
        fprintf(stderr, "Re-alloc not managed yet\n");
        return NULL;
    }
    return (ptr_lcl_browse_blocks);
}

struct s_block *Fptr_struct_block__get_first_free_block(struct s_block *first_block)
{
    struct s_block          *ptr_lcl_browse_blocks;

    ptr_lcl_browse_blocks    = NULL;
    ptr_lcl_browse_blocks    = first_block;
    while (ptr_lcl_browse_blocks != NULL
            && (ptr_lcl_browse_blocks->u8_status & FIRST_BIT) == TRUE)
    {
    ptr_lcl_browse_blocks   = ptr_lcl_browse_blocks->next;
    }
    if (ptr_lcl_browse_blocks == NULL)
        fprintf(stderr, "Returning NULL from Fptr_struct_block__get_first_free_block\n");
    return ptr_lcl_browse_blocks;
}

void *Fptr_void__set_memory_of_current_block_as_occupied_return_user_space(struct s_block *ptr_pssd_stc_memory_to_use, uint64_t u64_pssd_size_to_use)
{
    struct s_block          *ptr_lcl_next_block;
    uint64_t                u64_lcl_size_of_current_block;
    uint64_t                u64_lcl_size_before_alloc;

    ptr_lcl_next_block              = NULL;
    u64_lcl_size_of_current_block   = ZERO;
    u64_lcl_size_before_alloc       = ZERO;

    u64_lcl_size_before_alloc       = ptr_pssd_stc_memory_to_use->u64_size;

    fprintf(stderr, "Size before alloc is %llu\n", ptr_pssd_stc_memory_to_use->u64_size);
    /**
     *  Set the size of the current malloced block
     */
    u64_lcl_size_of_current_block = Fszt__align16(u64_pssd_size_to_use + sizeof(struct s_block));

    /**
     *  Set memory of current block
     */
    ptr_pssd_stc_memory_to_use->u64_size  = u64_lcl_size_of_current_block ;
    ptr_pssd_stc_memory_to_use->u8_status |= FIRST_BIT;

    /**
     * Set memory of next block
     */
    ptr_lcl_next_block              = (struct s_block*)(&((uint8_t*)ptr_pssd_stc_memory_to_use)[u64_lcl_size_of_current_block]);
    //ptr_lcl_next_block              = ptr_pssd_stc_memory_to_use + u64_lcl_size_of_current_block;
    //fprintf(stderr, "New block at %p is %20d bytes away from original block at %p\n", ptr_lcl_next_block, (unsigned char)(ptr_lcl_next_block - ptr_stc_lcl_static_base_memory_page), ptr_stc_lcl_static_base_memory_page);
    ptr_lcl_next_block->u64_size    = u64_lcl_size_before_alloc - u64_lcl_size_of_current_block;
    ptr_lcl_next_block->u8_status   = ZERO;
    ptr_lcl_next_block->next        = NULL;

    /**
     *  Attach next free block to the memory to use
     */
    ptr_pssd_stc_memory_to_use->next = ptr_lcl_next_block;              

    return ((void*)(ptr_pssd_stc_memory_to_use) + sizeof(struct s_block));
}

void    *Fptr_void_get_and_set_block(size_t size)
{
    struct s_block          *ptr_lcl_browse_blocks;

    ptr_lcl_browse_blocks   = NULL;
    /**
     *  Get the first used memory pointer.
     */
    ptr_lcl_browse_blocks   = Fptr_struct_block__get_first_free_block(ptr_stc_lcl_static_base_memory_page);
    /**
     * If no memory was set aside yet, or the found memory has insufficient user space
     * get a new memory page
     */
    if (ptr_lcl_browse_blocks  == NULL ||
        (ptr_lcl_browse_blocks->u64_size < Fszt__align16(size + sizeof(struct s_block) + sizeof(struct s_block))))
    {
        if ((NULL == (ptr_lcl_browse_blocks = Fptr_struct_block__get_first_free_block((ptr_stc_lcl_static_base_memory_page = Fptr_void__get_base_or_extend_base(ptr_stc_lcl_static_base_memory_page))))))
        {
            fprintf(stderr, "Could not use mman\n");
            return (NULL);
        }
    }
    return (Fptr_void__set_memory_of_current_block_as_occupied_return_user_space(ptr_lcl_browse_blocks, size));
}

void    *my_malloc(size_t size)
{
    if (size > (getpagesize() + sizeof(struct s_block) * 2))
    {
        fprintf(stderr, "Size asked by the malloc is superior to the size of getpagesize()\n");
        return (NULL);
    }
    return (Fptr_void_get_and_set_block(size));
}

