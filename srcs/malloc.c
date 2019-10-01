#include <stdio.h>
#include "malloc.h"


size_t Fszt__align16(size_t number)
{
    return (number + 16 - number % 16);
}


//struct s_block *Fptr_stc_block__get_first_free_block(struct s_block *first_block)
//{
//    struct s_block          *ptr_lcl_browse_blocks;
//
//    ptr_lcl_browse_blocks    = NULL;
//    ptr_lcl_browse_blocks    = first_block;
//    while (ptr_lcl_browse_blocks != NULL
//            && (ptr_lcl_browse_blocks->u8_status & FIRST_BIT) == TRUE)
//    {
//    ptr_lcl_browse_blocks   = ptr_lcl_browse_blocks->next;
//    }
//    return ptr_lcl_browse_blocks;
//}
//
//void *Fptr_void__set_memory_of_current_block_as_occupied_return_user_space(struct s_block *ptr_pssd_stc_memory_to_use, uint64_t u64_pssd_size_to_use)
//{
//    struct s_block          *ptr_lcl_next_block;
//    uint64_t                u64_lcl_size_of_current_block;
//    uint64_t                u64_lcl_size_before_alloc;
//
//    ptr_lcl_next_block              = NULL;
//    u64_lcl_size_of_current_block   = ZERO;
//    u64_lcl_size_before_alloc       = ZERO;
//
//    u64_lcl_size_before_alloc       = ptr_pssd_stc_memory_to_use->u64_size;
//
//    /**
//     *  Set the size of the current malloced block
//     */
//    u64_lcl_size_of_current_block = Fszt__align16(u64_pssd_size_to_use + sizeof(struct s_block));
//
//    /**
//     *  Set memory of current block
//     */
//    ptr_pssd_stc_memory_to_use->u64_size  = u64_lcl_size_of_current_block ;
//    ptr_pssd_stc_memory_to_use->u8_status |= FIRST_BIT;
//
//    /**
//     * Set memory of next block by correctly casting previous pointer.
//     */
//    ptr_lcl_next_block              = (struct s_block*)(&((uint8_t*)ptr_pssd_stc_memory_to_use)[u64_lcl_size_of_current_block]);
//    ptr_lcl_next_block->u64_size    = u64_lcl_size_before_alloc - u64_lcl_size_of_current_block;
//    ptr_lcl_next_block->u8_status   = ZERO;
//    ptr_lcl_next_block->next        = NULL;
//
//    /**
//     *  Attach next free block to the memory to use
//     */
//    ptr_pssd_stc_memory_to_use->next = ptr_lcl_next_block;              
//    return ((void*)(ptr_pssd_stc_memory_to_use) + sizeof(struct s_block));
//}

void    *my_malloc(size_t size)
{
    static struct s_manipulation    *ptr_stc_lcl_manipulation_structure = NULL;
    struct s_block                  *ptr_stc_struct_where_to_store_malloc;

    /**
     *  If the manipulation structure is set to NULL, initialize it.
     */
    if (ptr_stc_lcl_manipulation_structure == NULL)
    {
        ptr_stc_lcl_manipulation_structure = Fptr_stc_manipulation__create_manipulation_structure();
        if (ptr_stc_lcl_manipulation_structure  == NULL)
        {
            return NULL;
        }
    }
    if (size <= TINY)
    {
        if (ptr_stc_lcl_manipulation_structure->u64_size_of_tiny_memory_pointer - 1 == ptr_stc_lcl_manipulation_structure->u64_first_free_location_for_tiny_pointer)
        {
            fprintf(stderr, "Re-allocing double pointer for tiny mallocs not managed yet\n");
            return (NULL);
        }
        ptr_stc_struct_where_to_store_malloc = ptr_stc_lcl_manipulation_structure->ptr_stc_block_tiny_blocks[ptr_stc_lcl_manipulation_structure->u64_first_free_location_for_tiny_pointer];
    }
    else if (size <= SMALL)
    {
        if (ptr_stc_lcl_manipulation_structure->u64_size_of_small_memory_pointer - 1 == ptr_stc_lcl_manipulation_structure->u64_first_free_location_for_small_pointer)
        {
            fprintf(stderr, "Re-allocing double pointer for small mallocs not managed yet\n");
            return (NULL);
        }
        ptr_stc_struct_where_to_store_malloc = ptr_stc_lcl_manipulation_structure->ptr_stc_block_small_blocks[ptr_stc_lcl_manipulation_structure->u64_first_free_location_for_small_pointer];
    }
    else
    {
        fprintf(stderr, "Allocing double pointer & simple pointer for large mallocs not managed yet\n");
        return (NULL);
    }
    return NULL;
    //return (Fptr_void_get_and_set_block(size, ptr_stc_struct_where_to_store_malloc));
}
