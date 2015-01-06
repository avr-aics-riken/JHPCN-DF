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

# add optimize option
include(${PROJECT_SOURCE_DIR}/../../cmake/CompileOptionSelector.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    CompileOptionSelector()
endif()
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    ADD_DEFINITIONS(-DDEBUG)
endif()

option(REAL_8_BYTE "build test for double" OFF)
if(REAL_8_BYTE)
    add_definitions(-DREAL_8_BYTE)
endif()


###################################################################
# find libraries
###################################################################

# check OpenMP support and add option
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# zlib
find_package (ZLIB REQUIRED)

# JHPCNDF
set(JHPCNDF_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/../../include
    ${PROJECT_SOURCE_DIR}/../../src
    )
set(JHPCNDF_LIBRARIES jhpcndf)
link_directories( ${CMAKE_BINARY_DIR})

###################################################################
# show build settings
###################################################################
message( STATUS "Destination PATH: " ${CMAKE_INSTALL_PREFIX})
message( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
message( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})

###################################################################
# include files
###################################################################
include_directories( 
    ${JHPCNDF_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    )

###################################################################
# build & install
###################################################################
add_executable(PerformanceTest PerformanceTest.cpp)
target_link_libraries(PerformanceTest jhpcndf ${ZLIB_LIBRARIES})

add_executable(DumpTool DumpTool.cpp)
target_link_libraries(DumpTool jhpcndf ${ZLIB_LIBRARIES})
