##############################################################################
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
# 
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
# 
##############################################################################


lib_LIBRARIES = libJHPCNDF.a

libJHPCNDF_a_CXXFLAGS = -I$(top_srcdir)/include @ZLIB_FLAGS@ @LZ4_FLAGS@
libJHPCNDF_a_SOURCES= \
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


include_HEADERS = $(top_srcdir)/include/jhpcndf.h
