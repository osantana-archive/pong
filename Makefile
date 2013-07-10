#
# Makefile - Pong
#

PACKAGE=pong
CFLAGS+=-Wall `allegro-config --libs` -L./jgmod/lib/unix -I./jgmod/src
JGMOD_CFLAGS=-O3 -W -Wno-unused -Wall -ffast-math -fomit-frame-pointer -funroll-loops -fPIC
DESTDIR=
defaultprefix=/usr/local
prefix=$(DESTDIR)$(defaultprefix)
bindir=$(prefix)/bin
datadir=$(prefix)/share
gamedir=$(prefix)/games
docdir=$(datadir)/doc

all: $(PACKAGE)

pong: $(patsubst %.c,%.o,$(wildcard jgmod/*.c)) pong.o
	gcc -g -o pong ./jgmod/*.o pong.o `allegro-config --libs`

pong.o: pong.c
	gcc -c -g pong.c -I./jgmod

jgmod/%.o: jgmod/%.c
	gcc $(JGMOD_CFLAGS) -c -g -o $@ $< 

clean:
	rm -f $(PACKAGE) *~ core jgmod/*.o

install:
	mkdir -p $(datadir)/$(PACKAGE) $(docdir)/$(PACKAGE)
	cp *.pcx $(datadir)/$(PACKAGE)
	cp *.xm  $(datadir)/$(PACKAGE)
	cp README $(docdir)/$(PACKAGE)
	cp $(PACKAGE) $(gamedir)

.PHONY: clean all install
