A. CREATE NEW FREE LIST OF SIZE N

1.  Create a function to create list of N elements each of size 16_aligned(Z).

    If N is not given , create page with as many N elements in it as possible.

    Each element contains : its size ; its free size ; pointer to next element ;
    and pointer to pase "page pointer".

    Add each element to list of all pointers.

    Last element should use remaining free space in page.

    Function returns the pointer to "page pointer".

 2. Create a function to add received page pointer to list of page pointers of 
    similar size as FIFO.
    Page pointer contains pointer to other page pointer.
    Return first page pointer.

3. If chosen page pointer is the first of its type, add it to list of different 
   page pointer sizes.


B. INITIALIZATION :
                Create manipulation structure (on a page of its own ; we discard
                unused space).
                Create list of TINY  element => go to A with Z = TINY   , N = 100
                Create list of SMALL element => go to A with Z = SMALL  , N = 100
                Go to asked function.

C. Malloc :
            Required size S.
            Case 1 : Page Pointer W containing size Y >= S exists. 

                If last Page Pointer starting from W does not contain any free 
                element ; go to case 2.
                Else select first free element of page

            Case 2 : Page Pointer W containing size Y >= S does not exist.
                
                Go to A with Z = Y , N = 0.
                Return first element of first page pointer containing an 
                available element.
           
            In block itself :
                Note occuppied space in block header.

            In page block :
                Mark the next element as the first in list.
                Increase u64_number_of_used_blocks_in_page.

            In manipulation structure :
                Add pointer in list of malloced() pointers.

            Return pointer.

D. Free :

        Check if pointer belongs in list of all pointer.
        If it does not, return 

        In manipulation structure :
            Remove pointer from list of all pointers.

        In block itself : 
            Reset u64_size_ = u64_free_size_

        In s_page tied to block :
            Decrease u64_number_of_used_blocks_in_page
            if u64_number_of_used_blocks_in_page == 0 
                [ for now delete page ]
            else
                add the pointer as the first element in its "page pointer".

        Return

E. Realloc():

        Check if pointer belongs in list of all pointer.
        If it does not, return NULL

        Else get s_block structure corresponding to the malloced memory.

        If u64_free_size_ >= size ; increase used size ; return the structure.

        Else, call malloc() with required size ; memcpy ; and then free() 
        previous structure.
        

