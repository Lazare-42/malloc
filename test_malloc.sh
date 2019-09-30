#!/bin/bash

make
rm "./my_test"
gcc -Wall -Wextra -Werror -fsanitize=address -o "my_test" -L. -lmalloc main.c 
exec "./my_test"
