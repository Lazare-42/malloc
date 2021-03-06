#include "malloc.h"
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include "libft.h"

static struct s_manipulation    *ptr_stc_global_static_manipulation_structure = NULL;

void free(void *ptr)
{
    struct s_block  *ptr_stc_block_pointer_to_free;
    struct s_page   *ptr_stc_page_storing_block_pointer;

    ptr_stc_block_pointer_to_free       = NULL;
    ptr_stc_page_storing_block_pointer  = NULL;
    /**
     *  Should first check if passed pointer belongs the all pointers.
     */
    if (ptr_stc_global_static_manipulation_structure == NULL || ZERO == Fu8__bool_check_if_pointer_passed_to_free_was_previously_malloc(ptr_stc_global_static_manipulation_structure, ptr))
    {
        return ;
    }
    ptr_stc_block_pointer_to_free                                           = (struct s_block*)(((uint8_t*)(ptr)) - sizeof(struct s_block));
    ptr_stc_block_pointer_to_free->u64_free_size_                           = ptr_stc_block_pointer_to_free->u64_size_;
    ptr_stc_page_storing_block_pointer                                      = ptr_stc_block_pointer_to_free->ptr_page_base_;
    ptr_stc_page_storing_block_pointer->u64_number_of_used_blocks_in_page_  = ptr_stc_page_storing_block_pointer->u64_number_of_used_blocks_in_page_ - 1;

    ptr_stc_page_storing_block_pointer->ptr_first_occuppied_block_ = ptr_stc_block_pointer_to_free->ptr_next_block_in_page_;
    if (ptr_stc_page_storing_block_pointer->ptr_first_free_block_ != NULL)
        ptr_stc_block_pointer_to_free->ptr_next_block_in_page_ = ptr_stc_page_storing_block_pointer->ptr_first_free_block_;
    ptr_stc_page_storing_block_pointer->ptr_first_free_block_ = ptr_stc_block_pointer_to_free;

    if (ZERO == ptr_stc_page_storing_block_pointer->u64_number_of_used_blocks_in_page_)
    {
        ptr_stc_page_storing_block_pointer->ptr_base_page_category_->u64_number_of_used_pages_in_category_ -= 1;
    }
    if (ptr_stc_global_static_manipulation_structure->u64_pagesize < ptr_stc_block_pointer_to_free->u64_size_)
    {
        if (-1 == munmap(ptr_stc_block_pointer_to_free, ptr_stc_block_pointer_to_free->u64_size_))
        {
            ft_dprintf(2, "Munmap returned -1\n");
        }
    }
    else if ((ptr_stc_page_storing_block_pointer->ptr_base_page_category_->u64_total_number_of_pages_in_category_ / 3 * 2) > ptr_stc_page_storing_block_pointer->ptr_base_page_category_->u64_number_of_used_pages_in_category_)
    {
        Fvoid__free_half_of_used_pages_from_one_page_category(ptr_stc_page_storing_block_pointer->ptr_base_page_category_);
    }
}

void    *malloc(size_t size)
{
    /**
     *  If the manipulation structure is set to NULL, initialize it.
     */
    size = Fu64__align16(size);
    if (ptr_stc_global_static_manipulation_structure == NULL)
    {
        ptr_stc_global_static_manipulation_structure = Fptr_stc_manipulation__create_manipulation_structure();
        if (ptr_stc_global_static_manipulation_structure  == NULL)
        {
            return NULL;
        }
    }
    return (Fptr_void__return_memory(ptr_stc_global_static_manipulation_structure, size));
}

void *realloc(void *ptr, size_t size)
{
    struct s_block  *ptr_stc_lcl_block_pointer_to_realloc;
    void            *ptr_void_lcl_realloced_memory;

    size = Fu64__align16(size);
    ptr_stc_lcl_block_pointer_to_realloc = NULL;
    if (size == 0)
    {
        free(ptr);
        return (malloc(0));
    }
    if (ptr == NULL)
        return (malloc(size));
    if (ptr_stc_global_static_manipulation_structure == NULL || ZERO == Fu8__bool_check_if_pointer_passed_to_free_was_previously_malloc(ptr_stc_global_static_manipulation_structure, ptr))
    {
        return (NULL);
    }
    ptr_stc_lcl_block_pointer_to_realloc = (struct s_block*)(((uint8_t*)(ptr)) - sizeof(struct s_block));
    if (ptr_stc_lcl_block_pointer_to_realloc->u64_size_ < size)
    {
        if (NULL == (ptr_void_lcl_realloced_memory = malloc(size)))
            return (NULL);
        memcpy(ptr_void_lcl_realloced_memory, (void*)(((uint8_t*)ptr_stc_lcl_block_pointer_to_realloc) + Fu64__align16(sizeof(struct s_block))), ptr_stc_lcl_block_pointer_to_realloc->u64_size_);
        free((void*)(((uint8_t*)ptr_stc_lcl_block_pointer_to_realloc) + Fu64__align16(sizeof(struct s_block))));
        return (ptr_void_lcl_realloced_memory);
    }
    ptr_stc_lcl_block_pointer_to_realloc->u64_free_size_ = ptr_stc_lcl_block_pointer_to_realloc->u64_size_ - size;
    return ((void*)(((uint8_t*)ptr_stc_lcl_block_pointer_to_realloc) + Fu64__align16(sizeof(struct s_block))));
}

void show_alloc_mem()
{
    print_alloc_memory(ptr_stc_global_static_manipulation_structure);
}
