##############################################################################
#
# LPTlib - Lagrangian Particle Tracking library
# 
# Copyright (c) 2012-2014 Advanced Institute for Computational Science, RIKEN.
# All right reserved.
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
   Interface.cpp\
   Utility.h\
   jhpcndf.f90


JHPCNDF_includedir = $(includedir)
JHPCNDF_include_HEADERS = $(top_builddir)/include/jhpcndf.h

