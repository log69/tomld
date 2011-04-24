#!/bin/bash

# delete old binary
rm -f tomld

# compile tomld.c
export CFLAGS="-O2 -Wall -pedantic -Wcomment -Wextra"
cc $CFLAGS tomld.c -o tomld
