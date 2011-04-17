#!/bin/bash
# COMPILE TOMLD

export CFLAGS="-O1 -Wall -pedantic -Wcomment -Wextra"

cc $CFLAGS tomld.c -o tomld
