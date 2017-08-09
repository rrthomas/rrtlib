# Makefile.am for RRTLib

SUBDIRS = lib
ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = m4/gnulib-cache.m4 run-test

lib_LTLIBRARIES = librrt.la
librrt_includedir = $(includedir)/rrt

librrt_include_HEADERS = \
	except.h \
	memory.h \
	bsearch.h \
	stream.h \
	buffer.h \
	vector.h

librrt_la_SOURCES = \
	except.c \
	memory.c \
	bsearch.c \
	stream.c \
	buffer.c \
	vector.c \
	$(librrt_include_HEADERS)

AM_TESTS_ENVIRONMENT = \
	export LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH); \
	if test -n "$(VALGRIND)"; then export VALGRIND='$(VALGRIND)'; fi; \
	export LIBTOOL=$(top_builddir)/libtool;
check_PROGRAMS = buffer_test vector_test
LDADD = librrt.la
TESTS = $(check_PROGRAMS)
LOG_COMPILER = $(srcdir)/run-test
