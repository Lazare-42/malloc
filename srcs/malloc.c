#include <unistd.h>
#include <stdio.h>

#include <sys/mman.h>
#include "malloc.h"
#include <string.h>

void    *ptr_void_get_base_or_extend_base(void *ptr_pssd_base)
{
    struct s_block          *ptr_lcl_browse_blocks;

    ptr_lcl_browse_blocks    = NULL;
    if (NULL == ptr_pssd_base)
    {
        ptr_lcl_browse_blocks = mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
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

struct s_block *get_first_free_block(struct s_block *first_block)
{
    struct s_block          *ptr_lcl_browse_blocks;

    ptr_lcl_browse_blocks    = NULL;
    ptr_lcl_browse_blocks    = first_block;
    while (ptr_lcl_browse_blocks != NULL && (ptr_lcl_browse_blocks->u8_status & FIRST_BIT) != FALSE)
    {
    ptr_lcl_browse_blocks = ptr_lcl_browse_blocks->next;
    }
    if (ptr_lcl_browse_blocks == NULL)
        fprintf(stderr, "Returning NULL from get_first_free_block\n");
    return ptr_lcl_browse_blocks;
}

void *set_memory_of_current_block_as_occupied_return_user_space(struct s_block *ptr_pssd_stc_memory_to_use, uint64_t u64_pssd_size_to_use)
{
    struct s_block          *ptr_lcl_next_block;
    uint64_t                u64_lcl_size_of_current_block;

    ptr_lcl_next_block              = NULL;
    u64_lcl_size_of_current_block   = ZERO;

    /**
     *  Set the size of the current malloced block
     */
    u64_lcl_size_of_current_block = u64_pssd_size_to_use + sizeof(struct s_block);

    /**
     * Set memory of next block
     */
    ptr_lcl_next_block              = (void *)(ptr_pssd_stc_memory_to_use + ptr_pssd_stc_memory_to_use->u64_size);
    ptr_lcl_next_block->u64_size    = (uint64_t)(ptr_pssd_stc_memory_to_use->u64_size - ptr_pssd_stc_memory_to_use ->u64_size);
    ptr_lcl_next_block->u8_status   = ZERO;
    ptr_lcl_next_block->next        = NULL;

    /**
     *  Set memory of current block
     */
    ptr_pssd_stc_memory_to_use->u64_size  = u64_lcl_size_of_current_block ;
    ptr_pssd_stc_memory_to_use->u8_status = ZERO;
    ptr_pssd_stc_memory_to_use->u8_status |= FIRST_BIT;

    /**
     *  Attach next free block to the memory to use
     */
    ptr_pssd_stc_memory_to_use->next = ptr_lcl_next_block;              

    return ((void*)(ptr_pssd_stc_memory_to_use) + sizeof(struct s_block));
}

void    *get_and_set_block(size_t size)
{
    static struct s_block   *ptr_stc_lcl_static_base_memory_page = NULL;
    struct s_block          *ptr_lcl_browse_blocks;

    ptr_lcl_browse_blocks    = NULL;

    ptr_lcl_browse_blocks = get_first_free_block(ptr_stc_lcl_static_base_memory_page);
    /**
     * If no memory was set aside yet, or the found memory has insufficient user space
     * get a new memory page
     */
    if (ptr_lcl_browse_blocks  == NULL ||
        (ptr_lcl_browse_blocks->u64_size < size + sizeof(struct s_block)))
    {
        if ((NULL == (ptr_lcl_browse_blocks = get_first_free_block((ptr_stc_lcl_static_base_memory_page = ptr_void_get_base_or_extend_base(ptr_stc_lcl_static_base_memory_page))))))
        {
            fprintf(stderr, "Could not use mman\n");
            return (NULL);
        }
    }
    return (set_memory_of_current_block_as_occupied_return_user_space(ptr_lcl_browse_blocks, size));
}

void    *my_malloc(size_t size)
{
    if (size > (getpagesize() + sizeof(struct s_block) * 2))
    {
        fprintf(stderr, "Size asked by the malloc is superior to the size of getpagesize()\n");
        return (NULL);
    }
    return (get_and_set_block(size));
}

