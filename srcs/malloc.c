/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 15:23:42 by lazrossi          #+#    #+#             */
/*   Updated: 2019/10/13 15:33:03 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "malloc.h"
#include <sys/mman.h>
#include "libft.h"

static struct s_page	*add_or_extend(
		struct s_manipulation *ptr_pssd_stc_manipulation_structure,
		struct s_page *ptr_pssd_stc_last_page, uint64_t u64_pssd_required_size)
{
	if (ptr_pssd_stc_last_page->u64_block_size_ < u64_pssd_required_size)
	{
		if (NULL != (ptr_pssd_stc_last_page->ptr_next_page_upper_category_ =
		fu8__create_and_init_new_page_category(
			ptr_pssd_stc_manipulation_structure,
		fu64__return_upper_page_size(ptr_pssd_stc_last_page->u64_block_size_,
				u64_pssd_required_size), ZERO)))
			return (ptr_pssd_stc_last_page->ptr_next_page_upper_category_);
	}
	if (NULL != (ptr_pssd_stc_last_page->ptr_next_page_in_category_ =
	fu8__create_and_init_new_page_category(ptr_pssd_stc_manipulation_structure,
		ptr_pssd_stc_last_page->u64_block_size_, ZERO)))
		return (ptr_pssd_stc_last_page->ptr_next_page_in_category_);
	return (NULL);
}

static struct s_page	*fptr_page__return_corresponding_page_category(
	struct s_manipulation *ptr_pssd_stc_manipulation_structure,
	uint64_t u64_pssd_required_size)
{
	struct s_page					*browse;
	struct s_page					*previously_browsed;

	browse = ptr_pssd_stc_manipulation_structure->ptr_stc_page_linked_list;
	while (browse != NULL && u64_pssd_required_size > browse->u64_block_size_)
	{
		previously_browsed = browse;
		browse = browse->ptr_next_page_upper_category_;
	}
	if (browse != NULL && fu8_bool__check_if_chosen_page_size_too_big(
		browse->u64_block_size_, u64_pssd_required_size))
	{
		add_or_extend(ptr_pssd_stc_manipulation_structure,
		previously_browsed, u64_pssd_required_size);
		if (NULL == previously_browsed->ptr_next_page_upper_category_)
			return (NULL);
		previously_browsed->ptr_next_page_upper_category_->
		ptr_next_page_upper_category_ = browse;
		browse = previously_browsed->ptr_next_page_upper_category_;
	}
	if (browse == NULL && add_or_extend(ptr_pssd_stc_manipulation_structure,
		previously_browsed, u64_pssd_required_size) && (NULL == (browse =
		previously_browsed->ptr_next_page_upper_category_)))
		return (NULL);
	return (browse);
}

static struct s_page	*fptr_page__return_first_free_block_from_page_category(
		struct s_manipulation *ptr_pssd_stc_manipulation_structure,
		struct s_page *browse_page_category)
{
	struct s_page					*base_page_category;
	struct s_page					*ptr_stc_lcl_previously_browsed_page;

	ptr_stc_lcl_previously_browsed_page = NULL;
	base_page_category = browse_page_category;
	while (browse_page_category != NULL
		&& browse_page_category->ptr_first_free_block_ == NULL)
	{
		ptr_stc_lcl_previously_browsed_page = browse_page_category;
		browse_page_category = browse_page_category->ptr_next_page_in_category_;
	}
	if (browse_page_category == NULL)
	{
		add_or_extend(
		ptr_pssd_stc_manipulation_structure, ptr_stc_lcl_previously_browsed_page
		, ptr_stc_lcl_previously_browsed_page->u64_block_size_);
		if (NULL == (browse_page_category =
			ptr_stc_lcl_previously_browsed_page->ptr_next_page_in_category_))
			return (NULL);
		base_page_category->u64_total_number_of_pages_in_category_ += 1;
		base_page_category->u64_number_of_used_pages_in_category_ += 1;
	}
	browse_page_category->ptr_base_page_category_ =
		base_page_category;
	return (browse_page_category);
}

static struct s_page	*fptr_page__return_page_from_category(struct
		s_manipulation *ptr_pssd_stc_manipulation_structure,
		uint64_t u64_pssd_required_size,
		struct s_page *ptr_pssd_stc_browse_page_category)
{
	if (NULL == (ptr_pssd_stc_browse_page_category =
	fptr_page__return_corresponding_page_category(
	ptr_pssd_stc_manipulation_structure, u64_pssd_required_size)))
		return (NULL);
	if (NULL == (ptr_pssd_stc_browse_page_category =
				fptr_page__return_first_free_block_from_page_category(
			ptr_pssd_stc_manipulation_structure,
		ptr_pssd_stc_browse_page_category)))
		return (NULL);
	return (ptr_pssd_stc_browse_page_category);
}

void					*fptr_void__return_memory(struct s_manipulation
		*ptr_pssd_stc_manipulation_structure, uint64_t u64_pssd_required_size)
{
	struct s_page					*ptr_stc_lcl_browse_page_category;
	struct s_page					*ptr_stc_lcl_previously_browsed_page;
	struct s_block					*ptr_stc_lcl_memory_block_to_return;

	ptr_stc_lcl_browse_page_category = NULL;
	ptr_stc_lcl_previously_browsed_page = NULL;
	ptr_stc_lcl_memory_block_to_return = NULL;
	if (NULL == (ptr_stc_lcl_browse_page_category =
	fptr_page__return_page_from_category(
	ptr_pssd_stc_manipulation_structure,
	u64_pssd_required_size, ptr_stc_lcl_browse_page_category)))
		return (NULL);
	ptr_stc_lcl_memory_block_to_return =
		ptr_stc_lcl_browse_page_category->ptr_first_free_block_;
	ptr_stc_lcl_browse_page_category->ptr_first_free_block_ =
		ptr_stc_lcl_memory_block_to_return->ptr_next_block_in_page_;
	ptr_stc_lcl_memory_block_to_return->ptr_next_block_in_page_ =
		ptr_stc_lcl_browse_page_category->ptr_first_occuppied_block_;
	ptr_stc_lcl_browse_page_category->ptr_first_occuppied_block_ =
		ptr_stc_lcl_memory_block_to_return;
	ptr_stc_lcl_memory_block_to_return->u64_free_size_ -=
		u64_pssd_required_size;
	return ((void*)(((uint8_t*)ptr_stc_lcl_memory_block_to_return)
				+ fu64__align16(sizeof(struct s_block))));
}
