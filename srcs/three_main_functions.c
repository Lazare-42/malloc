/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   three_main_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 11:51:53 by lazrossi          #+#    #+#             */
/*   Updated: 2019/10/14 11:51:55 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>
#include "libft.h"

static struct s_manipulation	*g_static_manipulation_structure = NULL;

void	free(void *ptr)
{
	struct s_block	*tofreeptr;
	struct s_page	*page_storing_block;

	tofreeptr = NULL;
	page_storing_block = NULL;
	if (g_static_manipulation_structure == NULL || ZERO ==
			fu8__bool_check_if_malloced(g_static_manipulation_structure, ptr))
		return ;
	tofreeptr = (struct s_block*)(((uint8_t*)(ptr))
			- sizeof(struct s_block));
	tofreeptr->u64_free_size_ = tofreeptr->u64_size_;
	page_storing_block = tofreeptr->ptr_page_base_;
	page_storing_block->u64_number_of_used_blocks_in_page_ =
	page_storing_block->u64_number_of_used_blocks_in_page_ - 1;
	if (ZERO == page_storing_block->u64_number_of_used_blocks_in_page_)
		page_storing_block->ptr_base_page_category_->
			u64_number_of_used_pages_in_category_ -= 1;
	if (g_static_manipulation_structure->u64_pagesize < tofreeptr->u64_size_)
		munmap(tofreeptr, tofreeptr->u64_size_);
	else if ((page_storing_block->ptr_base_page_category_->
		u64_total_number_of_pages_in_category_ / 3 * 2) >
			page_storing_block->ptr_base_page_category_->
			u64_number_of_used_pages_in_category_)
		fvoid__free_half_of_used_pages_from_one_page(
				page_storing_block->ptr_base_page_category_);
}

void	*malloc(size_t size)
{
	if (g_static_manipulation_structure == NULL)
	{
		g_static_manipulation_structure =
			fptr_stc_manipulation__create_manipulation();
		if (g_static_manipulation_structure == NULL)
		{
			return (NULL);
		}
	}
	return (fptr_void__return_memory(g_static_manipulation_structure, size));
}

void	*realloc(void *ptr, size_t size)
{
	struct s_block	*ptr_stc_lcl_block_pointer_to_realloc;
	void			*ptr_void_lcl_realloced_memory;

	ptr_stc_lcl_block_pointer_to_realloc = NULL;
	if (g_static_manipulation_structure == NULL || ZERO ==
			fu8__bool_check_if_malloced(g_static_manipulation_structure, ptr))
		return (malloc(size));
	ptr_stc_lcl_block_pointer_to_realloc = (struct s_block*)
		(((uint8_t*)(ptr)) - sizeof(struct s_block));
	if (ptr_stc_lcl_block_pointer_to_realloc->u64_size_ < size)
	{
		ptr_void_lcl_realloced_memory = malloc(size);
		ft_memcpy(ptr_void_lcl_realloced_memory,
		(void*)(((uint8_t*)ptr_stc_lcl_block_pointer_to_realloc)
			+ fu64__align16(sizeof(struct s_block))),
		ptr_stc_lcl_block_pointer_to_realloc->u64_size_);
		free((void*)(((uint8_t*)ptr_stc_lcl_block_pointer_to_realloc)
					+ fu64__align16(sizeof(struct s_block))));
		return (ptr_void_lcl_realloced_memory);
	}
	ptr_stc_lcl_block_pointer_to_realloc->u64_free_size_ =
		ptr_stc_lcl_block_pointer_to_realloc->u64_size_ - size;
	return ((void*)(((uint8_t*)ptr_stc_lcl_block_pointer_to_realloc)
				+ fu64__align16(sizeof(struct s_block))));
}

void	*calloc(size_t elemn, size_t size)
{
	ft_printf("[[red]]In caloc, function not created yet ![[end]]\n");
	(void)elemn;
	(void)size;
	return (NULL);
}

void	show_alloc_mem(void)
{
	print_alloc_memory(g_static_manipulation_structure);
}
