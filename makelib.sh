#!/bin/sh
rm *.so
rm *.o
gcc -O2 -Wall -Werror -fPIC -c downscale.c
gcc -O2 -Wall -Werror -fPIC -c nd.c
gcc -O2 -Wall -Werror -fPIC -c precisionless.c
gcc -shared -o libdownscale.so downscale.o nd.o precisionless.o
rm *.o
