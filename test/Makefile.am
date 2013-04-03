AM_CPPFLAGS = \
	-Wall -x objective-c

noinst_PROGRAMS =

if HAVE_GTK3
noinst_PROGRAMS += testbed-gtk3
endif

if HAVE_GTK2
noinst_PROGRAMS += testbed-gtk2
endif

testbed_gtk3_CFLAGS = $(GTK3_CFLAGS)
testbed_gtk3_LDADD = $(GTK3_LIBS)
testbed_gtk3_SOURCES = \
	testbed.c

testbed_gtk2_CFLAGS = $(GTK2_CFLAGS)
testbed_gtk2_LDADD = $(GTK2_LIBS)
testbed_gtk2_SOURCES = \
	testbed.c
