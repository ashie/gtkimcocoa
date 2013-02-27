# -*- Mode: Makefile; tab-width: 8; indent-tabs-mode: t; -*-
SUBDIRS=test

AM_CPPFLAGS = \
	 -Wall -x objective-c \
	$(GTK3_CFLAGS)

module_LTLIBRARIES = im-cocoa.la
moduledir = $(GTK3_IM_MODULEDIR)

im_cocoa_la_LDFLAGS = \
	-rpath $(moduledir) -avoid-version -module $(no_undefined)
im_cocoa_la_LIBADD = $(GTK3_LIBS)
im_cocoa_la_SOURCES = \
	gtkimcontextcocoa.c gtkimcontextcocoa.h \
	GtkCocoaIMClient.c GtkCocoaIMClient.h \
	imcocoa.c
