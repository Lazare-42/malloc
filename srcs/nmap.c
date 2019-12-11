/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 16:50:45 by lazrossi          #+#    #+#             */
/*   Updated: 2019/12/11 18:18:10 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>
#include "libft.h"

void						*nmap(uint64_t page_nbr_to_allocate)
{
	void *new_memory;

	new_memory = NULL;
	if (MAP_FAILED == (new_memory = mmap(NULL, page_nbr_to_allocate,
	PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)))
	{
		ft_dprintf(2, "Mmap returned MAP_FAILED\n");
		return (NULL);
	}
	return (new_memory);
}

void						init_memory_blck(struct s_blck **mem_blck_to_init,
		struct s_page *page_base,
		uint64_t size_of_memory_to_initialize,
		struct s_blck *nt_blck)
{
	(*mem_blck_to_init)->size_ = size_of_memory_to_initialize;
	(*mem_blck_to_init)->free_size_ = size_of_memory_to_initialize;
	(*mem_blck_to_init)->nt_blck_in_page_ = nt_blck;
	(*mem_blck_to_init)->page_base_ = page_base;
}

struct s_page				*init_blcks_in_page(struct s_page *new_page)
{
	struct s_blck	*new_blck;
	struct s_blck	*prev_new_blck;
	uint64_t		browse_to_init;
	uint64_t		stc_assigned_siz;

	new_blck = NULL;
	prev_new_blck = NULL;
	browse_to_init = ZERO;
	new_blck = (struct s_blck*)&((uint8_t*)
			new_page)[align16(sizeof(struct s_page))];
	stc_assigned_siz = new_page->blck_size_;
	while (browse_to_init < new_page->nbr_of_used_blcks_in_page_)
	{
		browse_to_init = browse_to_init + 1;
		if (browse_to_init == new_page->nbr_of_used_blcks_in_page_)
			stc_assigned_siz = new_page->size_
	- (((void*)new_blck - (void*)new_page)) - align16(sizeof(struct s_blck));
		init_memory_blck(&new_blck, new_page, stc_assigned_siz, prev_new_blck);
		prev_new_blck = new_blck;
		if (browse_to_init != new_page->nbr_of_used_blcks_in_page_)
			new_blck = (struct s_blck*)&((uint8_t*)new_blck)
			[align16(sizeof(struct s_blck)) + align16(new_page->blck_size_)];
	}
	new_page->first_free_blck_ = new_blck;
	return (new_page);
}

struct s_page				*create_and_init_new_page_category(struct
		s_manipulation *stc_manipulation_structure,
		uint64_t required_size, uint64_t nbr_of_elements)
{
	uint64_t		min_siz;
	uint64_t		nbr_of_structures_for_asked_size;
	struct s_page	*new_page;

	if (required_size == get_max_uint64t())
		return (NULL);
	min_siz = ZERO;
	nbr_of_structures_for_asked_size = ZERO;
	min_siz = default_page_size_for_page_blck(
	stc_manipulation_structure, required_size, nbr_of_elements);
	nbr_of_structures_for_asked_size =
		get_nbr_of_structure_in_asked_page_size(min_siz, required_size);
	if (NULL == (new_page = nmap(min_siz)))
		return (NULL);
	new_page->size_ = min_siz;
	new_page->blck_size_ = required_size;
	new_page->nbr_of_used_blcks_in_page_ = nbr_of_structures_for_asked_size;
	new_page->nbr_pages_in_category = ZERO;
	new_page->nbr_of_used_pages_in_category_ = ZERO;
	new_page->first_free_blck_ = NULL;
	new_page->first_occuppied_blck_ = NULL;
	new_page->nt_page_same_category_ = NULL;
	new_page->nt_page_upper_category_ = NULL;
	return (init_blcks_in_page(new_page));
}
