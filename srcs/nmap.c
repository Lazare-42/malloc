#include "malloc.h"
#include <stdio.h>
#include <sys/mman.h>

/**
 *  Allocates new memory by 
 *  Calls nmap with the following arguments : 
 *      1. NULL to point to a "fake" address
 *      2. getpagesize() * szt_pssd_page_number_to_allocate to use a multiple of a system page
 *      3. PROT_READ | PROT_WRITE to read & write allocated memory
 *      4. MAP_SHARED | MAP_PRIVATE
 *   to get a fresh memory area that shares no pages with other mappings (obtained memory is initialized to ZERO)
 */
void    *Fptr_void__nmap(size_t szt_pssd_page_number_to_allocate)
{
    void *ptr_lcl_new_memory;

    ptr_lcl_new_memory = NULL;
    ptr_lcl_new_memory = mmap(NULL, szt_pssd_page_number_to_allocate, 
    PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
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

size_t Fst__find_upper_page_size(struct s_manipulation *ptr_pssd_manipulation_structure, size_t szt_pssd_requested_size)
{
    uint64_t    u64_lcl_upper_page_size;

    u64_lcl_upper_page_size = ZERO;
    u64_lcl_upper_page_size = (float)(((szt_pssd_requested_size / ptr_pssd_manipulation_structure->u64_pagesize) + 1) - (float)((float)(szt_pssd_requested_size) / (float)ptr_pssd_manipulation_structure->u64_pagesize)) * (float)ptr_pssd_manipulation_structure->u64_pagesize;
    
    return (u64_lcl_upper_page_size);
}

struct s_manipulation *Fptr_stc_manipulation__init_manipulation(struct s_manipulation *ptr_pssd_manipulation_structure)
{
    uint64_t                szt_lcl_minimum_initialization_size;

    szt_lcl_minimum_initialization_size                             = ZERO;
    ptr_pssd_manipulation_structure->u64_pagesize                   = getpagesize();
    ptr_pssd_manipulation_structure->ptr_stc_tiny_block_container   = NULL;
    ptr_pssd_manipulation_structure->ptr_stc_small_block_container  = NULL;
    ptr_pssd_manipulation_structure->ptr_stc_large_block_container  = NULL;
    ptr_pssd_manipulation_structure->ptr_stc_page_linked_list       = NULL;

    szt_lcl_minimum_initialization_size = Fszt__align16(sizeof(struct s_block_container))
    + MINIMUM_NUMBER_OF_TINY_SMALL_ALLOCATIONS 
    * (Fszt__align16(sizeof(struct s_block)) + TINY) 
    + Fszt__align16(sizeof(struct s_page_node));
    fprintf(stderr, "%ld\n", Fst__find_upper_page_size(ptr_pssd_manipulation_structure, szt_lcl_minimum_initialization_size));
    if (NULL == (ptr_pssd_manipulation_structure->ptr_stc_page_linked_list = Fptr_void__nmap(szt_lcl_minimum_initialization_size)))
    {
    return (NULL);
    }
    szt_lcl_minimum_initialization_size = Fszt__align16(sizeof(struct s_block_container))
    + MINIMUM_NUMBER_OF_TINY_SMALL_ALLOCATIONS 
    * (Fszt__align16(sizeof(struct s_block)) + SMALL) 
    + Fszt__align16(sizeof(struct s_page_node));
    fprintf(stderr, "%ld\n", Fst__find_upper_page_size(ptr_pssd_manipulation_structure, szt_lcl_minimum_initialization_size));
    if (NULL == (ptr_pssd_manipulation_structure->ptr_stc_page_linked_list = Fptr_void__nmap(szt_lcl_minimum_initialization_size)))
    {
    return (NULL);
    }
    return (ptr_pssd_manipulation_structure);
}

struct s_manipulation *Fptr_stc_manipulation__create_manipulation_structure(void)
{
    struct s_manipulation   *ptr_stc_lcl_manipulation_structure;
    uint64_t                u64_lcl_required_number_of_pages_for_manipulation_structure;

    ptr_stc_lcl_manipulation_structure = NULL;
    u64_lcl_required_number_of_pages_for_manipulation_structure = 1;
    while (Fszt__align16(sizeof(struct s_manipulation)) > u64_lcl_required_number_of_pages_for_manipulation_structure * getpagesize())
    {
    u64_lcl_required_number_of_pages_for_manipulation_structure++;
    }
    if (NULL == (ptr_stc_lcl_manipulation_structure = Fptr_void__nmap(u64_lcl_required_number_of_pages_for_manipulation_structure)))
    {
        return NULL;
    }
    return (Fptr_stc_manipulation__init_manipulation(ptr_stc_lcl_manipulation_structure));
}
