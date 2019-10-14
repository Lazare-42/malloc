/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mem_block.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 11:37:20 by lazrossi          #+#    #+#             */
/*   Updated: 2019/10/14 11:40:01 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void		fptr_stc_init_memory_block(struct s_block
		**ptr_pssd_memory_block_to_init, struct s_page *ptr_pssd_page_base,
		uint64_t u64_pssd_size_of_memory_to_initialize,
		struct s_block *ptr_pssd_next_block)
{
	(*ptr_pssd_memory_block_to_init)->u64_size_ =
		u64_pssd_size_of_memory_to_initialize;
	(*ptr_pssd_memory_block_to_init)->u64_free_size_ =
		u64_pssd_size_of_memory_to_initialize;
	(*ptr_pssd_memory_block_to_init)->ptr_next_block_in_page_ =
		ptr_pssd_next_block;
	(*ptr_pssd_memory_block_to_init)->ptr_page_base_ =
		ptr_pssd_page_base;
}

struct s_page	*fptr_stc_init_blocks_in_page(struct s_page *new_page,
		struct s_block *pssd_new_block, uint64_t block_size)
{
	struct s_block	*ptr_stc_lcl_previous_new_block;
	uint64_t		u64_browse;

	ptr_stc_lcl_previous_new_block = NULL;
	u64_browse = ZERO;
	while (u64_browse < new_page->u64_number_of_used_blocks_in_page_)
	{
		u64_browse = u64_browse + 1;
		if (u64_browse == new_page->u64_number_of_used_blocks_in_page_)
			block_size = new_page->u64_size_
				- (((void*)pssd_new_block - (void*)new_page))
			- fu64__align16(sizeof(struct s_block));
		fptr_stc_init_memory_block(&pssd_new_block, new_page,
		block_size, ptr_stc_lcl_previous_new_block);
		ptr_stc_lcl_previous_new_block = pssd_new_block;
		if (u64_browse != new_page->u64_number_of_used_blocks_in_page_)
			pssd_new_block = (struct s_block*)&((uint8_t*)pssd_new_block)[
			fu64__align16(sizeof(struct s_block)) + fu64__align16(
					new_page->u64_block_size_)];
	}
	new_page->ptr_first_free_block_ = pssd_new_block;
	return (new_page);
}
