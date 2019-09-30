#include "includes/malloc.h"

#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

int main()
{

    char    *test;
    char test_string[256] = "coucou";
    unsigned int i = 0;
    unsigned int z = 0;

    test = NULL;
    while (z < 100000)
    {
        fprintf(stderr, "Starting malloc number %d\n", z);
        i = 0;
        test = my_malloc(strlen(test_string));
        if (test == NULL)
        {
            fprintf(stderr, "FAILURE NULL pointer received after malloc\n");
            return FAILURE;
        }
        while (i < strlen(test_string))
        {
            test[i] = test_string[i];
            i++;
        }
        test[i] = '\0';
        printf("%s and %d strlen %zu\n", test, i, strlen(test_string));
        z++;
     }

    return (SUCCESS);
}
