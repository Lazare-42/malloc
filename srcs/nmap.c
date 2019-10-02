#include "malloc.h"
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

/**
 *  Allocates new memory by 
 *  Calls nmap with the following arguments : 
 *      1. NULL to point to a "fake" address
 *      2. getpagesize() * u64_pssd_page_number_to_allocate to use a multiple of a system page
 *      3. PROT_READ | PROT_WRITE to read & write allocated memory
 *      4. MAP_SHARED | MAP_PRIVATE
 *   to get a fresh memory area that shares no pages with other mappings (obtained memory is initialized to ZERO)
 *
 *
 *  SHOULD add page to linked list of pages and return pointer AFTER structure of
 *  linked list of pages
 *
 */
void    *Fptr_void__nmap(uint64_t u64_pssd_page_number_to_allocate)
{
    void *ptr_lcl_new_memory;

    ptr_lcl_new_memory = NULL;
    ptr_lcl_new_memory = mmap(NULL, u64_pssd_page_number_to_allocate, 
    PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    /**
     *  No need to check for NULL ; functions calling Fptr_void__nmap
     *  will do so
     */
    return (ptr_lcl_new_memory);
}

struct s_block        *Fptr_stc_init_memory_block(struct s_block *ptr_pssd_memory_block_to_init, uint64_t u64_pssd_size_of_memory_to_initialize)
{
    ptr_pssd_memory_block_to_init->u64_total_size                   = u64_pssd_size_of_memory_to_initialize;
    ptr_pssd_memory_block_to_init->u64_free_size                    = ZERO;
    ptr_pssd_memory_block_to_init->u64_position_in_double_pointer   = ZERO;
    return (ptr_pssd_memory_block_to_init);
}

uint8_t Fu8__alloc_realloc_block_container(struct s_manipulation *ptr_pssd_manipulation_structure, uint64_t u64_pssd_number_of_pointers_for_double_pointer)
{
    uint64_t                    u64_lcl_size_of_page;
    uint64_t                    u64_lcl_browse_page_to_initalize;
    struct s_block_container    *ptr_stc_lcl_new_block_container;
    struct s_block              *ptr_stc_new_block_to_init;

    u64_lcl_size_of_page = ZERO;
    u64_lcl_browse_page_to_initalize = ZERO;
    ptr_stc_new_block_to_init = NULL;
    ptr_stc_lcl_new_block_container = NULL;

    /**
     *  We pass the required size to the nmap function or increase the old size 
     *  by the size of one page
     */
    if (ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc != NULL)
    {
    u64_lcl_size_of_page = Fu64__get_default_page_size_for_page_double_pointer_block_container(ptr_pssd_manipulation_structure, ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc->u64_size_of_double_pointer + u64_pssd_number_of_pointers_for_double_pointer);
    }
    else
    {
    u64_lcl_size_of_page    = Fu64__get_default_page_size_for_page_double_pointer_block_container(ptr_pssd_manipulation_structure, u64_pssd_number_of_pointers_for_double_pointer);
    }

    /**
     *  Call nmap and check if it returns NULL
     */
    if (NULL == (ptr_stc_lcl_new_block_container = Fptr_void__nmap(u64_lcl_size_of_page)))
    {
        return FAILURE;
    }

    /**
     *  If we are re-allocating a page ; we should copy the old page
     */
    if (ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc != NULL)
    {
    memcpy(ptr_stc_lcl_new_block_container, ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc, Fu64__get_default_page_size_for_page_double_pointer_block_container(ptr_pssd_manipulation_structure, ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc->u64_size_of_double_pointer));
    u64_lcl_browse_page_to_initalize = ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc->u64_size_of_double_pointer;
    }
    /**
     *  Save the newly created block
     */
    ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc = (struct s_block_container*)(ptr_stc_lcl_new_block_container);

    /**
     *  Save the number of elements in the newly created block
     */
    ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc->u64_size_of_double_pointer = Fu64__get_number_of_structure_in_default_page_size(u64_lcl_size_of_page, sizeof(struct s_block*));    

    /**
     *  Initialize the new pointers by setting them to NULL
     */
    while (u64_lcl_browse_page_to_initalize < ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc->u64_size_of_double_pointer)
    {
        fprintf(stderr, "Here at %20llu\n", u64_lcl_browse_page_to_initalize);
        ptr_pssd_manipulation_structure->ptr_stc_container_to_alloc_realloc->ptr_stc_block_container[u64_lcl_browse_page_to_initalize] = NULL;
        u64_lcl_browse_page_to_initalize = u64_lcl_browse_page_to_initalize + 1;
    }

    return SUCCESS;
}

struct s_manipulation *Fptr_stc_manipulation__init_manipulation(struct s_manipulation *ptr_pssd_manipulation_structure)
{
    uint64_t                u64_lcl_minimum_initialization_size;

    u64_lcl_minimum_initialization_size                             = ZERO;
    ptr_pssd_manipulation_structure->u64_pagesize                   = getpagesize();
    Fu8__alloc_realloc_block_container(ptr_pssd_manipulation_structure, 345);
    ptr_pssd_manipulation_structure->ptr_stc_tiny_block_container   = NULL;
    ptr_pssd_manipulation_structure->ptr_stc_small_block_container  = NULL;
    ptr_pssd_manipulation_structure->ptr_stc_large_block_container  = NULL;
    ptr_pssd_manipulation_structure->ptr_stc_page_linked_list       = NULL;

    if (NULL == (ptr_pssd_manipulation_structure->ptr_stc_page_linked_list = Fptr_void__nmap(u64_lcl_minimum_initialization_size)))
    {
    return (NULL);
    }
    if (NULL == (ptr_pssd_manipulation_structure->ptr_stc_page_linked_list = Fptr_void__nmap(u64_lcl_minimum_initialization_size)))
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
    while (Fu64__align16(sizeof(struct s_manipulation)) > u64_lcl_required_number_of_pages_for_manipulation_structure * getpagesize())
    {
    u64_lcl_required_number_of_pages_for_manipulation_structure++;
    }
    if (NULL == (ptr_stc_lcl_manipulation_structure = Fptr_void__nmap(u64_lcl_required_number_of_pages_for_manipulation_structure)))
    {
        return NULL;
    }
    return (Fptr_stc_manipulation__init_manipulation(ptr_stc_lcl_manipulation_structure));
}
