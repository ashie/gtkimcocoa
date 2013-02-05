#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

dnl Process this file with autoconf to produce a configure script.
m4_define([gtkimcocoa_major_version], [0])
m4_define([gtkimcocoa_minor_version], [0])
m4_define([gtkimcocoa_micro_version], [0])
m4_define([gtkimcocoa_version],
    gtkimcocoa_major_version.gtkimcocoa_minor_version.gtkimcocoa_micro_version)

AC_INIT([gtkimcocoa],
        [gtkimcocoa_version],
        [https://github.com/ashie/gtkimcocoa/issues],
        [gtkimcocoa])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([1.11])
AM_SILENT_RULES([yes])


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
GTK3_REQUIRED=3.0.0

AC_SUBST(GTK3_REQUIRED)

PKG_CHECK_MODULES(GTK3, gtk+-3.0 >= GTK3_REQUIRED)

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
