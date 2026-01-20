CC=gcc
CFLAGS=-Wall -Werror
OBJS=pbm2sg4.o pbm.o sg4.o

all: pbm2sg4
pbm2sg4: $(OBJS)
pbm2sg4.o: pbm2sg4.c pbm.h sg4.h
pbm.o: pbm.c pbm.h
sg4.o: sg4.c pbm.h sg4.h

.PHONY: clean distclean

clean:
	rm -f $(OBJS) *~ *#

distclean: clean
	rm -f pbm2sg4
