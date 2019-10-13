/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 12:41:44 by lazrossi          #+#    #+#             */
/*   Updated: 2019/10/13 12:41:46 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>
#include "libft.h"

static uint8_t		fu8__bool_check_if_malloced_two(struct s_page
		*ptr_pssd_page_to_browse, void *ptr_pssd_pointer_free)
{
	struct s_block	*ptr_stc_lcl_browse_alloced_blocks;

	ptr_stc_lcl_browse_alloced_blocks =
		ptr_pssd_page_to_browse->ptr_first_occuppied_block_;
	while (NULL != ptr_stc_lcl_browse_alloced_blocks)
	{
		if ((void*)(((uint8_t*)ptr_stc_lcl_browse_alloced_blocks)
			+ fu64__align16(sizeof(struct s_block))) == ptr_pssd_pointer_free)
		{
			return ((uint8_t)1);
		}
		ptr_stc_lcl_browse_alloced_blocks =
		ptr_stc_lcl_browse_alloced_blocks->ptr_next_block_in_page_;
	}
	return (ZERO);
}

uint8_t				fu8__bool_check_if_malloced(struct s_manipulation *
		ptr_pssd_manipulation_structure, void *ptr_pssd_pointer_free)
{
	struct s_page	*ptr_stc_lcl_page_category_to_browse;
	struct s_page	*ptr_stc_lcl_page_inside_category_to_browse;

	ptr_stc_lcl_page_category_to_browse = NULL;
	ptr_stc_lcl_page_inside_category_to_browse = NULL;
	ptr_stc_lcl_page_category_to_browse =
		ptr_pssd_manipulation_structure->ptr_stc_page_linked_list;
	while (NULL != ptr_stc_lcl_page_category_to_browse)
	{
		ptr_stc_lcl_page_inside_category_to_browse =
			ptr_stc_lcl_page_category_to_browse;
		while (NULL != ptr_stc_lcl_page_inside_category_to_browse)
		{
			if (1 == fu8__bool_check_if_malloced_two(
			ptr_stc_lcl_page_inside_category_to_browse, ptr_pssd_pointer_free))
				return ((uint8_t)1);
			ptr_stc_lcl_page_inside_category_to_browse =
			ptr_stc_lcl_page_inside_category_to_browse->
			ptr_next_page_in_category_;
		}
		ptr_stc_lcl_page_category_to_browse =
		ptr_stc_lcl_page_category_to_browse->ptr_next_page_upper_category_;
	}
	return (ZERO);
}

static void			fvoid__free_chosen_page(struct s_page *
		ptr_pssd_fst_page_in_category, struct s_page **
		ptr_pssd_page_to_free_and_browse, struct s_page *
		ptr_pssd_page_before_page_to_free)
{
	struct s_page	*ptr_stc_lcl_page_to_free;

	ptr_stc_lcl_page_to_free = NULL;
	ptr_pssd_page_before_page_to_free->ptr_next_page_in_category_ =
		(*ptr_pssd_page_to_free_and_browse)->ptr_next_page_in_category_;
	ptr_stc_lcl_page_to_free = *ptr_pssd_page_to_free_and_browse;
	(*ptr_pssd_page_to_free_and_browse) =
		(*ptr_pssd_page_to_free_and_browse)->ptr_next_page_in_category_;
	munmap(ptr_stc_lcl_page_to_free, ptr_stc_lcl_page_to_free->u64_size_);
	ptr_pssd_fst_page_in_category->u64_total_number_of_pages_in_category_ -= 1;
}

void				fvoid__free_half_of_used_pages_from_one_page(struct s_page *
		ptr_pssd_fst_page_in_category)
{
	uint64_t		u64_lcl_pages_to_free;
	struct s_page	*page_to_browse;
	struct s_page	*ptr_stc_lcl_page_before_last_browsed;

	u64_lcl_pages_to_free = ZERO;
	ptr_stc_lcl_page_before_last_browsed = ptr_pssd_fst_page_in_category;
	page_to_browse = ptr_pssd_fst_page_in_category->ptr_next_page_in_category_;
	u64_lcl_pages_to_free = (ptr_pssd_fst_page_in_category->
	u64_total_number_of_pages_in_category_ - ptr_pssd_fst_page_in_category->
	u64_number_of_used_pages_in_category_) / 2;
	while (u64_lcl_pages_to_free > ZERO && NULL != page_to_browse)
	{
		if (ZERO == page_to_browse->
				u64_number_of_used_blocks_in_page_)
		{
			fvoid__free_chosen_page(ptr_pssd_fst_page_in_category,
			&page_to_browse, ptr_stc_lcl_page_before_last_browsed);
			u64_lcl_pages_to_free = u64_lcl_pages_to_free - 1;
		}
		else
		{
			ptr_stc_lcl_page_before_last_browsed = page_to_browse;
			page_to_browse = page_to_browse->ptr_next_page_in_category_;
		}
	}
}
