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

struct s_manipulation
{
    struct s_block  **ptr_stc_block_tiny_blocks;
    uint64_t        u64_median_size_of_tiny_blocks;
    uint64_t        u64_size_of_tiny_memory_pointer; 
    uint64_t        u64_first_free_location_for_tiny_pointer; 

    struct s_block  **ptr_stc_block_small_blocks;
    uint64_t        u64_median_size_of_small_blocks;
    uint64_t        u64_size_of_small_memory_pointer; 
    uint64_t        u64_first_free_location_for_small_pointer; 

    struct s_block  **ptr_stc_block_large_blocks;
    uint64_t        u64_median_size_of_large_blocks;
    uint64_t        u64_size_of_large_memory_pointer; 
    uint64_t        u64_first_free_location_for_large_pointer; 
};

/**
 * Function prototypes
 */
void    *my_malloc(size_t size);
void    *Fptr_void__allocate_fresh_memory_pages(size_t szt_pssd_page_number_to_allocate);
struct s_manipulation *Fptr_stc_manipulation__create_manipulation_structure(void);
size_t Fszt__align16(size_t number);

#endif
