#ifndef MALLOC_H

# define MALLOC_H
#include <inttypes.h>
#include <unistd.h>

# define FAILURE (1)
# define SUCCESS (0)

# define ZERO (0)

# define FALSE  (0)
# define TRUE   (1)

# define FIRST_BIT      (0x01)
# define SECOND_BIT     (0x02)
# define THIRD_BIT      (0x04)
# define FOURTH_BIT     (0x08)
# define FIFTH_BIT      (0x10)
# define SIXTH_BIT      (0x20)
# define SEVENTH_BIT    (0x40)
# define EIGHTH_BIT     (0x80) 

struct s_block 
{
    uint64_t            u64_size;
    /**
     * eight    bit = Y/N to define  
     * seventh  bit = Y/N to define
     * sixth    bit = Y/N to define
     * fifth    bit = Y/N to define
     * fourth   bit = Y/N to define
     * third    bit = Y/N to define
     * second   bit = Y/N to define
     * first    bit = Y/N location is occupied
    **/
    uint64_t            u8_status;
    struct s_block      *next;
};

void    *my_malloc(size_t size);

#endif
