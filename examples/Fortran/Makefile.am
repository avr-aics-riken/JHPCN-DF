##############################################################################
#
#  JHPCN-DF : Data compression library based on
#             Jointed Hierarchical Precision Compression Number Data Format
# 
#  Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
#  All rights reserved.
# 
##############################################################################

noinst_PROGRAMS=f_in_memory f_file_io


DISTCLEANFILES=*~
CLEANFILES=data/*.log

EXTRA_DIST=


f_in_memory_SOURCES  = encode_and_decode.f90
f_in_memory_FCFLAGS = -I$(top_srcdir)/include -I$(top_builddir)/src @ZLIB_FLAGS@ @LZ4_FLAGS@
#f_in_memory_LDADD = ../../src/libJHPCNDF.a  @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@
f_in_memory_LDADD = -L$(top_builddir)/src -lJHPCNDF @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@

f_file_io_SOURCES  = read_and_write.f90
f_file_io_FCFLAGS = -I$(top_srcdir)/include -I$(top_builddir)/src @ZLIB_FLAGS@  @LZ4_FLAGS@
#f_file_io_LDADD = ../../src/libJHPCNDF.a  @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@
f_file_io_LDADD = -L$(top_builddir)/src -lJHPCNDF  @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@

dist_noinst_DATA=

.PHONY:$(dist_noinst_DATA)
