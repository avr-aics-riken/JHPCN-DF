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

###################################################################
# build & install
###################################################################
add_executable(PerformanceTest PerformanceTest.cpp)
target_link_libraries(PerformanceTest JHPCNDF ${ZLIB_LIBRARIES} ${LZ4_LIBRARIES})

add_executable(DumpTool DumpTool.cpp)
target_link_libraries(DumpTool JHPCNDF ${ZLIB_LIBRARIES} ${LZ4_LIBRARIES})

add_executable(PerformanceTest_Double PerformanceTest.cpp)
target_link_libraries(PerformanceTest_Double JHPCNDF ${ZLIB_LIBRARIES} ${LZ4_LIBRARIES})

add_executable(DumpTool_Double DumpTool.cpp)
target_link_libraries(DumpTool_Double JHPCNDF ${ZLIB_LIBRARIES} ${LZ4_LIBRARIES})

set_target_properties(PerformanceTest_Double DumpTool_Double
  PROPERTIES COMPILE_DEFINITIONS _REAL_IS_DOUBLE_
  )

