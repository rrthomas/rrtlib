# Makefile for RRTLib C library
# (c) Reuben Thomas 1996-2003

# TODO: Use a more sensible build system that deals with dependencies
# properly; provide a trivial build script for people who don't have it.

LIBDIR = $(HOME)/lib
INCDIR = $(HOME)/include/rrt

CFLAGS = -O2 -fPIC -pedantic -Wall -W


# Targets

libs = except memory string stream list hash buffer
srcs = $(addsuffix .c, $(libs))
objs = $(addsuffix .o, $(libs))
docs = $(addsuffix .html, $(libs))


# How to make the libraries

lib: libRRT.so

#doc: $(docs)

libRRT.so: $(objs)
	$(CC) -s -shared -o $@ $(objs)

install: libRRT.so
	install libRRT.so $(LIBDIR)
	install -m 644 *.h $(INCDIR)

clean:
	rm -f *.o

veryclean: clean
	rm -f libRRT.so $(docs) rrtlib.zip

distclean: veryclean

dist:
	zip -qr rrtlib.zip Makefile *.c *.h \
	mv rrtlib.zip /home/rrt/public_html/download/software/
