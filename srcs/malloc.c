/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 18:01:52 by lazrossi          #+#    #+#             */
/*   Updated: 2019/12/10 11:22:47 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "malloc.h"
#include <sys/mman.h>
#include "libft.h"

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
    ptr_stc_lcl_browse_page_category->ptr_first_free_block_ = ptr_stc_lcl_memory_block_to_return->ptr_next_block_in_page_;
    ptr_stc_lcl_memory_block_to_return->ptr_next_block_in_page_ = ptr_stc_lcl_browse_page_category->ptr_first_occuppied_block_;
    ptr_stc_lcl_browse_page_category->ptr_first_occuppied_block_ = ptr_stc_lcl_memory_block_to_return;
    ptr_stc_lcl_memory_block_to_return->u64_free_size_      -= u64_pssd_required_size;
    return ((void*)(((uint8_t*)ptr_stc_lcl_memory_block_to_return) + Fu64__align16(sizeof(struct s_block))));
}

