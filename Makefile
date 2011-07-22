CC = cc
CFLAGS = -O2 -Wall -pedantic -Wcomment -Wextra
LDFLAGS = 
OBJS = tomld.o tio.o tpath.o tstring.o

PREFIX=/usr
SRV=/sbin/insserv


all: tomld

tomld: $(OBJS)
	$(CC) $(CFLAGS) -o tomld $(LDFLAGS) $(OBJS)

clean:
	rm -f tomld *.o

install: all
	test -d $(DESTDIR)$(PREFIX)                || mkdir $(DESTDIR)$(PREFIX)
	test -d $(DESTDIR)$(PREFIX)/sbin           || mkdir $(DESTDIR)$(PREFIX)/sbin
	test -d $(DESTDIR)$(PREFIX)/share          || mkdir $(DESTDIR)$(PREFIX)/share
	test -d $(DESTDIR)$(PREFIX)/share/man      || mkdir $(DESTDIR)$(PREFIX)/share/man
	test -d $(DESTDIR)$(PREFIX)/share/man/man1 || mkdir $(DESTDIR)$(PREFIX)/share/man/man1
	test -d $(DESTDIR)/etc                     || mkdir $(DESTDIR)/etc
	test -d $(DESTDIR)/etc/init.d              || mkdir $(DESTDIR)/etc/init.d
	test -d $(DESTDIR)/etc/tomld               || mkdir $(DESTDIR)/etc/tomld

	install --mode 0755 tomld $(DESTDIR)$(PREFIX)/sbin/
	install --mode 0755 tomld.init $(DESTDIR)/etc/init.d/tomld
	install --mode 0644 tomld.conf $(DESTDIR)/etc/tomld/
	install --mode 0644 tomld.manual $(DESTDIR)$(PREFIX)/share/man/man1/tomld.1

	test ! -x $(DESTDIR)$(SRV) || $(DESTDIR)$(SRV) tomld

uninstall:
	test ! -x $(DESTDIR)$(SRV) || $(DESTDIR)$(SRV) --remove tomld

	rm -f $(DESTDIR)$(PREFIX)/sbin/tomld
	rm -f $(DESTDIR)$(PREFIX)/share/man/man1/tomld.1
	rm -f $(DESTDIR)/etc/init.d/tomld
	rm -f $(DESTDIR)/etc/tomld/tomld.conf
	rmdir $(DESTDIR)/etc/tomld/
