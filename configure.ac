##############################################################################
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
# 
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
# 
##############################################################################
#
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.57])
AC_INIT([JHPCN-DF], [1.0.0], [keno@riken.jp], [JHPCN-DF])

echo "----------------------------------------"
echo "Running JHPCN-DF Configure Script"
echo "----------------------------------------"

AC_CONFIG_SRCDIR([src/FileInfoManager.h])


#
# Specify directory for auxillary build tools (e.g., install-sh, config.sub, config.guess) and M4 files.
#

AC_CONFIG_AUX_DIR(config)

#
# To prevent the source codes from breaking, we suppress to do configure in the source directory.
#

if test -e configure.ac ; then
  echo "Please run configure from a separate build directory. It is not allowd to run configure in the source directory."
  echo "Pleaes try ..."
  echo " "
  echo "$ cd BUILD_DIR"
  echo "$ ../configure [options]"
  exit
fi

#
# Detect target OS/host information
#
AC_CANONICAL_TARGET()




#
# Configure should create config.h from config.h.in
#

AC_CONFIG_HEADERS([config.h])


#
# Use automake
#

AM_INIT_AUTOMAKE()


#
# revision No.
#

JHPCNDF_REVISION="20150730_1500"
AC_SUBST(JHPCNDF_REVISION)

#
# check for fortran
#
AC_ARG_WITH(finterface, [AC_HELP_STRING([--with-finterface=(no|yes)],[make Fortran interface and example code [no]])], , with_finterface=no)

if test x"$with_finterface" = x"yes" ; then
  #
  # FORTRAN
  #

  FC=$F90
  FCFLAGS=$F90FLAGS


  #
  # Fortran REAL type
  #

  AC_ARG_WITH(f_precision, [AC_HELP_STRING([--with-f-precision=(single|double)],[Specify Fortran REAL type [single]])], , with_f_precision=single)
  AC_SUBST(REALOPT)
  AC_SUBST(FREALOPT)


  if test x"$with_f_precision" = x"double" ; then
    REALOPT=-D_REAL_IS_DOUBLE_
    case "$with_comp" in
      INTEL) FREALOPT="-r8" ;;
      FJ)    FREALOPT="-CcdRR8" ;;
      GNU)   FREALOPT="-fdefault-real-8" ;;
    esac
  else
    REALOPT=
    FREALOPT=
  fi
fi



#
# compiler
#

AC_ARG_WITH(comp, [AC_HELP_STRING([--with-comp=(INTEL|FJ|GNU)],[Specify Compiler type])], , with_comp=none)


#
# use OpenMP version or not
#
AC_ARG_WITH(openmp, [AC_HELP_STRING([--with-openmp=(yes|no)],[use OpenMP[yes]])], , with_openmp=yes)



#
# Check Zlib
#

AC_SUBST(ZLIB_DIR)
AC_SUBST(ZLIB_FLAGS)
AC_SUBST(ZLIB_LDFLAGS)
AC_SUBST(ZLIB_LIBS)
AC_ARG_WITH(zlib, AS_HELP_STRING( [--with-zlib=DIR], [Specify Zlib install directory.] ), ,
            AC_MSG_ERROR([Zlib is required in configure option (--with-zlib)!]) )
if test -e "$with_zlib" ; then
  ZLIB_DIR=$with_zlib;
  ZLIB_FLAGS=" -I$ZLIB_DIR/include"
  case $target in
    *apple-darwin* )
      ZLIB_LDFLAGS=""
      ZLIB_LIBS=$ZLIB_DIR/lib/libz.dylib
      echo "ZLIB_LDFLAGS="$ZLIB_LDFLAGS
      ;;
    *)
      ZLIB_LDFLAGS=-L$ZLIB_DIR/lib
      ZLIB_LIBS="-lz"
      echo "ZLIB_LDFLAGS="$ZLIB_LDFLAGS
      ;;
  esac
  echo "ZLIB_LDFLAGS="$ZLIB_LDFLAGS
else
 AC_MSG_ERROR([not found Zlib DIR : $with_zlib!])
fi


#
# check Zlib libray
#

CFLAGS="$CFLAGS $ZLIB_FLAGS"
LDFLAGS="$LDFLAGS $ZLIB_LDFLAGS"

#
# Check LZ4
#

AC_SUBST(LZ4_DIR)
AC_SUBST(LZ4_FLAGS)
AC_SUBST(LZ4_LDFLAGS)
AC_SUBST(LZ4_LIBS)
AC_ARG_WITH(lz4, AS_HELP_STRING( [--with-lz4=DIR], [Specify LZ4 install directory.] ), ,
            AC_MSG_ERROR([LZ4 is required in configure option (--with-lz4)!]) )
