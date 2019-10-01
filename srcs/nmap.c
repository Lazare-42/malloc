#include "malloc.h"
#include <stdio.h>
#include <sys/mman.h>

/**
 *  Allocates new memory by 
 *  Calls nmap with the following arguments : 
 *      1. NULL to point to a "fake" address
 *      2. getpagesize() * szt_pssd_page_number_to_allocate to use a multiple of a system page
 *      3. PROT_READ | PROT_WRITE to read & write allocated memory
 *      4. MAP_SHARED | MAP_ANONYMOUS to share modifications to all processes & sub-processes and
 *   to get a fresh memory area that shares no pages with other mappings (obtained memory is initialized to ZERO)
 */
void    *Fptr_void__nmap(size_t szt_pssd_page_number_to_allocate)
{
    void *ptr_lcl_new_memory;

    ptr_lcl_new_memory = NULL;
    ptr_lcl_new_memory = mmap(NULL, getpagesize()
    * szt_pssd_page_number_to_allocate, PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    /**
     *  No need to check for NULL ; functions calling Fptr_void__nmap
     *  will do so
     */
    return (ptr_lcl_new_memory);
}

struct s_block        *Fptr_stc_init_memory_block(struct s_block *ptr_pssd_memory_block_to_init, size_t szt_pssd_size_of_memory_to_initialize)
{
    ptr_pssd_memory_block_to_init->u64_total_size                   = szt_pssd_size_of_memory_to_initialize;
    ptr_pssd_memory_block_to_init->u64_free_size                    = ZERO;
    ptr_pssd_memory_block_to_init->u64_position_in_double_pointer   = ZERO;
    return (ptr_pssd_memory_block_to_init);
}

struct s_manipulation *Fptr_stc_manipulation__init_manipulation(struct s_manipulation *ptr_pssd_manipulation_structure, size_t szt_pssd_initialization_size)
{
    uint64_t                u64_lcl_browse_to_initialize_structs;

    u64_lcl_browse_to_initialize_structs            = ZERO;
    (void)u64_lcl_browse_to_initialize_structs;
    (void)ptr_pssd_manipulation_structure;
    (void)szt_pssd_initialization_size;
    return (ptr_pssd_manipulation_structure);
}

struct s_manipulation *Fptr_stc_manipulation__create_manipulation_structure(void)
{
    struct s_manipulation   *ptr_stc_lcl_manipulation_structure;
    size_t                  szt_lcl_minimum_initialization_size;
    size_t                  szt_lcl_left_over_space_for_tiny_block;

    ptr_stc_lcl_manipulation_structure              = NULL;
    szt_lcl_minimum_initialization_size             = ZERO;
    szt_lcl_left_over_space_for_tiny_block          = ZERO;
    /**
     *  Calculate page number sufficient to contain the manipulation structure
     *  and the zone for 
     *  (MINIMUM_NUMBER_OF_TINY_SMALL_ALLOCATIONS * TINY) 
     *  (MINIMUM_NUMBER_OF_TINY_SMALL_ALLOCATIONS * SMALL) 
     *  along with their back-pointer localisations in the main structure
     */
    szt_lcl_minimum_initialization_size = sizeof(struct s_manipulation)
    + MINIMUM_NUMBER_OF_TINY_SMALL_ALLOCATIONS 
    * (Fszt__align16(sizeof(struct s_block)) + TINY) 
    + MINIMUM_NUMBER_OF_TINY_SMALL_ALLOCATIONS 
    * (Fszt__align16(sizeof(struct s_block)) + SMALL)
    + sizeof(struct s_block*) * (MINIMUM_NUMBER_OF_TINY_SMALL_ALLOCATIONS * 2);

    /**
     *  Calculate the extra space left we will use for tiny allocations.
     */
    szt_lcl_left_over_space_for_tiny_block = ((float)(szt_lcl_minimum_initialization_size / getpagesize() + 1) - (float)((float)szt_lcl_minimum_initialization_size / (float)getpagesize())) * getpagesize() / (Fszt__align16(sizeof(struct s_block)) + TINY);
    //fprintf(stderr, "need %ld size which is %f pages\n", szt_lcl_minimum_initialization_size, );
    fprintf(stderr, "there is extra space for %ld tiny blocks\n", szt_lcl_left_over_space_for_tiny_block );
    //fprintf(stderr, "Leaves space for %d\n", 
    if (NULL == (ptr_stc_lcl_manipulation_structure = Fptr_void__nmap(1)))
    {
        return NULL;
    }
    return (ptr_stc_lcl_manipulation_structure);
}
