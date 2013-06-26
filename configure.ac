#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

dnl Process this file with autoconf to produce a configure script.
m4_define([gtkimcocoa_major_version], [0])
m4_define([gtkimcocoa_minor_version], [0])
m4_define([gtkimcocoa_micro_version], [1])
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
AC_LANG([Objective C])
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
PKG_CHECK_MODULES(GTK3, gtk+-3.0 >= GTK3_REQUIRED,
                  [have_gtk3=yes], [have_gtk3=no])
AM_CONDITIONAL(HAVE_GTK3, test $have_gtk3 = "yes")
if test $have_gtk3 = "yes"; then
    gtk3_binary_version=`$PKG_CONFIG --variable=gtk_binary_version gtk+-3.0`
    gtk3_libdir=`$PKG_CONFIG --variable=libdir gtk+-3.0`
    GTK3_IM_MODULEDIR="$gtk3_libdir"/gtk-3.0/$gtk3_binary_version/immodules
    AC_SUBST(GTK3_IM_MODULEDIR)
fi


GTK2_REQUIRED=2.24.0
PKG_CHECK_MODULES(GTK2, gtk+-2.0 >= GTK2_REQUIRED,
                  [have_gtk2=yes], [have_gtk2=no])
AM_CONDITIONAL(HAVE_GTK2, test $have_gtk2 = "yes")
if test $have_gtk2 = "yes"; then
    gtk2_binary_version=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
    gtk2_libdir=`$PKG_CONFIG --variable=libdir gtk+-2.0`
    GTK2_IM_MODULEDIR="$gtk2_libdir"/gtk-2.0/$gtk2_binary_version/immodules
    AC_SUBST(GTK2_IM_MODULEDIR)
fi


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


dnl
dnl Options to enable some features
dnl
AC_ARG_ENABLE([workaround],
              AS_HELP_STRING([--enable-workaround],
                             [Enable some workaround code]),
              enable_workaround=$enableval,
              enable_workaround=no)
AC_MSG_RESULT([$enable_workaround])

if test "$enable_workaround" = "yes"; then
  AC_DEFINE(GTK_IM_COCOA_ENABLE_JIS_KEYBOARD_WORKAROUND, 1,
            [Define to 1 if you enable workaround for JIS keyboard])
  AC_DEFINE(GTK_IM_COCOA_ENABLE_COLOR_WORKAROUND, 1,
            [Define to 1 if you enable workaround for color of preedit string])
fi


AC_CONFIG_FILES([
Makefile
src/Makefile
gtk2/Makefile
test/Makefile
])

AC_OUTPUT

AC_MSG_RESULT([
Configure result:

  GTK+3: ${have_gtk3}
  GTK+2: ${have_gtk2}
])
