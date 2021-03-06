#include "malloc.h"
#include <sys/mman.h>
#include "libft.h"


uint8_t Fu8__bool_check_if_pointer_passed_to_free_was_previously_malloc(struct s_manipulation *ptr_pssd_manipulation_structure, void *ptr_pssd_pointer_passed_to_free)
{
    struct s_page   *ptr_stc_lcl_page_category_to_browse;
    struct s_page   *ptr_stc_lcl_page_inside_category_to_browse;
    struct s_block  *ptr_stc_lcl_browse_alloced_blocks;

    ptr_stc_lcl_page_category_to_browse         = NULL;
    ptr_stc_lcl_page_inside_category_to_browse  = NULL;
    ptr_stc_lcl_browse_alloced_blocks           = NULL;
    ptr_stc_lcl_page_category_to_browse         = ptr_pssd_manipulation_structure->ptr_stc_page_linked_list;
    while (NULL != ptr_stc_lcl_page_category_to_browse)
    {
        ptr_stc_lcl_page_inside_category_to_browse = ptr_stc_lcl_page_category_to_browse;
        while (NULL != ptr_stc_lcl_page_inside_category_to_browse)
        {
            ptr_stc_lcl_browse_alloced_blocks = ptr_stc_lcl_page_inside_category_to_browse->ptr_first_occuppied_block_;
            while (NULL != ptr_stc_lcl_browse_alloced_blocks)
            {
                if ((void*)(((uint8_t*)ptr_stc_lcl_browse_alloced_blocks) + Fu64__align16(sizeof(struct s_block))) == ptr_pssd_pointer_passed_to_free)
                {
                    return ((uint8_t)1);
                }
                ptr_stc_lcl_browse_alloced_blocks = ptr_stc_lcl_browse_alloced_blocks->ptr_next_block_in_page_;
            }
        ptr_stc_lcl_page_inside_category_to_browse = ptr_stc_lcl_page_inside_category_to_browse->ptr_next_page_same_category_;
        }
        ptr_stc_lcl_page_category_to_browse = ptr_stc_lcl_page_category_to_browse->ptr_next_page_upper_category_;
    }
    return (ZERO);
}

uint8_t Fvoid__free_chosen_page(struct s_page *ptr_pssd_first_page_for_category, struct s_page **ptr_pssd_page_to_free_and_browse, struct s_page *ptr_pssd_page_before_page_to_free)
{
    struct s_page   *ptr_stc_lcl_page_to_free;

    ptr_stc_lcl_page_to_free = NULL;
    ptr_pssd_page_before_page_to_free->ptr_next_page_same_category_ = (*ptr_pssd_page_to_free_and_browse)->ptr_next_page_same_category_;
    ptr_stc_lcl_page_to_free = *ptr_pssd_page_to_free_and_browse;
    (*ptr_pssd_page_to_free_and_browse) = (*ptr_pssd_page_to_free_and_browse)->ptr_next_page_same_category_;
    if (-1 == munmap(ptr_stc_lcl_page_to_free, ptr_stc_lcl_page_to_free->u64_size_))
        {
        ft_dprintf(2, "Munmap returned -1\n");
        return RETURN_FAILURE;
        }
    ptr_pssd_first_page_for_category->u64_total_number_of_pages_in_category_ -= 1;
    return RETURN_SUCCESS;
}

void Fvoid__free_half_of_used_pages_from_one_page_category(struct s_page *ptr_pssd_first_page_for_category)
{
    uint64_t        u64_lcl_number_of_pages_to_free;
    struct s_page   *ptr_stc_lcl_page_to_browse;
    struct s_page   *ptr_stc_lcl_page_before_last_browsed_elem;

    u64_lcl_number_of_pages_to_free             = ZERO;
    ptr_stc_lcl_page_before_last_browsed_elem   = ptr_pssd_first_page_for_category;
    ptr_stc_lcl_page_to_browse                  = ptr_pssd_first_page_for_category->ptr_next_page_same_category_;
    u64_lcl_number_of_pages_to_free             = (ptr_pssd_first_page_for_category->u64_total_number_of_pages_in_category_ - ptr_pssd_first_page_for_category->u64_number_of_used_pages_in_category_) / 2;
    while (u64_lcl_number_of_pages_to_free > ZERO && NULL != ptr_stc_lcl_page_to_browse)
    {
        if (ZERO == ptr_stc_lcl_page_to_browse->u64_number_of_used_blocks_in_page_)
        {
            if (RETURN_FAILURE == (Fvoid__free_chosen_page(ptr_pssd_first_page_for_category, &ptr_stc_lcl_page_to_browse, ptr_stc_lcl_page_before_last_browsed_elem)))
                return ;
            u64_lcl_number_of_pages_to_free = u64_lcl_number_of_pages_to_free - 1;
        }
        else
        {
            ptr_stc_lcl_page_before_last_browsed_elem   = ptr_stc_lcl_page_to_browse;
            ptr_stc_lcl_page_to_browse                  = ptr_stc_lcl_page_to_browse->ptr_next_page_same_category_;
        }
    }
}

