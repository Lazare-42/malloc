#include "includes/malloc.h"

#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

int main()
{

    char    *test;
    char test_string[128] = "iiiiiisssssssssssssssssssssssssssssssssssssssssssssssosssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssso\0";
    char **test_container;
    unsigned int i = 0;
    unsigned int z = 0;
    unsigned int f = 0;

    test = NULL;
    f = 1000;
    //fprintf(stderr, "%lu is length of test string\n", strlen(test_string));
    test_container = malloc(sizeof(char *) *f);
    while (z < f)
    {
        //fprintf(stderr, "Starting malloc number %d\n", z);
        i = 0;
        test = malloc(strlen(test_string));
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
        /**
         *  This should segfault !!!
         */
        //test[i] = '\0';
        //printf("%s and %d strlen %zu\n", test, i, strlen(test_string));
        test_container[z] = test;
        z++;
     }
    z = 0;
    while (z < f)
    {
        //fprintf(stderr, "Reallocing from %lu to %lu\n", strlen(test_string), strlen(test_string) * 2);
        test_container[z] = realloc(test_container[z], strlen(test_string) * 2);
        z++;
    }
    //free(test_container);


    return (SUCCESS);
}
