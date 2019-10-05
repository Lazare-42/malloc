#include <stdio.h>
#include "malloc.h"

uint64_t Fu64__align16(size_t number)
{
    return (uint64_t)(( ( number - 1 ) | ( 16 - 1 ) ) + 1);
}

void    *my_malloc(size_t size)
{
    static struct s_manipulation    *ptr_stc_lcl_manipulation_structure = NULL;
    struct s_page                   *ptr_stc_lcl_browse_page_category;
    //struct s_page                   *ptr_stc_lcl_browse_same_page_category;

    (void)size;
    /**
     *  If the manipulation structure is set to NULL, initialize it.
     */
    ptr_stc_lcl_browse_page_category        = NULL;
    //ptr_stc_lcl_browse_same_page_category   = NULL;
    if (ptr_stc_lcl_manipulation_structure == NULL)
    {
        ptr_stc_lcl_manipulation_structure = Fptr_stc_manipulation__create_manipulation_structure();
        if (ptr_stc_lcl_manipulation_structure  == NULL)
        {
            return NULL;
        }
    }
    ptr_stc_lcl_browse_page_category = ptr_stc_lcl_manipulation_structure->ptr_stc_page_linked_list;
    struct s_block *tmp;
    uint64_t tmp_val = ZERO;
    uint64_t tmp_val_2 = ZERO;
    tmp = ptr_stc_lcl_browse_page_category->ptr_first_free_block_;
    while (tmp != NULL)
    {
        //fprintf(stderr, "Current size is %20llu\n", tmp->u64_size_);
        tmp_val +=  tmp->u64_size_;
        //fprintf(stderr, "Declared block size is %20llu, aligned 16 block size is %20llu\n", tmp->u64_size_, Fu64__align16(tmp->u64_size_));
        tmp = tmp->ptr_next_;
        tmp_val_2 = tmp_val_2 + 1;
    }
    fprintf(stderr, "Allocated %20llu of free space on %20llu elements which gets us a total space of %20llu\n", tmp_val, tmp_val_2, tmp_val + tmp_val_2 * Fu64__align16((sizeof(struct s_block))) + Fu64__align16(sizeof(struct s_page)));
    return NULL;
}
