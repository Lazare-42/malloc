#!/bin/bash

rm "./my_test"
make
if [ $? -ne 0 ]
then 
    echo "exiting script"
else
    #$(gcc -Wall -Wextra -Werror   -o "my_test" -L. -lmalloc main.c)
    $(gcc -Wall -Wextra -Werror  -fsanitize=address -o "my_test" main.c)
    if [ $? -ne 0 ]
    then
        echo "could not compile"
    else
        exec "./run.sh ./my_test"
    fi
fi
