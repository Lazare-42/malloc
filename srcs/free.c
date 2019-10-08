#include "malloc.h"
#include <stdio.h>
#include <sys/mman.h>

void Fvoid__free_chosen_page(struct s_page *ptr_pssd_first_page_for_category, struct s_page **ptr_pssd_page_to_free_and_browse, struct s_page *ptr_pssd_page_before_page_to_free)
{
    struct s_page   *ptr_stc_lcl_page_to_free;

    ptr_stc_lcl_page_to_free = NULL;
    ptr_pssd_page_before_page_to_free->ptr_next_page_same_category_ = (*ptr_pssd_page_to_free_and_browse)->ptr_next_page_same_category_;
    ptr_stc_lcl_page_to_free = *ptr_pssd_page_to_free_and_browse;
    (*ptr_pssd_page_to_free_and_browse) = (*ptr_pssd_page_to_free_and_browse)->ptr_next_page_same_category_;
    munmap(ptr_stc_lcl_page_to_free, ptr_stc_lcl_page_to_free->u64_size_);
    ptr_pssd_first_page_for_category->u64_total_number_of_pages_in_category_ -= 1;
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
    fprintf(stderr, "Number of pages to free is %20llu, number of pages in category %20llu, number of used pages %20llu\n", u64_lcl_number_of_pages_to_free, ptr_pssd_first_page_for_category->u64_total_number_of_pages_in_category_, ptr_pssd_first_page_for_category->u64_number_of_used_pages_in_category_);
    while (u64_lcl_number_of_pages_to_free > ZERO && NULL != ptr_stc_lcl_page_to_browse)
    {
        if (ZERO == ptr_stc_lcl_page_to_browse->u64_number_of_used_blocks_in_page_)
        {
            Fvoid__free_chosen_page(ptr_pssd_first_page_for_category, &ptr_stc_lcl_page_to_browse, ptr_stc_lcl_page_before_last_browsed_elem);
            u64_lcl_number_of_pages_to_free = u64_lcl_number_of_pages_to_free - 1;
        }
        else
        {
            ptr_stc_lcl_page_before_last_browsed_elem   = ptr_stc_lcl_page_to_browse;
            ptr_stc_lcl_page_to_browse                  = ptr_stc_lcl_page_to_browse->ptr_next_page_same_category_;
        }
    }
}

