/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cal_page_size.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 15:47:28 by lazrossi          #+#    #+#             */
/*   Updated: 2019/12/11 18:17:51 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"
#include <sys/mman.h>
#include <string.h>

uint64_t	align16(size_t nbr)
{
	return ((uint64_t)(((nbr - 1) | (16 - 1)) + 1));
}

uint8_t		check_if_page_siz_too_big(uint64_t nt_page_size,
		uint64_t required_size)
{
	if (nt_page_size > (required_size << 1) && required_size > TINY)
	{
		return ((uint8_t)1);
	}
	return ((uint8_t)ZERO);
}

uint64_t	get_upper_page_size_for_requested_size(struct
		s_manipulation *manipulation_stc, uint64_t requested_size)
{
	uint64_t upper_page_size;

	upper_page_size = ZERO;
	upper_page_size = (float)(((requested_size
	/ manipulation_stc->pagesize) + 1)
	- (float)((float)(requested_size)
	/ (float)manipulation_stc->pagesize))
	* (float)manipulation_stc->pagesize;
	return (upper_page_size + requested_size);
}

uint64_t	default_page_size_for_page_blck(struct
		s_manipulation *manipulation_stc,
		uint64_t blck_size, uint64_t nbr_of_elements)
{
	uint64_t	requested_size;

	requested_size = ZERO;
	requested_size = align16(sizeof(struct s_page))
	+ (nbr_of_elements != ZERO ? nbr_of_elements : 1)
	* (align16(sizeof(struct s_blck)) + align16(blck_size));
	if (blck_size
			* (nbr_of_elements != ZERO ? nbr_of_elements : 1) > requested_size)
		return (get_max_uint64t());
	return (get_upper_page_size_for_requested_size(manipulation_stc,
				requested_size));
}

uint64_t	get_nbr_of_structure_in_asked_page_size(uint64_t asked_page_size,
		uint64_t asked_malloc_size)
{
	return ((asked_page_size - align16(sizeof(struct s_page)))
	/ (align16(sizeof(struct s_blck)) + align16(asked_malloc_size)));
}
