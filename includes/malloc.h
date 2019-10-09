#ifndef MALLOC_H

# define MALLOC_H
#include <inttypes.h>
#include <unistd.h>

# define FAILURE (1)
# define SUCCESS (0)

# define ZERO (0)

# define FALSE  (0)
# define TRUE   (1)

# define TINY (128)
# define SMALL (256)
# define MINIMUM_NUMBER_OF_TINY_SMALL_ALLOCATIONS (100)

# define FIRST_BIT      (0x01)
# define SECOND_BIT     (0x02)
# define THIRD_BIT      (0x04)
# define FOURTH_BIT     (0x08)
# define FIFTH_BIT      (0x10)
# define SIXTH_BIT      (0x20)
# define SEVENTH_BIT    (0x40)
# define EIGHTH_BIT     (0x80) 

/**
 *  Used structures
 */
struct s_node_pointer_container {
    void                            *ptr_address_;
    struct s_node_pointer_container *left_;
    struct s_node_pointer_container *right_;
};

struct s_block {
    uint64_t                        u64_size_;
    uint64_t                        u64_free_size_;
    struct s_block                  *ptr_next_block_in_page_;
    struct s_page                   *ptr_page_base_;
};

struct s_page {
    uint64_t            u64_size_;
    uint64_t            u64_block_size_; 
    uint64_t            u64_number_of_used_blocks_in_page_;
    uint64_t            u64_number_of_used_pages_in_category_;
    uint64_t            u64_total_number_of_pages_in_category_;
    struct s_block      *ptr_first_free_block_;
    struct s_block      *ptr_first_occuppied_block_;
    struct s_page       *ptr_base_page_category_;
    struct s_page       *ptr_next_page_same_category_;
    struct s_page       *ptr_next_page_upper_category_;
};

struct s_manipulation
{
    uint64_t                        u64_pagesize;
    struct s_page                   *ptr_stc_page_linked_list;
    struct s_node_pointer_container *ptr_stc_binary_tree_of_malloced_nodes_;
};

/**
 * Function prototypes
 */
void                    *malloc(size_t size);
void                    free(void *ptr);
void                    *realloc(void *ptr, size_t size);

void                    *Fptr_void__allocate_fresh_memory_pages(size_t szt_pssd_page_number_to_allocate);
struct s_manipulation   *Fptr_stc_manipulation__create_manipulation_structure(void);
uint64_t                Fu64__align16(size_t number);
uint64_t                Fu64_get_upper_page_size_for_requested_size(struct s_manipulation *ptr_pssd_stc_manipulation_structure, uint64_t u64_pssd_requested_size);
uint64_t                Fu64__get_default_page_size_for_page_block_container(struct s_manipulation *ptr_pssd_stc_manipulation_structure, uint64_t u64_pssd_block_size, uint64_t u64_pssd_number_of_elements);
uint64_t                Fu64__get_number_of_structure_in_asked_page_size(uint64_t u64_pssd_asked_page_size, uint64_t u64_pssd_size_of_structure);
struct s_page           *Fu8__create_and_init_new_page_category(struct s_manipulation *ptr_pssd_stc_manipulation_structure, uint64_t u64_pssd_required_size, uint64_t u64_pssd_number_of_elements);
void                    *Fptr_void__return_memory(struct s_manipulation *ptr_pssd_stc_manipulation_structure, uint64_t u64_pssd_required_size);
uint8_t                 Fu8_bool__check_if_chosen_page_size_too_big_for_required_size(uint64_t u64_next_page_size, uint64_t u64_pssd_required_size);
uint64_t                Fu64__return_upper_page_size(uint64_t u64_previous_page_size, uint64_t u64_pssd_required_size);

void                    Fvoid__free_half_of_used_pages_from_one_page_category(struct s_page *ptr_pssd_first_page_for_category);

#endif
