/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 18:01:52 by lazrossi          #+#    #+#             */
/*   Updated: 2019/10/07 18:38:38 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "malloc.h"
#include <sys/mman.h>

static struct s_manipulation    *ptr_stc_lcl_manipulation_structure = NULL;

uint64_t Fu64__align16(size_t number)
{
    return ((uint64_t)(((number - 1) | (16 - 1)) + 1));
}

uint8_t  Fu8_bool__check_if_chosen_page_size_too_big_for_required_size(uint64_t u64_next_page_size, uint64_t u64_pssd_required_size)
{
    if (u64_next_page_size > (u64_pssd_required_size << 1) && u64_pssd_required_size > TINY)
    {
        return ((uint8_t)1);
    }
    return ((uint8_t)ZERO);
}

uint64_t Fu64__return_lower_page_size(uint64_t u64_pssd_next_page_size, uint64_t u64_pssd_required_size)
{
    uint64_t u64_lcl_new_page_size;

    u64_lcl_new_page_size = ZERO;
    u64_lcl_new_page_size = u64_pssd_next_page_size;
    while (u64_lcl_new_page_size > (u64_pssd_required_size << 1))
    {
    u64_lcl_new_page_size = u64_lcl_new_page_size >> 1;
    }
    return (u64_lcl_new_page_size);
}

uint64_t Fu64__return_upper_page_size(uint64_t u64_previous_page_size, uint64_t u64_pssd_required_size)
{
    uint64_t u64_lcl_new_page_size;

    u64_lcl_new_page_size = ZERO;
    u64_lcl_new_page_size = (u64_previous_page_size << 1);
    while (u64_lcl_new_page_size < u64_pssd_required_size) 
    {
    u64_lcl_new_page_size = (u64_lcl_new_page_size << 1);
    }
    return (u64_lcl_new_page_size);
}

struct s_page *Fu8__add_upper_page_category_or_extend_current_page_category(struct s_manipulation *ptr_pssd_stc_manipulation_structure, struct s_page *ptr_pssd_stc_last_page, uint64_t u64_pssd_required_size)
{
    if (ptr_pssd_stc_last_page->u64_block_size_ < u64_pssd_required_size)
    {
        if (NULL != (ptr_pssd_stc_last_page->ptr_next_page_upper_category_ = Fu8__create_and_init_new_page_category(ptr_pssd_stc_manipulation_structure, Fu64__return_upper_page_size(ptr_pssd_stc_last_page->u64_block_size_, u64_pssd_required_size), ZERO)))
        {
            return (ptr_pssd_stc_last_page->ptr_next_page_upper_category_);
        }
    }
    if (NULL != (ptr_pssd_stc_last_page->ptr_next_page_same_category_ = Fu8__create_and_init_new_page_category(ptr_pssd_stc_manipulation_structure, ptr_pssd_stc_last_page->u64_block_size_, ZERO)))
    {
        return (ptr_pssd_stc_last_page->ptr_next_page_same_category_);
    }
    return (NULL);
}

struct s_page *Fptr_page__return_corresponding_page_category(struct s_manipulation *ptr_pssd_stc_manipulation_structure, uint64_t u64_pssd_required_size)
{
    struct s_page                   *ptr_stc_lcl_browse_page_category;
    struct s_page                   *ptr_stc_lcl_previously_browsed_page;

    ptr_stc_lcl_browse_page_category        = NULL;
    ptr_stc_lcl_previously_browsed_page     = NULL;
    ptr_stc_lcl_browse_page_category       = ptr_pssd_stc_manipulation_structure->ptr_stc_page_linked_list;
    while (ptr_stc_lcl_browse_page_category != NULL && u64_pssd_required_size > ptr_stc_lcl_browse_page_category->u64_block_size_)
    {
    ptr_stc_lcl_previously_browsed_page = ptr_stc_lcl_browse_page_category;
    ptr_stc_lcl_browse_page_category = ptr_stc_lcl_browse_page_category->ptr_next_page_upper_category_;
    }
    if (ptr_stc_lcl_browse_page_category != NULL && Fu8_bool__check_if_chosen_page_size_too_big_for_required_size(ptr_stc_lcl_browse_page_category->u64_block_size_, u64_pssd_required_size))
    {
        Fu8__add_upper_page_category_or_extend_current_page_category(ptr_pssd_stc_manipulation_structure, ptr_stc_lcl_previously_browsed_page, u64_pssd_required_size);
        if (NULL == ptr_stc_lcl_previously_browsed_page->ptr_next_page_upper_category_)
        {
            return (NULL);
        }
        ptr_stc_lcl_previously_browsed_page->ptr_next_page_upper_category_->ptr_next_page_upper_category_ = ptr_stc_lcl_browse_page_category;
        ptr_stc_lcl_browse_page_category = ptr_stc_lcl_previously_browsed_page->ptr_next_page_upper_category_;
    }
    if (ptr_stc_lcl_browse_page_category == NULL) 
    {
        Fu8__add_upper_page_category_or_extend_current_page_category(ptr_pssd_stc_manipulation_structure, ptr_stc_lcl_previously_browsed_page, u64_pssd_required_size);
        if (NULL == (ptr_stc_lcl_browse_page_category = ptr_stc_lcl_previously_browsed_page->ptr_next_page_upper_category_))
        {
            return (NULL);
        }
    }
    return (ptr_stc_lcl_browse_page_category);
}

struct s_page *Fptr_page__return_first_free_block_from_page_category(struct s_manipulation *ptr_pssd_stc_manipulation_structure, struct s_page *ptr_pssd_stc_browse_page_category)
{
    struct s_page                   *ptr_stc_lcl_base_page_category;
    struct s_page                   *ptr_stc_lcl_previously_browsed_page;

    ptr_stc_lcl_previously_browsed_page     = NULL;
    ptr_stc_lcl_base_page_category          = ptr_pssd_stc_browse_page_category;
    while (ptr_pssd_stc_browse_page_category != NULL && ptr_pssd_stc_browse_page_category->ptr_first_free_block_ == NULL)
    {
    ptr_stc_lcl_previously_browsed_page = ptr_pssd_stc_browse_page_category;
    ptr_pssd_stc_browse_page_category = ptr_pssd_stc_browse_page_category->ptr_next_page_same_category_;
    }
    if (ptr_pssd_stc_browse_page_category == NULL) 
    {
        Fu8__add_upper_page_category_or_extend_current_page_category(ptr_pssd_stc_manipulation_structure, ptr_stc_lcl_previously_browsed_page, ptr_stc_lcl_previously_browsed_page->u64_block_size_);
        if (NULL == (ptr_pssd_stc_browse_page_category = ptr_stc_lcl_previously_browsed_page->ptr_next_page_same_category_))
        {
            return (NULL);
        }
        ptr_stc_lcl_base_page_category->u64_total_number_of_pages_in_category_  += 1;
        ptr_stc_lcl_base_page_category->u64_number_of_used_pages_in_category_   += 1;
    }
    ptr_pssd_stc_browse_page_category->ptr_base_page_category_              = ptr_stc_lcl_base_page_category;
    return (ptr_pssd_stc_browse_page_category);
}

void    *Fptr_void__return_memory(struct s_manipulation *ptr_pssd_stc_manipulation_structure, uint64_t u64_pssd_required_size)
{
    struct s_page                   *ptr_stc_lcl_browse_page_category;
    struct s_page                   *ptr_stc_lcl_previously_browsed_page;
    struct s_block                  *ptr_stc_lcl_memory_block_to_return;

    ptr_stc_lcl_browse_page_category        = NULL;
    ptr_stc_lcl_previously_browsed_page     = NULL;
    ptr_stc_lcl_memory_block_to_return      = NULL;
    if (NULL == (ptr_stc_lcl_browse_page_category = Fptr_page__return_corresponding_page_category(ptr_pssd_stc_manipulation_structure, u64_pssd_required_size)))
    {
        return (NULL);
    }
    if (NULL == (ptr_stc_lcl_browse_page_category = Fptr_page__return_first_free_block_from_page_category(ptr_pssd_stc_manipulation_structure, ptr_stc_lcl_browse_page_category)))
    {
        return (NULL);
    }
    ptr_stc_lcl_memory_block_to_return                      = ptr_stc_lcl_browse_page_category->ptr_first_free_block_;
    ptr_stc_lcl_browse_page_category->ptr_first_free_block_ = ptr_stc_lcl_browse_page_category->ptr_first_free_block_->ptr_next_;
    ptr_stc_lcl_memory_block_to_return->u64_free_size_      -= u64_pssd_required_size;
    return ((void*)(((uint8_t*)ptr_stc_lcl_memory_block_to_return) + Fu64__align16(sizeof(struct s_block))));
}

void    *my_malloc(size_t size)
{
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

void Fvoid__free_half_of_used_pages_from_one_page_category(struct s_page *ptr_pssd_first_page_for_category)
{
    uint64_t        u64_lcl_number_of_pages_to_free;
    struct s_page   *ptr_stc_lcl_page_to_browse;
    struct s_page   *ptr_stc_lcl_page_before_last_browsed_elem;
    struct s_page   *ptr_stc_lcl_page_to_free;

    u64_lcl_number_of_pages_to_free             = ZERO;
    ptr_stc_lcl_page_to_browse                  = ptr_pssd_first_page_for_category;
    u64_lcl_number_of_pages_to_free             = (ptr_pssd_first_page_for_category->u64_total_number_of_pages_in_category_ - ptr_pssd_first_page_for_category->u64_number_of_used_pages_in_category_) / 2;
    fprintf(stderr, "Number of pages to free is %20llu, number of pages in category %20llu, number of used pages %20llu\n", u64_lcl_number_of_pages_to_free, ptr_pssd_first_page_for_category->u64_total_number_of_pages_in_category_, ptr_pssd_first_page_for_category->u64_number_of_used_pages_in_category_);
    while (u64_lcl_number_of_pages_to_free > ZERO && NULL != ptr_stc_lcl_page_to_browse)
    {
        if (ZERO == ptr_stc_lcl_page_to_browse->u64_number_of_used_blocks_in_page_ && ptr_stc_lcl_page_to_browse != ptr_pssd_first_page_for_category)
        {
            fprintf(stderr, "Deleting a page\n");
            ptr_stc_lcl_page_before_last_browsed_elem->ptr_next_page_same_category_->ptr_next_page_same_category_ = ptr_stc_lcl_page_to_browse->ptr_next_page_same_category_;
            ptr_stc_lcl_page_to_free = ptr_stc_lcl_page_to_browse;
            ptr_stc_lcl_page_to_browse = ptr_stc_lcl_page_to_browse->ptr_next_page_same_category_;
            munmap(ptr_stc_lcl_page_to_free, ptr_stc_lcl_page_to_browse->u64_size_);
            ptr_pssd_first_page_for_category->u64_total_number_of_pages_in_category_ -= 1;
            u64_lcl_number_of_pages_to_free = u64_lcl_number_of_pages_to_free - 1;
        }
        else
        {
            ptr_stc_lcl_page_before_last_browsed_elem   = ptr_stc_lcl_page_to_browse;
            ptr_stc_lcl_page_to_browse                  = ptr_stc_lcl_page_to_browse->ptr_next_page_same_category_;
        }
    }
}

void my_free(void *ptr)
{
    struct s_block  *ptr_stc_block_pointer_to_free;
    struct s_page   *ptr_stc_page_storing_block_pointer;
    //uint64_t        u64_lcl_free_pages;

    ptr_stc_block_pointer_to_free       = NULL;
    ptr_stc_page_storing_block_pointer  = NULL;
    /**
     *  Should first check if passed pointer belongs the all pointers.
     */
    if (ptr_stc_lcl_manipulation_structure == NULL)
    {
        return ;
    }
    ptr_stc_block_pointer_to_free                                           = (struct s_block*)(((uint8_t*)(ptr)) - sizeof(struct s_block));
    ptr_stc_block_pointer_to_free->u64_free_size_                           = ptr_stc_block_pointer_to_free->u64_size_;
    ptr_stc_page_storing_block_pointer                                      = ptr_stc_block_pointer_to_free->ptr_page_base_;
    ptr_stc_page_storing_block_pointer->u64_number_of_used_blocks_in_page_  = ptr_stc_page_storing_block_pointer->u64_number_of_used_blocks_in_page_ - 1;
    if (ZERO == ptr_stc_page_storing_block_pointer->u64_number_of_used_blocks_in_page_)
    {
        ptr_stc_page_storing_block_pointer->ptr_base_page_category_->u64_number_of_used_pages_in_category_ -= 1;
    }
    if (ptr_stc_lcl_manipulation_structure->u64_pagesize < ptr_stc_block_pointer_to_free->u64_size_)
    {
        munmap(ptr_stc_block_pointer_to_free, ptr_stc_block_pointer_to_free->u64_size_);
    }
    else if ((ptr_stc_page_storing_block_pointer->ptr_base_page_category_->u64_total_number_of_pages_in_category_ / 3 * 2) > ptr_stc_page_storing_block_pointer->ptr_base_page_category_->u64_number_of_used_pages_in_category_)
    {
        //fprintf(stderr, "Decreasing used page category number ; now at : %20llu, with a total number of pages at %20llu\n", ptr_stc_page_storing_block_pointer->ptr_base_page_category_->u64_number_of_used_pages_in_category_, ptr_stc_page_storing_block_pointer->ptr_base_page_category_->u64_total_number_of_pages_in_category_);
        Fvoid__free_half_of_used_pages_from_one_page_category(ptr_stc_page_storing_block_pointer->ptr_base_page_category_);
    }
}
