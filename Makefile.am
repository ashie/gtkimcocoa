# -*- Mode: Makefile; tab-width: 8; indent-tabs-mode: t; -*-
SUBDIRS=test

AM_CPPFLAGS = \
	 -Wall -x objective-c

im_cocoa_LTLIBRARIES = im-cocoa.la
im_cocoadir = $(GTK3_IM_MODULEDIR)

im_cocoa_la_CFLAGS = \
	$(GTK3_CFLAGS)
im_cocoa_la_LDFLAGS = \
	-rpath $(im_cocoadir) -avoid-version -module $(no_undefined)
im_cocoa_la_LIBADD = $(GTK3_LIBS)
im_cocoa_la_SOURCES = \
	gtkimcontextcocoa.c gtkimcontextcocoa.h \
	GtkCocoaIMClient.c GtkCocoaIMClient.h \
	imcocoa.c

EXTRA_DIST = README.md
