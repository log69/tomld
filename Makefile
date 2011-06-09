CC = cc
CFLAGS = -O2 -Wall -pedantic -Wcomment -Wextra
LDFLAGS = 
OBJS = tomld.o tio.o tpath.o tstring.o

tomld: $(OBJS)
	$(CC) $(CFLAGS) -o tomld $(LDFLAGS) $(OBJS)

clean:
	rm -f tomld *.o
