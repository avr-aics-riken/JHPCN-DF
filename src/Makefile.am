##############################################################################
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
# 
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
# 
##############################################################################

AUTOMAKE_OPTIONS = subdir-objects

lib_LIBRARIES = libjhpcndf.a

libjhpcndf_a_CXXFLAGS = @JHPCNDF_CFLAGS@ \
                   -I$(top_builddir)/include \
                   -I$(top_builddir)/src 

libjhpcndf_a_SOURCES= \
   Decoder.h\
   Encoder.h\
   FInterface.f90\
   FileInfoManager.h\
   IO.h\
   BaseIO.h\
   lz4IO.h\
   zlibIO.h\
   Interface.cpp\
   Utility.h\
   jhpcndf.f90


JHPCNDF_includedir = $(includedir)
JHPCNDF_include_HEADERS = $(top_builddir)/include/jhpcndf.h

