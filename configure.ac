#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([mextract], [0.10.1-beta], [sramsay.unl@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_CONFIG_SRCDIR([src/mextract.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CC_C99

# Checks for libraries.
PKG_CHECK_MODULES(XML, libxml-2.0 >= 2.4)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_GNU_SOURCE
AC_CHECK_FUNCS([asprintf])

AC_OUTPUT
