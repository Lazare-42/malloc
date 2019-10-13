#include "malloc.h"
#include <stdio.h>
#include <sys/mman.h>

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
 *  SHOULD add page to linked list of pages and return pointer AfTER structure of
 *  linked list of pages
 *
 */
void    *fptr_void__nmap(uint64_t u64_pssd_page_number_to_allocate)
{
    void *ptr_lcl_new_memory;

    ptr_lcl_new_memory = NULL;
    ptr_lcl_new_memory = mmap(NULL, u64_pssd_page_number_to_allocate, 
    PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    /**
     *  No need to check for NULL ; functions calling fptr_void__nmap
     *  will do so
     */
    return (ptr_lcl_new_memory);
}

void    fptr_stc_init_memory_block(struct s_block 
        **ptr_pssd_memory_block_to_init, struct s_page *ptr_pssd_page_base, 
        uint64_t u64_pssd_size_of_memory_to_initialize, 
        struct s_block *ptr_pssd_next_block)
{
    (*ptr_pssd_memory_block_to_init)->u64_size_                 = u64_pssd_size_of_memory_to_initialize;
    (*ptr_pssd_memory_block_to_init)->u64_free_size_            = u64_pssd_size_of_memory_to_initialize;
    (*ptr_pssd_memory_block_to_init)->ptr_next_block_in_page_   = ptr_pssd_next_block;
    (*ptr_pssd_memory_block_to_init)->ptr_page_base_            = ptr_pssd_page_base;
}

struct s_page       *fptr_stc_init_blocks_in_page(struct s_page *ptr_pssd_new_page)
{
    struct s_block  *ptr_stc_lcl_new_block;
    struct s_block  *ptr_stc_lcl_previous_new_block;
    uint64_t        u64_lcl_browse_to_init;
    uint64_t        u64_lcl_size_assigned_to_structure;

    ptr_stc_lcl_new_block               = NULL;
    ptr_stc_lcl_previous_new_block      = NULL;
    u64_lcl_browse_to_init              = ZERO;
    ptr_stc_lcl_new_block               = (struct s_block*)&((uint8_t*)ptr_pssd_new_page)[fu64__align16(sizeof(struct s_page))];
    u64_lcl_size_assigned_to_structure  = ptr_pssd_new_page->u64_block_size_;
    while (u64_lcl_browse_to_init < ptr_pssd_new_page->u64_number_of_used_blocks_in_page_)
    {
        u64_lcl_browse_to_init                  = u64_lcl_browse_to_init + 1;
        if (u64_lcl_browse_to_init == ptr_pssd_new_page->u64_number_of_used_blocks_in_page_)
        {
            u64_lcl_size_assigned_to_structure      = ptr_pssd_new_page->u64_size_ - (((void*)ptr_stc_lcl_new_block - (void*)ptr_pssd_new_page)) - fu64__align16(sizeof(struct s_block));
        }
        fptr_stc_init_memory_block(&ptr_stc_lcl_new_block, ptr_pssd_new_page, u64_lcl_size_assigned_to_structure, ptr_stc_lcl_previous_new_block);
        ptr_stc_lcl_previous_new_block          = ptr_stc_lcl_new_block;
        if (u64_lcl_browse_to_init != ptr_pssd_new_page->u64_number_of_used_blocks_in_page_)
            ptr_stc_lcl_new_block                   = (struct s_block*)&((uint8_t*)ptr_stc_lcl_new_block)[fu64__align16(sizeof(struct s_block)) + fu64__align16(ptr_pssd_new_page->u64_block_size_)];
    }
    ptr_pssd_new_page->ptr_first_free_block_ = ptr_stc_lcl_new_block;
    return (ptr_pssd_new_page);
}

struct s_page *fu8__create_and_init_new_page_category(struct s_manipulation *ptr_pssd_stc_manipulation_structure, uint64_t u64_pssd_required_size, uint64_t u64_pssd_number_of_elements)
{
    uint64_t        u64_lcl_minimum_initialization_size;
    uint64_t        u64_lcl_number_of_structures_for_asked_size;
    struct s_page   *ptr_lcl_stc_new_page;

    u64_lcl_minimum_initialization_size         = ZERO;
    u64_lcl_number_of_structures_for_asked_size = ZERO;
    u64_lcl_minimum_initialization_size         = fu64__get_default_page_size_for_page_block(ptr_pssd_stc_manipulation_structure, u64_pssd_required_size, u64_pssd_number_of_elements); 
    u64_lcl_number_of_structures_for_asked_size = fu64__get_number_of_structure_in_asked_page(u64_lcl_minimum_initialization_size, u64_pssd_required_size);

    ptr_lcl_stc_new_page                        = fptr_void__nmap(u64_lcl_minimum_initialization_size);
    if (NULL == (ptr_lcl_stc_new_page = fptr_void__nmap(u64_lcl_minimum_initialization_size)))
    {
        return NULL;
    }
    ptr_lcl_stc_new_page->u64_size_                                 = u64_lcl_minimum_initialization_size;
    ptr_lcl_stc_new_page->u64_block_size_                           = u64_pssd_required_size; 
    ptr_lcl_stc_new_page->u64_number_of_used_blocks_in_page_        = u64_lcl_number_of_structures_for_asked_size;
    ptr_lcl_stc_new_page->u64_total_number_of_pages_in_category_    = ZERO;
    ptr_lcl_stc_new_page->u64_number_of_used_pages_in_category_     = ZERO;
    ptr_lcl_stc_new_page->ptr_first_free_block_                     = NULL;
    ptr_lcl_stc_new_page->ptr_first_occuppied_block_                = NULL;
    ptr_lcl_stc_new_page->ptr_next_page_in_category_              = NULL;
    ptr_lcl_stc_new_page->ptr_next_page_upper_category_             = NULL;
    return (fptr_stc_init_blocks_in_page(ptr_lcl_stc_new_page));
}

struct s_manipulation *fptr_stc_manipulation__init_manipulation(struct s_manipulation *ptr_pssd_stc_manipulation_structure)
{
    ptr_pssd_stc_manipulation_structure->u64_pagesize                               = getpagesize();
    ptr_pssd_stc_manipulation_structure->ptr_stc_page_linked_list                   = NULL;
    ptr_pssd_stc_manipulation_structure->ptr_stc_binary_tree_of_malloced_nodes_     = NULL;

    if (NULL != (ptr_pssd_stc_manipulation_structure->ptr_stc_page_linked_list = fu8__create_and_init_new_page_category(ptr_pssd_stc_manipulation_structure, TINY, MINIMUM_NUMBER_OF_INITIAL_ALLOCATIONS)))
    {
        if (NULL != (ptr_pssd_stc_manipulation_structure->ptr_stc_page_linked_list->ptr_next_page_upper_category_ = fu8__create_and_init_new_page_category(ptr_pssd_stc_manipulation_structure, SMALL, MINIMUM_NUMBER_OF_INITIAL_ALLOCATIONS)))
        {
        return (ptr_pssd_stc_manipulation_structure);
        }
    }
    return (NULL);
}

struct s_manipulation *fptr_stc_manipulation__create_manipulation(void)
{
    struct s_manipulation   *ptr_stc_lcl_manipulation_structure;
    uint64_t                u64_lcl_required_number_of_pages_for_manipulation_structure;

    ptr_stc_lcl_manipulation_structure = NULL;
    u64_lcl_required_number_of_pages_for_manipulation_structure = 1;
    while (fu64__align16(sizeof(struct s_manipulation)) > u64_lcl_required_number_of_pages_for_manipulation_structure * getpagesize())
    {
    u64_lcl_required_number_of_pages_for_manipulation_structure++;
    }
    if (NULL != (ptr_stc_lcl_manipulation_structure = fptr_void__nmap(u64_lcl_required_number_of_pages_for_manipulation_structure * getpagesize())))
    {
        return (fptr_stc_manipulation__init_manipulation(ptr_stc_lcl_manipulation_structure));
    }
    return NULL;
}
