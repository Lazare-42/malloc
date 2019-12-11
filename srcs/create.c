/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 17:07:29 by lazrossi          #+#    #+#             */
/*   Updated: 2019/12/11 18:18:35 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>
#include "libft.h"

uint64_t
	get_max_uint64t(void)
{
	return (~(uint64_t)ZERO);
}

struct s_manipulation
	*init_manipulation(struct s_manipulation *stc_manipulation_structure)
{
	stc_manipulation_structure->pagesize = getpagesize();
	stc_manipulation_structure->page_linked_list = NULL;
	stc_manipulation_structure->binary_tree_of_malloced_nodes_ = NULL;
	if (NULL != (stc_manipulation_structure->page_linked_list =
				create_and_init_new_page_category(
	stc_manipulation_structure, TINY, MINIMUM_NBR_OF_INITIAL_ALLOCATIONS)))
	{
		if (NULL != (stc_manipulation_structure->page_linked_list
		->nt_page_upper_category_ =
		create_and_init_new_page_category(stc_manipulation_structure,
		SMALL, MINIMUM_NBR_OF_INITIAL_ALLOCATIONS)))
			return (stc_manipulation_structure);
	}
	return (NULL);
}

struct s_manipulation
	*create_manipulation_structure(void)
{
	struct s_manipulation		*manipulation_structure;
	uint64_t					required_nbr_of_pages_for_manipulation;

	manipulation_structure = NULL;
	required_nbr_of_pages_for_manipulation = 1;
	while (align16(sizeof(struct s_manipulation))
	> required_nbr_of_pages_for_manipulation * getpagesize())
		required_nbr_of_pages_for_manipulation++;
	if (NULL != (manipulation_structure =
	nmap(required_nbr_of_pages_for_manipulation * getpagesize())))
		return (init_manipulation(manipulation_structure));
	return (NULL);
}
