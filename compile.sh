#!/bin/sh

# delete old binary
rm -f tomld
rm -f *.o

# compile tomld.c
export CFLAGS="-O2 -Wall -pedantic -Wcomment -Wextra"

cc $CFLAGS -c tstring.c 	-o tstring.o
cc $CFLAGS -c tpath.c 		-o tpath.o
cc $CFLAGS -c tio.c 		-o tio.o
cc $CFLAGS -c tomld.c 		-o tomld.o

cc $CFLAGS -o tomld tomld.o tio.o tpath.o tstring.o

