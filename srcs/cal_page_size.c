#include "malloc.h"
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

uint64_t Fu64_get_upper_page_size_for_requested_size(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_requested_size)
{
    uint64_t    u64_lcl_upper_page_size;

    fprintf(stderr, "coucou, requested page size is %llu\n", u64_pssd_requested_size);
    u64_lcl_upper_page_size = ZERO;
    u64_lcl_upper_page_size = (float)(((u64_pssd_requested_size 
    / ptr_pssd_manipulation_structure->u64_pagesize) + 1)
    - (float)((float)(u64_pssd_requested_size)
    / (float)ptr_pssd_manipulation_structure->u64_pagesize))
    * (float)ptr_pssd_manipulation_structure->u64_pagesize;
    return (u64_lcl_upper_page_size);
}

uint64_t Fu64__get_default_page_size_for_page_block_container(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_block_size)
{
    uint64_t    u64_lcl_requested_size;

    u64_lcl_requested_size  = ZERO;

    u64_lcl_requested_size = (Fu64__align16(sizeof(struct s_block_container))
    + MINIMUM_NUMBER_OF_TINY_SMALL_ALLOCATIONS 
    * (Fu64__align16(sizeof(struct s_block)) + u64_pssd_block_size) 
    + Fu64__align16(sizeof(struct s_page_node)));

    return (Fu64_get_upper_page_size_for_requested_size(ptr_pssd_manipulation_structure, u64_lcl_requested_size));
}

uint64_t Fu64__get_default_page_size_for_page_double_pointer_block_container(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_number_of_structures)
{
    uint64_t    u64_lcl_requested_size;

    u64_lcl_requested_size  = ZERO;

    u64_lcl_requested_size = Fu64__align16(sizeof(struct s_block_container *))
    * u64_pssd_number_of_structures
    + Fu64__align16(sizeof(struct s_page_node))
    + Fu64__align16(sizeof(struct s_block_container));
    return (Fu64_get_upper_page_size_for_requested_size(ptr_pssd_manipulation_structure, u64_lcl_requested_size));
}

uint64_t Fu64__get_number_of_structure_in_default_page_size(uint64_t u64_pssd_default_page_size, uint64_t u64_pssd_size_of_structure)
{
    return (u64_pssd_default_page_size / u64_pssd_size_of_structure);
}
