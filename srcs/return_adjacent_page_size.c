/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_adjacent_page_size.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 15:48:12 by lazrossi          #+#    #+#             */
/*   Updated: 2019/12/11 15:48:40 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "malloc.h"

uint64_t	return_lower_page_size(uint64_t nt_page_size,
		uint64_t required_size)
{
	uint64_t new_page_size;

	new_page_size = ZERO;
	new_page_size = nt_page_size;
	while (new_page_size > (required_size << 1))
	{
		new_page_size = new_page_size >> 1;
	}
	return (new_page_size);
}

uint64_t	return_upper_page_size(uint64_t previous_page_size,
		uint64_t required_size)
{
	uint64_t counter;
	uint64_t new_page_size;

	new_page_size = 1;
	counter = 1;
	while (new_page_size < previous_page_size && counter < 64)
	{
		new_page_size = (new_page_size << 1);
		counter = counter + 1;
	}
	while (new_page_size < required_size && counter < 64)
	{
		new_page_size = (new_page_size << 1);
		counter = counter + 1;
	}
	if (new_page_size < required_size)
		new_page_size = required_size;
	return (new_page_size);
}
