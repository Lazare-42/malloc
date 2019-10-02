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
# define SMALL (1024)
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
struct s_block 
{
    uint64_t            u64_total_size;
    uint64_t            u64_free_size;
    uint64_t            u64_position_in_double_pointer;
};

struct s_block_container
{
    uint64_t        u64_first_free_position_in_double_pointer;
    uint64_t        u64_size_of_double_pointer;
    struct s_block  **ptr_stc_block_container;
};

struct s_page_node
{
    uint64_t            u64_total_size;
    uint64_t            u64_free_size;
    struct s_page_node  *ptr_stc_page_node_next;
    struct s_page_node  *ptr_stc_page_node_previous;
};

struct s_manipulation
{
    uint64_t                    u64_pagesize;
    struct s_block_container    *ptr_stc_tiny_block_container;
    struct s_block_container    *ptr_stc_small_block_container;
    struct s_block_container    *ptr_stc_large_block_container;

    uint64_t                    u64_size_of_blocks_in_block_container;
    struct s_block_container    *ptr_stc_container_to_alloc_realloc;

    struct s_page_node          *ptr_stc_page_linked_list;
};

//    struct s_block  **ptr_stc_block_tiny_blocks;
//    uint64_t        u64_median_size_of_tiny_blocks;
//    uint64_t        u64_size_of_tiny_memory_pointer; 
//    uint64_t        u64_first_free_location_for_tiny_pointer; 
//
//    struct s_block  **ptr_stc_block_small_blocks;
//    uint64_t        u64_median_size_of_small_blocks;
//    uint64_t        u64_size_of_small_memory_pointer; 
//    uint64_t        u64_first_free_location_for_small_pointer; 
//
//    struct s_block  **ptr_stc_block_large_blocks;
//    uint64_t        u64_median_size_of_large_blocks;
//    uint64_t        u64_size_of_large_memory_pointer; 
//    uint64_t        u64_first_free_location_for_large_pointer; 
//};

/**
 * Function prototypes
 */
void    *my_malloc(size_t size);
void    *Fptr_void__allocate_fresh_memory_pages(size_t szt_pssd_page_number_to_allocate);
struct s_manipulation *Fptr_stc_manipulation__create_manipulation_structure(void);
size_t Fu64__align16(size_t number);
uint64_t Fu64_get_upper_page_size_for_requested_size(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_requested_size);
uint64_t Fu64__get_default_page_size_for_page_block_container(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_block_size);
uint64_t Fu64__get_default_page_size_for_page_double_pointer_block_container(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_number_of_structures);
uint64_t Fu64__get_number_of_structure_in_default_page_size(uint64_t u64_pssd_default_page_size, uint64_t u64_pssd_size_of_structure);

#endif
