##############################################################################
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
# 
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
# 
##############################################################################
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.57])
AC_INIT([JHPCN-DF], [1.0], [keno@riken.jp], [JHPCN-DF])
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR([src/FileInfoManager.h])
AC_CONFIG_HEADERS([config.h])

# revision No.
JHPCNDF_REVISION="20150303_0000"
AC_SUBST(JHPCNDF_REVISION)


#
# platform
#
JHPCNDF_DEVICE="$($SHELL ./jhpcndf-uname)"
AC_SUBST(JHPCNDF_DEVICE)


#
# JHPCNDF special flags
#
JHPCNDF_CFLAGS="-I$JHPCNDF_INST_DIR/include"
AC_SUBST(JHPCNDF_CFLAGS)

JHPCNDF_LDFLAGS="-L$JHPCNDF_INST_DIR/lib"
AC_SUBST(JHPCNDF_LDFLAGS)

JHPCNDF_LIBS="-ljhpcndf -lz"
AC_SUBST(JHPCNDF_LIBS)

JHPCNDF_CXX="$CXX"
AC_SUBST(JHPCNDF_CXX)




# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_F77
AC_PROG_FC
AC_PROG_RANLIB

#
# MAKE_SUB_DIRS
#
AC_SUBST(MAKE_SUB_DIRS)
MAKE_SUB_DIRS="src"

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h stdint.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([pow])

AC_CONFIG_SUBDIRS([tests/UnitTest/gtest-1.7.0])
AC_OUTPUT

#
# default prefix setting
#
AC_PREFIX_DEFAULT([/usr/local/JHPCNDF])
if test "$prefix" = "NONE" ; then
  JHPCNDF_INST_DIR=/usr/local/JHPCNDF
else
  JHPCNDF_INST_DIR="$prefix"
fi



AC_CONFIG_FILES([Makefile \
                 jhpcndf-config \
                 src/Makefile \
                 ])

AC_OUTPUT
