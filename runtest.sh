#!/bin/sh
rm *.o
rm "test"
libpath=$(pwd)
export LD_LIBRARY_PATH=$libpath:$LD_LIBRARY_PATH
gcc -L$libpath -Wall -o test test.c -ldownscale -lm
./test
