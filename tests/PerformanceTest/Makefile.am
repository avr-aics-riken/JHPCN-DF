##############################################################################
#
#  JHPCN-DF : Data compression library based on
#             Jointed Hierarchical Precision Compression Number Data Format
# 
#  Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
#  All rights reserved.
# 
##############################################################################


noinst_PROGRAMS=PerformanceTest DumpTool PerformanceTest_double DumpTool_double

PerformanceTest_SOURCES  = PerformanceTest.cpp
PerformanceTest_CXXFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src @ZLIB_FLAGS@ @LZ4_FLAGS@
PerformanceTest_LDADD = ../../src/libJHPCNDF.a  @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@

DumpTool_SOURCES  = DumpTool.cpp
DumpTool_CXXFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src @ZLIB_FLAGS@ @LZ4_FLAGS@
DumpTool_LDADD = ../../src/libJHPCNDF.a  @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@

PerformanceTest_double_SOURCES  = PerformanceTest.cpp
PerformanceTest_double_CXXFLAGS = -D_REAL_IS_DOUBLE_ -I$(top_srcdir)/include -I$(top_srcdir)/src @ZLIB_FLAGS@ @LZ4_FLAGS@
PerformanceTest_double_LDADD = ../../src/libJHPCNDF.a  @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@

DumpTool_double_SOURCES  = DumpTool.cpp
DumpTool_double_CXXFLAGS = -D_REAL_IS_DOUBLE_ -I$(top_srcdir)/include -I$(top_srcdir)/src @ZLIB_FLAGS@ @LZ4_FLAGS@
DumpTool_double_LDADD = ../../src/libJHPCNDF.a  @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@

