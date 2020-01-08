/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   three_main_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 17:11:31 by lazrossi          #+#    #+#             */
/*   Updated: 2020/01/08 09:53:58 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include "libft.h"

static struct s_manipulation *static_manipulation = NULL;

void	free(void *ptr)
{
	struct s_blck *blck_to_free;
	struct s_page *page_blck;

	blck_to_free = NULL;
	page_blck = NULL;
	if (static_manipulation == NULL
			|| ZERO == check_prev_realloc(static_manipulation, ptr))
		return ;
	blck_to_free = (struct s_blck*)(((uint8_t*)(ptr)) - sizeof(struct s_blck));
	blck_to_free->free_size_ = blck_to_free->size_;
	page_blck = blck_to_free->page_base_;
	push_freed_block_on_page_stack(blck_to_free, page_blck);
	if (ZERO == page_blck->nbr_of_used_blcks_in_page_)
		page_blck->base_page_category_->nbr_of_used_pages_in_category_ -= 1;
	if (static_manipulation->pagesize < blck_to_free->size_)
	{
		munmap(blck_to_free, blck_to_free->size_);
	}
	else if ((page_blck->base_page_category_->nbr_pages_in_category / 3 * 2)
		> page_blck->base_page_category_->nbr_of_used_pages_in_category_)
		free_half_page_category(page_blck->base_page_category_);
}

void	*malloc(size_t size)
{
	size = align16(size);
	if (static_manipulation == NULL)
	{
		static_manipulation = create_manipulation_structure();
		if (static_manipulation == NULL)
		{
			return (NULL);
		}
	}
	return (return_memory(static_manipulation, size));
}

void	*realloc_uppersize(struct s_blck *blck_to_realloc, size_t size)
{
	void			*void_realloced_memory;

	void_realloced_memory = NULL;
	if (NULL == (void_realloced_memory = malloc(size)))
		return (NULL);
	memcpy(void_realloced_memory, (void*)(((uint8_t*)
	blck_to_realloc) +
	align16(sizeof(struct s_blck))), blck_to_realloc->size_);
	free((void*)(((uint8_t*)blck_to_realloc)
	+ align16(sizeof(struct s_blck))));
	return (void_realloced_memory);
}

void	*realloc(void *ptr, size_t size)
{
	struct s_blck *blck_to_realloc;

	size = align16(size);
	blck_to_realloc = NULL;
	if (size == 0)
	{
		free(ptr);
		return (malloc(0));
	}
	if (static_manipulation == NULL
			|| ZERO == check_prev_realloc(static_manipulation, ptr))
		return (malloc(size));
	blck_to_realloc = (struct s_blck*)(((uint8_t*)(ptr))
			- sizeof(struct s_blck));
	if (blck_to_realloc->size_ < size)
		return (realloc_uppersize(blck_to_realloc, size));
	blck_to_realloc->free_size_ = blck_to_realloc->size_ - size;
	return ((void*)(((uint8_t*)blck_to_realloc)
				+ align16(sizeof(struct s_blck))));
}

void	show_alloc_mem(void)
{
	print_alloc_memory(static_manipulation);
}
