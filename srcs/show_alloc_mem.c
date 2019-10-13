/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 12:06:54 by lazrossi          #+#    #+#             */
/*   Updated: 2019/10/13 12:20:24 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

static void		show_one_address(struct s_block *ptr_pssd_stc_block_to_show)
{
	ft_printf("%p - %p : %d bytes\n", (void*)(((uint8_t*)
	ptr_pssd_stc_block_to_show) + fu64__align16(sizeof(struct s_block))),
	(void*)(((uint8_t*)ptr_pssd_stc_block_to_show) + fu64__align16(
	sizeof(struct s_block))) + ptr_pssd_stc_block_to_show->u64_size_
	- ptr_pssd_stc_block_to_show->u64_free_size_,
	ptr_pssd_stc_block_to_show->u64_size_
	- ptr_pssd_stc_block_to_show->u64_free_size_);
}

static void		show_one_list_category(struct s_page *ptr_pssd_page_to_show)
{
	struct s_block	*ptr_stc_lcl_browse_alloced_blocks;

	ptr_stc_lcl_browse_alloced_blocks = NULL;
	ptr_stc_lcl_browse_alloced_blocks =
		ptr_pssd_page_to_show->ptr_first_occuppied_block_;
	if (ptr_pssd_page_to_show->u64_block_size_ == TINY)
		ft_printf("TINY : %p\n",
		(void*)(((uint8_t*)ptr_pssd_page_to_show)
			+ fu64__align16(sizeof(struct s_page))));
	else if (ptr_pssd_page_to_show->u64_block_size_ == SMALL)
		ft_printf("SMALL : %p\n",
		(void*)(((uint8_t*)ptr_pssd_page_to_show)
			+ fu64__align16(sizeof(struct s_page))));
	else
		ft_printf("LARGE : %p, with a block size of %d bytes\n",
		(void*)(((uint8_t*)ptr_pssd_page_to_show)
		+ fu64__align16(sizeof(struct s_page))),
		ptr_pssd_page_to_show->u64_block_size_);
	while (NULL != ptr_stc_lcl_browse_alloced_blocks)
	{
		show_one_address(ptr_stc_lcl_browse_alloced_blocks);
		ptr_stc_lcl_browse_alloced_blocks =
			ptr_stc_lcl_browse_alloced_blocks->ptr_next_block_in_page_;
	}
}

void			print_alloc_memory(struct s_manipulation
		*ptr_pssd_manipulation_structure)
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
			show_one_list_category(ptr_stc_lcl_page_inside_category_to_browse);
			ptr_stc_lcl_page_inside_category_to_browse =
				ptr_stc_lcl_page_inside_category_to_browse->
				ptr_next_page_in_category_;
		}
		ptr_stc_lcl_page_category_to_browse =
			ptr_stc_lcl_page_category_to_browse->ptr_next_page_upper_category_;
	}
}
