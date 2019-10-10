#include "malloc.h"
#include <sys/mman.h>
#include <string.h>
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
        ft_printf("[[red]]Rejecting free because pointer was not previously allocated[[end]]\n");
        return ;
    }
    ft_printf("Entering free\n");
    ptr_stc_block_pointer_to_free                                           = (struct s_block*)(((uint8_t*)(ptr)) - sizeof(struct s_block));
    ptr_stc_block_pointer_to_free->u64_free_size_                           = ptr_stc_block_pointer_to_free->u64_size_;
    ptr_stc_page_storing_block_pointer                                      = ptr_stc_block_pointer_to_free->ptr_page_base_;
    ptr_stc_page_storing_block_pointer->u64_number_of_used_blocks_in_page_  = ptr_stc_page_storing_block_pointer->u64_number_of_used_blocks_in_page_ - 1;
    if (ZERO == ptr_stc_page_storing_block_pointer->u64_number_of_used_blocks_in_page_)
    {
        ptr_stc_page_storing_block_pointer->ptr_base_page_category_->u64_number_of_used_pages_in_category_ -= 1;
    }
    if (ptr_stc_global_static_manipulation_structure->u64_pagesize < ptr_stc_block_pointer_to_free->u64_size_)
    {
        munmap(ptr_stc_block_pointer_to_free, ptr_stc_block_pointer_to_free->u64_size_);
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

    ptr_stc_lcl_block_pointer_to_realloc = NULL;
    if (ptr_stc_global_static_manipulation_structure == NULL || ZERO == Fu8__bool_check_if_pointer_passed_to_free_was_previously_malloc(ptr_stc_global_static_manipulation_structure, ptr))
    {
        ft_printf("[[red]]Redirecting call to realloc to malloc() because pointer %p because pointer was not previously allocated[[end]]\n", ptr);
        return (malloc(size));
    }
    ft_printf("Entering realloc\n");
    ptr_stc_lcl_block_pointer_to_realloc = (struct s_block*)(((uint8_t*)(ptr)) - sizeof(struct s_block));
    if (ptr_stc_lcl_block_pointer_to_realloc->u64_size_ < size)
    {
        ptr_void_lcl_realloced_memory = malloc(size);
        memcpy(ptr_void_lcl_realloced_memory, (void*)(((uint8_t*)ptr_stc_lcl_block_pointer_to_realloc) + Fu64__align16(sizeof(struct s_block))), ptr_stc_lcl_block_pointer_to_realloc->u64_size_);
        free((void*)(((uint8_t*)ptr_stc_lcl_block_pointer_to_realloc) + Fu64__align16(sizeof(struct s_block))));
        return (ptr_void_lcl_realloced_memory);
    }
    ptr_stc_lcl_block_pointer_to_realloc->u64_free_size_ = ptr_stc_lcl_block_pointer_to_realloc->u64_size_ - size;
    return ((void*)(((uint8_t*)ptr_stc_lcl_block_pointer_to_realloc) + Fu64__align16(sizeof(struct s_block))));
}

void *calloc(size_t elemn, size_t size)
{
    ft_printf("[[red]]In caloc, function not created yet ![[end]]\n");
    (void)elemn;
    (void)size;
    return NULL;
}
