# -*- Mode: Makefile; tab-width: 8; indent-tabs-mode: t; -*-

AM_CPPFLAGS = \
	-Wall -x objective-c

if HAVE_GTK3
im_cocoa_LTLIBRARIES = im-cocoa.la
endif

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
