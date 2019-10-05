#include <stdio.h>
#include "malloc.h"

uint64_t Fu64__align16(size_t number)
{
    return (uint64_t)(( ( number - 1 ) | ( 16 - 1 ) ) + 1);
}

void    *Fptr_void__return_memory(struct s_manipulation *ptr_pssd_stc_manipulation_structure, uint64_t u64_pssd_required_size)
{
    struct s_page                   *ptr_stc_lcl_browse_page_category;
    struct s_block                  *ptr_stc_lcl_memory_block_to_return;

    ptr_stc_lcl_browse_page_category        = NULL;
    /**
     *  Find page of required size
     */
    ptr_stc_lcl_browse_page_category       = ptr_pssd_stc_manipulation_structure->ptr_stc_page_linked_list;
    while (ptr_stc_lcl_browse_page_category != NULL && u64_pssd_required_size > ptr_stc_lcl_browse_page_category->u64_block_size_)
    {
    ptr_stc_lcl_browse_page_category = ptr_stc_lcl_browse_page_category->ptr_next_page_upper_category_;
    }
    while (ptr_stc_lcl_browse_page_category != NULL && ptr_stc_lcl_browse_page_category->ptr_first_free_block_ == NULL)
    {
    ptr_stc_lcl_browse_page_category = ptr_stc_lcl_browse_page_category->ptr_next_page_same_category_;
    }
    if (ptr_stc_lcl_browse_page_category == NULL) 
    {
        fprintf(stderr, "Required size does not exist yet\n");
        return (NULL);
    }
    ptr_stc_lcl_memory_block_to_return = ptr_stc_lcl_browse_page_category->ptr_first_free_block_;
    ptr_stc_lcl_browse_page_category->ptr_first_free_block_ = ptr_stc_lcl_browse_page_category->ptr_first_free_block_->ptr_next_;
    ptr_stc_lcl_memory_block_to_return->u64_free_size_ -= u64_pssd_required_size;
    //fprintf(stderr, "Free space is %llu\n", ptr_stc_lcl_memory_block_to_return->u64_free_size_);
    return ((void*)(((uint8_t*)ptr_stc_lcl_memory_block_to_return) + Fu64__align16(sizeof(struct s_block))));
}

void    *my_malloc(size_t size)
{
    static struct s_manipulation    *ptr_stc_lcl_manipulation_structure = NULL;
    //struct s_page                   *ptr_stc_lcl_browse_same_page_category;

    (void)size;
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
    return (Fptr_void__return_memory(ptr_stc_lcl_manipulation_structure, size));
}
