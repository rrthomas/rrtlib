# Makefile for RRTLib C library
# (c) Reuben Thomas 1996-2003

# TODO: Use a more sensible build system that deals with dependencies
# properly; provide a trivial build script for people who don't have it.

LIBDIR = $(HOME)/lib
INCDIR = $(HOME)/include/rrt

CFLAGS = -g -fPIC -pedantic -Wall -W
LDFLAGS = -L. -lRRT


# Targets

lib = libRRT.so
libs = except memory string stream list hash buffer
srcs = $(addsuffix .c, $(libs))
objs = $(addsuffix .o, $(libs))
docs = $(addsuffix .html, $(libs))
tests = $(addsuffix _test, $(libs))


# How to make the libraries

lib: $(lib)

#doc: $(docs)

$(lib): $(objs)
	$(CC) -shared -o $@ $(objs)

$(tests): $(lib)

test: hash_test
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):. hash_test

install: libRRT.so
	install -s libRRT.so $(LIBDIR)
	install -m 644 *.h $(INCDIR)

clean:
	rm -f *.o

veryclean: clean
	rm -f libRRT.so $(docs) rrtlib.zip

distclean: veryclean

dist:
	zip -qr rrtlib.zip Makefile *.c *.h \
	mv rrtlib.zip /home/rrt/public_html/download/software/
