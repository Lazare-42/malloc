/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 15:57:28 by lazrossi          #+#    #+#             */
/*   Updated: 2019/12/11 17:09:56 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>
#include "libft.h"

uint8_t		check_prev_realloc_two(struct s_page *page_to_browse,
					void *ptr_passed_to_free)
{
	struct s_blck *browse_alloced_blcks;

	browse_alloced_blcks = NULL;
	browse_alloced_blcks = page_to_browse->first_occuppied_blck_;
	while (NULL != browse_alloced_blcks)
	{
		if ((void*)(((uint8_t*)browse_alloced_blcks)
			+ align16(sizeof(struct s_blck))) == ptr_passed_to_free)
			return ((uint8_t)1);
		browse_alloced_blcks = browse_alloced_blcks->nt_blck_in_page_;
	}
	return (ZERO);
}

uint8_t		check_prev_realloc(struct s_manipulation *manipulation_stc,
					void *ptr_passed_to_free)
{
	struct s_page *page_category_to_browse;
	struct s_page *page_to_browse;

	page_category_to_browse = NULL;
	page_to_browse = NULL;
	page_category_to_browse = manipulation_stc->page_linked_list;
	while (NULL != page_category_to_browse)
	{
		page_to_browse = page_category_to_browse;
		while (NULL != page_to_browse)
		{
			if (check_prev_realloc_two(page_to_browse, ptr_passed_to_free))
				return ((uint8_t)1);
			page_to_browse = page_to_browse->nt_page_same_category_;
		}
		page_category_to_browse =
			page_category_to_browse->nt_page_upper_category_;
	}
	return (ZERO);
}

uint8_t		free_chosen_page(struct s_page *first_page_for_category,
					struct s_page **page_to_free_and_browse,
					struct s_page *page_before_page_to_free)
{
	struct s_page *page_to_free;

	page_to_free = NULL;
	page_before_page_to_free->nt_page_same_category_ =
		(*page_to_free_and_browse)->nt_page_same_category_;
	page_to_free = *page_to_free_and_browse;
	(*page_to_free_and_browse) =
		(*page_to_free_and_browse)->nt_page_same_category_;
	if (-1 == munmap(page_to_free, page_to_free->size_))
	{
		ft_dprintf(2, "Munmap returned -1\n");
		return (RETURN_FAILURE);
	}
	first_page_for_category->nbr_pages_in_category -= 1;
	return (RETURN_SUCCESS);
}

void		free_half_page_category(
					struct s_page *first_page_for_category)
{
	uint64_t					nbr_of_pages_to_free;
	struct s_page				*page_to_browse;
	struct s_page				*page_before_last_browsed_elem;

	nbr_of_pages_to_free = ZERO;
	page_before_last_browsed_elem = first_page_for_category;
	page_to_browse = first_page_for_category->nt_page_same_category_;
	nbr_of_pages_to_free = (first_page_for_category->nbr_pages_in_category
			- first_page_for_category->nbr_of_used_pages_in_category_) / 2;
	while (nbr_of_pages_to_free > ZERO && NULL != page_to_browse)
	{
		if (ZERO == page_to_browse->nbr_of_used_blcks_in_page_)
		{
			if (RETURN_FAILURE ==
			(free_chosen_page(first_page_for_category,
			&page_to_browse, page_before_last_browsed_elem)))
				return ;
			nbr_of_pages_to_free = nbr_of_pages_to_free - 1;
		}
		else
		{
			page_before_last_browsed_elem = page_to_browse;
			page_to_browse = page_to_browse->nt_page_same_category_;
		}
	}
}
