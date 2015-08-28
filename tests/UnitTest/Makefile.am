##############################################################################
#
#  JHPCN-DF : Data compression library based on
#             Jointed Hierarchical Precision Compression Number Data Format
# 
#  Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
#  All rights reserved.
# 
##############################################################################

AUTOMAKE_OPTIONS = subdir-objects

noinst_PROGRAMS=UnitTest
UnitTest_SOURCES  = gtest/gtest_main.cc \
					gtest/gtest-all.cc \
					src/TestXOR.cpp \
					src/TestOR.cpp \
					src/TestAND.cpp \
					src/TestZeroPadding.cpp \
					src/TestFileInfoManager.cpp \
					src/TestIO.cpp
UnitTest_CXXFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src -I./ @ZLIB_FLAGS@ @LZ4_FLAGS@
UnitTest_LDADD = ../../src/libJHPCNDF.a  @ADDITIONAL_LIBS@ @ZLIB_LIBS@ @LZ4_LIBS@
