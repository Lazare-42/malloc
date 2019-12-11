/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 16:30:47 by lazrossi          #+#    #+#             */
/*   Updated: 2019/12/11 16:58:33 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "malloc.h"
#include <sys/mman.h>
#include "libft.h"

struct s_page			*add_upper_or_extend_current_page(struct s_manipulation
		*stc_manipulation_structure,
		struct s_page *stc_last_page, uint64_t required_size)
{
	if (stc_last_page->blck_size_ < required_size)
	{
		if (NULL != (stc_last_page->nt_page_upper_category_ =
			create_and_init_new_page_category(stc_manipulation_structure,
			return_upper_page_size(stc_last_page->blck_size_,
			required_size), ZERO)))
			return (stc_last_page->nt_page_upper_category_);
	}
	if (NULL != (stc_last_page->nt_page_same_category_ =
				create_and_init_new_page_category(stc_manipulation_structure,
					stc_last_page->blck_size_, ZERO)))
		return (stc_last_page->nt_page_same_category_);
	return (NULL);
}

struct s_page			*page__return_corresponding_page_category_two(struct
		s_manipulation *stc_manipulation_structure,
		struct s_page *prev_page,
		uint64_t required_size,
		struct s_page *browse_cats)
{
	if (browse_cats == NULL)
	{
		add_upper_or_extend_current_page(stc_manipulation_structure,
				prev_page, required_size);
		if (NULL == (browse_cats = prev_page->nt_page_upper_category_))
			return (NULL);
	}
	return (browse_cats);
}

struct s_page			*page__return_corresponding_page_category(struct
		s_manipulation *stc_manipulation_structure,
		uint64_t required_size)
{
	struct s_page	*browse_cats;
	struct s_page	*prev_page;

	browse_cats = NULL;
	prev_page = NULL;
	browse_cats = stc_manipulation_structure->page_linked_list;
	while (browse_cats != NULL && required_size > browse_cats->blck_size_)
	{
		prev_page = browse_cats;
		browse_cats = browse_cats->nt_page_upper_category_;
	}
	if (browse_cats != NULL &&
			check_if_page_siz_too_big(browse_cats->blck_size_, required_size))
	{
		add_upper_or_extend_current_page(stc_manipulation_structure,
				prev_page, required_size);
		if (NULL == prev_page->nt_page_upper_category_)
			return (NULL);
		prev_page->nt_page_upper_category_->nt_page_upper_category_ =
			browse_cats;
		browse_cats = prev_page->nt_page_upper_category_;
	}
	return (page__return_corresponding_page_category_two(
	stc_manipulation_structure, prev_page, required_size, browse_cats));
}

struct s_page			*page__return_first_free_blck_from_page_category(struct
		s_manipulation *stc_manipulation_structure,
		struct s_page *stc_browse_cats)
{
	struct s_page			*base_page_category;
	struct s_page			*prev_page;

	prev_page = NULL;
	base_page_category = stc_browse_cats;
	while (stc_browse_cats != NULL && stc_browse_cats->first_free_blck_ == NULL)
	{
		prev_page = stc_browse_cats;
		stc_browse_cats = stc_browse_cats->nt_page_same_category_;
	}
	if (stc_browse_cats == NULL)
	{
		add_upper_or_extend_current_page(stc_manipulation_structure,
				prev_page, prev_page->blck_size_);
		if (NULL == (stc_browse_cats = prev_page->nt_page_same_category_))
			return (NULL);
		base_page_category->nbr_pages_in_category += 1;
		base_page_category->nbr_of_used_pages_in_category_ += 1;
	}
	stc_browse_cats->base_page_category_ = base_page_category;
	return (stc_browse_cats);
}

void					*return_memory(struct s_manipulation
		*stc_manipulation_structure, uint64_t required_size)
{
	struct s_page *browse_cats;
	struct s_page *prev_page;
	struct s_blck *memory_blck_to_return;

	browse_cats = NULL;
	prev_page = NULL;
	memory_blck_to_return = NULL;
	if (NULL == (browse_cats =
		page__return_corresponding_page_category(stc_manipulation_structure,
		required_size)))
		return (NULL);
	if (NULL == (browse_cats =
		page__return_first_free_blck_from_page_category(
		stc_manipulation_structure, browse_cats)))
		return (NULL);
	memory_blck_to_return = browse_cats->first_free_blck_;
	browse_cats->first_free_blck_ = memory_blck_to_return->nt_blck_in_page_;
	memory_blck_to_return->nt_blck_in_page_ =
		browse_cats->first_occuppied_blck_;
	browse_cats->first_occuppied_blck_ = memory_blck_to_return;
	memory_blck_to_return->free_size_ -= required_size;
	return ((void*)(((uint8_t*)memory_blck_to_return)
				+ align16(sizeof(struct s_blck))));
}
