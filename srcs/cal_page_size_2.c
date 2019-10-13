/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cal_page_size_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 15:42:15 by lazrossi          #+#    #+#             */
/*   Updated: 2019/10/13 15:42:46 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

uint64_t	fu64__align16(size_t number)
{
	return ((uint64_t)(((number - 1) | (16 - 1)) + 1));
}

uint8_t		fu8_bool__check_if_chosen_page_size_too_big(
		uint64_t u64_next_page_size, uint64_t u64_pssd_required_size)
{
	if (u64_next_page_size > (u64_pssd_required_size << 1)
			&& u64_pssd_required_size > TINY)
	{
		return ((uint8_t)1);
	}
	return ((uint8_t)ZERO);
}
