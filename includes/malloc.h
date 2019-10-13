/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 12:06:30 by lazrossi          #+#    #+#             */
/*   Updated: 2019/10/13 12:20:32 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H

# define MALLOC_H
# include <inttypes.h>
# include <unistd.h>

# define FAILURE (1)
# define SUCCESS (0)

# define ZERO (0)

# define FALSE	(0)
# define TRUE	(1)

# define TINY (128)
# define SMALL (256)
# define MINIMUM_NUMBER_OF_INITIAL_ALLOCATIONS (100)

struct								s_node_pointer_container {
	void							*ptr_address_;
	struct s_node_pointer_container *left_;
	struct s_node_pointer_container *right_;
};

struct								s_block {
	uint64_t						u64_size_;
	uint64_t						u64_free_size_;
	struct s_block					*ptr_next_block_in_page_;
	struct s_page					*ptr_page_base_;
};

struct								s_page {
	uint64_t						u64_size_;
	uint64_t						u64_block_size_;
	uint64_t						u64_number_of_used_blocks_in_page_;
	uint64_t						u64_number_of_used_pages_in_category_;
	uint64_t						u64_total_number_of_pages_in_category_;
	struct s_block					*ptr_first_free_block_;
	struct s_block					*ptr_first_occuppied_block_;
	struct s_page					*ptr_base_page_category_;
	struct s_page					*ptr_next_page_in_category_;
	struct s_page					*ptr_next_page_upper_category_;
};

struct								s_manipulation
{
	uint64_t						u64_pagesize;
	struct s_page					*ptr_stc_page_linked_list;
	struct s_node_pointer_container *ptr_stc_binary_tree_of_malloced_nodes_;
};

void								*malloc(size_t size);
void								free(void *ptr);
void								*realloc(void *ptr, size_t size);
void								*fptr_void__allocate_fresh_memory_pages
(size_t szt_pssd_page_number_to_allocate);
struct s_manipulation				*fptr_stc_manipulation__create_manipulation
(void);
uint64_t							fu64__align16(size_t number);
uint64_t							fu64_get_upper_page_size_for_requested_size
(struct s_manipulation *ptr_pssd_stc_manipulation_structure,
uint64_t u64_pssd_requested_size);
uint64_t							fu64__get_default_page_size_for_page_block
(struct s_manipulation *ptr_pssd_stc_manipulation_structure,
uint64_t u64_pssd_block_size, uint64_t u64_pssd_number_of_elements);
uint64_t							fu64__get_number_of_structure_in_asked_page
(uint64_t u64_pssd_asked_page_size, uint64_t u64_pssd_size_of_structure);
struct s_page						*fu8__create_and_init_new_page_category
(struct s_manipulation *ptr_pssd_stc_manipulation_structure,
uint64_t u64_pssd_required_size, uint64_t u64_pssd_number_of_elements);
void								*fptr_void__return_memory
(struct s_manipulation *ptr_pssd_stc_manipulation_structure, uint64_t
	u64_pssd_required_size);
uint8_t								fu8_bool__check_if_chosen_page_size_too_big
(uint64_t u64_next_page_size, uint64_t u64_pssd_required_size);
uint64_t							fu64__return_upper_page_size(uint64_t
		u64_previous_page_size, uint64_t u64_pssd_required_size);
void								fvoid__free_half_of_used_pages_from_one_page
						(struct s_page *ptr_pssd_first_page_for_category);
uint8_t								fu8__bool_check_if_malloced
(struct s_manipulation *ptr_pssd_manipulation_structure,
void *ptr_pssd_pointer_free);
void								print_alloc_memory(struct s_manipulation
		*ptr_pssd_manipulation_structure);
void								show_alloc_mem();

#endif
