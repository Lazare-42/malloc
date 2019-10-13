# malloc()

A re-implementation of the malloc(), realloc() and free() functions from the libc, as a 42 school project.
See attached subject in ./doc

## About
The implementation policy chosen is segregated free lists.
Each list holds free blocks of a particular size ; and when a block of memory
is freed, it is simply pushed onto the free list for that size.
When a request is serviced, the free list for the appropriate size is used to 
satisfy the request.
The lists are themselves tied together using lists.

The project architecture can be summarized as follows :
[Project implementation](doc/my_malloc_architecture-1.png)


## Usage
Simple create the library using `make` and dynamically load the created library
using the script `run.sh`.
