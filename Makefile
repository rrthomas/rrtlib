# Makefile for RRTLib

# TODO: Use a more sensible build system that deals with dependencies
# properly; provide a trivial build script for people who don't have it.

# FIXME: test arch for LIBDIR
LIBDIR = $(HOME)/local/i686/lib
INCDIR = $(HOME)/local/include/rrt

CFLAGS = -g -pedantic -fPIC -Wall -W
LDFLAGS = -L. -lRRT -fPIC


# Targets

lib = libRRT.so
zip = rrtlib.zip
libs = except memory string stream list buffer vector
srcs = $(addsuffix .c, $(libs))
hdrs = $(addsuffix .h, $(libs))
objs = $(addsuffix .o, $(libs))
docs = $(addsuffix .html, $(libs))
tests = list_test buffer_test vector_test # $(addsuffix _test, $(libs))


# How to make the libraries

lib: $(lib)

#doc: $(docs)

$(lib): $(objs) $(hdrs)
	$(CC) -shared -o $@ $(objs)

$(tests): $(lib)

test: $(tests)
	@for i in $^; do \
		LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH) $$i; \
	done

install: test $(lib)
	install $(lib) $(LIBDIR)
	install -m 644 *.h $(INCDIR)

clean:
	rm -f $(objs) $(tests)

veryclean: clean
	rm -f $(lib) $(docs) $(zip)

distclean: veryclean

dist:
	zip -qr $(zip) Makefile *.c *.h \
	mv rrtlib.zip /home/rrt/public_html/download/software/
