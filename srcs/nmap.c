/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 11:36:25 by lazrossi          #+#    #+#             */
/*   Updated: 2019/10/14 11:39:43 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>

static void				*fptr_void__nmap(
		uint64_t u64_pssd_page_number_to_allocate)
{
	void *ptr_lcl_new_memory;

	ptr_lcl_new_memory = NULL;
	ptr_lcl_new_memory = mmap(NULL, u64_pssd_page_number_to_allocate,
	PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	return (ptr_lcl_new_memory);
}

struct s_page			*fv__init_new_page(struct s_page *ptr_pssd_stc_new_page,
		uint64_t u64_pssd_required_size,
		uint64_t u64_pssd_minimum_initialization_size,
		uint64_t u64_pssd_number_of_structures_for_asked_size)
{
	ptr_pssd_stc_new_page->u64_size_ = u64_pssd_minimum_initialization_size;
	ptr_pssd_stc_new_page->u64_block_size_ = u64_pssd_required_size;
	ptr_pssd_stc_new_page->u64_number_of_used_blocks_in_page_ =
		u64_pssd_number_of_structures_for_asked_size;
	ptr_pssd_stc_new_page->u64_total_number_of_pages_in_category_ = ZERO;
	ptr_pssd_stc_new_page->u64_number_of_used_pages_in_category_ = ZERO;
	ptr_pssd_stc_new_page->ptr_first_free_block_ = NULL;
	ptr_pssd_stc_new_page->ptr_first_occuppied_block_ = NULL;
	ptr_pssd_stc_new_page->ptr_next_page_in_category_ = NULL;
	ptr_pssd_stc_new_page->ptr_next_page_upper_category_ = NULL;
	return (ptr_pssd_stc_new_page);
}

struct s_page			*fu8__create_and_init_new_page_category(
		struct s_manipulation *ptr_pssd_stc_manipulation_structure,
		uint64_t u64_pssd_required_size,
		uint64_t u64_pssd_number_of_elements)
{
	uint64_t		min_init_size;
	uint64_t		u64_lcl_number_of_structures_for_asked_size;
	struct s_page	*ptr_lcl_stc_new_page;
	struct s_block	*ptr_stc_lcl_new_block;

	min_init_size = ZERO;
	u64_lcl_number_of_structures_for_asked_size = ZERO;
	ptr_stc_lcl_new_block = NULL;
	min_init_size =
	fu64__get_default_page_size_for_page_block(
	ptr_pssd_stc_manipulation_structure, u64_pssd_required_size,
	u64_pssd_number_of_elements);
	u64_lcl_number_of_structures_for_asked_size =
	fu64__get_number_of_structure_in_asked_page(
	min_init_size, u64_pssd_required_size);
	ptr_lcl_stc_new_page = fptr_void__nmap(min_init_size);
	if (NULL == (ptr_lcl_stc_new_page = fptr_void__nmap(min_init_size)))
		return (NULL);
	ptr_lcl_stc_new_page = fv__init_new_page(ptr_lcl_stc_new_page,
	u64_pssd_required_size, min_init_size,
	u64_lcl_number_of_structures_for_asked_size);
	ptr_stc_lcl_new_block = (struct s_block*)&((uint8_t*)ptr_lcl_stc_new_page)
		[fu64__align16(sizeof(struct s_page))];
	return (fptr_stc_init_blocks_in_page(ptr_lcl_stc_new_page,
				ptr_stc_lcl_new_block, ptr_lcl_stc_new_page->u64_block_size_));
}

struct s_manipulation	*fptr_stc_manipulation__init_manipulation(
		struct s_manipulation *ptr_pssd_stc_manipulation_structure)
{
	ptr_pssd_stc_manipulation_structure->u64_pagesize = getpagesize();
	ptr_pssd_stc_manipulation_structure->ptr_stc_page_linked_list = NULL;
	if (NULL != (ptr_pssd_stc_manipulation_structure->ptr_stc_page_linked_list =
	fu8__create_and_init_new_page_category(ptr_pssd_stc_manipulation_structure,
		TINY, MINIMUM_NUMBER_OF_INITIAL_ALLOCATIONS)))
	{
		if (NULL != (ptr_pssd_stc_manipulation_structure->
			ptr_stc_page_linked_list->ptr_next_page_upper_category_ =
			fu8__create_and_init_new_page_category(
			ptr_pssd_stc_manipulation_structure, SMALL,
			MINIMUM_NUMBER_OF_INITIAL_ALLOCATIONS)))
		{
			return (ptr_pssd_stc_manipulation_structure);
		}
	}
	return (NULL);
}

struct s_manipulation	*fptr_stc_manipulation__create_manipulation(void)
{
	struct s_manipulation	*manipulation_structure;
	uint64_t				required_number_of_pages_for_manipulation;

	manipulation_structure = NULL;
	required_number_of_pages_for_manipulation = 1;
	while (fu64__align16(sizeof(struct s_manipulation))
		> required_number_of_pages_for_manipulation
		* getpagesize())
	{
		required_number_of_pages_for_manipulation++;
	}
	if (NULL != (manipulation_structure =
	fptr_void__nmap(required_number_of_pages_for_manipulation
	* getpagesize())))
	{
		return (
			fptr_stc_manipulation__init_manipulation(manipulation_structure));
	}
	return (NULL);
}
