# Makefile for RRTLib C library
# Reuben Thomas   26/10/96-20/10/03

LIBDIR = $(HOME)/lib
INCDIR = $(HOME)/include/rrt

CFLAGS = -O2 -fPIC


# Targets

libs = list except hash string stream memory
srcs = $(addsuffix .c, $(libs))
objs = $(addsuffix .o, $(libs))
docs = $(addsuffix .tex, $(libs))


# How to make the libraries

lib: libRRT.so

list.c: except.h

rrtlib.c: $(srcs)
	cat $(srcs) > $@

doc: $(docs) libdoc.tex
	prv libdoc

libRRT.so: $(objs)
	$(CC) -s -shared -o $@ $(objs)

install: libRRT.so rrtlib.c
	install libRRT.so $(LIBDIR)
	install -m 644 *.h rrtlib.c $(INCDIR)
	deline $(INCDIR)/{*.h,rrtlib.c}
	./repath.sh $(INCDIR)/{*.h,rrtlib.c}

clean:
	rm -f *.o *.so *.c rrtlib.zip
	prv -clean libdoc

veryclean: clean
	rm -f *.h libRRT.so $(docs)

dist:
	zip -qr rrtlib.zip Makefile *.w libdoc.tex \
	mv rrtlib.zip /home/rrt/public_html/download/software/
