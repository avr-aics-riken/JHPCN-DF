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

# Build GoogleTest
add_subdirectory(${PROJECT_SOURCE_DIR}/gtest-1.7.0)
include_directories(${PROJECT_SOURCE_DIR}/gtest-1.7.0/include)

###################################################################
# build & install
###################################################################
add_executable(UnitTest
    ${PROJECT_SOURCE_DIR}/gtest-1.7.0/src/gtest_main.cc
    ${PROJECT_SOURCE_DIR}/src/TestXOR.cpp
    ${PROJECT_SOURCE_DIR}/src/TestOR.cpp
    ${PROJECT_SOURCE_DIR}/src/TestAND.cpp
    ${PROJECT_SOURCE_DIR}/src/TestZeroPadding.cpp
    ${PROJECT_SOURCE_DIR}/src/TestFileInfoManager.cpp
    ${PROJECT_SOURCE_DIR}/src/TestIO.cpp
    )
target_link_libraries(UnitTest gtest ${ZLIB_LIBRARIES} ${LZ4_LIBRARIES})
