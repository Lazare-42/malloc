#include "malloc.h"
#include "libft.h"
#include <sys/mman.h>
#include <string.h>

    //ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc->ptr_stc_block_container = (struct s_block  **)((uint8_t*)&(ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc[sizeof(struct s_block_container) - sizeof(struct s_block  **)]));

uint64_t fu64__align16(size_t number)
{
    return ((uint64_t)(((number - 1) | (16 - 1)) + 1));
}

uint8_t  fu8_bool__check_if_chosen_page_size_too_big(uint64_t u64_next_page_size, uint64_t u64_pssd_required_size)
{
    if (u64_next_page_size > (u64_pssd_required_size << 1) && u64_pssd_required_size > TINY)
    {
        return ((uint8_t)1);
    }
    return ((uint8_t)ZERO);
}

uint64_t fu64__return_lower_page_size(uint64_t u64_pssd_next_page_size, uint64_t u64_pssd_required_size)
{
    uint64_t u64_lcl_new_page_size;

    u64_lcl_new_page_size = ZERO;
    u64_lcl_new_page_size = u64_pssd_next_page_size;
    while (u64_lcl_new_page_size > (u64_pssd_required_size << 1))
    {
    u64_lcl_new_page_size = u64_lcl_new_page_size >> 1;
    }
    return (u64_lcl_new_page_size);
}

uint64_t fu64__return_upper_page_size(uint64_t u64_previous_page_size, uint64_t u64_pssd_required_size)
{
    uint64_t u64_lcl_new_page_size;

    u64_lcl_new_page_size = ZERO;
    u64_lcl_new_page_size = (u64_previous_page_size << 1);
    while (u64_lcl_new_page_size < u64_pssd_required_size) 
    {
    u64_lcl_new_page_size = (u64_lcl_new_page_size << 1);
    }
    return (u64_lcl_new_page_size);
}

uint64_t fu64_get_upper_page_size_for_requested_size(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_requested_size)
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

uint64_t fu64__get_default_page_size_for_page_block(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_block_size, uint64_t u64_pssd_number_of_elements)
{
    uint64_t    u64_lcl_requested_size;

    u64_lcl_requested_size  = ZERO;

    u64_lcl_requested_size = fu64__align16(sizeof(struct s_page))
    + (u64_pssd_number_of_elements != ZERO ? u64_pssd_number_of_elements : 1)
    * (fu64__align16(sizeof(struct s_block)) +  fu64__align16(u64_pssd_block_size));

    return (fu64_get_upper_page_size_for_requested_size(ptr_pssd_manipulation_structure, u64_lcl_requested_size));
}

uint64_t fu64__get_number_of_structure_in_asked_page(uint64_t u64_pssd_asked_page_size, uint64_t u64_pssd_asked_malloc_size)
{
    //ft_printf("[[blue]]Default number of structures is %20llu[[end]]\n", (u64_pssd_asked_page_size - fu64__align16(sizeof(struct s_page))) / (fu64__align16(sizeof(struct s_block)) +  fu64__align16(u64_pssd_asked_malloc_size)));
    return ((u64_pssd_asked_page_size - fu64__align16(sizeof(struct s_page))) / (fu64__align16(sizeof(struct s_block)) +  fu64__align16(u64_pssd_asked_malloc_size)));
}
