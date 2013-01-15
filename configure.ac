#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(tinyutils, 0.1, dev@djurdjevic.ch)
AC_CONFIG_SRCDIR([src/b64.c])
AC_CONFIG_HEADERS([config.h])
AC_OUTPUT(Makefile src/Makefile doc/Makefile examples/Makefile scripts/Makefile man/Makefile)

# Checks for programs.
AC_PROG_CC

AM_INIT_AUTOMAKE([1.9])

AC_LANG([C])
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_OUTPUT
