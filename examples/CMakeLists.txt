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
project(JHPCNDFlib_Examples CXX C Fortran)

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    ADD_DEFINITIONS(-DDEBUG)
endif()

option(REAL_8_BYTE "build sample code for double" OFF)
if(REAL_8_BYTE)
    add_definitions(-DREAL_8_BYTE)
endif()

include(${PROJECT_SOURCE_DIR}/../cmake/CompileOptionSelector.cmake)
FreeForm()
C99()
CPP11()

###################################################################
# find libraries
###################################################################
# check OpenMP support and add option
if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
    FindOpenMP_quick_hack_fx10()
else()
    find_package(OpenMP REQUIRED)
endif()

#cmake 2.8 can not find OpenMP_Fortran_FLAGS (both Intel and GNU compiler)
set(OpenMP_Fortran_FLAGS ${OpenMP_C_FLAGS})

set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS}     ${OpenMP_CXX_FLAGS}")
set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}       ${OpenMP_C_FLAGS}")
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${OpenMP_Fortran_FLAGS}")

# zlib
if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
    FindZlib_quick_hack_fx10()
else()
    find_package (ZLIB REQUIRED)
endif()

# LZ4
find_package(LZ4 REQUIRED)

# JHPCNDF
find_package (JHPCNDF REQUIRED)
message(STATUS ${JHPCNDF_LIBRARIES})


###################################################################
# show build settings
###################################################################
message( STATUS "Destination PATH: "       ${CMAKE_INSTALL_PREFIX})
message( STATUS "CMAKE_CXX_COMPILER: "     ${CMAKE_CXX_COMPILER})
message( STATUS "CMAKE_CXX_FLAGS: "        ${CMAKE_CXX_FLAGS})
message( STATUS "CMAKE_C_COMPILER: "       ${CMAKE_C_COMPILER})
message( STATUS "CMAKE_C_FLAGS: "          ${CMAKE_C_FLAGS})
message( STATUS "CMAKE_Fortran_COMPILER: " ${CMAKE_Fortran_COMPILER})
message( STATUS "CMAKE_Fortran_FLAGS: "    ${CMAKE_Fortran_FLAGS})


###################################################################
# include files
###################################################################
include_directories( 
    ${JHPCNDF_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${LZ4_INCLUDE_DIRS}
    )

###################################################################
# build & install
###################################################################
add_executable(cpp_file_io ${PROJECT_SOURCE_DIR}/C++/read_and_write.cpp)
target_link_libraries(cpp_file_io ${JHPCNDF_LIBRARIES} ${ZLIB_LIBRARIES} ${LZ4_LIBRARIES})
install (TARGETS cpp_file_io DESTINATION bin)

add_executable(cpp_in_memory ${PROJECT_SOURCE_DIR}/C++/encode_and_decode.cpp)
target_link_libraries(cpp_in_memory ${JHPCNDF_LIBRARIES} ${ZLIB_LIBRARIES} ${LZ4_LIBRARIES})
install (TARGETS cpp_in_memory DESTINATION bin)

add_executable(c_file_io ${PROJECT_SOURCE_DIR}/C/read_and_write.c )
target_link_libraries(c_file_io ${JHPCNDF_LIBRARIES} ${ZLIB_LIBRARIES} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES} ${LZ4_LIBRARIES})
install (TARGETS c_file_io DESTINATION bin)

add_executable(c_in_memory ${PROJECT_SOURCE_DIR}/C/encode_and_decode.c )
target_link_libraries(c_in_memory ${JHPCNDF_LIBRARIES} ${ZLIB_LIBRARIES} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES} ${LZ4_LIBRARIES})
install (TARGETS c_in_memory DESTINATION bin)

add_executable(f_file_io ${PROJECT_SOURCE_DIR}/Fortran/read_and_write.F90 )
target_link_libraries(f_file_io  ${JHPCNDF_LIBRARIES} ${ZLIB_LIBRARIES} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES} ${LZ4_LIBRARIES})
install (TARGETS f_file_io  DESTINATION bin)

add_executable(f_in_memory ${PROJECT_SOURCE_DIR}/Fortran/read_and_write.F90 )
target_link_libraries(f_in_memory  ${JHPCNDF_LIBRARIES} ${ZLIB_LIBRARIES} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES} ${LZ4_LIBRARIES})
install (TARGETS f_in_memory  DESTINATION bin)