if test -e "$with_lz4" ; then
  LZ4_DIR=$with_lz4;
  LZ4_FLAGS=" -I$LZ4_DIR/include -DUSE_LZ4"
  case $target in
    *apple-darwin* )
      LZ4_LDFLAGS=""
      LZ4_LIBS=$LZ4_DIR/lib/liblz4.dylib
      ;;
    *)
      LZ4_LDFLAGS=-L$LZ4_DIR/lib
      LZ4_LIBS="-llz4"
      ;;
  esac
else
 AC_MSG_ERROR([not found LZ4 DIR : $with_lz4!])
fi


#
# check LZ4 libray
#

CFLAGS="$CFLAGS $LZ4_FLAGS"
LDFLAGS="$LDFLAGS $LZ4_LDFLAGS"




#
# example
#
AC_ARG_WITH(example, [AC_HELP_STRING([--with-example=(no|yes)],[make example [no]])], , with_example=no)

# EXAMPLE_TARGET = true, if compile example
AM_CONDITIONAL([EXAMPLE_TARGET], [test "$with_example" = "yes"])

#
# JHPCNDF special flags
#

JHPCNDF_CFLAGS="-I$JHPCNDF_INST_DIR/include"
AC_SUBST(JHPCNDF_CFLAGS)

JHPCNDF_LDFLAGS="-L$JHPCNDF_INST_DIR/lib"
AC_SUBST(JHPCNDF_LDFLAGS)

JHPCNDF_LIBS="-lJHPCNDF"
AC_SUBST(JHPCNDF_LIBS)

JHPCNDF_FC="$FC"
AC_SUBST(JHPCNDF_FC)

JHPCNDF_FC_LD="$FC"
AC_SUBST(JHPCNDF_FC_LD)



#
# default prefix setting
#

AC_PREFIX_DEFAULT([/usr/local/JHPCNDF])

if test x"$prefix" = x"NONE" ; then
  JHPCNDF_INST_DIR=/usr/local/JHPCNDF
else
  JHPCNDF_INST_DIR="$prefix"
fi

if test x"$with_comp" = x"FJ"; then
  JHPCNDF_FC_LD="$CXX"" --linkfortran"
elif test x"$with_comp" = x"INTEL" ; then
  JHPCNDF_LIBS="$JHPCNDF_LIBS -lstdc++ -lifport -lifcore"
  ADDITIONAL_LIBS=" -lstdc++ -lifport -lifcore"
elif test x"$with_comp" = x"GNU" ; then
  JHPCNDF_LIBS="$JHPCNDF_LIBS -lstdc++"
  ADDITIONAL_LIBS=" -lstdc++"
  CXXFLAGS="$CXXFLAGS -std=c++11"
fi
AC_SUBST(ADDITIONAL_LIBS)


if test x"$with_openmp" = x"yes" ; then
  if test x"$with_comp" = x"FJ"; then
    CXXFLAGS="$CXXFLAGS -Kopenmp"
    CFLAGS="$CCFLAGS -Kopenmp"
    FCFLAGS="$FCFLAGS -Kopenmp"
  elif test x"$with_comp" = x"INTEL" ; then
    CXXFLAGS="$CXXFLAGS -openmp"
    CFLAGS="$CFLAGS -openmp"
    FCFLAGS="$FCFLAGS -openmp"
  elif test x"$with_comp" = x"GNU" ; then
    CXXFLAGS="$CXXFLAGS -fopenmp"
    CFLAGS="$CFLAGS -fopenmp"
    FCFLAGS="$FCFLAGS -fopenmp"
  fi
fi

#
# MAKE_SUB_DIRS
#

AC_SUBST(MAKE_SUB_DIRS)
MAKE_SUB_DIRS="src doc"

if test x"$with_example" = x"yes" ; then
  MAKE_SUB_DIRS="$MAKE_SUB_DIRS examples/C++ examples/C"
  if test x"$with_finterface" = x"yes" ; then
    MAKE_SUB_DIRS="$MAKE_SUB_DIRS examples/Fortran"
  fi
fi


#
# Checks for programs.
#

AC_PROG_CXX
AC_PROG_CC
AC_PROG_F77
AC_PROG_FC
AC_PROG_RANLIB

AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_CHECK_LIB([stdc++], [main])

#
# Checks for libraries.
#

#
# Checks for header files.
#

AC_CHECK_HEADERS([float.h limits.h stdint.h stdlib.h])


#
# Checks for typedefs, structures, and compiler characteristics.
#

AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T


#
# Checks for library functions.
#

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
                 doc/Makefile \
                 include/jhpcndfVersion.h \
                 examples/C++/Makefile \
                 examples/C/Makefile \
                 examples/Fortran/Makefile \
                 ])

AC_OUTPUT

chmod +x ./jhpcndf-config

echo "---------------------------------------------"
echo "Finished Running JHPCN-DF Configure Script"
echo "---------------------------------------------"
