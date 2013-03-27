# -*- Mode: Makefile; tab-width: 8; indent-tabs-mode: t; -*-

AM_CPPFLAGS = \
	-Wall -x objective-c

if HAVE_GTK2
im_cocoa_LTLIBRARIES = im-cocoa.la
endif

im_cocoadir = $(GTK3_IM_MODULEDIR)

im_cocoa_la_CFLAGS = \
	$(GTK2_CFLAGS)
im_cocoa_la_LDFLAGS = \
	-rpath $(im_cocoadir) -avoid-version -module $(no_undefined)
im_cocoa_la_LIBADD = $(GTK2_LIBS)
im_cocoa_la_SOURCES = \
	$(top_srcdir)/src/gtkimcontextcocoa.c \
	$(top_srcdir)/src/gtkimcontextcocoa.h \
	$(top_srcdir)/src/GtkCocoaIMClient.c \
	$(top_srcdir)/src/GtkCocoaIMClient.h \
	$(top_srcdir)/src/imcocoa.c
