/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 18:15:25 by lazrossi          #+#    #+#             */
/*   Updated: 2019/12/11 18:21:20 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H

# define MALLOC_H
# include <inttypes.h>
# include <unistd.h>

# define FAILURE 1
# define SUCCESS 0

# define RETURN_FAILURE FAILURE
# define RETURN_SUCCESS SUCCESS

# define ZERO 0

# define FALSE  0
# define TRUE   1

# define TINY 16
# define SMALL 32
# define MINIMUM_NBR_OF_INITIAL_ALLOCATIONS 100

# define FIRST_BIT	  	0x01
# define SECOND_BIT	 	0x02
# define THIRD_BIT	  	0x04
# define FOURTH_BIT	 	0x08
# define FIFTH_BIT	  	0x10
# define SIXTH_BIT	  	0x20
# define SEVENTH_BIT	0x40
# define EIGHTH_BIT	 	0x80

struct								s_node_pointer_container {
	void							*address_;
	struct s_node_pointer_container	*left_;
	struct s_node_pointer_container	*right_;
};

struct								s_blck {
	uint64_t						size_;
	uint64_t						free_size_;
	struct s_blck					*nt_blck_in_page_;
	struct s_page					*page_base_;
};

struct								s_page {
	uint64_t						size_;
	uint64_t						blck_size_;
	uint64_t						nbr_of_used_blcks_in_page_;
	uint64_t						nbr_of_used_pages_in_category_;
	uint64_t						nbr_pages_in_category;
	struct s_blck					*first_free_blck_;
	struct s_blck					*first_occuppied_blck_;
	struct s_page					*base_page_category_;
	struct s_page					*nt_page_same_category_;
	struct s_page					*nt_page_upper_category_;
};

struct								s_manipulation
{
	uint64_t						pagesize;
	struct s_page					*page_linked_list;
	struct s_node_pointer_container *binary_tree_of_malloced_nodes_;
};

void								*malloc(size_t size);
void								free(void *ptr);
void								*realloc(void *ptr, size_t size);
void								*allocate_fresh_memory_pages(size_t
		szt_pssd_page_nbr_to_allocate);
struct s_manipulation				*create_manipulation_structure(void);
uint64_t							align16(size_t nbr);
uint64_t							get_upper_page_size_for_requested_size(
		struct s_manipulation *pssd_stc_manipulation_structure,
		uint64_t pssd_requested_size);
uint64_t							default_page_size_for_page_blck(
		struct s_manipulation *pssd_stc_manipulation_structure,
		uint64_t pssd_blck_size, uint64_t pssd_nbr_of_elements);
uint64_t							get_nbr_of_structure_in_asked_page_size(
		uint64_t pssd_asked_page_size, uint64_t pssd_size_of_structure);
struct s_page						*create_and_init_new_page_category(
		struct s_manipulation *pssd_stc_manipulation_structure,
		uint64_t pssd_required_size, uint64_t pssd_nbr_of_elements);
void								*return_memory(
		struct s_manipulation *pssd_stc_manipulation_structure,
		uint64_t pssd_required_size);
uint8_t								check_if_page_siz_too_big(
		uint64_t nt_page_size, uint64_t pssd_required_size);
uint64_t							return_upper_page_size(
		uint64_t previous_page_size, uint64_t pssd_required_size);
void								free_half_page_category(
		struct s_page *pssd_first_page_for_category);
uint8_t								check_prev_realloc(
		struct s_manipulation *pssd_manipulation_structure,
		void *pssd_pointer_passed_to_free);
void								print_alloc_memory(
		struct s_manipulation *pssd_manipulation_structure);
void								show_alloc_mem();
void								*nmap(uint64_t page_nbr_to_allocate);
uint64_t							get_max_uint64t();

#endif
