#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

dnl Process this file with autoconf to produce a configure script.
AC_INIT(gtkimcontextcocoa.c)
PACKAGE=imcocoa

dnl version number
MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=0
EXTRA_VERSION=
VERSION=$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION$EXTRA_VERSION


dnl 
AM_INIT_AUTOMAKE($PACKAGE, $VERSION)
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE")
AC_DEFINE_UNQUOTED(VERSION, "$VERSION")

AC_CONFIG_HEADER([config.h])


dnl
dnl Checks for programs.
dnl
AC_PROG_CPP
AC_PROG_CC
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET


dnl
dnl Checks for host
dnl
AC_CANONICAL_HOST


dnl
dnl libtool
dnl
AC_ENABLE_SHARED(yes)
AC_ENABLE_STATIC(no)
AM_PROG_LIBTOOL


dnl
dnl Checks for gtk
dnl
GLIB2_REQUIRED=2.34.0
GMODULE2_REQUIRED=2.34.0
GTK3_REQUIRED=3.0.0

AC_SUBST(GLIB2_REQUIRED)
AC_SUBST(GMODULE2_REQUIRED)
AC_SUBST(GTK3_REQUIRED)

AM_PATH_GLIB_2_0($GLIB2_REQUIRED, , AC_MSG_ERROR(Cannot find GLib2))
AM_PATH_GTK_3_0($GTK3_REQUIRED,, AC_MSG_ERROR(Cannot find GTK3))

gtk3_binary_version=`$PKG_CONFIG --variable=gtk_binary_version gtk+-3.0`
gtk3_libdir=`$PKG_CONFIG --variable=libdir gtk+-3.0`
GTK3_IM_MODULEDIR="$gtk3_libdir"/gtk-3.0/$gtk3_binary_version/immodules
AC_SUBST(GTK3_IM_MODULEDIR)


dnl
dnl Checks for header files.
dnl
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h])


dnl
dnl Checks for typedefs, structures, and compiler characteristics.
dnl


dnl
dnl Checks for library functions.
dnl


AC_CONFIG_FILES([
Makefile
])

AC_OUTPUT
