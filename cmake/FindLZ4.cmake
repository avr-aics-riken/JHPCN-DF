###################################################################################
#
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
#
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
###################################################################################

# - Try to find LZ4
# Once done, this will define
#
#  LZ4_FOUND - system has LZ4
#  LZ4_INCLUDE_DIRS - the LZ4 include directories
#  LZ4_LIBRARIES - link these to use LZ4

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(LZ4_PKGCONF LZ4)

if(CMAKE_PREFIX_PATH)
  set(LZ4_CANDIDATE_PATH ${CMAKE_PREFIX_PATH})
  file(GLOB tmp "${CMAKE_PREFIX_PATH}/[Ll][Zz]4*/")
  list(APPEND LZ4_CANDIDATE_PATH ${tmp})
endif()

# Include dir
find_path(LZ4_INCLUDE_DIR
  NAMES lz4frame.h
  PATHS ${LZ4_ROOT} ${LZ4_PKGCONF_INCLUDE_DIRS} ${LZ4_CANDIDATE_PATH}
  PATH_SUFFIXES include
)

# Finally the library itself
find_library(LZ4_LIBRARY
  NAMES lz4
  PATHS ${LZ4_ROOT} ${LZ4_PKGCONF_LIBRARY_DIRS} ${LZ4_CANDIDATE_PATH}
  PATH_SUFFIXES lib 
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(LZ4_PROCESS_INCLUDES LZ4_INCLUDE_DIR)
set(LZ4_PROCESS_LIBS LZ4_LIBRARY)
libfind_process(LZ4)

