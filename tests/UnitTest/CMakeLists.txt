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

# add optimize option
include(${PROJECT_SOURCE_DIR}/../../cmake/CompileOptionSelector.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    CompileOptionSelector()
endif()
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    ADD_DEFINITIONS(-DDEBUG)
endif()

###################################################################
# find libraries
###################################################################
# check OpenMP support and add option
if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
    FindOpenMP_quick_hack_fx10()
else()
    find_package(OpenMP REQUIRED)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# zlib
if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
    FindZlib_quick_hack_fx10()
else()
    find_package (ZLIB REQUIRED)
endif()

###################################################################
# show build settings
###################################################################
message( STATUS "Destination PATH: " ${CMAKE_INSTALL_PREFIX})
message( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
message( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})

# Build GoogleTest
add_subdirectory(${PROJECT_SOURCE_DIR}/gtest-1.7.0)
include_directories(${PROJECT_SOURCE_DIR}/gtest-1.7.0/include)

###################################################################
# include files
###################################################################
include_directories(
    ${PROJECT_SOURCE_DIR}/../../src
    ${ZLIB_INCLUDE_DIRS}
    )

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
    ${PROJECT_SOURCE_DIR}/src/TestZlibIO.cpp
    )
target_link_libraries(UnitTest gtest ${ZLIB_LIBRARIES})
