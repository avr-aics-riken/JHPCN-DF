##############################################################################
#
#  JHPCN-DF : Data compression library based on
#             Jointed Hierarchical Precision Compression Number Data Format
# 
#  Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
#  All rights reserved.
# 
##############################################################################

noinst_PROGRAMS=c_in_memory c_file_io


DISTCLEANFILES=*~
CLEANFILES=data/*.log

EXTRA_DIST=


c_in_memory_SOURCES  = encode_and_decode.c
c_in_memory_CFLAGS = -I$(top_srcdir)/include @ZLIB_FLAGS@ @LZ4_FLAGS@
c_in_memory_LDADD = ../../src/libJHPCNDF.a  @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@


c_file_io_SOURCES  = read_and_write.c
c_file_io_CFLAGS = -I$(top_srcdir)/include @ZLIB_FLAGS@ @LZ4_FLAGS@
c_file_io_LDADD = ../../src/libJHPCNDF.a  @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@

dist_noinst_DATA=

.PHONY:$(dist_noinst_DATA)
