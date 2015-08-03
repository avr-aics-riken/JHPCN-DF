###################################################################################
#
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
#
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
###################################################################################

cmake_minimum_required(VERSION 2.6)
project(JHPCNDFlib_PerformanceTest CXX)

option(REAL_8_BYTE "build test for double" OFF)
if(REAL_8_BYTE)
    add_definitions(-DREAL_8_BYTE)
endif()

###################################################################
# build & install
###################################################################
add_executable(PerformanceTest PerformanceTest.cpp)
target_link_libraries(PerformanceTest jhpcndf ${ZLIB_LIBRARIES} ${LZ4_LIBRARIES})

add_executable(DumpTool DumpTool.cpp)
target_link_libraries(DumpTool jhpcndf ${ZLIB_LIBRARIES} ${LZ4_LIBRARIES})
