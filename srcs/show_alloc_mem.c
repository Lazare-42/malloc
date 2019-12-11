/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 18:14:10 by lazrossi          #+#    #+#             */
/*   Updated: 2019/12/10 18:14:19 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

void	show_one_address(struct s_blck *stc_blck_to_show)
{
	ft_printf("%p - %p : %d bytes\n", (void*)(((uint8_t*)stc_blck_to_show)
	+ align16(sizeof(struct s_blck))), (void*)(((uint8_t*)stc_blck_to_show)
	+ align16(sizeof(struct s_blck))) + stc_blck_to_show->size_
	- stc_blck_to_show->free_size_, stc_blck_to_show->size_
	- stc_blck_to_show->free_size_);
}

void	show_one_list_category(struct s_page *page_to_show)
{
	struct s_blck *browse_alloced_blcks;

	browse_alloced_blcks = NULL;
	browse_alloced_blcks = page_to_show->first_occuppied_blck_;
	if (page_to_show->blck_size_ == TINY)
		ft_printf("TINY : %p\n", (void*)(((uint8_t*)page_to_show)
					+ align16(sizeof(struct s_page))));
	else if (page_to_show->blck_size_ == SMALL)
		ft_printf("SMALL : %p\n", (void*)(((uint8_t*)page_to_show)
					+ align16(sizeof(struct s_page))));
	else
		ft_printf("LARGE : %p, with a blck size of %d bytes\n",
		(void*)(((uint8_t*)page_to_show) + align16(sizeof(struct s_page))),
		page_to_show->blck_size_);
	while (NULL != browse_alloced_blcks)
	{
		show_one_address(browse_alloced_blcks);
		browse_alloced_blcks = browse_alloced_blcks->nt_blck_in_page_;
	}
}

void	print_alloc_memory(struct s_manipulation *manipulation_stc)
{
	struct s_page *page_category_to_browse;
	struct s_page *page_inside_category_to_browse;

	page_category_to_browse = NULL;
	page_inside_category_to_browse = NULL;
	page_category_to_browse = manipulation_stc->page_linked_list;
	while (NULL != page_category_to_browse)
	{
		page_inside_category_to_browse = page_category_to_browse;
		while (NULL != page_inside_category_to_browse)
		{
			show_one_list_category(page_inside_category_to_browse);
			page_inside_category_to_browse =
				page_inside_category_to_browse->nt_page_same_category_;
		}
		page_category_to_browse =
			page_category_to_browse->nt_page_upper_category_;
	}
}
