CC = cc
CFLAGS = -O2 -Wall -pedantic -Wcomment -Wextra -g
LDFLAGS = 
OBJS = tomld.o tio.o tpath.o tstring.o
PREFIX=/usr/local

tomld: $(OBJS)
	$(CC) $(CFLAGS) -o tomld $(LDFLAGS) $(OBJS)

clean:
	rm -f tomld *.o

install:
	test -d $(PREFIX)                || mkdir $(PREFIX)
	test -d $(PREFIX)/sbin           || mkdir $(PREFIX)/sbin
	test -d $(PREFIX)/share          || mkdir $(PREFIX)/share
	test -d $(PREFIX)/share/man      || mkdir $(PREFIX)/share/man
	test -d $(PREFIX)/share/man/man1 || mkdir $(PREFIX)/share/man/man1
	install --mode 0755 tomld $(PREFIX)/sbin
	install --mode 0644 tomld.manual $(PREFIX)/share/man/man1/tomld.1
