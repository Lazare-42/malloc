#include "malloc.h"
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

    //ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc->ptr_stc_block_container = (struct s_block  **)((uint8_t*)&(ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc[sizeof(struct s_block_container) - sizeof(struct s_block  **)]));
uint64_t Fu64_get_upper_page_size_for_requested_size(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_requested_size)
{
    uint64_t    u64_lcl_upper_page_size;

    u64_lcl_upper_page_size = ZERO;
    u64_lcl_upper_page_size = (float)(((u64_pssd_requested_size 
    / ptr_pssd_manipulation_structure->u64_pagesize) + 1)
    - (float)((float)(u64_pssd_requested_size)
    / (float)ptr_pssd_manipulation_structure->u64_pagesize))
    * (float)ptr_pssd_manipulation_structure->u64_pagesize;
    return (u64_lcl_upper_page_size + u64_pssd_requested_size);
}

uint64_t Fu64__get_default_page_size_for_page_block_container(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_block_size, uint64_t u64_pssd_number_of_elements)
{
    uint64_t    u64_lcl_requested_size;

    u64_lcl_requested_size  = ZERO;

    u64_lcl_requested_size = Fu64__align16(sizeof(struct s_page))
    + (u64_pssd_number_of_elements != ZERO ? u64_pssd_number_of_elements : 1)
    * (Fu64__align16(sizeof(struct s_block)) +  Fu64__align16(u64_pssd_block_size));

    return (Fu64_get_upper_page_size_for_requested_size(ptr_pssd_manipulation_structure, u64_lcl_requested_size));
}

uint64_t Fu64__get_number_of_structure_in_asked_page_size(uint64_t u64_pssd_asked_page_size, uint64_t u64_pssd_asked_malloc_size)
{
    return ((u64_pssd_asked_page_size - Fu64__align16(sizeof(struct s_page))) / (Fu64__align16(sizeof(struct s_block)) +  Fu64__align16(u64_pssd_asked_malloc_size)));
}
