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
project(JHPCNDFlib_UnitTest CXX)

# disable pthread
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTEST_HAS_PTHREAD=0")

###################################################################
# build & install
###################################################################
include_directories(${PROJECT_SOURCE_DIR})
add_executable(UnitTest
    ${PROJECT_SOURCE_DIR}/gtest/gtest_main.cc
    ${PROJECT_SOURCE_DIR}/gtest/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/src/TestXOR.cpp
    ${PROJECT_SOURCE_DIR}/src/TestOR.cpp
    ${PROJECT_SOURCE_DIR}/src/TestAND.cpp
    ${PROJECT_SOURCE_DIR}/src/TestZeroPadding.cpp
    ${PROJECT_SOURCE_DIR}/src/TestFileInfoManager.cpp
    ${PROJECT_SOURCE_DIR}/src/TestIO.cpp
    )
target_link_libraries(UnitTest ${ZLIB_LIBRARIES} ${LZ4_LIBRARIES})
