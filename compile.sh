#!/bin/bash

# compile tomld.c
export CFLAGS="-O1 -Wall -pedantic -Wcomment -Wextra"
cc $CFLAGS tomld.c -o tomld
